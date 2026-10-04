#include <stdlib.h>
#include "ct_arena.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "re.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cc_set(Cc *cc, unsigned char c) { cc->bits[c>>3] |= (1u << (c&7)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cc_setrange(Cc *cc, unsigned char lo, unsigned char hi) {
    for (unsigned c = lo; c <= hi; c++) cc_set(cc, (unsigned char)c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cc_invert(Cc *cc) { for (int i=0;i<32;i++) cc->bits[i]^=0xFFu; }
int cc_test(const Cc *cc, unsigned char c) { return (cc->bits[c>>3]>>(c&7))&1; }
static void cc_fill_digit(Cc *cc) { cc_setrange(cc,'0','9'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cc_fill_word(Cc *cc)  { cc_setrange(cc,'a','z'); cc_setrange(cc,'A','Z');
                                          cc_setrange(cc,'0','9'); cc_set(cc,'_'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cc_fill_space(Cc *cc) { cc_set(cc,' '); cc_set(cc,'\t'); cc_set(cc,'\n');
                                          cc_set(cc,'\r'); cc_set(cc,'\f'); cc_set(cc,'\v'); }
#define NFA_INIT_CAP 64
struct Nfa {
    Nfa_state *states;
    int        n;
    int        cap;
    int        start;
    int        accept;
    int        ngroups;
    char       group_name[MAX_GROUPS][64];
    char       group_repeatable[MAX_GROUPS];
    Code_fn code_fn;
    void        *code_ud;
    int          has_code;
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int nfa_alloc(Nfa *nfa) {
    if (nfa->n >= nfa->cap) {
        nfa->cap *= 2;
        nfa->states = ct_grow(nfa->states, (size_t)nfa->cap * sizeof(Nfa_state));
    }
    int id = nfa->n++;
    memset(&nfa->states[id], 0, sizeof(Nfa_state));
    nfa->states[id].id      = id;
    nfa->states[id].out1    = NFA_NULL;
    nfa->states[id].out2    = NFA_NULL;
    nfa->states[id].cap_idx = -1;
    nfa->states[id].kind    = NK_EPS;
    return id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int nfa_state(Nfa *nfa, Nfa_kind kind, int out1, int out2) {
    int id = nfa_alloc(nfa);
    nfa->states[id].kind = kind;
    nfa->states[id].out1 = out1;
    nfa->states[id].out2 = out2;
    return id;
}
typedef struct {
    const char *pat;
    int         pos;
    int         len;
    Nfa   *nfa;
    const char *err;
    int         ok;
    int         group_counter;
} Re_parser;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char peek(Re_parser *p)    { return p->pos < p->len ? p->pat[p->pos] : '\0'; }
static char consume(Re_parser *p) { return p->pos < p->len ? p->pat[p->pos++] : '\0'; }
static int  at_end(Re_parser *p)  { return p->pos >= p->len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void re_err(Re_parser *p, const char *msg) {
    if (p->ok) { p->err = msg; p->ok = 0; }
}
static int parse_alt(Re_parser *p, int *out_start, int *out_accept);
static int parse_concat(Re_parser *p, int *out_start, int *out_accept);
static int parse_quantified(Re_parser *p, int *out_start, int *out_accept);
static int parse_atom(Re_parser *p, int *out_start, int *out_accept);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_charclass(Re_parser *p) {
    int id = nfa_alloc(p->nfa);
    Nfa_state *s = &p->nfa->states[id];
    s->kind = NK_CLASS;
    s->out1 = NFA_NULL; s->out2 = NFA_NULL;
    int negate = 0, first = 1;
    if (peek(p) == '^') { negate = 1; consume(p); }
    while (!at_end(p)) {
        char c = peek(p);
        if (c == ']' && !first) { consume(p); break; }
        first = 0; consume(p);
        if (c == '\\') {
            if (at_end(p)) { re_err(p,"truncated escape in []"); return id; }
            char esc = consume(p);
            switch (esc) {
                case 'd': cc_fill_digit(&s->cc); break;
                case 'D': { Cc t={0}; cc_fill_digit(&t); cc_invert(&t);
                             for(int i=0;i<32;i++) s->cc.bits[i]|=t.bits[i]; } break;
                case 'w': cc_fill_word(&s->cc); break;
                case 'W': { Cc t={0}; cc_fill_word(&t); cc_invert(&t);
                             for(int i=0;i<32;i++) s->cc.bits[i]|=t.bits[i]; } break;
                case 's': cc_fill_space(&s->cc); break;
                case 'S': { Cc t={0}; cc_fill_space(&t); cc_invert(&t);
                             for(int i=0;i<32;i++) s->cc.bits[i]|=t.bits[i]; } break;
                default:  cc_set(&s->cc,(unsigned char)esc); break;
            }
        } else {
            if (peek(p)=='-' && p->pos+1<p->len && p->pat[p->pos+1]!=']') {
                consume(p);
                char hi = consume(p);
                if ((unsigned char)c <= (unsigned char)hi)
                    cc_setrange(&s->cc,(unsigned char)c,(unsigned char)hi);
                else re_err(p,"invalid range");
            } else { cc_set(&s->cc,(unsigned char)c); }
        }
    }
    if (negate) cc_invert(&s->cc);
    return id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void wire_quantifier(Nfa *nfa, int a_start, int a_acc, char q, int *out_start, int *out_accept) {
    if (q == '*') {
        int split = nfa_alloc(nfa), acc = nfa_alloc(nfa);
        nfa->states[split].kind = NK_SPLIT; nfa->states[split].out1 = a_start; nfa->states[split].out2 = acc;
        nfa->states[a_acc].out1 = split;
        nfa->states[acc].kind = NK_EPS; nfa->states[acc].out1 = NFA_NULL; nfa->states[acc].out2 = NFA_NULL;
        *out_start = split; *out_accept = acc;
    } else if (q == '+') {
        int split = nfa_alloc(nfa), acc = nfa_alloc(nfa);
        nfa->states[split].kind = NK_SPLIT; nfa->states[split].out1 = a_start; nfa->states[split].out2 = acc;
        nfa->states[a_acc].out1 = split;
        nfa->states[acc].kind = NK_EPS; nfa->states[acc].out1 = NFA_NULL; nfa->states[acc].out2 = NFA_NULL;
        *out_start = a_start; *out_accept = acc;
    } else {
        int split = nfa_alloc(nfa), acc = nfa_alloc(nfa);
        nfa->states[split].kind = NK_SPLIT; nfa->states[split].out1 = a_start; nfa->states[split].out2 = acc;
        nfa->states[a_acc].out1 = acc;
        nfa->states[acc].kind = NK_EPS; nfa->states[acc].out1 = NFA_NULL; nfa->states[acc].out2 = NFA_NULL;
        *out_start = split; *out_accept = acc;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_atom(Re_parser *p, int *out_start, int *out_accept) {
    if (at_end(p)) { re_err(p,"unexpected end of pattern"); return 0; }
    char c = peek(p);
    if (c == '(' && p->pos + 2 < p->len && p->pat[p->pos + 1] == '?' && p->pat[p->pos + 2] == ':') {
        consume(p); consume(p); consume(p);
        int inner_start, inner_acc;
        if (!parse_alt(p, &inner_start, &inner_acc)) return 0;
        if (peek(p) != ')') { re_err(p,"missing ')'"); return 0; }
        consume(p);
        *out_start = inner_start; *out_accept = inner_acc;
        return 1;
    }
    if (c == '(') {
        consume(p);
        int gidx = p->group_counter++;
        if (gidx >= MAX_GROUPS) { re_err(p,"too many capture groups"); return 0; }
        int cap_open = nfa_alloc(p->nfa);
        p->nfa->states[cap_open].kind    = NK_CAP_OPEN;
        p->nfa->states[cap_open].cap_idx = gidx;
        p->nfa->states[cap_open].out1    = NFA_NULL;
        p->nfa->states[cap_open].out2    = NFA_NULL;
        int inner_start, inner_acc;
        if (!parse_alt(p, &inner_start, &inner_acc)) return 0;
        if (peek(p) != ')') { re_err(p,"missing ')'"); return 0; }
        consume(p);
        int cap_close = nfa_alloc(p->nfa);
        p->nfa->states[cap_close].kind    = NK_CAP_CLOSE;
        p->nfa->states[cap_close].cap_idx = gidx;
        p->nfa->states[cap_close].out1    = NFA_NULL;
        p->nfa->states[cap_close].out2    = NFA_NULL;
        p->nfa->states[cap_open].out1  = inner_start;
        p->nfa->states[inner_acc].out1 = cap_close;
        *out_start  = cap_open;
        *out_accept = cap_close;
        if (gidx + 1 > p->nfa->ngroups) p->nfa->ngroups = gidx + 1;
        return 1;
    }
    if (c == '<') {
        consume(p);
        char capname[p->len - p->pos + 1]; int nlen=0;
        while (!at_end(p) && (isalpha((unsigned char)peek(p)) ||
               (nlen>0 && (isalnum((unsigned char)peek(p))||peek(p)=='_'))))
            capname[nlen++]=consume(p);
        capname[nlen]='\0';
        if (nlen==0||peek(p)!='>') { re_err(p,"bad named capture <n>"); return 0; }
        consume(p);
        if (peek(p)=='[') {
            consume(p);
            int gidx=p->group_counter++;
            if (gidx>=MAX_GROUPS) { re_err(p,"too many groups"); return 0; }
            snprintf(p->nfa->group_name[gidx],64,"%s",capname);
            int inner_start,inner_acc;
            if (!parse_alt(p,&inner_start,&inner_acc)) return 0;
            if (peek(p)!=']') { re_err(p,"missing ] in <n>[...]"); return 0; }
            consume(p);
            int q_start=inner_start, q_acc=inner_acc;
            char q=peek(p);
            if (q=='*'||q=='+'||q=='?') { consume(p); wire_quantifier(p->nfa,inner_start,inner_acc,q,&q_start,&q_acc); }
            int cap_open=nfa_alloc(p->nfa);
            p->nfa->states[cap_open].kind=NK_CAP_OPEN;
            p->nfa->states[cap_open].cap_idx=gidx;
            p->nfa->states[cap_open].out1=q_start;
            p->nfa->states[cap_open].out2=NFA_NULL;
            int cap_close=nfa_alloc(p->nfa);
            p->nfa->states[cap_close].kind=NK_CAP_CLOSE;
            p->nfa->states[cap_close].cap_idx=gidx;
            p->nfa->states[cap_close].out1=NFA_NULL;
            p->nfa->states[cap_close].out2=NFA_NULL;
            p->nfa->states[q_acc].out1=cap_close;
            *out_start=cap_open; *out_accept=cap_close;
            if (gidx+1>p->nfa->ngroups) p->nfa->ngroups=gidx+1;
            return 1;
        }
        if (peek(p)!='(') { re_err(p,"<n> must be followed by (...) or [...]"); return 0; }
        consume(p);
        int gidx=p->group_counter++;
        if (gidx>=MAX_GROUPS) { re_err(p,"too many groups"); return 0; }
        snprintf(p->nfa->group_name[gidx],64,"%s",capname);
        int cap_open=nfa_alloc(p->nfa);
        p->nfa->states[cap_open].kind=NK_CAP_OPEN;
        p->nfa->states[cap_open].cap_idx=gidx;
        p->nfa->states[cap_open].out1=NFA_NULL;
        p->nfa->states[cap_open].out2=NFA_NULL;
        int inner_start,inner_acc;
        if (!parse_alt(p,&inner_start,&inner_acc)) return 0;
        if (peek(p)!=')') { re_err(p,"missing ) in <n>(...)"); return 0; }
        consume(p);
        int cap_close=nfa_alloc(p->nfa);
        p->nfa->states[cap_close].kind=NK_CAP_CLOSE;
        p->nfa->states[cap_close].cap_idx=gidx;
        p->nfa->states[cap_close].out1=NFA_NULL;
        p->nfa->states[cap_close].out2=NFA_NULL;
        p->nfa->states[cap_open].out1=inner_start;
        p->nfa->states[inner_acc].out1=cap_close;
        *out_start=cap_open; *out_accept=cap_close;
        if (gidx+1>p->nfa->ngroups) p->nfa->ngroups=gidx+1;
        return 1;
    }
    if (c == '{') {
        consume(p);
        int depth=1; int cs=p->pos;
        while (!at_end(p)&&depth>0){char x=consume(p);if(x=='{')depth++;else if(x=='}')depth--;}
        int ce=p->pos-1;
        int clen=ce-cs;
        char *code=ct_alloc(clen+1); memcpy(code,p->pat+cs,clen); code[clen]='\0';
        int id=nfa_alloc(p->nfa);
        p->nfa->states[id].kind=(!strcmp(code,"!ww")) ? NK_ASSERT_NOT_WW : (!strcmp(code,"!sp")) ? NK_ASSERT_NOT_SP : NK_CODE_ASSERT;
        p->nfa->states[id].code_str=code;
        p->nfa->states[id].out1=NFA_NULL;
        p->nfa->states[id].out2=NFA_NULL;
        p->nfa->has_code=1;
        *out_start=*out_accept=id; return 1;
    }
    if (c == '[') { consume(p); int id=parse_charclass(p); if(!p->ok)return 0;
                    *out_start=*out_accept=id; return 1; }
    if (c == '^') { consume(p); int id=nfa_state(p->nfa,NK_ANCHOR_BOL,NFA_NULL,NFA_NULL);
                    *out_start=*out_accept=id; return 1; }
    if (c == '$') { consume(p); int id=nfa_state(p->nfa,NK_ANCHOR_EOL,NFA_NULL,NFA_NULL);
                    *out_start=*out_accept=id; return 1; }
    if (c == '.') { consume(p); int id=nfa_state(p->nfa,NK_ANY,NFA_NULL,NFA_NULL);
                    *out_start=*out_accept=id; return 1; }
    if (c == '\\') {
        consume(p);
        if (at_end(p)) { re_err(p,"truncated escape"); return 0; }
        char esc = consume(p);
        int id = nfa_alloc(p->nfa);
        Nfa_state *s = &p->nfa->states[id];
        s->out1=NFA_NULL; s->out2=NFA_NULL;
        switch (esc) {
            case 'd': s->kind=NK_CLASS; cc_fill_digit(&s->cc);  break;
            case 'D': s->kind=NK_CLASS; cc_fill_digit(&s->cc); cc_invert(&s->cc); break;
            case 'w': s->kind=NK_CLASS; cc_fill_word(&s->cc);   break;
            case 'W': s->kind=NK_CLASS; cc_fill_word(&s->cc);  cc_invert(&s->cc); break;
            case 's': s->kind=NK_CLASS; cc_fill_space(&s->cc);  break;
            case 'S': s->kind=NK_CLASS; cc_fill_space(&s->cc); cc_invert(&s->cc); break;
            default:  s->kind=NK_CHAR; s->ch=(unsigned char)esc; break;
        }
        *out_start=*out_accept=id; return 1;
    }
    if (c==')' || c==']' || c=='|' || c=='*' || c=='+' || c=='?') {
        re_err(p,"unexpected meta"); return 0;
    }
    consume(p);
    int id=nfa_state(p->nfa,NK_CHAR,NFA_NULL,NFA_NULL);
    p->nfa->states[id].ch=(unsigned char)c;
    *out_start=*out_accept=id; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_quantified(Re_parser *p, int *out_start, int *out_accept) {
    int a_start, a_acc;
    if (!parse_atom(p,&a_start,&a_acc)) return 0;
    char q = peek(p);
    if (q=='*'||q=='+'||q=='?') {
        consume(p);
        Nfa *nfa=p->nfa;
        if ((q=='*'||q=='+') && nfa->states[a_start].kind==NK_CAP_OPEN) nfa->group_repeatable[nfa->states[a_start].cap_idx]=1;
        wire_quantifier(nfa,a_start,a_acc,q,out_start,out_accept);
    } else { *out_start=a_start; *out_accept=a_acc; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_concat(Re_parser *p, int *out_start, int *out_accept) {
    int started=0, c_start=NFA_NULL, c_acc=NFA_NULL;
    while (!at_end(p) && peek(p)!='|' && peek(p)!=')' && peek(p)!=']') {
        int q_start, q_acc;
        if (!parse_quantified(p,&q_start,&q_acc)) return 0;
        if (!started) { c_start=q_start; c_acc=q_acc; started=1; }
        else { p->nfa->states[c_acc].out1=q_start; c_acc=q_acc; }
    }
    if (!started) { int id=nfa_state(p->nfa,NK_EPS,NFA_NULL,NFA_NULL); c_start=c_acc=id; }
    *out_start=c_start; *out_accept=c_acc; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_alt(Re_parser *p, int *out_start, int *out_accept) {
    int l_start, l_acc;
    if (!parse_concat(p,&l_start,&l_acc)) return 0;
    while (peek(p)=='|') {
        consume(p);
        int r_start, r_acc;
        if (!parse_concat(p,&r_start,&r_acc)) return 0;
        Nfa *nfa=p->nfa;
        int split=nfa_alloc(nfa), join=nfa_alloc(nfa);
        nfa->states[split].kind=NK_SPLIT; nfa->states[split].out1=l_start; nfa->states[split].out2=r_start;
        nfa->states[l_acc].out1=join; nfa->states[r_acc].out1=join;
        nfa->states[join].kind=NK_EPS; nfa->states[join].out1=NFA_NULL; nfa->states[join].out2=NFA_NULL;
        l_start=split; l_acc=join;
    }
    *out_start=l_start; *out_accept=l_acc; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Nfa *nfa_build(const char *pattern) {
    Nfa *nfa = ct_alloc(sizeof *nfa);
    nfa->cap=NFA_INIT_CAP; nfa->n=0; nfa->ngroups=0;
    memset(nfa->group_name,0,sizeof nfa->group_name);
    memset(nfa->group_repeatable,0,sizeof nfa->group_repeatable);
    nfa->states=ct_alloc((size_t)nfa->cap*sizeof(Nfa_state));
    nfa->start=NFA_NULL; nfa->accept=NFA_NULL;
    Re_parser p;
    p.pat=pattern; p.pos=0; p.len=(int)strlen(pattern);
    p.nfa=nfa; p.ok=1; p.err=""; p.group_counter=0;
    int frag_start, frag_acc;
    if (!parse_alt(&p,&frag_start,&frag_acc)||!p.ok) {
        fprintf(stderr,"re: compile error: %s\n",p.err);
        nfa_free(nfa); return NULL;
    }
    int acc=nfa_state(nfa,NK_ACCEPT,NFA_NULL,NFA_NULL);
    nfa->states[frag_acc].out1=acc;
    nfa->start=frag_start; nfa->accept=acc;
    return nfa;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int        nfa_state_count(const Nfa *nfa) { return nfa?nfa->n:0; }
void nfa_free(Nfa *nfa) { if(!nfa)return; ct_drop(nfa->states); ct_drop(nfa); }
#define SNAP_LW(ng) ((ng) ? MAX_CAPLOG : 0)
#define SNAP_W(ng) (2 * (ng) + 1 + 3 * SNAP_LW(ng))
typedef struct { int *ids; int *snaps; int n; int cap; int w; char *mark; } State_set;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int re_is_word_ch(unsigned char c) { return isalnum(c) || c=='_'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ss_compact(State_set *ss, int nstates) {
    int k = 0;
    memset(ss->mark, 0, (size_t)nstates);
    for (int i = ss->n; i-- > 0;) { if (ss->mark[ss->ids[i]]) ss->ids[i] = NFA_NULL; else ss->mark[ss->ids[i]] = 1; }
    for (int i = 0; i < ss->n; i++) {
        if (ss->ids[i] == NFA_NULL) continue;
        if (k != i) { ss->ids[k] = ss->ids[i]; memcpy(&ss->snaps[(size_t)k * (size_t)ss->w], &ss->snaps[(size_t)i * (size_t)ss->w], (size_t)ss->w * sizeof(int)); }
        k++;
    }
    ss->n = k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ss_add(State_set *ss, const Nfa *nfa, int id,
                   char *visited, int pos, int slen, const int *cur_snap, const char *subj) {
    if (id==NFA_NULL||visited[id]) return;
    visited[id]=1;
    Nfa_state *s=&nfa->states[id];
    int ng=nfa->ngroups, lw=SNAP_LW(ng);
    switch (s->kind) {
        case NK_EPS:
            ss_add(ss,nfa,s->out1,visited,pos,slen,cur_snap,subj); break;
        case NK_SPLIT:
            ss_add(ss,nfa,s->out1,visited,pos,slen,cur_snap,subj);
            ss_add(ss,nfa,s->out2,visited,pos,slen,cur_snap,subj); break;
        case NK_ANCHOR_BOL:
            if (pos==0) ss_add(ss,nfa,s->out1,visited,pos,slen,cur_snap,subj); break;
        case NK_ANCHOR_EOL:
            if (pos==slen) ss_add(ss,nfa,s->out1,visited,pos,slen,cur_snap,subj); break;
        case NK_ASSERT_NOT_SP: {
            int sp = subj && pos<slen && isspace((unsigned char)subj[pos]);
            if (!sp) ss_add(ss,nfa,s->out1,visited,pos,slen,cur_snap,subj); break; }
        case NK_ASSERT_NOT_WW: {
            int ww = subj && pos>0 && pos<slen && re_is_word_ch((unsigned char)subj[pos-1]) && re_is_word_ch((unsigned char)subj[pos]);
            if (!ww) ss_add(ss,nfa,s->out1,visited,pos,slen,cur_snap,subj); break; }
        case NK_CAP_OPEN: {
            int ns[ss->w]; memcpy(ns,cur_snap,sizeof ns);
            ns[s->cap_idx]=pos; ns[ng+s->cap_idx]=-1;
            ss_add(ss,nfa,s->out1,visited,pos,slen,ns,subj); break; }
        case NK_CAP_CLOSE: {
            int ns[ss->w]; memcpy(ns,cur_snap,sizeof ns);
            int g=s->cap_idx, *nl=&ns[2*ng];
            ns[ng+g]=pos;
            if (*nl<lw && ns[g]>=0) { nl[1+*nl]=g; nl[1+lw+*nl]=ns[g]; nl[1+2*lw+*nl]=pos; (*nl)++; }
            ss_add(ss,nfa,s->out1,visited,pos,slen,ns,subj); break; }
        default:
            if (ss->n==ss->cap) ss_compact(ss,nfa->n);
            memcpy(&ss->snaps[(size_t)ss->n*(size_t)ss->w],cur_snap,(size_t)ss->w*sizeof(int));
            ss->ids[ss->n++]=id; break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eps_closure_into(State_set *ss, const Nfa *nfa,
                              int start, int pos, int slen, const int *snap, const char *subj) {
    char visited[nfa->n]; memset(visited,0,(size_t)nfa->n);
    ss_add(ss,nfa,start,visited,pos,slen,snap,subj);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void nfa_exec(const Nfa *nfa, const char *subject, Match *result) {
    memset(result,0,sizeof *result);
    result->matched=0;
    for (int i=0;i<MAX_GROUPS;i++) { result->group_start[i]=-1; result->group_end[i]=-1; }
    result->ngroups=nfa->ngroups;
    if (!nfa||!subject) return;
    int slen=(int)strlen(subject), nst=nfa->n, ng=nfa->ngroups, lw=SNAP_LW(ng), w=SNAP_W(ng);
    int anchored_bol=(nfa->states[nfa->start].kind==NK_ANCHOR_BOL);
    int blank[w]; memset(blank,0,sizeof blank); for(int i=0;i<2*ng;i++) blank[i]=-1;
    int ids_a[2*nst], ids_b[2*nst], snaps_a[2*nst*w], snaps_b[2*nst*w]; char mark[nst];
    for (int start_pos=0; start_pos<=slen; start_pos++) {
        State_set cur={ids_a,snaps_a,0,2*nst,w,mark}, nxt={ids_b,snaps_b,0,2*nst,w,mark};
        eps_closure_into(&cur,nfa,nfa->start,start_pos,slen,blank,subject);
        int pos=start_pos;
        int best_end = -1;
        int best[w]; memset(best,0xff,sizeof best);
        while (1) {
            for (int i=0;i<cur.n;i++) {
                if (nfa->states[cur.ids[i]].kind==NK_ACCEPT && pos>=best_end) {
                    best_end  = pos;
                    memcpy(best,&cur.snaps[(size_t)i*(size_t)w],sizeof best);
                }
            }
            if (pos>=slen) break;
            unsigned char ch=(unsigned char)subject[pos];
            nxt.n=0;
            for (int i=0;i<cur.n;i++) {
                Nfa_state *s=&nfa->states[cur.ids[i]];
                int advance=0;
                switch (s->kind) {
                    case NK_CHAR:  advance=(s->ch==ch); break;
                    case NK_ANY:   advance=(ch!='\n');  break;
                    case NK_CLASS: advance=cc_test(&s->cc,ch); break;
                    default: break;
                }
                if (advance) {
                    char visited[nst]; memset(visited,0,(size_t)nst);
                    ss_add(&nxt,nfa,s->out1,visited,pos+1,slen,&cur.snaps[(size_t)i*(size_t)w],subject);
                }
            }
            ss_compact(&nxt,nst);
            { State_set t=cur; cur=nxt; nxt=t; }
            pos++;
            if (cur.n==0) break;
        }
        if (best_end >= 0) {
            result->matched    = 1;
            result->full_start = start_pos;
            result->full_end   = best_end;
            for (int g=0;g<nfa->ngroups;g++) {
                result->group_start[g] = best[g];
                result->group_end[g]   = best[ng+g];
                memcpy(result->group_name[g], nfa->group_name[g], 64);
                result->group_repeatable[g] = nfa->group_repeatable[g];
            }
            int nlog=best[2*ng];
            result->ncaplog = (nlog<0) ? 0 : ((nlog>lw) ? lw : nlog);
            for (int k=0;k<result->ncaplog;k++) { result->caplog_group[k]=best[2*ng+1+k]; result->caplog_start[k]=best[2*ng+1+lw+k]; result->caplog_end[k]=best[2*ng+1+2*lw+k]; }
            return;
        }
        if (anchored_bol) break;
    }
}

#include <stdlib.h>
#include "ct_arena.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "re.h"
#include "rt/gc_heap.h"
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
typedef struct { int name_off; int name_len; int repeatable; } Re_group;
struct Nfa {
    Nfa_state *states;
    int        n;
    int        cap;
    int        start;
    int        accept;
    int        ngroups;
    const char *pat;
    Code_fn code_fn;
    void        *code_ud;
    int          has_code;
    Re_group     groups[];
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
        int noff=p->pos, nlen=0;
        while (!at_end(p) && (isalpha((unsigned char)peek(p)) ||
               (nlen>0 && (isalnum((unsigned char)peek(p))||peek(p)=='_'))))
            { consume(p); nlen++; }
        if (nlen==0||peek(p)!='>') { re_err(p,"bad named capture <n>"); return 0; }
        consume(p);
        if (peek(p)=='[') {
            consume(p);
            int gidx=p->group_counter++;
            p->nfa->groups[gidx].name_off=noff; p->nfa->groups[gidx].name_len=nlen;
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
        p->nfa->groups[gidx].name_off=noff; p->nfa->groups[gidx].name_len=nlen;
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
        if ((q=='*'||q=='+') && nfa->states[a_start].kind==NK_CAP_OPEN) nfa->groups[nfa->states[a_start].cap_idx].repeatable=1;
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
    int plen=(int)strlen(pattern), gmax=0;
    for (int i=0;i<plen;i++) gmax+=(pattern[i]=='('||pattern[i]=='<');
    size_t hb=sizeof(Nfa)+(size_t)gmax*sizeof(Re_group);
    Nfa *nfa = ct_alloc(hb+(size_t)plen+1);
    memset(nfa,0,hb);
    memcpy((char *)nfa+hb,pattern,(size_t)plen+1); nfa->pat=(char *)nfa+hb;
    nfa->cap=NFA_INIT_CAP; nfa->n=0; nfa->ngroups=0;
    nfa->states=ct_alloc((size_t)nfa->cap*sizeof(Nfa_state));
    nfa->start=NFA_NULL; nfa->accept=NFA_NULL;
    Re_parser p;
    p.pat=nfa->pat; p.pos=0; p.len=plen;
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
#define SNAP_W(ng) (2 * (ng) + 1)
typedef struct { int *node; int n; int cap; } Cap_pool;
typedef struct { int *ids; int *snaps; int n; int cap; int w; char *mark; Cap_pool *pool; } State_set;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cap_pool_push(Cap_pool *pl, int g, int cs, int ce, int prev) {
    if (pl->n == pl->cap) { pl->cap = pl->cap ? 2 * pl->cap : 64; pl->node = rt_wsb_realloc(pl->node, (size_t)pl->cap * 4 * sizeof(int)); }
    int *e = pl->node + 4 * (size_t)pl->n;
    e[0] = g; e[1] = cs; e[2] = ce; e[3] = prev;
    return pl->n++;
}
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
    int ng=nfa->ngroups;
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
            int g=s->cap_idx;
            ns[ng+g]=pos;
            if (ns[g]>=0) ns[2*ng]=cap_pool_push(ss->pool,g,ns[g],pos,ns[2*ng]);
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
static void match_fill(Match *r, const Nfa *nfa, int nlog) {
    int ng = nfa->ngroups, nb = 0;
    for (int g = 0; g < ng; g++) nb += nfa->groups[g].name_len + 1;
    size_t ib = ((size_t)4 * (size_t)ng + (size_t)3 * (size_t)nlog) * sizeof(int);
    r->ngroups = ng; r->ncaplog = nlog; r->blk = (ib + (size_t)nb) ? rt_wsb_alloc(ib + (size_t)nb) : (char *)0;
    char *nm = r->blk + ib;
    for (int g = 0; g < ng; g++) {
        const Re_group *gr = &nfa->groups[g];
        match_group_repeatable(r, g) = gr->repeatable; MATCH_GRP(r, g)[3] = (int)(nm - r->blk);
        memcpy(nm, nfa->pat + gr->name_off, (size_t)gr->name_len); nm[gr->name_len] = '\0'; nm += gr->name_len + 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void nfa_exec(const Nfa *nfa, const char *subject, Match *result) {
    result->matched=0; result->full_start=0; result->full_end=0; result->ngroups=0; result->ncaplog=0; result->blk=(char *)0;
    if (!nfa||!subject) return;
    int slen=(int)strlen(subject), nst=nfa->n, ng=nfa->ngroups, w=SNAP_W(ng);
    int anchored_bol=(nfa->states[nfa->start].kind==NK_ANCHOR_BOL);
    int blank[w]; memset(blank,0xff,sizeof blank);
    int ids_a[2*nst], ids_b[2*nst], snaps_a[2*nst*w], snaps_b[2*nst*w]; char mark[nst];
    Cap_pool pool={(int *)0,0,0};
    for (int start_pos=0; start_pos<=slen; start_pos++) {
        State_set cur={ids_a,snaps_a,0,2*nst,w,mark,&pool}, nxt={ids_b,snaps_b,0,2*nst,w,mark,&pool};
        pool.n=0;
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
            int nlog=0;
            for (int k=best[2*ng]; k>=0; k=pool.node[4*k+3]) nlog++;
            match_fill(result, nfa, nlog);
            result->matched    = 1;
            result->full_start = start_pos;
            result->full_end   = best_end;
            for (int g=0;g<ng;g++) { match_group_start(result, g) = best[g]; match_group_end(result, g) = best[ng+g]; }
            int j=nlog;
            for (int k=best[2*ng]; k>=0; k=pool.node[4*k+3]) {
                j--; match_caplog_group(result, j)=pool.node[4*k]; match_caplog_start(result, j)=pool.node[4*k+1]; match_caplog_end(result, j)=pool.node[4*k+2];
            }
            return;
        }
        if (anchored_bol) break;
    }
}

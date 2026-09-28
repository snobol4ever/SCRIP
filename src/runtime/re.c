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
    cv_t       gname_off;
    cv_t       gname_pool;
    Code_fn code_fn;
    void        *code_ud;
    int          has_code;
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void nfa_set_gname(Nfa *nfa, int g, const char *nm, int nlen) {
    uint32_t k = nfa->gname_off.len;
    cv_reserve(&nfa->gname_off, (uint32_t)sizeof(int), (uint64_t)g + 1, "nfa.gname_off");
    for (; k <= (uint32_t)g; k++) CV_AT(nfa->gname_off, int, k) = -1;
    if (nfa->gname_off.len < (uint32_t)g + 1) nfa->gname_off.len = (uint32_t)g + 1;
    cv_reserve(&nfa->gname_pool, 1, (uint64_t)nfa->gname_pool.len + (uint64_t)nlen + 1, "nfa.gname_pool");
    CV_AT(nfa->gname_off, int, g) = (int)nfa->gname_pool.len;
    memcpy((char *)nfa->gname_pool.p + nfa->gname_pool.len, nm, (size_t)nlen);
    ((char *)nfa->gname_pool.p)[nfa->gname_pool.len + (uint32_t)nlen] = 0;
    nfa->gname_pool.len += (uint32_t)nlen + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *nfa_gname(const Nfa *nfa, int g) {
    if (g < 0 || (uint32_t)g >= nfa->gname_off.len || CV_AT(nfa->gname_off, int, g) < 0) return "";
    return (const char *)nfa->gname_pool.p + CV_AT(nfa->gname_off, int, g);
}
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
        const char *capname = p->pat + p->pos; int nlen=0;
        while (!at_end(p) && (isalpha((unsigned char)peek(p)) ||
               (nlen>0 && (isalnum((unsigned char)peek(p))||peek(p)=='_'))))
            { consume(p); nlen++; }
        if (nlen==0||peek(p)!='>') { re_err(p,"bad named capture <n>"); return 0; }
        consume(p);
        if (peek(p)!='(') { re_err(p,"<n> must be followed by (...)"); return 0; }
        consume(p);
        int gidx=p->group_counter++;
        nfa_set_gname(p->nfa, gidx, capname, nlen);
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
    if (c==')' || c=='|' || c=='*' || c=='+' || c=='?') {
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
        if (q=='*') {
            int split=nfa_alloc(nfa), acc=nfa_alloc(nfa);
            nfa->states[split].kind=NK_SPLIT; nfa->states[split].out1=a_start; nfa->states[split].out2=acc;
            nfa->states[a_acc].out1=split;
            nfa->states[acc].kind=NK_EPS; nfa->states[acc].out1=NFA_NULL; nfa->states[acc].out2=NFA_NULL;
            *out_start=split; *out_accept=acc;
        } else if (q=='+') {
            int split=nfa_alloc(nfa), acc=nfa_alloc(nfa);
            nfa->states[split].kind=NK_SPLIT; nfa->states[split].out1=a_start; nfa->states[split].out2=acc;
            nfa->states[a_acc].out1=split;
            nfa->states[acc].kind=NK_EPS; nfa->states[acc].out1=NFA_NULL; nfa->states[acc].out2=NFA_NULL;
            *out_start=a_start; *out_accept=acc;
        } else {
            int split=nfa_alloc(nfa), acc=nfa_alloc(nfa);
            nfa->states[split].kind=NK_SPLIT; nfa->states[split].out1=a_start; nfa->states[split].out2=acc;
            nfa->states[a_acc].out1=acc;
            nfa->states[acc].kind=NK_EPS; nfa->states[acc].out1=NFA_NULL; nfa->states[acc].out2=NFA_NULL;
            *out_start=split; *out_accept=acc;
        }
    } else { *out_start=a_start; *out_accept=a_acc; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_concat(Re_parser *p, int *out_start, int *out_accept) {
    int started=0, c_start=NFA_NULL, c_acc=NFA_NULL;
    while (!at_end(p) && peek(p)!='|' && peek(p)!=')') {
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
    memset(&nfa->gname_off,0,sizeof nfa->gname_off); memset(&nfa->gname_pool,0,sizeof nfa->gname_pool);
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
void nfa_free(Nfa *nfa) { if(!nfa)return; ct_drop(nfa->gname_off.p); ct_drop(nfa->gname_pool.p); ct_drop(nfa->states); ct_drop(nfa); }
typedef struct { int *ids; int n; } State_set;
typedef struct Re_ev { int g; int s; int e; int depth; const struct Re_ev *prev; } Re_ev;
typedef struct { const int *gse; const int *log; int loglen; const Re_ev *ev; } Re_snap;
typedef struct { int w; cv_t gse; cv_t lo; cv_t ll; cv_t log; } Re_store;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int *re_ints(cv_t *v, uint64_t k, const char *nm) { cv_reserve(v, (uint32_t)sizeof(int), k ? k : 1, nm); return (int *)v->p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void re_store_put(Re_store *st, int slot, const Re_snap *sn) {
    int evn = sn->ev ? sn->ev->depth : 0, len = sn->loglen + evn;
    int *g = re_ints(&st->gse, (uint64_t)(slot + 1) * (uint64_t)st->w, "re.gse") + (size_t)slot * (size_t)st->w;
    int base = (int)st->log.len;
    int *lg;
    memcpy(g, sn->gse, (size_t)st->w * sizeof(int));
    re_ints(&st->lo, (uint64_t)slot + 1, "re.lo")[slot] = base;
    re_ints(&st->ll, (uint64_t)slot + 1, "re.ll")[slot] = len;
    lg = re_ints(&st->log, (uint64_t)base + (uint64_t)len * 3, "re.log") + base;
    st->log.len = (uint32_t)(base + len * 3);
    if (sn->loglen) memcpy(lg, sn->log, (size_t)sn->loglen * 3 * sizeof(int));
    for (const Re_ev *e = sn->ev; e; e = e->prev) { int k = (sn->loglen + e->depth - 1) * 3; lg[k] = e->g; lg[k + 1] = e->s; lg[k + 2] = e->e; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Re_snap re_store_get(const Re_store *st, int slot) {
    Re_snap sn;
    sn.gse = (const int *)st->gse.p + (size_t)slot * (size_t)st->w;
    sn.log = (const int *)st->log.p + ((const int *)st->lo.p)[slot];
    sn.loglen = ((const int *)st->ll.p)[slot];
    sn.ev = (const Re_ev *)0;
    return sn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void re_store_drop(Re_store *st) { ct_drop(st->gse.p); ct_drop(st->lo.p); ct_drop(st->ll.p); ct_drop(st->log.p); }
static int re_is_word_ch(unsigned char c) { return isalnum(c) || c=='_'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ss_add(State_set *ss, Re_store *snaps, const Nfa *nfa, int id,
                   char *visited, int pos, int slen, const Re_snap *cur_snap, const char *subj) {
    if (id==NFA_NULL||visited[id]) return;
    visited[id]=1;
    Nfa_state *s=&nfa->states[id];
    switch (s->kind) {
        case NK_EPS:
            ss_add(ss,snaps,nfa,s->out1,visited,pos,slen,cur_snap,subj); break;
        case NK_SPLIT:
            ss_add(ss,snaps,nfa,s->out1,visited,pos,slen,cur_snap,subj);
            ss_add(ss,snaps,nfa,s->out2,visited,pos,slen,cur_snap,subj); break;
        case NK_ANCHOR_BOL:
            if (pos==0) ss_add(ss,snaps,nfa,s->out1,visited,pos,slen,cur_snap,subj); break;
        case NK_ANCHOR_EOL:
            if (pos==slen) ss_add(ss,snaps,nfa,s->out1,visited,pos,slen,cur_snap,subj); break;
        case NK_ASSERT_NOT_SP: {
            int sp = subj && pos<slen && isspace((unsigned char)subj[pos]);
            if (!sp) ss_add(ss,snaps,nfa,s->out1,visited,pos,slen,cur_snap,subj); break; }
        case NK_ASSERT_NOT_WW: {
            int ww = subj && pos>0 && pos<slen && re_is_word_ch((unsigned char)subj[pos-1]) && re_is_word_ch((unsigned char)subj[pos]);
            if (!ww) ss_add(ss,snaps,nfa,s->out1,visited,pos,slen,cur_snap,subj); break; }
        case NK_CAP_OPEN: {
            int ng[snaps->w]; Re_snap ns=*cur_snap;
            memcpy(ng, cur_snap->gse, sizeof ng); ng[2*s->cap_idx]=pos; ng[2*s->cap_idx+1]=-1; ns.gse=ng;
            ss_add(ss,snaps,nfa,s->out1,visited,pos,slen,&ns,subj); break; }
        case NK_CAP_CLOSE: {
            int ng[snaps->w]; Re_snap ns=*cur_snap; Re_ev ev;
            memcpy(ng, cur_snap->gse, sizeof ng); ng[2*s->cap_idx+1]=pos; ns.gse=ng;
            if (ng[2*s->cap_idx]>=0) { ev.g=s->cap_idx; ev.s=ng[2*s->cap_idx]; ev.e=pos; ev.prev=cur_snap->ev; ev.depth=(cur_snap->ev?cur_snap->ev->depth:0)+1; ns.ev=&ev; }
            ss_add(ss,snaps,nfa,s->out1,visited,pos,slen,&ns,subj); break; }
        default:
            re_store_put(snaps, ss->n, cur_snap);
            ss->ids[ss->n++]=id; break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eps_closure_into(State_set *ss, Re_store *snaps, const Nfa *nfa,
                              int start, int pos, int slen, const Re_snap *snap, const char *subj) {
    char visited[nfa->n > 0 ? nfa->n : 1]; memset(visited,0,sizeof visited);
    ss_add(ss,snaps,nfa,start,visited,pos,slen,snap,subj);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void re_result(Match *result, const Nfa *nfa, int matched, int fs, int fe, const int *gse, const int *log, int loglen) {
    int G = nfa ? nfa->ngroups : 0;
    size_t names = 0, ptrs, ints, total;
    cv_t st;
    char *pool;
    int *iv;
    for (int g = 0; g < G; g++) names += strlen(nfa_gname(nfa, g)) + 1;
    ptrs = (size_t)G * sizeof(const char *); ints = ((size_t)G * 2 + (size_t)loglen * 3) * sizeof(int); total = ptrs + ints + names;
    memset(&st, 0, sizeof st);
    cv_reserve(&st, 1, (uint64_t)(total ? total : 1), "match.store");
    result->store = st.p; result->matched = matched; result->full_start = fs; result->full_end = fe; result->ngroups = G; result->ncaplog = loglen;
    result->group_name = (const char **)st.p; iv = (int *)((char *)st.p + ptrs); pool = (char *)st.p + ptrs + ints;
    result->group_start = iv; result->group_end = iv + G; result->caplog_group = iv + 2 * G; result->caplog_start = iv + 2 * G + loglen;
    result->caplog_end = iv + 2 * G + 2 * loglen;
    for (int g = 0; g < G; g++) { const char *nm = nfa_gname(nfa, g); size_t l = strlen(nm) + 1; memcpy(pool, nm, l); result->group_name[g] = pool; pool += l;
        result->group_start[g] = gse ? gse[2 * g] : -1; result->group_end[g] = gse ? gse[2 * g + 1] : -1; }
    for (int k = 0; k < loglen; k++) { result->caplog_group[k] = log[3 * k]; result->caplog_start[k] = log[3 * k + 1]; result->caplog_end[k] = log[3 * k + 2]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void match_free(Match *m) { if (!m) return; ct_drop(m->store); memset(m, 0, sizeof *m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void nfa_exec(const Nfa *nfa, const char *subject, Match *result) {
    memset(result,0,sizeof *result);
    if (!nfa||!subject) { re_result(result, nfa, 0, 0, 0, (const int *)0, (const int *)0, 0); return; }
    int slen=(int)strlen(subject);
    int anchored_bol=(nfa->states[nfa->start].kind==NK_ANCHOR_BOL);
    int w = nfa->ngroups > 0 ? 2 * nfa->ngroups : 1, nn = nfa->n > 0 ? nfa->n : 1;
    int blank_g[w]; for (int i = 0; i < w; i++) blank_g[i] = -1;
    Re_snap blank; blank.gse = blank_g; blank.log = (const int *)0; blank.loglen = 0; blank.ev = (const Re_ev *)0;
    Re_store sa, sb; memset(&sa, 0, sizeof sa); memset(&sb, 0, sizeof sb); sa.w = w; sb.w = w;
    Re_store *cs = &sa, *ns_st = &sb;
    cv_t ida, idb, bestg, bestl; memset(&ida, 0, sizeof ida); memset(&idb, 0, sizeof idb); memset(&bestg, 0, sizeof bestg); memset(&bestl, 0, sizeof bestl);
    int *cur_ids = re_ints(&ida, (uint64_t)nn, "re.ids"), *nxt_ids = re_ints(&idb, (uint64_t)nn, "re.ids");
    int found = 0, found_start = 0, found_end = 0, best_len = 0;
    for (int start_pos=0; start_pos<=slen; start_pos++) {
        State_set cur; cur.ids = cur_ids; cur.n=0;
        cs->log.len = 0;
        eps_closure_into(&cur,cs,nfa,nfa->start,start_pos,slen,&blank,subject);
        int pos=start_pos;
        int best_end = -1;
        while (1) {
            for (int i=0;i<cur.n;i++) {
                if (nfa->states[cur.ids[i]].kind==NK_ACCEPT && pos>=best_end) {
                    Re_snap b = re_store_get(cs, i);
                    best_end  = pos;
                    memcpy(re_ints(&bestg, (uint64_t)w, "re.bestg"), b.gse, (size_t)w * sizeof(int));
                    if (b.loglen) memcpy(re_ints(&bestl, (uint64_t)b.loglen * 3, "re.bestl"), b.log, (size_t)b.loglen * 3 * sizeof(int));
                    best_len = b.loglen;
                }
            }
            if (pos>=slen) break;
            unsigned char ch=(unsigned char)subject[pos];
            State_set nxt; nxt.ids = nxt_ids; nxt.n=0;
            ns_st->log.len = 0;
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
                    char visited[nn]; memset(visited,0,sizeof visited);
                    Re_snap from = re_store_get(cs, i);
                    ss_add(&nxt,ns_st,nfa,s->out1,visited,pos+1,slen,&from,subject);
                }
            }
            { Re_store *t = cs; cs = ns_st; ns_st = t; }
            { int *t = cur_ids; cur_ids = nxt_ids; nxt_ids = t; }
            cur = nxt;
            pos++;
            if (cur.n==0) break;
        }
        if (best_end >= 0) { found = 1; found_start = start_pos; found_end = best_end; break; }
        if (anchored_bol) break;
    }
    if (found) re_result(result, nfa, 1, found_start, found_end, (const int *)bestg.p, (const int *)bestl.p, best_len);
    else re_result(result, nfa, 0, 0, 0, (const int *)0, (const int *)0, 0);
    re_store_drop(&sa); re_store_drop(&sb); ct_drop(ida.p); ct_drop(idb.p); ct_drop(bestg.p); ct_drop(bestl.p);
}

#include <string.h>
#include <stdlib.h>
#include <gc/gc.h>
#include "raku_re.h"
#include "BB.h"
static int nfa_bt(const Nfa_state *st, int sid, const char *subj, int pos, int slen, int depth) {
    if (sid == NFA_NULL) return -1;
    if (depth > 100000) return -1;
    const Nfa_state *s = &st[sid];
    switch (s->kind) {
        case NK_ACCEPT:
            return pos;
        case NK_EPS:
            return nfa_bt(st, s->out1, subj, pos, slen, depth + 1);
        case NK_ANCHOR_BOL:
            return (pos == 0)    ? nfa_bt(st, s->out1, subj, pos, slen, depth + 1) : -1;
        case NK_ANCHOR_EOL:
            return (pos == slen) ? nfa_bt(st, s->out1, subj, pos, slen, depth + 1) : -1;
        case NK_CHAR:
            if (pos < slen && (unsigned char)subj[pos] == s->ch)
                return nfa_bt(st, s->out1, subj, pos + 1, slen, depth + 1);
            return -1;
        case NK_ANY:
            if (pos < slen && subj[pos] != '\n')
                return nfa_bt(st, s->out1, subj, pos + 1, slen, depth + 1);
            return -1;
        case NK_CLASS:
            if (pos < slen && raku_cc_test(&s->cc, (unsigned char)subj[pos]))
                return nfa_bt(st, s->out1, subj, pos + 1, slen, depth + 1);
            return -1;
        case NK_SPLIT: {
            int r = nfa_bt(st, s->out1, subj, pos, slen, depth + 1);
            if (r >= 0) return r;
            return nfa_bt(st, s->out2, subj, pos, slen, depth + 1);
        }
        case NK_CAP_OPEN:
        case NK_CAP_CLOSE:
            return nfa_bt(st, s->out1, subj, pos, slen, depth + 1);
        default:
            return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int raku_nfa_bb_match(const Raku_nfa *nfa, const char *subject) {
    if (!nfa || !subject) return 0;
    const Nfa_state *st = raku_nfa_states((Raku_nfa *)nfa);
    int start = raku_nfa_start(nfa);
    int slen  = (int)strlen(subject);
    int anchored_bol = (start != NFA_NULL && st[start].kind == NK_ANCHOR_BOL);
    for (int sp = 0; sp <= slen; sp++) {
        if (nfa_bt(st, start, subject, sp, slen, 0) >= 0) return 1;
        if (anchored_bol) break;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static BB_op_t nfa_kind_to_bb(Nfa_kind k) {
    switch (k) {
        case NK_CHAR:       return BB_NFA_CHAR;
        case NK_ANY:        return BB_NFA_ANY;
        case NK_CLASS:      return BB_NFA_CLASS;
        case NK_SPLIT:      return BB_NFA_SPLIT;
        case NK_EPS:        return BB_NFA_EPS;
        case NK_ANCHOR_BOL: return BB_NFA_BOL;
        case NK_ANCHOR_EOL: return BB_NFA_EOL;
        case NK_CAP_OPEN:   return BB_NFA_CAP_OPEN;
        case NK_CAP_CLOSE:  return BB_NFA_CAP_CLOSE;
        case NK_ACCEPT:     return BB_NFA_ACCEPT;
        default:            return BB_OP_COUNT;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *raku_nfa_to_bb(Raku_nfa *nfa) {
    if (!nfa) return NULL;
    const Nfa_state *st = raku_nfa_states(nfa);
    int ns = raku_nfa_state_count(nfa);
    if (!st || ns <= 0) return NULL;
    for (int i = 0; i < ns; i++)
        if (nfa_kind_to_bb(st[i].kind) == BB_OP_COUNT) return NULL;
    IR_graph_t *bbg = BB_alloc(ns, BB_LANG_RKU);
    if (!bbg) return NULL;
    BB_t **node = (BB_t **)GC_malloc((size_t)ns * sizeof(BB_t *));
    for (int i = 0; i < ns; i++) {
        node[i] = BB_node_alloc(bbg, nfa_kind_to_bb(st[i].kind));
        if (!node[i]) { BB_free(bbg); return NULL; }
    }
    for (int i = 0; i < ns; i++) {
        BB_t *b = node[i];
        const Nfa_state *s = &st[i];
        b->α = b; b->β = NULL; b->γ = NULL; b->ω = NULL;
        b->ival = 0; b->sval = NULL; b->counter = 0; b->state = 0;
        if (s->out1 != NFA_NULL && s->out1 < ns) b->γ = node[s->out1];
        if (s->kind == NK_SPLIT) {
            if (s->out2 != NFA_NULL && s->out2 < ns) b->β = node[s->out2];
        }
        switch (s->kind) {
            case NK_CHAR:      b->ival = (int64_t)(unsigned char)s->ch; break;
            case NK_CLASS:   { char *blob = (char *)GC_malloc(32); memcpy(blob, s->cc.bits, 32); b->sval = blob; } break;
            case NK_CAP_OPEN:
            case NK_CAP_CLOSE: b->ival = (int64_t)s->cap_idx; break;
            default: break;
        }
    }
    int start = raku_nfa_start(nfa);
    bbg->entry = (start != NFA_NULL && start < ns) ? node[start] : node[0];
    return bbg;
}

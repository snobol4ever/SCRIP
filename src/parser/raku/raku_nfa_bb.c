#include <string.h>
#include <stdlib.h>
#include <gc/gc.h>
#include "raku_re.h"
#include "IR.h"
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { int gs[MAX_GROUPS]; int ge[MAX_GROUPS]; } Bb_cap;
/*--------------------------------------------------------------------------------------------------------------------*/
static int nfa_bt_ir_cap(IR_t *s, const char *subj, int pos, int slen, int depth, Bb_cap *cap,
                         char *vis, int stride) {
    if (!s) return -1;
    if (depth > 100000) return -1;
    /* (node,pos) visited cut — mirrors the parallel-NFA oracle's per-step visited[] (raku_re.c
       ss_add). A quantifier over an empty-matchable subpattern (e.g. (a?)*, (a*)*, ()*, (|a)*, and
       the same shape followed by a constraint that forces backtracking like (a?)*$ on "aab") builds
       an epsilon loop: SPLIT -> ... -> back to SPLIT with no char consumed. A naive backtracker spins
       forever and overflows the C stack (the depth>100000 guard dies of stack exhaustion long first).
       We memo each (node, pos): the FIRST arrival explores it; any later arrival at the SAME (node,pos)
       is cut (return -1). Keying by BOTH node and pos is essential — a single per-node stamp is
       overwritten when a node is legitimately in-progress at several positions during backtracking,
       re-opening the cycle. `vis` is an n*(slen+1) byte grid (node id = s->counter, stride = slen+1),
       cleared per leftmost-sweep iteration in raku_nfa_bb_exec; set-without-restore. SOUND for the
       verdict and for ordered (||-style) leftmost captures: the first arrival at (node,pos) is the
       highest-priority path, a winning path returns before any sibling re-arrival, and a failed
       (node,pos) subtree yields nothing new on revisit. As a bonus the memo bounds total work at
       n*(slen+1), so the ordered walk no longer degrades to exponential backtracking. */
    {
        int vi = (int)s->counter * stride + pos;
        if (vis[vi]) return -1;
        vis[vi] = 1;
    }
    switch (s->t) {
        case IR_NFA_ACCEPT:
            return pos;
        case IR_NFA_EPS:
            return nfa_bt_ir_cap(s->γ, subj, pos, slen, depth + 1, cap, vis, stride);
        case IR_NFA_CAP_OPEN: {
            int idx = (int)s->ival;
            int save_gs = -2, save_ge = -2;
            if (idx >= 0 && idx < MAX_GROUPS) { save_gs = cap->gs[idx]; save_ge = cap->ge[idx]; cap->gs[idx] = pos; cap->ge[idx] = -1; }
            int r = nfa_bt_ir_cap(s->γ, subj, pos, slen, depth + 1, cap, vis, stride);
            if (r < 0 && idx >= 0 && idx < MAX_GROUPS) { cap->gs[idx] = save_gs; cap->ge[idx] = save_ge; }
            return r;
        }
        case IR_NFA_CAP_CLOSE: {
            int idx = (int)s->ival;
            int save_ge = -2;
            if (idx >= 0 && idx < MAX_GROUPS) { save_ge = cap->ge[idx]; cap->ge[idx] = pos; }
            int r = nfa_bt_ir_cap(s->γ, subj, pos, slen, depth + 1, cap, vis, stride);
            if (r < 0 && idx >= 0 && idx < MAX_GROUPS) cap->ge[idx] = save_ge;
            return r;
        }
        case IR_NFA_BOL:
            return (pos == 0)    ? nfa_bt_ir_cap(s->γ, subj, pos, slen, depth + 1, cap, vis, stride) : -1;
        case IR_NFA_EOL:
            return (pos == slen) ? nfa_bt_ir_cap(s->γ, subj, pos, slen, depth + 1, cap, vis, stride) : -1;
        case IR_NFA_CHAR:
            if (pos < slen && (unsigned char)subj[pos] == (unsigned char)s->ival)
                return nfa_bt_ir_cap(s->γ, subj, pos + 1, slen, depth + 1, cap, vis, stride);
            return -1;
        case IR_NFA_ANY:
            if (pos < slen && subj[pos] != '\n')
                return nfa_bt_ir_cap(s->γ, subj, pos + 1, slen, depth + 1, cap, vis, stride);
            return -1;
        case IR_NFA_CLASS: {
            const unsigned char *bits = (const unsigned char *)s->sval;
            if (pos < slen && bits) {
                unsigned char c = (unsigned char)subj[pos];
                if ((bits[c >> 3] >> (c & 7)) & 1)
                    return nfa_bt_ir_cap(s->γ, subj, pos + 1, slen, depth + 1, cap, vis, stride);
            }
            return -1;
        }
        case IR_NFA_SPLIT: {
            int r = nfa_bt_ir_cap(s->γ, subj, pos, slen, depth + 1, cap, vis, stride);
            if (r >= 0) return r;
            return nfa_bt_ir_cap(s->β, subj, pos, slen, depth + 1, cap, vis, stride);
        }
        default:
            return -1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void raku_nfa_bb_exec(const Raku_nfa *nfa, const char *subject, Raku_match *result) {
    memset(result, 0, sizeof *result);
    for (int i = 0; i < MAX_GROUPS; i++) { result->group_start[i] = -1; result->group_end[i] = -1; }
    if (!nfa || !subject || !result) return;
    int ng = raku_nfa_ngroups(nfa);
    result->ngroups = ng;
    for (int g = 0; g < ng && g < MAX_GROUPS; g++) raku_nfa_group_name_copy(nfa, g, result->group_name[g]);
    IR_graph_t *bbg = raku_nfa_to_bb((Raku_nfa *)nfa);
    if (!bbg || !bbg->entry) return;
    IR_t *start = bbg->entry;
    int slen = (int)strlen(subject);
    int anchored_bol = (start->t == IR_NFA_BOL);
    int n = bbg->n;
    int stride = slen + 1;
    for (int i = 0; i < n; i++) if (bbg->all[i]) bbg->all[i]->counter = i; /* node id for the (node,pos) memo */
    char *vis = (char *)GC_malloc((size_t)n * (size_t)stride);             /* (node,pos) visited grid */
    for (int sp = 0; sp <= slen; sp++) {
        if (vis) memset(vis, 0, (size_t)n * (size_t)stride);               /* fresh memo per leftmost-sweep iteration */
        Bb_cap cap; for (int i = 0; i < MAX_GROUPS; i++) { cap.gs[i] = -1; cap.ge[i] = -1; }
        int end = vis ? nfa_bt_ir_cap(start, subject, sp, slen, 0, &cap, vis, stride) : -1;
        if (end >= 0) {
            result->matched    = 1;
            result->full_start = sp;
            result->full_end   = end;
            for (int g = 0; g < ng && g < MAX_GROUPS; g++) { result->group_start[g] = cap.gs[g]; result->group_end[g] = cap.ge[g]; }
            return;
        }
        if (anchored_bol) break;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int raku_nfa_bb_match(const Raku_nfa *nfa, const char *subject) {
    Raku_match m;
    raku_nfa_bb_exec(nfa, subject, &m);
    return m.matched ? 1 : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_e nfa_kind_to_bb(Nfa_kind k) {
    switch (k) {
        case NK_CHAR:       return IR_NFA_CHAR;
        case NK_ANY:        return IR_NFA_ANY;
        case NK_CLASS:      return IR_NFA_CLASS;
        case NK_SPLIT:      return IR_NFA_SPLIT;
        case NK_EPS:        return IR_NFA_EPS;
        case NK_ANCHOR_BOL: return IR_NFA_BOL;
        case NK_ANCHOR_EOL: return IR_NFA_EOL;
        case NK_CAP_OPEN:   return IR_NFA_CAP_OPEN;
        case NK_CAP_CLOSE:  return IR_NFA_CAP_CLOSE;
        case NK_ACCEPT:     return IR_NFA_ACCEPT;
        default:            return IR_OP_COUNT;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *raku_nfa_to_bb(Raku_nfa *nfa) {
    if (!nfa) return NULL;
    const Nfa_state *st = raku_nfa_states(nfa);
    int ns = raku_nfa_state_count(nfa);
    if (!st || ns <= 0) return NULL;
    for (int i = 0; i < ns; i++)
        if (nfa_kind_to_bb(st[i].kind) == IR_OP_COUNT) return NULL;
    IR_graph_t *bbg = IR_alloc(ns, IR_LANG_RKU);
    if (!bbg) return NULL;
    IR_t **node = (IR_t **)GC_malloc((size_t)ns * sizeof(IR_t *));
    for (int i = 0; i < ns; i++) {
        node[i] = IR_node_alloc(bbg, nfa_kind_to_bb(st[i].kind));
        if (!node[i]) { IR_free(bbg); return NULL; }
    }
    for (int i = 0; i < ns; i++) {
        IR_t *b = node[i];
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

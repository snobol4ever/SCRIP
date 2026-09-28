/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef NFA_RE_H
#define NFA_RE_H
#include <stddef.h>
#include <alloca.h>
typedef struct { unsigned char bits[32]; } Cc;
int cc_test(const Cc *cc, unsigned char c);
typedef enum {
    NK_EPS,
    NK_SPLIT,
    NK_CHAR,
    NK_ANY,
    NK_CLASS,
    NK_ANCHOR_BOL,
    NK_ANCHOR_EOL,
    NK_CAP_OPEN,
    NK_CAP_CLOSE,
    NK_CODE_ASSERT,
    NK_ASSERT_NOT_WW,
    NK_ASSERT_NOT_SP,
    NK_CODE_PRED,
    NK_SUB_CALL,
    NK_ACCEPT
} Nfa_kind;
#define NFA_NULL   (-1)
typedef struct {
    int       id;
    Nfa_kind  kind;
    unsigned char ch;
    Cc   cc;
    int       out1;
    int       out2;
    int       cap_idx;
    char     *code_str;
    const char *cap_name;
    int       pred_neg;
    int       bb_id;
} Nfa_state;
typedef struct {
    int matched;
    int full_start;
    int full_end;
    int         *group_start;
    int         *group_end;
    const char **group_name;
    int          ngroups;
    int          ncaplog;
    int         *caplog_group;
    int         *caplog_start;
    int         *caplog_end;
} Match;
typedef struct Nfa Nfa;
typedef struct { void *p; size_t cap; } Match_buf;
size_t     nfa_head_size(void);
size_t     nfa_state_size(void);
int        nfa_build_into(Nfa *nfa, void *mem, int scap, size_t bcap, const char *pattern, int *need_s, size_t *need_b);
int        nfa_exec_into(const Nfa *nfa, const char *subject, Match *result, Match_buf *mb);
const char *match_keep(Match *dst, const Match *src, const char *subject);
#define NFA_ON_STACK(var, pattern) Nfa *var = (Nfa *)alloca(nfa_head_size()); do { int s_ = 64, ns_ = 0; size_t b_ = 256, nb_ = 0; \
    const char *p_ = (pattern); int r_ = nfa_build_into(var, alloca((size_t)s_ * nfa_state_size() + b_), s_, b_, p_, &ns_, &nb_); \
    if (r_ < 0) r_ = nfa_build_into(var, alloca((size_t)ns_ * nfa_state_size() + nb_), ns_, nb_, p_, &ns_, &nb_); if (r_ <= 0) var = (Nfa *)0; } while (0)
#define MATCH_BUF(mb) Match_buf mb = { alloca(1024), 1024 }
#define NFA_EXEC(nfa, subject, m, mb) do { while (nfa_exec_into((nfa), (subject), (m), &(mb)) < 0) (mb).p = alloca((mb).cap); } while (0)
int        nfa_state_count(const Nfa *nfa);
int        nfa_start(const Nfa *nfa);
int        nfa_accept(const Nfa *nfa);
int        nfa_ngroups(const Nfa *nfa);
int        nfa_match(const Nfa *nfa, const char *subject);
Nfa_state *nfa_states(Nfa *nfa);
int        nfa_group_by_name(const Nfa *nfa, const char *name);
#endif
typedef int (*Code_fn)(const char *code, int pos, const char *subject,
                            void *userdata);
void nfa_set_code_fn(Nfa *nfa, Code_fn fn, void *userdata);
int  nfa_has_code(const Nfa *nfa);

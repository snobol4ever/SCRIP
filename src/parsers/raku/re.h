/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef NFA_RE_H
#define NFA_RE_H
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
    NK_CODE_PRED,
    NK_SUB_CALL,
    NK_ACCEPT
} Nfa_kind;
#define NFA_NULL   (-1)
#define MAX_GROUPS  16
typedef struct {
    int       id;
    Nfa_kind  kind;
    unsigned char ch;
    Cc   cc;
    int       out1;
    int       out2;
    int       cap_idx;
    char     *code_str;
    int       pred_neg;
    int       bb_id;
} Nfa_state;
typedef struct {
    int matched;
    int full_start;
    int full_end;
    int  group_start[MAX_GROUPS];
    int  group_end[MAX_GROUPS];
    char group_name[MAX_GROUPS][64];
    int  ngroups;
} Match;
typedef struct Nfa Nfa;
Nfa  *nfa_build(const char *pattern);
int        nfa_state_count(const Nfa *nfa);
int        nfa_start(const Nfa *nfa);
int        nfa_accept(const Nfa *nfa);
int        nfa_ngroups(const Nfa *nfa);
int        nfa_match(const Nfa *nfa, const char *subject);
void       nfa_exec(const Nfa *nfa, const char *subject, Match *result);
void       nfa_free(Nfa *nfa);
Nfa_state *nfa_states(Nfa *nfa);
int        nfa_group_by_name(const Nfa *nfa, const char *name);
void       nfa_group_name_copy(const Nfa *nfa, int g, char *dst64);
#endif
typedef int (*Code_fn)(const char *code, int pos, const char *subject,
                            void *userdata);
void nfa_set_code_fn(Nfa *nfa, Code_fn fn, void *userdata);
int  nfa_has_code(const Nfa *nfa);

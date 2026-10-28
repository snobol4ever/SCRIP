#ifndef LOWER_PASCAL_TYPES_H
#define LOWER_PASCAL_TYPES_H
typedef enum { PTK_UNKNOWN, PTK_INT, PTK_CHAR, PTK_BOOL, PTK_ENUM, PTK_REAL, PTK_STRING, PTK_ARRAY, PTK_RECORD, PTK_SET, PTK_POINTER, PTK_FILE, PTK_TEXT, PTK_PROC, PTK_NIL, PTK_VOID } pty_kind;
typedef struct pty pty_t;
typedef struct { const char *name; pty_t *type; } pty_field_t;
struct pty {
    pty_kind k;
    const char *name;
    long long lo, hi;
    int bounded, uns, dyn, open, packed, strmax, nix, nf, cf, isfunc, wbool;
    pty_t *base;
    pty_t **ix;
    pty_field_t *f;
    const char *fwd;
    const void *bnd;
};
struct pty_state;
void pty_run(tree_t *prog, int fpc);
const pty_t *pty_of_src(const tree_t *n);
const pty_t *pty_of_low(const tree_t *e);
void pty_note_low(const tree_t *low, const tree_t *src);
int pty_answer(const tree_t *low, pty_kind want);
void pty_census(const char *pred, const tree_t *low, int legacy, int typer);
const char *pty_kind_name(pty_kind k);
void pty_census_report(void);
void pty_note_shortstring(const tree_t *leaf, const tree_t *bound);
int pty_shortmax(const tree_t *low);
int pty_string_within(const tree_t *low, long long n);
long long pty_sizeof_low(const tree_t *low);
void lower_pascal_dump_types_on(void);
#endif

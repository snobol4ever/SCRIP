#ifndef DRIVER_PRIVATE_H
#define DRIVER_PRIVATE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <time.h>
#include "parsers/snobol4/scrip_cc.h"
#include "parsers/snocone/snocone_driver.h"
#include "parsers/prolog/prolog_driver.h"
#include "parsers/prolog/prolog_atom.h"
#include "parsers/raku/re.h"
#include "parsers/icon/icon_driver.h"
#include "parsers/raku/raku_driver.h"
#include "parsers/rebus/rebus_lower.h"
#include "runtime/builtins/gen.h"
#include "parsers/icon/icon_lex.h"
extern void ir_print_node   (const tree_t *e, FILE *f);
extern void ir_print_node_nl(const tree_t *e, FILE *f);
#include "core.h"
#include "sil_macros.h"
#include "runtime_shim.h"
#include "lower.h"
#include "SM.h"
#include "bb_build.h"
extern DESCR_t pat_at_cursor(const char *varname);
#include "runtime/builtins/gen_runtime.h"
extern DESCR_t      eval_expr(const char *src);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
extern int         Σlen;
#include "driver.h"
extern char  g_script_exception[512];
extern int   g_script_try_depth;
void rt_script_die_surface(const char *msg);
extern Match  g_match;
extern const char *g_subject;
extern int   g_kw_ctx;
#define FH_MAX 64
extern FILE *fh_table[FH_MAX];
extern char *fh_name[FH_MAX];
extern int   fh_init;
void  fh_ensure_init(void);
int   fh_alloc(FILE *fp);
FILE *fh_get(int idx);
void  fh_free(int idx);
int   fh_current_input(void);
int   fh_current_output(void);
void  fh_set_input(int idx);
void  fh_set_output(int idx);
FILE *fh_cur_out_fp(void);
FILE *fh_cur_in_fp(void);
int   fh_capture_begin(char **bufp, size_t *szp, int *saved_out);
void  fh_capture_end(int idx, int saved_out);
const char *define_spec_from_expr(tree_t *subj);
const char *define_entry_from_expr(tree_t *subj);
#include "stage2.h"
#include "driver.h"
#define CALL_STACK_MAX 256
#define SHADOW_MAX 32
typedef struct { char name[64]; DESCR_t val; } ShadowEntry;
typedef struct {
    jmp_buf  ret_env;
    char     fname[128];
    char   **saved_names;
    DESCR_t *saved_vals;
    int      nsaved;
    DESCR_t  retval_cell;
    int      retval_set;
    ShadowEntry shadow[SHADOW_MAX];
    int         nshadow;
} CallFrame;
extern CallFrame call_stack[CALL_STACK_MAX];
extern int       call_depth;
int  shadow_get(const char *name, DESCR_t *out);
void shadow_set_cur(const char *name, DESCR_t val);
int  shadow_has(const char *name);
int  is_current_frame_local(const char *name);
#define INIT_MAX   64
#define INIT_SLOTS  8
typedef struct { char nm[64]; DESCR_t val; } InitSlot;
typedef struct { int id; int ns; InitSlot s[INIT_SLOTS]; } InitEnt;
extern InitEnt init_tab[INIT_MAX];
extern int        init_n;
void init_update_snapshot(char **snames, DESCR_t *svals, int nsaved);
int _is_pat_fnc_name(const char *s);
int _expr_is_pat(tree_t *e);
void set_and_trace(const char *name, DESCR_t val);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline DESCR_t NAME_DEREF(DESCR_t d) {
    if (IS_NAME(d)) {
        if (IS_NAMEPTR(d)) return NAME_DEREF_PTR(d);
        if (IS_NAMEVAL(d)) return NV_GET_fn(d.s);
    }
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int NAME_SET(DESCR_t nd, DESCR_t val) {
    if (IS_NAME(nd)) {
        if (IS_NAMEPTR(nd)) { NAME_DEREF_PTR(nd) = val; return 1; }
        if (IS_NAMEVAL(nd)) { set_and_trace(nd.s, val); return 1; }
    }
    return 0;
}
DESCR_t *eval_ast_ref(tree_t *e);
DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
int string_section_assign(tree_t *lhs, DESCR_t val);
typedef struct {
    char name[64]; int nfields; char fields[64][64]; char parent[64]; DESCR_t defaults[64]; char has_default[64]; char required[64];
    char rw[64]; char sigil[64]; char priv[64]; char mro[64][64]; int mro_len; char parents[8][64]; int nparents;
    char roles[8][64]; int nroles; char methods[32][64]; int nmethods; char has_build; char build_keys[16][64]; int nbuild_keys;
    char handles_meth[32][64]; char handles_fld[32][64]; int nhandles;
    long serial_next;
} DatType;
DatType *dat_register(const char *spec);
DatType *dat_find_type(const char *name);
DatType *dat_find_field(const char *name, int *fidx);
DESCR_t    dat_construct(DatType *t, DESCR_t *args, int nargs);
void class_inherit_multi(const char *child, const char **parents, int nparents);
void class_compose_role(const char *child, const char *role);
void dat_add_method(const char *type, const char *mname);
void dat_set_field_default_i(const char *cls, const char *field, int64_t v);
void dat_set_field_priv(const char *cls, const char *field);
int dat_field_is_private(const char *cls, const char *field);
void dat_set_field_default_s(const char *cls, const char *field, const char *v);
void dat_set_field_default_r(const char *cls, const char *field, double v);
DESCR_t    dat_field_get(const char *fname, DESCR_t obj);
DESCR_t call_user_function(const char *fname, DESCR_t *args, int nargs);
DESCR_t call_builtin(tree_t *call, DESCR_t *args, int nargs);
int try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
const char *real_str(double r, char *buf, int bufsz);
int try_call_builtin(tree_t *call, DESCR_t *out);
int scan_try_call_builtin(tree_t *call, DESCR_t *args, int nargs, DESCR_t *out);
#define RESOLVE_PRED_TABLE_SIZE RESOLVE_PRED_TABLE_SIZE_FWD
#endif

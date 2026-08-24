/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef DRIVER_H
#define DRIVER_H
#include <stdint.h>
#include <setjmp.h>
#include "frontend/snobol4/scrip_cc.h"
#include "stage2.h"
extern int g_opt_dump_bb;
extern int g_polyglot;
#define SCRIP_MOD_MAX STAGE2_MOD_MAX
void polyglot_init(stage2_t *s2, const tree_t *prog);
int  polyglot_module_open(stage2_t *s2, const tree_t *s);
void polyglot_module_extend(stage2_t *s2, int mod_idx, const tree_t *s);
void record_register(const char *spec);
void    label_table_build(stage2_t *s2, const tree_t *prog);
const tree_t *label_lookup(const char *name);
extern const tree_t *g_exec_prog;
void    prescan_defines(const tree_t *prog);
DESCR_t eval_ast    (tree_t *e);
DESCR_t eval_ast_pat(tree_t *e);
void    execute_program(const tree_t *prog);
void    ir_dump_program(const tree_t *prog, FILE *f);
DESCR_t _builtin_IDENT  (DESCR_t *args, int nargs);
DESCR_t _builtin_DIFFER (DESCR_t *args, int nargs);
DESCR_t _builtin_EVAL   (DESCR_t *args, int nargs);
DESCR_t _builtin_CODE   (DESCR_t *args, int nargs);
DESCR_t _builtin_DATA   (DESCR_t *args, int nargs);
DESCR_t _builtin_print  (DESCR_t *args, int nargs);
DESCR_t _usercall_hook  (const char *name, DESCR_t *args, int nargs);
int     _label_exists_fn(const char *name);
DESCR_t _eval_str_impl_fn(const char *s);
DESCR_t _eval_pat_impl_fn(DESCR_t pat);
#endif

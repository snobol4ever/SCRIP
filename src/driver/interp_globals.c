#include "interp_private.h"
char g_script_exception[512] = "";
Match g_match;
const char *g_subject = "";
#define FH_MAX 64
FILE *fh_table[FH_MAX];
char *fh_name[FH_MAX];
int   fh_init = 0;
void fh_ensure_init(void) {
    if (fh_init) return;
    memset(fh_table,0,sizeof fh_table);
    memset(fh_name,0,sizeof fh_name);
    fh_table[0]=stdin; fh_table[1]=stdout; fh_table[2]=stderr;
    fh_name[0]="&input"; fh_name[1]="&output"; fh_name[2]="&errout";
    fh_init=1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int fh_alloc(FILE *fp) {
    fh_ensure_init();
    for(int i=3;i<FH_MAX;i++) if(!fh_table[i]){fh_table[i]=fp;fh_name[i]=NULL;return i;}
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
FILE *fh_get(int idx){
    fh_ensure_init();
    if(idx<0||idx>=FH_MAX) return NULL;
    return fh_table[idx];
}
/*--------------------------------------------------------------------------------------------------------------------*/
void fh_free(int idx){
    if(fh_init&&idx>=3&&idx<FH_MAX){ fh_table[idx]=NULL; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void stmt_init(void) {}
extern DESCR_t      eval_expr(const char *src);
extern const char  *exec_code(DESCR_t code_block);
extern int exec_stmt(const char *subj_name,
                          DESCR_t    *subj_var,
                          DESCR_t     pat,
                          DESCR_t    *repl,
                          int         has_repl);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
extern int         Σlen;
int g_polyglot = 0;
int g_opt_dump_bb = 0;
int      g_ir_step_limit = 0;
int      g_ir_steps_done = 0;
jmp_buf  g_ir_step_jmp;
unsigned long *rs24_diag_hits_ptr = NULL;
static const char *rs24_diag_kind_name(int k);
void rs24_diag_dump(void) {
    if (!rs24_diag_hits_ptr) return;
    FILE *fp = fopen("/tmp/rs24_diag_hits.log", "a");
    if (!fp) return;
    fprintf(fp, "=== RS-24 Icon-frame switch hits (pid=%d) ===\n", (int)getpid());
    for (int k = 0; k < (int)TT_KIND_COUNT; k++) {
        if (rs24_diag_hits_ptr[k]) {
            fprintf(fp, "  kind=%-3d %-20s hits=%lu\n",
                    k, rs24_diag_kind_name(k), rs24_diag_hits_ptr[k]);
        }
    }
    fclose(fp);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *rs24_diag_kind_name(int k) {
    switch (k) {
    case TT_VAR:        return "TT_VAR";
    case TT_ASSIGN:     return "TT_ASSIGN";
    case TT_FNC:        return "TT_FNC";
    case TT_IF:         return "TT_IF";
    case TT_WHILE:      return "TT_WHILE";
    case TT_UNTIL:      return "TT_UNTIL";
    case TT_REPEAT:     return "TT_REPEAT";
    case TT_EVERY:      return "TT_EVERY";
    case TT_SEQ:        return "TT_SEQ";
    case TT_SEQ_EXPR:   return "TT_SEQ_EXPR";
    case TT_ALT:        return "TT_ALT";
    case TT_ALTERNATE:  return "TT_ALTERNATE";
    case TT_REVASSIGN:  return "TT_REVASSIGN";
    case TT_LOOP_NEXT:  return "TT_LOOP_NEXT";
    case TT_SUSPEND:    return "TT_SUSPEND";
    case TT_RETURN:     return "TT_RETURN";
    case TT_PROC_FAIL:  return "TT_PROC_FAIL";
    default:           return "?";
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void set_and_trace(const char *name, DESCR_t val) {
    if (shadow_has(name)) { shadow_set_cur(name, val); goto trace_hook; }
    NV_SET_fn(name, val);
trace_hook:
    if (call_depth > 0) {
        CallFrame *fr = &call_stack[call_depth - 1];
        if (name && fr->fname[0] && strcmp(name, fr->fname) == 0) {
            fr->retval_cell = val;
            fr->retval_set  = 1;
        }
    }
    if (shadow_has(name) && name && name[0] != '&' && trace_is_active(name))
        comm_var(name, val);
}

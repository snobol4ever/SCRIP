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

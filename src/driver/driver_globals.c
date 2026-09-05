#include "driver_private.h"
char g_script_exception[512] = "";
int g_script_try_depth = 0;
Match g_match;
const char *g_subject = "";
#define FH_MAX 64
FILE *fh_table[FH_MAX];
char *fh_name[FH_MAX];
char  fh_mode[FH_MAX];
char  fh_type[FH_MAX];
int   fh_init = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fh_ensure_init(void) {
    if (fh_init) return;
    memset(fh_table,0,sizeof fh_table);
    memset(fh_name,0,sizeof fh_name);
    memset(fh_mode,0,sizeof fh_mode);
    memset(fh_type,0,sizeof fh_type);
    fh_table[0]=stdin; fh_table[1]=stdout; fh_table[2]=stderr;
    fh_name[0]="&input"; fh_name[1]="&output"; fh_name[2]="&errout";
    fh_mode[0]='r'; fh_mode[1]='w'; fh_mode[2]='w'; fh_type[0]='t'; fh_type[1]='t'; fh_type[2]='t';
    fh_init=1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fh_alloc(FILE *fp) {
    fh_ensure_init();
    for(int i=3;i<FH_MAX;i++) if(!fh_table[i]){fh_table[i]=fp;fh_name[i]=NULL;fh_mode[i]=0;fh_type[i]='t';return i;}
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
FILE *fh_get(int idx){
    fh_ensure_init();
    if(idx<0||idx>=FH_MAX) return NULL;
    return fh_table[idx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fh_free(int idx){
    if(fh_init&&idx>=3&&idx<FH_MAX){ fh_table[idx]=NULL; }
}
int   fh_cur_in  = 0;
int   fh_cur_out = 1;
int   fh_cur_init = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fh_cur_ensure(void){ if(fh_cur_init) return; fh_ensure_init(); fh_cur_in=0; fh_cur_out=1; fh_cur_init=1; }
int fh_current_input(void){ fh_cur_ensure(); return fh_cur_in; }
int fh_current_output(void){ fh_cur_ensure(); return fh_cur_out; }
void fh_set_input(int idx){ fh_cur_ensure(); if(idx>=0&&idx<FH_MAX&&fh_table[idx]) fh_cur_in=idx; }
void fh_set_output(int idx){ fh_cur_ensure(); if(idx>=0&&idx<FH_MAX&&fh_table[idx]) fh_cur_out=idx; }
FILE *fh_cur_out_fp(void){ fh_cur_ensure(); FILE *fp=fh_get(fh_cur_out); return fp?fp:stdout; }
FILE *fh_cur_in_fp(void){ fh_cur_ensure(); FILE *fp=fh_get(fh_cur_in); return fp?fp:stdin; }
int fh_capture_begin(char **bufp, size_t *szp, int *saved_out){ fh_cur_ensure(); FILE *ms=open_memstream(bufp,szp); if(!ms) return -1; int idx=fh_alloc(ms); if(idx<0){ fclose(ms); return -1; } *saved_out=fh_current_output(); fh_set_output(idx); return idx; }
void fh_capture_end(int idx, int saved_out){ fh_cur_ensure(); FILE *ms=fh_get(idx); if(ms){ fflush(ms); fclose(ms); } fh_free(idx); fh_set_output(saved_out); }
extern const char *Σ;
extern int         Ω;
extern int         Δ;
extern int         Σlen;
int g_polyglot = 0;
int g_opt_dump_bb = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
        comm_var(name, val, stmt_src_get_file(), 0, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_script_die_surface(const char *msg) {
    const char *m = msg ? msg : "Died";
    size_t mlen = strlen(m); if (mlen > 511) mlen = 511;
    memcpy(g_script_exception, m, mlen); g_script_exception[mlen] = '\0';
    if (g_script_try_depth > 0) return;
    fflush(NULL);
    fprintf(stderr, "%s\n", g_script_exception);
    exit(1);
}

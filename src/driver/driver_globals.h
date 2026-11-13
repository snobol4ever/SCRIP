#ifndef DRIVER_GLOBALS_H
#define DRIVER_GLOBALS_H
#include <stdio.h>
#include <stddef.h>
#include "ir/ct_vec.h"
#include "ir/descr.h"
#include "runtime/re.h"
extern char g_script_exception[512];
extern int g_script_try_depth;
void rt_script_die_surface(const char *msg);
extern Match g_match;
extern const char *g_subject;
typedef struct {
    FILE *fp;
    char *name;
    char *alias;
    char *enc;
    char mode;
    char type;
    char untrans;
    char bom;
    char repos;
    char closed;
    char eof;
    char rd_last;
    long pos_off;
    long pos_chars;
    long pos_lines;
    long pos_lpos;
    long rd_line;
    long rd_col;
    FILE *raw;
    char xk;
    char xin_n;
    char xout_n;
    unsigned int xin;
    unsigned int xout;
    int *xal;
    int nxal;
    int *mir;
    int nmir;
    char tsk;
    char buf;
    char cnt;
    char tty;
    long pos_pre;
    long in_fold;
    long in_fill;
    long in_head;
    long gpos_chars;
    long gpos_lines;
    long gpos_lpos;
    int lastc;
    int sink;
    unsigned int *mgr;
    int nmgr;
} fh_slot_t;
extern cv_t g_fhv;
void fh_ensure_init(void);
#define g_fh (fh_ensure_init(), (fh_slot_t *)g_fhv.p)
#define FH_N (fh_ensure_init(), (int)g_fhv.len)
void drv_gc_roots(void);
extern int fh_init;
int fh_alias_idx(const char *nm);
void fh_set_alias(int idx, const char *nm);
void fh_set_encoding(int idx, const char *nm);
const char *fh_encoding(int idx);
void fh_set_bom(int idx, int v);
int fh_transcode(int idx, int kind, const char *mode);
int fh_untranscode(int idx);
int fh_bom(int idx);
void fh_set_repos(int idx, int v);
int fh_repos(int idx);
void fh_set_eof(int idx, int v);
int fh_eof(int idx);
void fh_ensure_init(void);
int fh_alloc(FILE *fp);
int fh_find_fp(FILE *fp);
const char *fh_alias_name(int idx);
FILE *fh_get(int idx);
void fh_free(int idx);
int fh_current_input(void);
int fh_current_output(void);
void fh_set_input(int idx);
void fh_set_output(int idx);
FILE *fh_cur_out_fp(void);
FILE *fh_cur_in_fp(void);
int fh_capture_begin(char **bufp, size_t *szp, int *saved_out);
void fh_capture_end(int idx, int saved_out);
void set_and_trace(const char *name, DESCR_t val);
#endif

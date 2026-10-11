#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "driver_private.h"
char g_script_exception[512] = "";
int g_script_try_depth = 0;
Match g_match;
const char *g_subject = "";
cv_t g_fhv;
int fh_init = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fh_ensure_init(void) {
    if (fh_init) return;
    fh_init=1;
    cv_reserve(&g_fhv, (uint32_t)sizeof(fh_slot_t), 8, "g_fh");
    g_fhv.len = 3;
    memset(g_fhv.p, 0, 3 * sizeof(fh_slot_t));
    {
        fh_slot_t *h = (fh_slot_t *)g_fhv.p;
        h[0].fp=stdin;
        h[1].fp=stdout;
        h[2].fp=stderr;
        h[0].name="&input";
        h[1].name="&output";
        h[2].name="&errout";
        h[0].mode='r';
        h[1].mode='w';
        h[2].mode='w';
        h[0].type='t';
        h[1].type='t';
        h[2].type='t';
        for(int i=1;i<3;i++){ off_t o=lseek(i,0,SEEK_CUR); h[i].pos_off = o>0 ? (long)o : 0; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fh_is_closed(int idx){ fh_ensure_init(); return (idx>=0 && idx<FH_N) ? g_fh[idx].closed : 0; }
int fh_is_untranslated(int idx){ fh_ensure_init(); return (idx>=0 && idx<FH_N) ? g_fh[idx].untrans : 0; }
void fh_set_untranslated(int idx, int v){ fh_ensure_init(); if(idx>=0 && idx<FH_N) g_fh[idx].untrans=(char)(v?1:0); }
int fh_alias_idx(const char *nm){ fh_ensure_init(); if(!nm) return -1; for(int i=3;i<FH_N;i++) if(g_fh[i].fp&&g_fh[i].alias&&!strcmp(g_fh[i].alias,nm)) return i; return -1; }
void fh_set_alias(int idx, const char *nm){ extern char *rt_heap_strdup_c(const char *); fh_ensure_init(); if(idx>=3&&idx<FH_N) g_fh[idx].alias = nm ? rt_heap_strdup_c(nm) : (char *)0; }
void fh_set_encoding(int idx, const char *nm){ extern char *rt_heap_strdup_c(const char *); fh_ensure_init(); if(idx>=0&&idx<FH_N) g_fh[idx].enc = nm ? rt_heap_strdup_c(nm) : (char *)0; }
const char *fh_encoding(int idx){
    fh_ensure_init();
    if(idx<0||idx>=FH_N) return (const char *)0;
    if(g_fh[idx].type=='b'||g_fh[idx].untrans) return "octet";
    return g_fh[idx].enc ? g_fh[idx].enc : "UTF-8";
}
void fh_set_bom(int idx, int v){ fh_ensure_init(); if(idx>=0&&idx<FH_N) g_fh[idx].bom=(char)(v?1:0); }
int fh_bom(int idx){ fh_ensure_init(); if(idx<0||idx>=FH_N) return 0; return g_fh[idx].bom?1:0; }
void fh_set_repos(int idx, int v){ fh_ensure_init(); if(idx>=0&&idx<FH_N) g_fh[idx].repos=(char)(v?1:0); }
int fh_repos(int idx){ fh_ensure_init(); if(idx<0||idx>=FH_N) return 0; return g_fh[idx].repos?1:0; }
void fh_set_eof(int idx, int v){ fh_ensure_init(); if(idx>=0&&idx<FH_N) g_fh[idx].eof=(char)v; }
int fh_eof(int idx){ fh_ensure_init(); if(idx<0||idx>=FH_N) return 0; return g_fh[idx].eof; }
static void fh_pos_reset(int idx){ g_fh[idx].pos_off=0; g_fh[idx].pos_chars=0; g_fh[idx].pos_lines=0; g_fh[idx].pos_lpos=0; g_fh[idx].rd_line=0; g_fh[idx].rd_col=0; g_fh[idx].rd_last=0; }
static int fh_pos_scan(fh_slot_t *s, int fd, long off) {
    if(off<s->pos_off){ s->pos_off=0; s->pos_chars=0; s->pos_lines=0; s->pos_lpos=0; }
    while(s->pos_off<off){
        size_t want=(size_t)(off-s->pos_off);
        if(want>65536) want=65536;
        char buf[want];
        ssize_t got=pread(fd,buf,want,(off_t)s->pos_off);
        if(got<=0) return -2;
        for(ssize_t i=0;i<got;i++){ unsigned char c=(unsigned char)buf[i]; if((c&0xC0)==0x80) continue; s->pos_chars++; if(c=='\n'){ s->pos_lines++; s->pos_lpos=0; } else s->pos_lpos++; }
        s->pos_off+=got;
    }
    return 1;
}
static int fh_out_position(fh_slot_t *s) {
    struct stat st;
    int fd, rfd, r, pn;
    off_t off;
    fflush(s->fp);
    fd=fileno(s->fp);
    if(fd<0||fstat(fd,&st)!=0||!S_ISREG(st.st_mode)) return -1;
    off=lseek(fd,0,SEEK_CUR);
    if(off<0) return -1;
    pn=snprintf((char *)0,0,"/proc/self/fd/%d",fd)+1;
    char pth[pn];
    snprintf(pth,(size_t)pn,"/proc/self/fd/%d",fd);
    rfd=open(pth,O_RDONLY);
    if(rfd<0) return -1;
    r=fh_pos_scan(s,rfd,(long)off);
    close(rfd);
    return r;
}
int fh_in_position(int idx, long *chars, long *lines, long *lpos) {
    fh_ensure_init();
    if(idx<0||idx>=FH_N||!g_fh[idx].fp) return 0;
    fh_slot_t *s=&g_fh[idx];
    int r;
    if(idx!=0&&s->mode!='r') r=fh_out_position(s);
    else { long off=ftell(s->fp); if(off<0) return -2; r=fh_pos_scan(s,fileno(s->fp),off); }
    if(r!=1) return r;
    *chars=s->pos_chars;
    *lines=s->pos_lines;
    *lpos=s->pos_lpos;
    return 1;
}
void fh_note_read_start(int idx) {
    long c=0,l=0,p=0;
    int ok=fh_in_position(idx,&c,&l,&p);
    for(int i=0;i<FH_N;i++) g_fh[i].rd_last=0;
    if(idx<0||idx>=FH_N) return;
    g_fh[idx].rd_line = ok==1 ? l+1 : 0;
    g_fh[idx].rd_col = ok==1 ? p : 0;
    g_fh[idx].rd_last=1;
}
int fh_last_read_start(long *line, long *col) { fh_ensure_init(); for(int i=0;i<FH_N;i++) if(g_fh[i].rd_last){ *line=g_fh[i].rd_line; *col=g_fh[i].rd_col; return 1; } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fh_slot_init(fh_slot_t *s, FILE *fp) {
    s->fp=fp;
    s->name=NULL;
    s->alias=NULL;
    s->enc=NULL;
    s->mode=0;
    s->type='t';
    s->untrans=0;
    s->bom=0;
    s->repos=1;
    s->closed=0;
    s->eof=0;
    s->rd_last=0;
    s->pos_off=0;
    s->pos_chars=0;
    s->pos_lines=0;
    s->pos_lpos=0;
    s->rd_line=0;
    s->rd_col=0;
    s->raw=NULL;
    s->xk=0;
    s->xin_n=0;
    s->xout_n=0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fh_alloc(FILE *fp) {
    fh_ensure_init();
    for(int i=3;i<FH_N;i++) if(!g_fh[i].fp){ fh_slot_init(&g_fh[i], fp); return i; }
    { int i=FH_N; (void)CV_PUSH(g_fhv, fh_slot_t); fh_slot_init(&g_fh[i], fp); return i; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
FILE *fh_get(int idx){ fh_ensure_init(); if(idx<0||idx>=FH_N) return NULL; return g_fh[idx].fp; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fh_free(int idx){
    if(fh_init&&idx>=3&&idx<FH_N) fh_pos_reset(idx);
    if(fh_init&&idx>=3&&idx<FH_N){ g_fh[idx].fp=NULL; g_fh[idx].alias=NULL; g_fh[idx].enc=NULL; g_fh[idx].bom=0; g_fh[idx].repos=1; g_fh[idx].eof=0; }
}
int fh_cur_in = 0;
int fh_cur_out = 1;
int fh_cur_init = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fh_cur_ensure(void){ if(fh_cur_init) return; fh_ensure_init(); fh_cur_in=0; fh_cur_out=1; fh_cur_init=1; }
int fh_current_input(void){ fh_cur_ensure(); return fh_cur_in; }
int fh_current_output(void){ fh_cur_ensure(); return fh_cur_out; }
void fh_set_input(int idx){ fh_cur_ensure(); if(idx>=0&&idx<FH_N&&g_fh[idx].fp) fh_cur_in=idx; }
void fh_set_output(int idx){ fh_cur_ensure(); if(idx>=0&&idx<FH_N&&g_fh[idx].fp) fh_cur_out=idx; }
FILE *fh_cur_out_fp(void){ fh_cur_ensure(); FILE *fp=fh_get(fh_cur_out); return fp?fp:stdout; }
FILE *fh_cur_in_fp(void){ fh_cur_ensure(); FILE *fp=fh_get(fh_cur_in); return fp?fp:stdin; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fh_x_utf8(unsigned long c, unsigned char *o){
    if(c<0x80){ o[0]=(unsigned char)c; return 1; }
    if(c<0x800){ o[0]=(unsigned char)(0xC0|(c>>6)); o[1]=(unsigned char)(0x80|(c&0x3F)); return 2; }
    if(c<0x10000){ o[0]=(unsigned char)(0xE0|(c>>12)); o[1]=(unsigned char)(0x80|((c>>6)&0x3F)); o[2]=(unsigned char)(0x80|(c&0x3F)); return 3; }
    o[0]=(unsigned char)(0xF0|(c>>18));
    o[1]=(unsigned char)(0x80|((c>>12)&0x3F));
    o[2]=(unsigned char)(0x80|((c>>6)&0x3F));
    o[3]=(unsigned char)(0x80|(c&0x3F));
    return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fh_x_unit(FILE *r, int k, unsigned long *u){
    unsigned char b[4];
    int w=k<=2?2:4;
    if(fread(b,1,(size_t)w,r)!=(size_t)w) return 0;
    if(w==2) *u=k==1?((unsigned long)b[0]<<8|b[1]):((unsigned long)b[1]<<8|b[0]);
    else *u=k==3?((unsigned long)b[0]<<24|(unsigned long)b[1]<<16|(unsigned long)b[2]<<8|b[3]):((unsigned long)b[3]<<24|(unsigned long)b[2]<<16|(unsigned long)b[1]<<8|b[0]);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ssize_t fh_x_read(void *c, char *buf, size_t n){
    long i=(long)(intptr_t)c;
    size_t o=0;
    while(o<n){
        fh_slot_t *s=&g_fh[i];
        unsigned long u, v;
        if(s->xout_n){ int m=0; while(m<s->xout_n&&o<n) buf[o++]=(char)s->xout[m++]; memmove(s->xout,s->xout+m,(size_t)(s->xout_n-m)); s->xout_n=(char)(s->xout_n-m); continue; }
        if(!fh_x_unit(s->raw,s->xk,&u)) break;
        if(s->xk<=2&&u>=0xD800&&u<=0xDBFF&&fh_x_unit(s->raw,s->xk,&v)&&v>=0xDC00&&v<=0xDFFF) u=0x10000+((u-0xD800)<<10)+(v-0xDC00);
        s->xout_n=(char)fh_x_utf8(u,s->xout);
    }
    return (ssize_t)o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fh_x_put(FILE *r, int k, unsigned long u){
    unsigned char b[4];
    if(k<=2&&u>=0x10000){ fh_x_put(r,k,0xD800+((u-0x10000)>>10)); fh_x_put(r,k,0xDC00+((u-0x10000)&0x3FF)); return; }
    if(k==1){ b[0]=(unsigned char)(u>>8); b[1]=(unsigned char)u; fwrite(b,1,2,r); return; }
    if(k==2){ b[0]=(unsigned char)u; b[1]=(unsigned char)(u>>8); fwrite(b,1,2,r); return; }
    if(k==3){ b[0]=(unsigned char)(u>>24); b[1]=(unsigned char)(u>>16); b[2]=(unsigned char)(u>>8); b[3]=(unsigned char)u; fwrite(b,1,4,r); return; }
    b[0]=(unsigned char)u;
    b[1]=(unsigned char)(u>>8);
    b[2]=(unsigned char)(u>>16);
    b[3]=(unsigned char)(u>>24);
    fwrite(b,1,4,r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ssize_t fh_x_write(void *c, const char *p, size_t n){
    long i=(long)(intptr_t)c;
    fh_slot_t *s=&g_fh[i];
    for(size_t k=0;k<n;k++){
        unsigned char b0;
        int need;
        unsigned long u;
        s->xin[(int)s->xin_n]=(unsigned char)p[k];
        s->xin_n++;
        b0=s->xin[0];
        need=b0<0x80?1:(b0&0xE0)==0xC0?2:(b0&0xF0)==0xE0?3:(b0&0xF8)==0xF0?4:1;
        if(s->xin_n<need) continue;
        u=need==1?b0:need==2?(unsigned long)(b0&0x1F):need==3?(unsigned long)(b0&0x0F):(unsigned long)(b0&0x07);
        for(int m=1;m<need;m++) u=(u<<6)|(unsigned long)(s->xin[m]&0x3F);
        s->xin_n=0;
        fh_x_put(s->raw,s->xk,u);
    }
    return (ssize_t)n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fh_x_close(void *c){ long i=(long)(intptr_t)c; FILE *r=g_fh[i].raw; g_fh[i].raw=NULL; g_fh[i].xk=0; return r?fclose(r):0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fh_transcode(int idx, int kind, const char *mode){
    cookie_io_functions_t io={ fh_x_read, fh_x_write, NULL, fh_x_close };
    FILE *w;
    fh_ensure_init();
    if(idx<3||idx>=FH_N||!g_fh[idx].fp||kind<1||kind>4) return 0;
    w=fopencookie((void *)(intptr_t)idx,mode,io);
    if(!w) return 0;
    g_fh[idx].raw=g_fh[idx].fp;
    g_fh[idx].fp=w;
    g_fh[idx].xk=(char)kind;
    g_fh[idx].xin_n=0;
    g_fh[idx].xout_n=0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *g_fh_sink_v = NULL;
long g_fh_sink_cap = 0;
static DESCR_t *fh_sink_at(long i){ return g_fh_sink_v + i * 6; }
static void fh_sink_pub(long i){ DESCR_t *s=fh_sink_at(i); char **bp=(char **)(intptr_t)s[3].i; size_t *sp=(size_t *)(intptr_t)s[4].i; if(bp) *bp=s[0].s; if(sp) *sp=(size_t)s[1].i; }
static void fh_sink_drop(long i){ DESCR_t *s=fh_sink_at(i); for(int k=0;k<6;k++) s[k]=INTVAL(0); }
static ssize_t fh_sink_write(void *c, const char *p, size_t n){
    extern void *rt_wsb_realloc(void *, size_t);
    long i=(long)(intptr_t)c;
    DESCR_t *s=fh_sink_at(i);
    size_t len=(size_t)s[1].i, cap=(size_t)s[2].i;
    char *b=s[0].s;
    if(len+n+1>cap){ size_t nc=cap?cap:64; while(nc<len+n+1) nc*=2; b=(char *)rt_wsb_realloc(b,nc); s=fh_sink_at(i); s[0].s=b; s[2]=INTVAL((int64_t)nc); }
    memcpy(b+len,p,n);
    len+=n;
    b[len]=0;
    s[0].slen=(uint32_t)len;
    s[1]=INTVAL((int64_t)len);
    fh_sink_pub(i);
    return (ssize_t)n;
}
static int fh_sink_close(void *c){ long i=(long)(intptr_t)c; fh_sink_pub(i); if(fh_sink_at(i)[3].i) fh_sink_drop(i); return 0; }
static FILE *fh_sink_open(char **bufp, size_t *sizep, int *sip){
    extern void *rt_wsb_alloc(size_t);
    extern void *rt_ws_alloc_descr(size_t);
    cookie_io_functions_t io={ NULL, fh_sink_write, NULL, fh_sink_close };
    long i=0;
    FILE *fp;
    DESCR_t *s;
    char *b;
    while(i<g_fh_sink_cap&&fh_sink_at(i)[5].i) i++;
    if(i>=g_fh_sink_cap){
        long nc=g_fh_sink_cap?g_fh_sink_cap*2:8;
        DESCR_t *nv=(DESCR_t *)rt_ws_alloc_descr((size_t)nc*6);
        for(long k=0;k<nc*6;k++) nv[k]=INTVAL(0);
        if(g_fh_sink_cap) memcpy(nv,g_fh_sink_v,(size_t)g_fh_sink_cap*6*sizeof(DESCR_t));
        g_fh_sink_v=nv;
        g_fh_sink_cap=nc;
    }
    b=(char *)rt_wsb_alloc(64);
    b[0]=0;
    s=fh_sink_at(i);
    s[0]=INTVAL(0);
    s[0].v=DT_S;
    s[0].s=b;
    s[1]=INTVAL(0);
    s[2]=INTVAL(64);
    s[3]=INTVAL((int64_t)(intptr_t)bufp);
    s[4]=INTVAL((int64_t)(intptr_t)sizep);
    s[5]=INTVAL(1);
    fh_sink_pub(i);
    fp=fopencookie((void *)(intptr_t)i,"w",io);
    if(!fp){ fh_sink_drop(i); return NULL; }
    if(sip) *sip=(int)i;
    return fp;
}
FILE *fh_memsink_open(char **bufp, size_t *sizep){ return fh_sink_open(bufp,sizep,NULL); }
FILE *fh_memsink_open_slot(int *sip){ return fh_sink_open(NULL,NULL,sip); }
char *fh_memsink_take(int si, size_t *lenp){
    char *b;
    if(si<0||si>=g_fh_sink_cap||!fh_sink_at(si)[5].i){ if(lenp) *lenp=0; return NULL; }
    b=fh_sink_at(si)[0].s;
    if(lenp) *lenp=(size_t)fh_sink_at(si)[1].i;
    fh_sink_drop(si);
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fh_capture_begin(char **bufp, size_t *szp, int *saved_out){
    fh_cur_ensure();
    FILE *ms=fh_memsink_open(bufp,szp);
    if(!ms) return -1;
    int idx=fh_alloc(ms);
    if(idx<0){ fclose(ms); return -1; }
    *saved_out=fh_current_output();
    fh_set_output(idx);
    return idx;
}
void fh_capture_end(int idx, int saved_out){ fh_cur_ensure(); FILE *ms=fh_get(idx); if(ms){ fflush(ms); fclose(ms); } fh_free(idx); fh_set_output(saved_out); }
extern const char *Σ;
extern int Ω;
extern int Δ;
extern int Σlen;
int g_polyglot = 0;
int g_opt_dump_bb = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_and_trace(const char *name, DESCR_t val) { NV_SET_fn(name, val); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_script_die_surface(const char *msg) {
    const char *m = msg ? msg : "Died";
    size_t mlen = strlen(m);
    if (mlen > 511) mlen = 511;
    memcpy(g_script_exception, m, mlen);
    g_script_exception[mlen] = '\0';
    if (g_script_try_depth > 0) return;
    fflush(NULL);
    fprintf(stderr, "%s\n", g_script_exception);
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void drv_gc_roots(void) {
    extern void rt_gc_visit_raw(const char **loc);
    for (int i = 0; i < FH_N; i++) {
        if (g_fh[i].name) rt_gc_visit_raw((const char **)&g_fh[i].name);
        if (g_fh[i].alias) rt_gc_visit_raw((const char **)&g_fh[i].alias);
        if (g_fh[i].enc) rt_gc_visit_raw((const char **)&g_fh[i].enc);
    }
    if (g_fh_sink_v) rt_gc_visit_raw((const char **)&g_fh_sink_v);
}

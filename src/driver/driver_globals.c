#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio_ext.h>
#include "driver_private.h"
char g_script_exception[512] = "";
int g_script_try_depth = 0;
Match g_match;
const char *g_subject = "";
cv_t g_fhv;
int fh_init = 0;
extern FILE _IO_2_1_stdout_;
static int fh_sink_position(fh_slot_t *s, long *chars, long *lines, long *lpos);
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
        if(stdout!=&_IO_2_1_stdout_){ long o=ftell(stdin); h[1].cnt=1; h[2].cnt=1; h[1].pos_off=o>0?o:0; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fh_is_closed(int idx){ fh_ensure_init(); return (idx>=0 && idx<FH_N) ? g_fh[idx].closed : 0; }
int fh_is_untranslated(int idx){ fh_ensure_init(); return (idx>=0 && idx<FH_N) ? g_fh[idx].untrans : 0; }
void fh_set_untranslated(int idx, int v){ fh_ensure_init(); if(idx>=0 && idx<FH_N) g_fh[idx].untrans=(char)(v?1:0); }
int fh_alias_idx(const char *nm){
    extern const char *prolog_atom_name(int);
    fh_ensure_init();
    if(!nm) return -1;
    for(int i=3;i<FH_N;i++) if(g_fh[i].fp&&g_fh[i].alias&&!strcmp(g_fh[i].alias,nm)) return i;
    for(int i=3;i<FH_N;i++) for(int k=0;g_fh[i].fp&&k<g_fh[i].nxal;k++) if(!strcmp(prolog_atom_name(g_fh[i].xal[k]),nm)) return i;
    return -1;
}
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
static void fh_pos_fold(fh_slot_t *gs, fh_slot_t *ws, const char *p, size_t n){
    const char *q=p, *e=p+n, *last=(const char *)0;
    size_t k=0, cont=0;
    uint64_t hi=0;
    long nl=0;
    while(q<e&&(q=memchr(q,'\n',(size_t)(e-q)))!=(const char *)0){ nl++; last=q; q++; }
    if(gs){ gs->gpos_chars+=(long)n; gs->gpos_lines+=nl; gs->gpos_lpos=last?(long)(e-last-1):gs->gpos_lpos+(long)n; }
    if(!ws) return;
    for(;k+32<=n;k+=32){ uint64_t x[4]; memcpy(x,p+k,32); hi|=x[0]|x[1]|x[2]|x[3]; }
    for(;k+8<=n;k+=8){ uint64_t x; memcpy(&x,p+k,8); hi|=x; }
    for(;k<n;k++) hi|=(unsigned char)p[k];
    if(hi&0x8080808080808080ull) for(k=0;k<n;k++) if(((unsigned char)p[k]&0xC0)==0x80) cont++;
    ws->pos_chars+=(long)(n-cont);
    ws->pos_lines+=nl;
    if(last) ws->pos_lpos=0;
    for(q=last?last+1:p;q<e;q++){
        unsigned char c=(unsigned char)*q;
        if((c&0xC0)==0x80) continue;
        if(c=='\r') ws->pos_lpos=0;
        else if(c=='\b'){ if(ws->pos_lpos>0) ws->pos_lpos--; } else if(c=='\t') ws->pos_lpos=(ws->pos_lpos|7)+1;
        else ws->pos_lpos++;
    }
}
static int fh_last_cp(const char *p, size_t n){
    size_t k=n;
    unsigned cp;
    int need;
    while(k>0&&n-k<3&&((unsigned char)p[k-1]&0xC0)==0x80) k--;
    if(!k--) return 0x80;
    cp=(unsigned char)p[k];
    need=cp<0x80?1:(cp&0xE0)==0xC0?2:(cp&0xF0)==0xE0?3:(cp&0xF8)==0xF0?4:1;
    if(k+(size_t)need!=n) return 0x80;
    if(need>1) cp&=need==2?0x1Fu:need==3?0x0Fu:0x07u;
    for(size_t j=k+1;j<n;j++) cp=(cp<<6)|((unsigned char)p[j]&0x3Fu);
    return (int)cp;
}
static void fh_std_count(fh_slot_t *h, int i, const char *p, size_t n){ fh_pos_fold(&h[i],&h[1],p,n); if(n) h[i].lastc=fh_last_cp(p,n); }
static void fh_std_in_fold_pipe(fh_slot_t *h, FILE *in){
    struct stat st;
    long cp, fill;
    if(fstat(fileno(in),&st)||!(S_ISFIFO(st.st_mode)||S_ISSOCK(st.st_mode))){ h[1].cnt=2; return; }
    cp=(long)(in->_IO_read_ptr-in->_IO_read_base);
    fill=(long)(in->_IO_read_end-in->_IO_read_base);
    if(fill!=h[0].in_fill||cp<h[0].in_fold){ h[0].in_fill=fill; h[0].in_fold=0; }
    if(cp>h[0].in_fold){ fh_pos_fold((fh_slot_t *)0,&h[1],in->_IO_read_base+h[0].in_fold,(size_t)(cp-h[0].in_fold)); h[0].in_fold=cp; }
}
static void fh_std_in_fold(fh_slot_t *h){
    FILE *in=h[0].fp;
    long off;
    if(h[1].cnt!=1||!in||!in->_IO_read_base) return;
    off=ftell(in);
    if(off<0){ fh_std_in_fold_pipe(h,in); return; }
    if(off<h[1].pos_off){ h[1].pos_off=off; return; }
    while(h[1].pos_off<off){
        size_t want=(size_t)(off-h[1].pos_off);
        if(want>65536) want=65536;
        char buf[want];
        ssize_t got=pread(fileno(in),buf,want,(off_t)h[1].pos_off);
        if(got<=0){ h[1].pos_off=off; return; }
        fh_pos_fold((fh_slot_t *)0,&h[1],buf,(size_t)got);
        h[1].pos_off+=got;
    }
}
static void fh_std_prefold(fh_slot_t *h, int i){
    FILE *fp=h[i].fp;
    long pend;
    if(!h[i].cnt||!fp||fp!=(i==1?stdout:stderr)) return;
    pend=(long)__fpending(fp);
    if(pend<h[i].pos_pre) h[i].pos_pre=pend;
    if(pend>h[i].pos_pre){ fh_std_count(h,i,fp->_IO_write_base+h[i].pos_pre,(size_t)(pend-h[i].pos_pre)); h[i].pos_pre=pend; }
}
static ssize_t fh_std_write(void *c, const char *p, size_t n){
    int i=(int)(intptr_t)c;
    size_t o=0;
    if(fh_init){
        fh_slot_t *h=(fh_slot_t *)g_fhv.p;
        size_t sk=h[i].pos_pre<=0?0:(size_t)h[i].pos_pre<n?(size_t)h[i].pos_pre:n;
        h[i].pos_pre-=(long)sk;
        if(sk<n){ fh_std_in_fold(h); fh_std_count(h,i,p+sk,n-sk); }
    }
    while(o<n){ ssize_t w=write(i,p+o,n-o); if(w<0&&errno==EINTR) continue; if(w<=0) return o?(ssize_t)o:-1; o+=(size_t)w; }
    return (ssize_t)n;
}
static int fh_std_seek(void *c, off64_t *pos, int whence){ off_t r=lseek((int)(intptr_t)c,(off_t)*pos,whence); if(r<0) return -1; *pos=(off64_t)r; return 0; }
static int fh_std_close(void *c){ return close((int)(intptr_t)c); }
__attribute__((constructor)) static void fh_std_ctor(void){
    cookie_io_functions_t io={ NULL, fh_std_write, fh_std_seek, fh_std_close };
    for(int i=1;i<3;i++){
        FILE *f=i==1?stdout:stderr, *c;
        struct stat st;
        int lb=__flbf(f), mode;
        size_t sz=BUFSIZ;
        if(fstat(i,&st)==0){ if(S_ISCHR(st.st_mode)&&isatty(i)) lb=1; if(st.st_blksize>0&&st.st_blksize<BUFSIZ) sz=(size_t)st.st_blksize; }
        mode=i==2?_IONBF:lb?_IOLBF:_IOFBF;
        fflush(f);
        c=fopencookie((void *)(intptr_t)i,"w",io);
        if(!c) return;
        c->_fileno=i;
        setvbuf(c,NULL,mode,sz);
        if(i==1) stdout=c;
        else stderr=c;
    }
}
static int fh_std_position(int idx, long *chars, long *lines, long *lpos){
    extern int rt_pl_dialect_gnu(void);
    fh_slot_t *h=(fh_slot_t *)g_fhv.p;
    fh_std_in_fold(h);
    for(int i=1;i<3;i++) fh_std_prefold(h,i);
    if(rt_pl_dialect_gnu()){ *chars=h[idx].gpos_chars; *lines=h[idx].gpos_lines; *lpos=h[idx].gpos_lpos; return 1; }
    *chars=h[1].pos_chars;
    *lines=h[1].pos_lines;
    *lpos=h[1].pos_lpos;
    return 1;
}
int fh_in_position(int idx, long *chars, long *lines, long *lpos) {
    fh_ensure_init();
    if(idx<0||idx>=FH_N||!g_fh[idx].fp) return 0;
    if((idx==1||idx==2)&&g_fh[idx].cnt) return fh_std_position(idx,chars,lines,lpos);
    if(g_fh[idx].sink) return fh_sink_position(&g_fh[idx],chars,lines,lpos);
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
int fh_last_char(int idx) {
    fh_slot_t *h;
    fh_ensure_init();
    h=(fh_slot_t *)g_fhv.p;
    if(idx>=0&&idx<FH_N&&h[idx].sink){ long c, l, p; return fh_sink_position(&h[idx],&c,&l,&p)==1&&h[idx].lastc>0?h[idx].lastc:-1; }
    if((idx!=1&&idx!=2)||!h[idx].cnt) return -1;
    fh_std_in_fold(h);
    fh_std_prefold(h,idx);
    return h[idx].lastc>0?h[idx].lastc:-1;
}
int fh_query_position(int idx, long *chars, long *lines, long *lpos) {
    extern int rt_pl_dialect_gnu(void);
    fh_ensure_init();
    if(idx==0&&g_fh[1].cnt&&!rt_pl_dialect_gnu()) return fh_std_position(0,chars,lines,lpos);
    return fh_in_position(idx,chars,lines,lpos);
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
    s->xal=NULL;
    s->nxal=0;
    s->mir=NULL;
    s->nmir=0;
    s->tsk=0;
    s->buf=0;
    s->cnt=0;
    s->pos_pre=0;
    s->gpos_chars=0;
    s->gpos_lines=0;
    s->gpos_lpos=0;
    s->lastc=0;
    s->sink=0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fh_alloc(FILE *fp) {
    fh_ensure_init();
    for(int i=3;i<FH_N;i++) if(!g_fh[i].fp){ fh_slot_init(&g_fh[i], fp); return i; }
    { int i=FH_N; (void)CV_PUSH(g_fhv, fh_slot_t); fh_slot_init(&g_fh[i], fp); return i; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *fh_alias_name(int idx){
    fh_ensure_init();
    if(idx==0) return "user_input";
    if(idx==1) return "user_output";
    if(idx==2) return "user_error";
    return (idx>=3&&idx<FH_N) ? g_fh[idx].alias : (const char *)0;
}
int fh_find_fp(FILE *fp){ fh_ensure_init(); for(int i=0;i<FH_N;i++) if(g_fh[i].fp==fp) return i; return -1; }
FILE *fh_get(int idx){ fh_ensure_init(); if(idx<0||idx>=FH_N) return NULL; return g_fh[idx].fp; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void fh_free(int idx){
    if(fh_init&&idx>=3&&idx<FH_N) fh_pos_reset(idx);
    if(fh_init&&idx>=3&&idx<FH_N){ g_fh[idx].fp=NULL; g_fh[idx].alias=NULL; g_fh[idx].enc=NULL; g_fh[idx].bom=0; g_fh[idx].repos=1; g_fh[idx].eof=0; }
    if(fh_init&&idx>=3&&idx<FH_N){ g_fh[idx].xal=NULL; g_fh[idx].nxal=0; g_fh[idx].mir=NULL; g_fh[idx].nmir=0; g_fh[idx].tsk=0; g_fh[idx].buf=0; g_fh[idx].sink=0; }
    for(int i=0;fh_init&&i<FH_N;i++){ int w=0; for(int k=0;k<g_fh[i].nmir;k++) if(g_fh[i].mir[k]!=idx) g_fh[i].mir[w++]=g_fh[i].mir[k]; g_fh[i].nmir=w; }
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
static int fh_x_utf8(unsigned long c, unsigned int *o){
    if(c<0x80){ *o=(unsigned int)c; return 1; }
    if(c<0x800){ *o=(0xC0|(unsigned int)(c>>6))|(0x80|(unsigned int)(c&0x3F))<<8; return 2; }
    if(c<0x10000){ *o=(0xE0|(unsigned int)(c>>12))|(0x80|(unsigned int)((c>>6)&0x3F))<<8|(0x80|(unsigned int)(c&0x3F))<<16; return 3; }
    *o=(0xF0|(unsigned int)(c>>18))|(0x80|(unsigned int)((c>>12)&0x3F))<<8|(0x80|(unsigned int)((c>>6)&0x3F))<<16|(0x80|(unsigned int)(c&0x3F))<<24;
    return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fh_x_unit(FILE *r, int k, unsigned long *u){
    unsigned long x=0;
    int w=k==5?1:k<=2?2:4;
    for(int m=0;m<w;m++){ int ch=fgetc(r); if(ch==EOF) return 0; x|=(unsigned long)(unsigned char)ch<<(8*m); }
    if(w==1) *u=x;
    else if(w==2) *u=k==1?((x&0xFF)<<8|x>>8):x;
    else *u=k==3?((x&0xFF)<<24|((x>>8)&0xFF)<<16|((x>>16)&0xFF)<<8|x>>24):x;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ssize_t fh_x_read(void *c, char *buf, size_t n){
    long i=(long)(intptr_t)c;
    size_t o=0;
    while(o<n){
        fh_slot_t *s=&g_fh[i];
        unsigned long u, v;
        if(s->xout_n){ while(s->xout_n&&o<n){ buf[o++]=(char)(s->xout&0xFF); s->xout>>=8; s->xout_n--; } continue; }
        if(!s->xk) return (ssize_t)(o+fread(buf+o,1,n-o,s->raw));
        if(!fh_x_unit(s->raw,s->xk,&u)) break;
        if(s->xk<=2&&u>=0xD800&&u<=0xDBFF&&fh_x_unit(s->raw,s->xk,&v)&&v>=0xDC00&&v<=0xDFFF) u=0x10000+((u-0xD800)<<10)+(v-0xDC00);
        s->xout_n=(char)fh_x_utf8(u,&s->xout);
    }
    return (ssize_t)o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fh_x_put(FILE *r, int k, unsigned long u){
    unsigned char b[4];
    if(k==5){ b[0]=(unsigned char)(u<256?u:'?'); fwrite(b,1,1,r); return; }
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
    if(!s->xk) return (ssize_t)fwrite(p,1,n,s->raw);
    for(size_t k=0;k<n;k++){
        unsigned char b0;
        int need;
        unsigned long u;
        if(!s->xin_n) s->xin=0;
        s->xin|=(unsigned int)(unsigned char)p[k]<<(8*s->xin_n);
        s->xin_n++;
        b0=(unsigned char)(s->xin&0xFF);
        need=b0<0x80?1:(b0&0xE0)==0xC0?2:(b0&0xF0)==0xE0?3:(b0&0xF8)==0xF0?4:1;
        if(s->xin_n<need) continue;
        u=need==1?b0:need==2?(unsigned long)(b0&0x1F):need==3?(unsigned long)(b0&0x0F):(unsigned long)(b0&0x07);
        for(int m=1;m<need;m++) u=(u<<6)|(unsigned long)((s->xin>>(8*m))&0x3F);
        s->xin_n=0;
        fh_x_put(s->raw,s->xk,u);
    }
    return (ssize_t)n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fh_x_close(void *c){ long i=(long)(intptr_t)c; FILE *r=g_fh[i].raw; g_fh[i].raw=NULL; g_fh[i].xk=0; return r?fclose(r):0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fh_untranscode(int idx){
    fh_ensure_init();
    if(idx<3||idx>=FH_N||!g_fh[idx].raw) return 0;
    if(g_fh[idx].mode!='r') fflush(g_fh[idx].fp);
    g_fh[idx].xk=0;
    g_fh[idx].xin_n=0;
    g_fh[idx].xout_n=0;
    return 1;
}
int fh_transcode(int idx, int kind, const char *mode){
    cookie_io_functions_t io={ fh_x_read, fh_x_write, NULL, fh_x_close };
    FILE *w;
    fh_ensure_init();
    if(idx<3||idx>=FH_N||!g_fh[idx].fp||kind<1||kind>5) return 0;
    if(g_fh[idx].raw){ if(g_fh[idx].mode!='r') fflush(g_fh[idx].fp); g_fh[idx].xk=(char)kind; g_fh[idx].xin_n=0; g_fh[idx].xout_n=0; return 1; }
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
static int fh_sink_position(fh_slot_t *s, long *chars, long *lines, long *lpos){
    extern int rt_pl_dialect_gnu(void);
    long si=(long)s->sink-1, len;
    DESCR_t *k;
    if(s->fp) fflush(s->fp);
    if(si<0||si>=g_fh_sink_cap||!fh_sink_at(si)[5].i) return -1;
    k=fh_sink_at(si);
    len=(long)k[1].i;
    if(len<s->pos_off){ s->pos_off=0; s->pos_chars=s->pos_lines=s->pos_lpos=0; s->gpos_chars=s->gpos_lines=s->gpos_lpos=0; }
    if(len>s->pos_off){ fh_pos_fold(s,s,k[0].s+s->pos_off,(size_t)(len-s->pos_off)); s->lastc=fh_last_cp(k[0].s,(size_t)len); s->pos_off=len; }
    if(rt_pl_dialect_gnu()){ *chars=s->gpos_chars; *lines=s->gpos_lines; *lpos=s->gpos_lpos; return 1; }
    *chars=s->pos_chars;
    *lines=s->pos_lines;
    *lpos=s->pos_lpos;
    return 1;
}
void fh_set_sink(int idx, int si){ fh_ensure_init(); if(idx>=3&&idx<FH_N&&si>=0) g_fh[idx].sink=si+1; }
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
    int si=-1;
    FILE *ms=fh_sink_open(bufp,szp,&si);
    if(!ms) return -1;
    int idx=fh_alloc(ms);
    if(idx<0){ fclose(ms); return -1; }
    fh_set_sink(idx,si);
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
    extern void rt_gc_run_ptr_fields(const void *, long, long, long, const long *);
    static const long off[5] = { (long) __builtin_offsetof(fh_slot_t, name), (long) __builtin_offsetof(fh_slot_t, alias), (long) __builtin_offsetof(fh_slot_t, enc),
        (long) __builtin_offsetof(fh_slot_t, xal), (long) __builtin_offsetof(fh_slot_t, mir) };
    fh_ensure_init();
    if (g_fhv.len > 0) rt_gc_run_ptr_fields(g_fhv.p, (long) g_fhv.len, (long) sizeof(fh_slot_t), 5, off);
    if (g_fh_sink_v) rt_gc_visit_raw((const char **)&g_fh_sink_v);
}

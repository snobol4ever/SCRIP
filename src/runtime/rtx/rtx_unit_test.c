#include <stdio.h>
#include <string.h>
#include "descr.h"
int rt_is_truthy(DESCR_t v);
DESCR_t rt_faildescr(void);
static int fails = 0, n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void chk(const char *what, DESCR_t v, int expected) {
    int a = rt_is_truthy(v); n++;
    if (a != expected) { fails++; printf("  MISMATCH %-22s asm=%d golden=%d\n", what, a, expected); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    DESCR_t d; char *s0="", *s1="0", *s2="00", *s3="x", *s4="0x";
    memset(&d,0,sizeof d); d.v=DT_FAIL;             chk("FAIL", d, 0);
    memset(&d,0,sizeof d); d.v=DT_I; d.i=0;         chk("INT 0", d, 0);
    memset(&d,0,sizeof d); d.v=DT_I; d.i=1;         chk("INT 1", d, 1);
    memset(&d,0,sizeof d); d.v=DT_I; d.i=-1;        chk("INT -1", d, 1);
    memset(&d,0,sizeof d); d.v=DT_I; d.i=1LL<<62;   chk("INT big", d, 1);
    memset(&d,0,sizeof d); d.v=DT_R; d.r=0.0;       chk("REAL +0.0", d, 0);
    memset(&d,0,sizeof d); d.v=DT_R; d.r=-0.0;      chk("REAL -0.0", d, 0);
    memset(&d,0,sizeof d); d.v=DT_R; d.r=1.5;       chk("REAL 1.5", d, 1);
    memset(&d,0,sizeof d); d.v=DT_R; d.r=-1.5;      chk("REAL -1.5", d, 1);
    memset(&d,0,sizeof d); d.v=DT_R; d.i=0x7FF8000000000000LL; chk("REAL NaN", d, 1);
    memset(&d,0,sizeof d); d.v=DT_R; d.i=0x7FF0000000000000LL; chk("REAL +Inf", d, 1);
    memset(&d,0,sizeof d); d.v=DT_SNUL;             chk("SNUL", d, 0);
    memset(&d,0,sizeof d); d.v=DT_S; d.s=NULL;      chk("STR NULL", d, 0);
    memset(&d,0,sizeof d); d.v=DT_S; d.s=s0;        chk("STR \"\"", d, 0);
    memset(&d,0,sizeof d); d.v=DT_S; d.s=s1;        chk("STR \"0\"", d, 0);
    memset(&d,0,sizeof d); d.v=DT_S; d.s=s2;        chk("STR \"00\"", d, 1);
    memset(&d,0,sizeof d); d.v=DT_S; d.s=s3;        chk("STR \"x\"", d, 1);
    memset(&d,0,sizeof d); d.v=DT_S; d.s=s4;        chk("STR \"0x\"", d, 1);
    memset(&d,0,sizeof d); d.v=DT_P; d.s=s3;        chk("PAT-as-ptr", d, 1);
    memset(&d,0,sizeof d); d.v=DT_S; d.slen=7; d.s=s3; chk("STR slen!=0", d, 1);
    DESCR_t fa = rt_faildescr(), fg = FAILDESCR; n++;
    if (memcmp(&fa,&fg,sizeof fa)) { fails++; printf("  MISMATCH faildescr asm{v=%d,slen=%u,i=%lld} golden{v=%d,slen=%u,i=%lld}\n",
        fa.v,fa.slen,(long long)fa.i, fg.v,fg.slen,(long long)fg.i); }
    printf("RTX unit: %d checks, %d mismatches -> %s\n", n, fails, fails ? "FAIL" : "PASS");
    return fails != 0;
}

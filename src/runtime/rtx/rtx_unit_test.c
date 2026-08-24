#include <stdio.h>
#include <string.h>
#include "descr.h"
#include "descr_tags.inc"   /* MOD_OP_* provenance stamps -- #define-only and guarded, so C may read the same file the asm does */
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
    /*⛔ STALE GOLDEN, not an asm defect (diagnosed 2026-08-23 s269, hq_C).  This compared all 16 bytes of the asm mint against the bare C FAILDESCR literal, which is UNSTAMPED.  rtx_misc.S:20 stamps mod_op = MOD_OP_RT_FAILDESCR (130) on purpose -- provenance naming which mint produced the descriptor, landed by row descr-stamp-asm-mints in 6ba28e5e, the same commit that cured Milestone 1.  So the memcmp reported a differential red for an INTENTIONAL feature, and stood red long enough to hide any real rtx regression behind it.  The golden now expects the stamp: payload compared semantically, provenance asserted separately.*/
    DESCR_t fa = rt_faildescr(), fg = FAILDESCR; n++;
    fg.mod_op = MOD_OP_RT_FAILDESCR;
    if (memcmp(&fa,&fg,sizeof fa)) { fails++;
        /*⛔ This report used to print only {v,slen,i} while memcmp compares all 16 bytes -- so it printed asm and golden as IDENTICAL and still said MISMATCH.  The differing fields were the two stamp fields it never showed.  Print every byte: a report that cannot express the failure it detects is not a measurement.*/
        printf("  MISMATCH faildescr asm{v=%d,mod_op=%u,src_node=%u,slen=%u,i=%lld} golden{v=%d,mod_op=%u,src_node=%u,slen=%u,i=%lld}\n",
            fa.v,fa.mod_op,fa.src_node,fa.slen,(long long)fa.i, fg.v,fg.mod_op,fg.src_node,fg.slen,(long long)fg.i);
        { const unsigned char *pa=(const unsigned char*)&fa, *pg=(const unsigned char*)&fg; unsigned k;
          printf("    bytes asm ="); for (k=0;k<sizeof fa;k++) printf(" %02x", pa[k]);
          printf("\n    bytes gold="); for (k=0;k<sizeof fg;k++) printf(" %02x", pg[k]);
          printf("\n    differ at ="); for (k=0;k<sizeof fa;k++) if (pa[k]!=pg[k]) printf(" [%u]", k);
          printf("\n"); } }
    printf("RTX unit: %d checks, %d mismatches -> %s\n", n, fails, fails ? "FAIL" : "PASS");
    return fails != 0;
}

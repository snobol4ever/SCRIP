#include <stdio.h>
#include <string.h>
extern void *rt_icn_gen_frame_alloc(void *gen_fb, long bytes);
extern void  rt_icn_gen_frame_retire(void *gen_fb);
static int fails = 0;
static void ck(int c, const char *m) { printf("  %s %s\n", c ? "ok  " : "FAIL", m); if (!c) fails++; }
int main(void) {
    void *k1 = (void *)0x1001, *k2 = (void *)0x1002, *k3 = (void *)0x1003;
    char *a = (char *)rt_icn_gen_frame_alloc(k1, 200);
    ck(a != 0, "alloc returns non-NULL");
    ck(((unsigned long)a & 15u) == 0, "record is 16-byte aligned");
    long cap = ((long *)a)[-1];
    ck(cap >= 224, "capacity word covers bytes+24 rounded up");
    int z = 1; for (int i = 0; i < 200; i++) if (a[i]) z = 0;
    ck(z, "record arrives zeroed");
    memset(a, 0xAB, 200);                                  /* dirty it, so reuse-zeroing is observable */
    char *b = (char *)rt_icn_gen_frame_alloc(k2, 200);
    ck(b != a, "a SECOND live generator gets a DISTINCT record (no aliasing while both live)");
    rt_icn_gen_frame_retire(k1);
    char *c = (char *)rt_icn_gen_frame_alloc(k3, 200);
    ck(c == a, "after retire, the SAME record is reused (free list works -- no island leak)");
    z = 1; for (int i = 0; i < 200; i++) if (c[i]) z = 0;
    ck(z, "reused record is re-zeroed, so no bleed from the prior activation");
    char *d = (char *)rt_icn_gen_frame_alloc((void *)0x1004, 4096);
    ck(d != 0 && d != a && d != b, "an oversized request does NOT reuse a too-small free record");
    ck(((long *)d)[-1] >= 4120, "oversized record's capacity covers the larger request");
    rt_icn_gen_frame_retire((void *)0xDEAD);               /* unknown key */
    ck(1, "retiring an unknown key is a no-op, not a crash");
    printf("%s (%d failure(s))\n", fails ? "TEST FAIL" : "TEST PASS", fails);
    return fails ? 1 : 0;
}

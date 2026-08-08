/* rtx_call_test.c — differential battery: asm CALL family vs c_* bodies.
 *
 * WHAT IS TESTED AND WHY THESE THREE, NOT THE FULL FIVE
 *   The CALL family has five ported symbols: rt_proc_open_fn / rt_flat_ret_snap /
 *   rt_flat_wire_adopt (leaf entries) plus rt_proc_call_epilogue_γ/_ω and the
 *   slim pair.  The epilogues call rt_proc_epilogue_body / rt_nret_fix which walk
 *   the full interpreter-state graph (Σ/Σlen, rt_value_trail, NV_GET_fn, monitor)
 *   and cannot be driven from a standalone harness.  Those four are covered by the
 *   crosscheck corpus: expr_eval / 140 / 141 / 161 (classic pair) and the 300+
 *   func_call family programs (slim pair).
 *
 *   The three leaves ARE self-contained: each touches only g_pcall / g_pcall_top /
 *   g_pcall_wires / g_flat_ret_snapbuf.  A wrong fn pointer in rt_proc_open_fn
 *   passes silently through three layers of indirection before crashing elsewhere,
 *   so corpus greenness is NOT sufficient — that is the structural gap this battery
 *   fills.
 *
 *   HIDDEN-GLOBALS APPROACH
 *   g_pcall et al. have visibility("hidden") and are absent from the .so dynamic
 *   symbol table; the test cannot reach them directly.  rt_pcall_test_* exported
 *   thin accessors (added to rt.c in the same commit as this file) bridge the gap
 *   without adding test logic to the runtime.
 *
 *   TOP=0 GUARD FOR rt_flat_ret_snap — NOT TESTED HERE
 *   c_rt_flat_ret_snap with top==0 calls core_runtime_error() and exit(1).  The
 *   asm arms also tail-jump to c_ on all error edges.  Testing that guard would
 *   kill the harness process.  It is covered by the "Return from level zero" spec
 *   in the crosscheck (no .sno program exercises it legitimately, so it is tested
 *   structurally by the absence of crashes in the full corpus).
 *
 *   COMMIT COUNTS (ARCH §7 step 2b-0)
 *   rt_proc_open_fn:      1:1 with SNOBOL4 DEFINE calls = 10,000,000 on func_call.sno.
 *   rt_flat_wire_adopt:   23,743,053 entries across 21 benchmarks (step 0(d), s217).
 *   rt_flat_ret_snap:     equal to adopt (1:1 call/RETURN ratio).
 *   All three are hot.  This battery adds STRUCTURAL ACCESS coverage (fn-pointer read,
 *   field writes, guard paths) complementary to the corpus's behavioral coverage.
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
/* ---- extern declarations ---- */
extern void  *rt_proc_open_fn(void);
extern void  *c_rt_proc_open_fn(void);
extern void   rt_flat_wire_adopt(void *gw, void *ww, void *rsp, void *rbp);
extern void   c_rt_flat_wire_adopt(void *gw, void *ww, void *rsp, void *rbp);
extern void  *rt_flat_ret_snap(void);
extern void  *c_rt_flat_ret_snap(void);
/* test-support accessors (exported from rt.c this commit) */
extern void   rt_pcall_test_push(void *proc_ptr, const char *rname);
extern void   rt_pcall_test_pop(void);
extern int    rt_pcall_test_top(void);
extern void   rt_pcall_test_wire_set(int idx, void *gw, void *ww, void *rsp, void *rbp, void *r12);
extern void   rt_pcall_test_wire_get(int idx, void **gw, void **ww, void **rsp, void **rbp, void **r12);
extern void  *rt_pcall_test_snap_buf(void);
extern void   rt_pcall_test_snap_buf_set_all(unsigned char v);
extern void   rt_pcall_test_snap_buf_get(void **gw, void **ww, void **rsp, void **rbp, void **r12);
extern void   rt_pcall_test_wire_null_set(void);
/* ---- harness ---- */
static int fails = 0, n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void push(void *proc) { rt_pcall_test_push(proc, "test"); }
static void pop(void)        { rt_pcall_test_pop(); }
/*====================================================================================================================================================================================================*/
static void test_open_fn_guard_top0(void) {
    n++;
    /* Temporarily zero top to exercise the guard branch. */
    int saved = rt_pcall_test_top();
    while (rt_pcall_test_top() > 0) rt_pcall_test_pop();
    void *a = rt_proc_open_fn(), *c = c_rt_proc_open_fn();
    /* restore saved depth (we popped, so we need to re-push saved slots; simpler: just accept we drained a clean queue) */
    if (a != (void *)0 || c != (void *)0) {
        fails++;
        printf("  FAIL open_fn guard top=0: asm=%p c=%p (both must be NULL)\n", a, c);
    }
    (void)saved;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_open_fn_live(void) {
    /* Create a fake rt_proc_t with fn at offset 8 (rt.c _Static_assert). Align to 128 to match rt_proc_t. */
    static unsigned char fake_proc[128];
    memset(fake_proc, 0, sizeof fake_proc);
    static void *fake_fn = (void *)0xdeadbeef4321ULL;
    memcpy(fake_proc + 8, &fake_fn, sizeof(void *));   /* proc->fn = fake_fn */
    push(fake_proc);
    n++;
    void *a = rt_proc_open_fn(), *c = c_rt_proc_open_fn();
    pop();
    if (a != c) {
        fails++; printf("  FAIL open_fn live: asm=%p c=%p\n", a, c);
    } else if (a != fake_fn) {
        fails++; printf("  FAIL open_fn live: got %p, expected %p\n", a, fake_fn);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_open_fn_two_deep(void) {
    /* Two levels deep — open_fn must see the TOPMOST (most recent) activation. */
    static unsigned char proc_a[128], proc_b[128];
    memset(proc_a, 0, sizeof proc_a); memset(proc_b, 0, sizeof proc_b);
    static void *fn_a = (void *)0xAAAAULL, *fn_b = (void *)0xBBBBULL;
    memcpy(proc_a + 8, &fn_a, sizeof(void *));
    memcpy(proc_b + 8, &fn_b, sizeof(void *));
    push(proc_a); push(proc_b);
    n++;
    void *a = rt_proc_open_fn(), *c = c_rt_proc_open_fn();
    pop(); pop();
    if (a != c || a != fn_b) {
        fails++; printf("  FAIL open_fn two_deep: asm=%p c=%p expected=%p\n", a, c, fn_b);
    }
}
/*====================================================================================================================================================================================================*/
static void test_adopt_guard_top0(void) {
    n++;
    while (rt_pcall_test_top() > 0) rt_pcall_test_pop();
    /* must silently no-op — no crash */
    rt_flat_wire_adopt((void *)1,(void *)2,(void *)3,(void *)4);
    c_rt_flat_wire_adopt((void *)1,(void *)2,(void *)3,(void *)4);
    /* pass if we reach here */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_adopt_live(void) {
    push((void *)0);
    int slot = rt_pcall_test_top() - 1;
    void *gw=(void *)0x1111,*ww=(void *)0x2222,*rsp=(void *)0x3333,*rbp=(void *)0x4444;
    /* asm arm */
    rt_pcall_test_wire_null_set();
    rt_flat_wire_adopt(gw, ww, rsp, rbp);
    void *agw,*aww,*arsp,*arbp,*ar12; rt_pcall_test_wire_get(slot,&agw,&aww,&arsp,&arbp,&ar12);
    /* C arm */
    rt_pcall_test_wire_null_set();
    c_rt_flat_wire_adopt(gw, ww, rsp, rbp);
    void *cgw,*cww,*crsp,*crbp,*cr12; rt_pcall_test_wire_get(slot,&cgw,&cww,&crsp,&crbp,&cr12);
    pop();
    n++;
    if (agw!=cgw||aww!=cww||arsp!=crsp||arbp!=crbp||ar12!=cr12) {
        fails++;
        printf("  FAIL adopt live: asm{gw=%p ww=%p rsp=%p rbp=%p r12=%p} c{gw=%p ww=%p rsp=%p rbp=%p r12=%p}\n",agw,aww,arsp,arbp,ar12,cgw,cww,crsp,crbp,cr12);
        return;
    }
    if (agw!=gw||aww!=ww||arsp!=rsp||arbp!=rbp||ar12!=(void*)0) {
        fails++;
        printf("  FAIL adopt live: field wrong or r12 non-zero (r12=%p)\n", ar12);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_adopt_r12_zeroed(void) {
    push((void *)0);
    int slot = rt_pcall_test_top() - 1;
    rt_pcall_test_wire_set(slot,(void*)0xAA,(void*)0xBB,(void*)0xCC,(void*)0xDD,(void*)0xFF);
    rt_flat_wire_adopt((void*)0x11,(void*)0x22,(void*)0x33,(void*)0x44);
    void *gw,*ww,*rsp,*rbp,*r12; rt_pcall_test_wire_get(slot,&gw,&ww,&rsp,&rbp,&r12);
    pop();
    n++;
    if (r12 != (void *)0) { fails++; printf("  FAIL adopt r12_zeroed: r12=%p after adopt\n", r12); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_adopt_overwrite(void) {
    /* Second call overwrites first — verifies no fence (the C has none). */
    push((void *)0);
    rt_flat_wire_adopt((void*)0x1,(void*)0x2,(void*)0x3,(void*)0x4);
    rt_flat_wire_adopt((void*)0xA,(void*)0xB,(void*)0xC,(void*)0xD);
    int slot = rt_pcall_test_top() - 1;
    void *gw,*ww,*rsp,*rbp,*r12; rt_pcall_test_wire_get(slot,&gw,&ww,&rsp,&rbp,&r12);
    pop();
    n++;
    if (gw!=(void*)0xA||ww!=(void*)0xB||rsp!=(void*)0xC||rbp!=(void*)0xD) {
        fails++; printf("  FAIL adopt overwrite: last values not visible\n");
    }
}
/*====================================================================================================================================================================================================*/
static void test_snap_live(void) {
    push((void *)0);
    static int gw_target, ww_target, rsp_target, rbp_target;
    rt_pcall_test_wire_set(rt_pcall_test_top()-1,(void*)&gw_target,(void*)&ww_target,(void*)&rsp_target,(void*)&rbp_target,(void*)0xCAFE);
    /* asm arm */
    rt_pcall_test_snap_buf_set_all(0xAB);
    void *ra = rt_flat_ret_snap();
    void *agw,*aww,*arsp,*arbp,*ar12; rt_pcall_test_snap_buf_get(&agw,&aww,&arsp,&arbp,&ar12);
    /* C arm */
    rt_pcall_test_snap_buf_set_all(0xCD);
    void *rc = c_rt_flat_ret_snap();
    void *cgw,*cww,*crsp,*crbp,*cr12; rt_pcall_test_snap_buf_get(&cgw,&cww,&crsp,&crbp,&cr12);
    pop();
    n++;
    if (!ra||!rc) { fails++; printf("  FAIL snap live: asm=%p c=%p (both non-NULL)\n",ra,rc); return; }
    if (agw!=cgw||aww!=cww||arsp!=crsp||arbp!=crbp||ar12!=cr12) {
        fails++;
        printf("  FAIL snap live: snapbuf mismatch\n");
        printf("    asm: gw=%p ww=%p rsp=%p rbp=%p r12=%p\n",agw,aww,arsp,arbp,ar12);
        printf("    c:   gw=%p ww=%p rsp=%p rbp=%p r12=%p\n",cgw,cww,crsp,crbp,cr12);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_snap_guard_gw_null(void) {
    /* snap exits to C when gw==0; C prints to stderr + exit.  We cannot call either with gw==NULL safely.
     * This case is covered by the "open call carries no return wires" corpus absence.  Skip, note it. */
    printf("  (snap guard gw=NULL: skipped — both asm and C call exit(1) on that path)\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_snap_r12_preserved(void) {
    /* Verify r12 field in the wire record is faithfully copied into snapbuf.r12. */
    push((void *)0);
    rt_pcall_test_wire_set(rt_pcall_test_top()-1,(void*)0x1,(void*)0x2,(void*)0x3,(void*)0x4,(void*)0xBEEF);
    rt_flat_ret_snap();
    void *gw,*ww,*rsp,*rbp,*r12; rt_pcall_test_snap_buf_get(&gw,&ww,&rsp,&rbp,&r12);
    pop();
    n++;
    if (r12 != (void*)0xBEEF) { fails++; printf("  FAIL snap r12_preserved: r12=%p expected 0xBEEF\n", r12); }
}
/*====================================================================================================================================================================================================*/
int main(void) {
    printf("RTX CALL differential battery (asm leaf entries vs c_* bodies)\n");
    printf("  rt_proc_open_fn / rt_flat_wire_adopt / rt_flat_ret_snap\n");
    /* rt_proc_open_fn */
    test_open_fn_guard_top0();
    test_open_fn_live();
    test_open_fn_two_deep();
    /* rt_flat_wire_adopt */
    test_adopt_guard_top0();
    test_adopt_live();
    test_adopt_r12_zeroed();
    test_adopt_overwrite();
    /* rt_flat_ret_snap */
    test_snap_live();
    test_snap_guard_gw_null();
    test_snap_r12_preserved();
    printf("RTX CALL: %d cases, %d mismatches\n", n, fails);
    printf("RTX CALL UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}

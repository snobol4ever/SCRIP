#!/usr/bin/env bash
# scripts/test_gate_coexpr_stack_of_uncreated_thread.sh -- row
# coexpr-stack-of-calls-pthread-getattr-np-on-an-uncreated-thread (hq_C 2026-09-01, corroborated by
# inspection at SCRIP bcb0ec1e; folded into coexpr-stack-leaves-the-compacting-gc-heap's landing per
# Lon's own ballot ruling: "fix its thread_id=0 defect on the way").
#
# scrip_co_stack_of (src/runtime/rt/rt_coexpr.c) used to guard only `!ctx->alive` before calling
# pthread_getattr_np(ctx->thread, ...). new_ctx->alive was set to 1 in scrip_coswitch BEFORE the
# co-expression's thread was ever created (pre-fix ~:62, well before pthread_create), so a co-expression
# reached by the GC's root-scan walk (gc_heap.c:572, unconditional over every entry in g_co_gc_head)
# during that window hit pthread_getattr_np on an uncreated thread -- undefined behaviour; on this
# glibc/Linux target it dereferences a struct pthread at the thread_id address (0 here), which reliably
# SIGSEGVs.
#
# THIS WITNESS FORCES THE EXACT STATE BY HAND rather than racing a real GC into that narrow window:
# create a real, properly-initialized scrip_coctx_t (so ctx->thread is genuinely whatever
# scrip_coexpr_create leaves it as -- proving the construction-time zero-init fix too), set ->alive = 1
# without ever creating a thread for it, and call scrip_co_stack_of directly. A fixed build must return 0
# (not-yet-created) without ever reaching pthread_getattr_np; a pre-fix build reaches
# pthread_getattr_np(0, ...) and crashes. Negative-tested both ways at mint (git-stash the fix, rebuild,
# confirm rc=139; restore, rebuild, confirm clean PASS) -- see this row's LEDGER, not asserted from
# reading the source alone.
#
# Usage: bash scripts/test_gate_coexpr_stack_of_uncreated_thread.sh
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT" || exit 2
[ -r "$ROOT/out/libscrip_rt.so" ] || { echo "REFUSE: out/libscrip_rt.so missing -- build first (make)"; exit 2; }
[ -r "$ROOT/src/runtime/rt/rt_coexpr.h" ] || { echo "REFUSE: rt_coexpr.h not where expected -- re-anchor"; exit 2; }
SRC=/tmp/coexpr_stack_of_uncreated_thread_witness.c
BIN=/tmp/coexpr_stack_of_uncreated_thread_witness
cat > "$SRC" <<'EOF'
#include <stdio.h>
#include <stdint.h>
#include "rt_coexpr.h"
int main(void) {
    uint64_t regs[6] = {0,0,0,0,0,0};
    scrip_coctx_t *ctx = scrip_coexpr_create((void *)0x1, regs, 0);
    if (ctx->thread != 0) {
        fprintf(stderr, "FAIL: ctx->thread not zero-initialized after scrip_coexpr_create\n");
        return 1;
    }
    ctx->alive = 1;   /* the exact pre-thread-creation state: alive but no thread created yet */
    char *lo = (char *)0x1, *hi = (char *)0x1;   /* poison -- must not be left looking like a valid answer */
    int r = scrip_co_stack_of(ctx, &lo, &hi);
    if (r != 0) {
        fprintf(stderr, "FAIL: scrip_co_stack_of claimed bounds (return %d) for an alive-but-uncreated thread\n", r);
        return 1;
    }
    printf("PASS: scrip_co_stack_of correctly refused an alive-but-uncreated thread\n");
    return 0;
}
EOF
gcc -O0 -g -I "$ROOT/src/runtime/rt" -o "$BIN" "$SRC" "$ROOT/out/libscrip_rt.so" -Wl,-rpath,"$ROOT/out" -lpthread 2>/tmp/coexpr_witness_build.log
bc=$?
if [ "$bc" -ne 0 ]; then
    echo "REFUSE: witness failed to build"
    cat /tmp/coexpr_witness_build.log
    exit 2
fi
out=$("$BIN" 2>&1)
rc=$?
echo "$out"
if [ "$rc" -eq 139 ]; then
    echo "RED: witness SIGSEGV'd (rc=139) -- pthread_getattr_np reached on an uncreated thread"
    exit 1
fi
if [ "$rc" -eq 0 ] && echo "$out" | grep -q "^PASS:"; then
    echo "GREEN: uncreated-thread witness passed"
    exit 0
fi
echo "FAIL: witness exited rc=$rc without a clean PASS"
exit 1

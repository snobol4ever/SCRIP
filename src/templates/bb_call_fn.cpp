#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
DESCR_t rt_pl_dop_unify_ci(DESCR_t * args, long long imm);
DESCR_t rt_pl_dop_unify_cs(DESCR_t * args, const char * cs);
int bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym);
extern "C" char g_pl_trail[];
extern "C" char g_hp_fr[];
extern "C" uint32_t g_plw_dot_sl;
extern "C" int g_plw_cellws_on;
extern "C" int g_zeta_mode;
/* PL-SINK-1 (2026-07-24) — EMITTED $unify FAST PATH.  The data-plane leaves measured 86% of Prolog wall live in C (s141 FINDING §ARCHITECTURAL VERDICT); this sinks the hot arms of plw_unify_cells
 * (by_name_dispatch.c) into the box itself: deref chase (DT_PLVAR chain), ptr-equal, one-side bind (inline trail push + 16-byte cell copy), int==int, and a bit-identical-descr shortcut.  Every arm the
 * fast path cannot decide EXACTLY (DT_N entry forms, both-unbound join/VVB, compound recursion, floats/NaN, non-identical atoms → rt_descr_equal, trail uninitialized/full → area grow) falls into the
 * UNTOUCHED C leaf rt_pl_dop_unify with UNMODIFIED inputs, so outcomes are bit-identical by construction: the inline path either completes a whole plw arm verbatim or defers the whole call.  The GC
 * safepoint + unwind-floor discipline are alloc/throw-time concerns — the inline arms allocate nothing and cannot throw, so both stay in the leaf (the slow path).  Layout literals (tag offsets 0/+8,
 * trail base+0/cap+24/top+32, 24-byte entries) are _Static_assert-anchored beside plw_bind in by_name_dispatch.c.  Internal label ids 40..54 (marshal uses idx*2/idx*2+1 ≤ 3).  Kill switch:
 * SCRIP_NO_SINK=1 at emit time (the SCRIP_SLOT_ELIDE / SCRIP_NO_CU pattern); the SCRIP_NO_DOP / SCRIP_DOP_SKIP hatches disable it too (the sink nests inside the dfp branch), and poison-trap debug
 * sessions (SCRIP_PL_POISON_TRAP) should set SCRIP_NO_SINK since inline arms skip poison reads. */
static std::string sink_deref(const char * reg, int lh, int ld, int ln2) {
    return x86_deflabel_id(lh)
         + x86("mov", "eax", (std::string("dword ptr [") + reg + " + 0]").c_str())
         + x86("cmp", "eax", (long)9)  + x86_jcc_id("jne", ln2 + 1)
         + x86("mov", "esi", (std::string("dword ptr [") + reg + " + 4]").c_str())
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("cmp", "esi", (long)1)  + x86_jcc_id("jne", ln2)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ln2)
         + x86("cmp", "esi", (long)2)  + x86_jcc_id("jne", ld)
         + x86("mov", "rax", "[rax + 0]")
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ln2 + 1)
         + x86("cmp", "eax", (long)13) + x86_jcc_id("jne", ld)
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("cmp", "rax", reg)      + x86_jcc_id("je", ld)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ld);
}
static std::string sink_unb(const char * reg, int lyes, int lno) {
    return x86("mov", "eax", (std::string("dword ptr [") + reg + " + 0]").c_str())
         + x86("cmp", "eax", (long)0)  + x86_jcc_id("je", lyes)
         + x86("cmp", "eax", (long)99) + x86_jcc_id("je", lyes)
         + x86("cmp", "eax", (long)13) + x86_jcc_id("jne", lno)
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("cmp", "rax", reg)      + x86_jcc_id("je", lyes)
         + x86_jmp_id(lno);
}
static std::string sink_trailpush(const char * creg, int lslow) {
    return x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail")
         + x86("mov", "r11", "[r10 + 0]")
         + x86("test", "r11", "r11")   + x86_jcc_id("je", lslow)
         + x86("mov", "eax", "dword ptr [r10 + 32]")
         + x86("mov32", "esi", (long)24)
         + x86("imul", "rsi", "rax")
         + x86("mov", "rax", "[r10 + 24]")
         + x86("sub", "rax", (long)24)
         + x86("cmp", "rsi", "rax")    + x86_jcc_id("ja", lslow)
         + x86("add", "r11", "rsi")
         + x86("mov", "[r11 + 0]", creg)
         + x86("mov", "rax", (std::string("[") + creg + " + 0]").c_str())
         + x86("mov", "[r11 + 8]", "rax")
         + x86("mov", "rax", (std::string("[") + creg + " + 8]").c_str())
         + x86("mov", "[r11 + 16]", "rax")
         + x86("mov", "eax", "dword ptr [r10 + 32]")
         + x86("add", "eax", (long)1)
         + x86("mov", "dword ptr [r10 + 32]", "eax");
}
static std::string sink_cp16(const char * dst, const char * src) {
    return x86("mov", "rax", (std::string("[") + src + " + 0]").c_str())
         + x86("mov", (std::string("[") + dst + " + 0]").c_str(), "rax")
         + x86("mov", "rax", (std::string("[") + src + " + 8]").c_str())
         + x86("mov", (std::string("[") + dst + " + 8]").c_str(), "rax");
}
/* PL-SINK-3 (2026-07-25) — THE CARVE.  sink_tp_nc is sink_trailpush MINUS the base-null and room tests: the WRITE arm pre-reserves room for its THREE worst-case entries (H, T, subject) in ONE check before
 * any mutation, so the per-push tests are provably dominated and re-emitting them would be dead code on the hot path.  Requires r10 = &g_pl_trail (caller loads it); clobbers rax/rsi/r11; reads creg. */
static std::string sink_tp_nc(const char * creg) {
    return x86("mov", "r11", "[r10 + 0]")
         + x86("mov", "eax", "dword ptr [r10 + 32]")
         + x86("mov32", "esi", (long)24)
         + x86("imul", "rsi", "rax")
         + x86("add", "r11", "rsi")
         + x86("mov", "[r11 + 0]", creg)
         + x86("mov", "rax", (std::string("[") + creg + " + 0]").c_str())
         + x86("mov", "[r11 + 8]", "rax")
         + x86("mov", "rax", (std::string("[") + creg + " + 8]").c_str())
         + x86("mov", "[r11 + 16]", "rax")
         + x86("mov", "eax", "dword ptr [r10 + 32]")
         + x86("add", "eax", (long)1)
         + x86("mov", "dword ptr [r10 + 32]", "eax");
}
/* Inline rt_gcheap_alloc's DETAX fast path (gc_heap.c ~:170) for EXACTLY one shape: HB_PLJ, 32-byte payload (two DESCR_t kids) => total 48.  armed==0 or top+48 > end -> SLOW, so the C keeps sole ownership
 * of init / grow / collect / the fill window (contract §4).  The carve's payload memset is SKIPPED because the WRITE arm overwrites all 32 payload bytes unconditionally (both kid arms write a full 16-byte
 * DESCR) — bit-identical by construction, and the one per-alloc lever s141 measured as safe-and-free here precisely because we are the sole writer.  Leaves rdx = kids, clobbers rax/r10/r11. */
static std::string sink_carve48(int lslow) {
    return x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_hp_fr, "g_hp_fr")
         + x86("mov", "eax", "dword ptr [r10 + 24]")
         + x86("test", "eax", "eax")            + x86_jcc_id("je", lslow)
         + x86("mov", "r11", "[r10 + 0]")
         + x86("mov", "rax", "[r10 + 8]")
         + x86("sub", "rax", (long)48)
         + x86("cmp", "r11", "rax")             + x86_jcc_id("ja", lslow);
}
static std::string sink_carve48_take(void) {
    return x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_hp_fr, "g_hp_fr")
         + x86("mov", "r11", "[r10 + 0]")
         + x86("mov", "[r11 + 0]", (long)0)
         + x86("mov", "dword ptr [r11 + 8]", (long)48)
         + x86("mov", "dword ptr [r11 + 12]", (long)(209 | (1 << 16)))
         + x86("lea", "rdx", "[r11 + 16]")
         + x86("mov", "rax", "r11")
         + x86("add", "rax", (long)48)
         + x86("mov", "[r10 + 0]", "rax")
         + x86("mov", "rax", "[r10 + 16]")
         + x86("add", "rax", (long)1)
         + x86("mov", "[r10 + 16]", "rax");
}
/* One kid of plw_mkc_kids (by_name_dispatch.c :1335), ar==2.  creg = the already-derefed source cell; koff = 0 or 16 into rdx=kids.  UNBOUND source -> seed kids[i] as a SELF-PLVAR and forward the source to
 * &kids[i] (gprolog write-mode Pl_Unify_Variable's shape: *S itself is the fresh REF).  BOUND source -> kids[i] = *F.  Both arms are TOTAL — no sub-shape defers — which is why the carve above can safely
 * precede them (contract §1: nothing after the first mutation can reach SLOW).  Requires the 3-entry trail reservation already checked. */
static std::string sink_kid(const char * creg, int koff, int lunb, int lbnd, int ljoin) {
    std::string s = sink_unb(creg, lunb, lbnd);
    s += x86_deflabel_id(lunb);
    s += x86("mov", (std::string("[rdx + ") + std::to_string(koff) + "]").c_str(), (long)13);
    s += x86("lea", "rax", (std::string("[rdx + ") + std::to_string(koff) + "]").c_str());
    s += x86("mov", (std::string("[rdx + ") + std::to_string(koff + 8) + "]").c_str(), "rax");
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += sink_tp_nc(creg);
    s += x86("mov", (std::string("[") + creg + " + 0]").c_str(), (long)13);
    s += x86("lea", "rax", (std::string("[rdx + ") + std::to_string(koff) + "]").c_str());
    s += x86("mov", (std::string("[") + creg + " + 8]").c_str(), "rax");
    s += x86_jmp_id(ljoin);
    s += x86_deflabel_id(lbnd);
    s += x86("mov", "rax", (std::string("[") + creg + " + 0]").c_str());
    s += x86("mov", (std::string("[rdx + ") + std::to_string(koff) + "]").c_str(), "rax");
    s += x86("mov", "rax", (std::string("[") + creg + " + 8]").c_str());
    s += x86("mov", (std::string("[rdx + ") + std::to_string(koff + 8) + "]").c_str(), "rax");
    s += x86_deflabel_id(ljoin);
    return s;
}
static std::string sink_unify2_str(int argbase, uint64_t ufp, const char * usym) {
    std::string s = x86("comment", "PL-SINK-1 inline $unify fast path: deref/bind/trail/int-eq emitted; rt_pl_dop_unify stays the slow-path oracle (bit-identical fallback, unmodified args)");
    s += x86("lea", "rdi", FRQ(argbase));
    s += x86("lea", "r8",  FRQ(argbase));
    s += sink_deref("r8", 40, 41, 55);
    s += x86("lea", "r9",  FRQ(argbase + 16));
    s += sink_deref("r9", 42, 43, 57);
    s += x86("cmp", "r8", "r9");
    s += x86_jcc_id("je", 51);
    s += sink_unb("r8", 44, 45);
    s += x86_deflabel_id(44);
    s += sink_unb("r9", 53, 46);
    s += x86_deflabel_id(46);
    s += sink_trailpush("r8", 53);
    s += sink_cp16("r8", "r9");
    s += x86_jmp_id(51);
    s += x86_deflabel_id(45);
    s += sink_unb("r9", 47, 48);
    s += x86_deflabel_id(47);
    s += sink_trailpush("r9", 53);
    s += sink_cp16("r9", "r8");
    s += x86_jmp_id(51);
    s += x86_deflabel_id(48);
    s += x86("mov", "ecx", "dword ptr [r8 + 0]");
    s += x86("mov", "edx", "dword ptr [r9 + 0]");
    s += x86("cmp", "ecx", (long)14);
    s += x86_jcc_id("jne", 49);
    s += x86("cmp", "edx", (long)14);
    s += x86_jcc_id("je", 53);
    s += x86_jmp_id(52);
    s += x86_deflabel_id(49);
    s += x86("cmp", "edx", (long)14);
    s += x86_jcc_id("je", 52);
    s += x86("cmp", "ecx", (long)7);
    s += x86_jcc_id("je", 53);
    s += x86("cmp", "edx", (long)7);
    s += x86_jcc_id("je", 53);
    s += x86("cmp", "ecx", (long)6);
    s += x86_jcc_id("jne", 50);
    s += x86("cmp", "edx", (long)6);
    s += x86_jcc_id("jne", 50);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("cmp64", "rax", (long)6);
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r9 + 0]");
    s += x86("cmp64", "rax", (long)6);
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r8 + 8]");
    s += x86("mov", "rsi", "[r9 + 8]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("je", 51);
    s += x86_jmp_id(52);
    s += x86_deflabel_id(50);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("mov", "rsi", "[r9 + 0]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r8 + 8]");
    s += x86("mov", "rsi", "[r9 + 8]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("jne", 53);
    s += x86_deflabel_id(51);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("mov", "rdx", "[r8 + 8]");
    s += x86_jmp_id(54);
    s += x86_deflabel_id(52);
    s += x86("mov32", "eax", (long)99);
    s += x86("mov32", "edx", (long)0);
    s += x86_jmp_id(54);
    s += x86_deflabel_id(53);
    s += x86("mov32", "esi", (long)2);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(54);
    return s;
}
/* PL-SINK-2 (2026-07-25) — EMITTED $unify_lst(Subject,Head,Tail) READ-MODE FAST PATH.  Mirrors dop_unify_lst (by_name_dispatch.c) arm-for-arm; rt_pl_dop_unify_lst stays the slow-path oracle, entered with
 * UNMODIFIED rdi=args (esi=3), so every deferred shape is bit-identical by construction (contract §1 WHOLE-ARM-OR-DEFER).  INLINE ARM = the nrev input-list destructuring hot spot: bound './2 subject, BOTH
 * H and T deref to DISTINCT unbound cells -> double bind (H<-kids[0], T<-kids[1]) with a single 2-entry trail push.  DEFERRED to slow: unbound subject (WRITE mode allocates kids -> SINK-3), H/T aliasing (the
 * C's sequential deref sees H's bind when tc==hc; a double-bind would diverge), and any BOUND H or T arm (int-eq / bit-ident / recursive-unify arms are SINK-2's follow-on; deferring them is zero-partial-state).
 * dot_sl (intern(".")<<16|2) is RUNTIME-assigned and UN-BAKEABLE (contract §3): RIPSEAL-load the exported g_plw_dot_sl cell (the leaf fills it on first slow hit); ==0 -> SLOW so a not-yet-interned run defers to
 * the leaf (which interns + answers) rather than mis-failing a real cons.  Layout (DESCR 16B q0@0/slen@4/p@8, trail base@0/cap@24[bytes]/top@32[count], 24B entries) reuses SINK-1's _Static_asserts beside
 * plw_bind.  Internal label ids 60..77 (SINK-1 owns 40..58; marshal owns idx*2 <= 5). */
static std::string sink_unify_lst_str(int argbase, uint64_t ufp, const char * usym) {
    std::string s = x86("comment", "PL-SINK-2 inline $unify_lst READ-mode fast path: bound './2 subject + both-unbound-distinct H,T -> double bind; rt_pl_dop_unify_lst is the slow-path oracle (unmodified args)");
    s += x86("lea", "rdi", FRQ(argbase));
    s += x86("lea", "r8",  FRQ(argbase));
    s += sink_deref("r8", 60, 61, 62);
    s += sink_unb("r8", 80, 74);
    s += x86_deflabel_id(74);
    s += x86("mov", "ecx", "dword ptr [r8 + 0]");
    s += x86("cmp", "ecx", (long)14);
    s += x86_jcc_id("jne", 73);
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_dot_sl, "g_plw_dot_sl");
    s += x86("mov", "eax", "dword ptr [r10 + 0]");
    s += x86("test", "eax", "eax");
    s += x86_jcc_id("je", 72);
    s += x86("mov", "edx", "dword ptr [r8 + 4]");
    s += x86("cmp", "eax", "edx");
    s += x86_jcc_id("jne", 73);
    s += x86("lea", "r9",  FRQ(argbase + 16));
    s += sink_deref("r9", 64, 65, 66);
    s += x86("lea", "rcx", FRQ(argbase + 32));
    s += sink_deref("rcx", 68, 69, 70);
    s += x86("cmp", "r9", "rcx");
    s += x86_jcc_id("je", 72);
    s += sink_unb("r9", 75, 72);
    s += x86_deflabel_id(75);
    s += sink_unb("rcx", 76, 72);
    s += x86_deflabel_id(76);
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += x86("mov", "r11", "[r10 + 0]");
    s += x86("test", "r11", "r11")   + x86_jcc_id("je", 72);
    s += x86("mov", "eax", "dword ptr [r10 + 32]");
    s += x86("mov32", "esi", (long)24);
    s += x86("imul", "rsi", "rax");
    s += x86("mov", "rax", "[r10 + 24]");
    s += x86("sub", "rax", (long)48);
    s += x86("cmp", "rsi", "rax")    + x86_jcc_id("ja", 72);
    s += x86("add", "r11", "rsi");
    s += x86("mov", "[r11 + 0]", "r9");
    s += x86("mov", "rax", "[r9 + 0]");
    s += x86("mov", "[r11 + 8]", "rax");
    s += x86("mov", "rax", "[r9 + 8]");
    s += x86("mov", "[r11 + 16]", "rax");
    s += x86("mov", "[r11 + 24]", "rcx");
    s += x86("mov", "rax", "[rcx + 0]");
    s += x86("mov", "[r11 + 32]", "rax");
    s += x86("mov", "rax", "[rcx + 8]");
    s += x86("mov", "[r11 + 40]", "rax");
    s += x86("mov", "eax", "dword ptr [r10 + 32]");
    s += x86("add", "eax", (long)2);
    s += x86("mov", "dword ptr [r10 + 32]", "eax");
    s += x86("mov", "r10", "[r8 + 8]");
    s += x86("mov", "rax", "[r10 + 0]");
    s += x86("mov", "[r9 + 0]", "rax");
    s += x86("mov", "rax", "[r10 + 8]");
    s += x86("mov", "[r9 + 8]", "rax");
    s += x86("mov", "rax", "[r10 + 16]");
    s += x86("mov", "[rcx + 0]", "rax");
    s += x86("mov", "rax", "[r10 + 24]");
    s += x86("mov", "[rcx + 8]", "rax");
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("mov", "rdx", "[r8 + 8]");
    s += x86_jmp_id(77);
    /* PL-SINK-3 WRITE ARM (unbound subject) — dop_unify_lst's first branch inlined: kids = plw_mkc_kids(args+1, 2); w = {DT_PLREF, dot_sl, kids}; plw_bind(subject, w).  ALL FOUR DEFERRAL TESTS RUN BEFORE
     * ANY MUTATION (contract §1): dot_sl interned, carve armed + 48 bytes of room, trail live + room for THREE entries.  Past that point every remaining step is total, so no partial state can reach SLOW.
     * H and T are derefed SEQUENTIALLY, each immediately before its own bind — which is exactly the C's order, so the ALIASING case SINK-2 had to defer (H and T the same cell) falls out correct for free:
     * the second deref observes the first bind and chases into kids[0], producing the C's chain rather than a double bind.  Labels 80..94. */
    s += x86_deflabel_id(80);
    s += x86("comment", "PL-SINK-3 inline $unify_lst WRITE mode: carve 2 kids off the PLJ frontier, join unbound args, bind subject to the './2 cell");
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_dot_sl, "g_plw_dot_sl");
    s += x86("mov", "eax", "dword ptr [r10 + 0]");
    s += x86("test", "eax", "eax");
    s += x86_jcc_id("je", 72);
    s += sink_carve48(72);
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += x86("mov", "r11", "[r10 + 0]");
    s += x86("test", "r11", "r11");
    s += x86_jcc_id("je", 72);
    s += x86("mov", "eax", "dword ptr [r10 + 32]");
    s += x86("mov32", "esi", (long)24);
    s += x86("imul", "rsi", "rax");
    s += x86("mov", "rax", "[r10 + 24]");
    s += x86("sub", "rax", (long)72);
    s += x86("cmp", "rsi", "rax");
    s += x86_jcc_id("ja", 72);
    s += sink_carve48_take();
    s += x86("lea", "r9", FRQ(argbase + 16));
    s += sink_deref("r9", 81, 82, 83);
    s += sink_kid("r9", 0, 85, 86, 87);
    s += x86("lea", "rcx", FRQ(argbase + 32));
    s += sink_deref("rcx", 88, 89, 90);
    s += sink_kid("rcx", 16, 92, 93, 94);
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += sink_tp_nc("r8");
    s += x86("mov", "dword ptr [r8 + 0]", (long)14);
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_dot_sl, "g_plw_dot_sl");
    s += x86("mov", "eax", "dword ptr [r10 + 0]");
    s += x86("mov", "dword ptr [r8 + 4]", "eax");
    s += x86("mov", "[r8 + 8]", "rdx");
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86_jmp_id(77);
    s += x86_deflabel_id(73);
    s += x86("mov32", "eax", (long)99);
    s += x86("mov32", "edx", (long)0);
    s += x86_jmp_id(77);
    s += x86_deflabel_id(72);
    s += x86("mov32", "esi", (long)3);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(77);
    return s;
}
/* PL-SINK-8 (2026-07-25) — EMITTED $trail_mark FAST PATH.  The leaf is one load (`return t->top`) wrapped in a ceremony that measured ~12% of Prolog wall (s145 45-sample profile): rt_pl_dop_trail_mark saves
 * and restores g_plw_unwind_floor, calls rt_gc_point_arr, then calls plw_zh_mark_push -> plw_cw_mark_push -> rt_pl_cellws_on and rt_zeta_mode, EVERY ONE of which early-returns in the default configuration.
 * That default is the whole rung: the cellws island is off unless SCRIP_PL_WS_RECLAIM=1, and the zh pair stack is live only under --zeta=zh, so in the shipped configuration the entire zh/cw push is a
 * PROVEN NO-OP and the leaf's observable effect is exactly `{DT_I, 0, (long long)g_pl_trail.top}`.  The inline arm proves that precondition at runtime rather than baking it (contract §3 — both cells are
 * runtime state, and --zeta= is a CLI flag the m4 compile cannot see): read g_plw_cellws_on (-1 unresolved / 0 off / 1 on) and reject anything but 0, so an UNRESOLVED cell defers to the leaf exactly like a
 * not-yet-interned dot_sl (correctness never depends on the cell being populated); then read g_zeta_mode LIVE and reject ZH(2).  Either guard failing -> SLOW with UNMODIFIED rdi=args, esi=0, bit-identical
 * by construction (contract §1: the guards run BEFORE any state is touched, and this arm never touches any).  The GC safepoint and unwind floor stay in the leaf per contract §4 — this arm allocates nothing,
 * writes nothing and cannot throw, so it is the same non-allocating class as SINK-1; skipping a safepoint only defers collection to the next one, and with zero args there is nothing to protect.  Result is
 * built in the rax:rdx return pair the call convention already uses: rax = q0 = {v=DT_I(6), slen=0}, rdx = the payload.  `top` is a 32-bit signed int widened by the C's (long long) cast, so the widening is
 * movsxd (via eax) and NOT a bare 32-bit mov — provably identical here since top is a count that only ever rises from 0 or resets to an earlier mark, but written exactly rather than resting on that proof.
 * Internal label ids 100..101 (SINK-1 owns 40..58, SINK-2 60..77, SINK-3 80..99; marshal owns idx*2, unused at nargs==0). */
static std::string sink_trail_mark_str(int argbase, uint64_t ufp, const char * usym) {
    std::string s = x86("comment", "PL-SINK-8 inline $trail_mark fast path: guards prove the zh/cw mark push is a no-op, then mark = g_pl_trail.top; rt_pl_dop_trail_mark is the slow-path oracle (unmodified args)");
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_cellws_on, "g_plw_cellws_on");
    s += x86("mov", "eax", "dword ptr [r10 + 0]");
    s += x86("test", "eax", "eax");
    s += x86_jcc_id("jne", 100);
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)&g_zeta_mode, "g_zeta_mode");
    s += x86("mov", "eax", "dword ptr [r10 + 0]");
    s += x86("cmp", "eax", (long)2);
    s += x86_jcc_id("je", 100);
    s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += x86("mov", "eax", "dword ptr [r10 + 32]");
    s += x86("movsxd", "rdx", "eax");
    s += x86("mov32", "eax", (long)6);
    s += x86_jmp_id(101);
    s += x86_deflabel_id(100);
    s += x86("lea", "rdi", FRQ(argbase));
    s += x86("mov32", "esi", (long)0);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(101);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-SINK-4 (2026-07-25) — EMITTED $ix_g SPECIALIZED INDEX GUARD.  Measured the HOTTEST un-sunk dop leaf by DYNAMIC count (PLT interposition; perf/gdb are absent in this container): nrev 1022 calls vs
 * unify 31 and mkc 30, qsort 649 vs 448/100 — and static site counts INVERT that ranking (mkc has 51 sites in qsort to ix_g's 5), so site counts must never be used to pick a rung.  rt_pl_dop_ix_g has NO
 * wrapper fast path and does NOT go through dop_call (no setjmp ceremony), so unlike the s147-falsified SINK-6/7 the cost really is the call + leaf body.  kk (index kind) and the key are EMIT-TIME LITs
 * (lower_prolog.c:837 builds args[1] as LIT_INTEGER kk|kar<<8 and args[2] as LIT int/string), so this emits a guard SPECIALIZED PER kk rather than the leaf's general switch.  Arms mirror dop_ix_g
 * (:1425) tag-for-tag; anything not inline-decidable falls into the UNTOUCHED leaf with UNMODIFIED rdi=args (esi=3), bit-identical by construction (contract §1).  MEASURED: args[0] is DT_N in 100% of
 * calls, the contract §2 trap that made SINK-1's first cut a NET LOSS — sink_deref is reused VERBATIM so the name-ref chase is faithful.  kk==3 reuses SINK-2's exported g_plw_dot_sl (==0 -> SLOW, so a
 * not-yet-interned run defers rather than mis-failing); kk==2 inlines the hot PLREF->FAIL / DT_I->OK tag arms and defers ONLY the atom-vs-atom strcmp; kk==1 compares against the emit-time imm; kk==4
 * (functor) takes NO sink here — it needs a per-site intern cache (contract §3) and is the follow-on rung.  NO NEW GLOBALS: the no_new_global floor does not move.  Internal label ids 110..120 (SINK-1
 * 40..58, SINK-2 60..77, SINK-3 80..99, SINK-8 100..101).  Kill switches: SCRIP_NO_SINK (family) + SCRIP_NO_SINK4 (this rung, per the s146 isolation amendment — the family switch CANNOT measure a rung). */
static std::string sink_ix_g_str(int argbase, uint64_t ufp, const char * usym, int kk, long long kival) {
    std::string s = x86("comment", "PL-SINK-4 inline $ix_g specialized guard (kk emit-time constant); rt_pl_dop_ix_g stays the slow-path oracle (unmodified args)");
    s += x86("lea", "rdi", FRQ(argbase));
    s += x86("lea", "r8",  FRQ(argbase));
    s += sink_deref("r8", 110, 111, 112);
    s += sink_unb("r8", 114, 118);
    s += x86_deflabel_id(118);
    s += x86("mov", "eax", "dword ptr [r8 + 0]");
    if (kk == 3) {
        s += x86("cmp", "eax", (long)14) + x86_jcc_id("jne", 119);
        s += x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_dot_sl, "g_plw_dot_sl");
        s += x86("mov", "edx", "dword ptr [r10 + 0]");
        s += x86("cmp", "edx", (long)0)  + x86_jcc_id("je", 116);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", "edx")    + x86_jcc_id("jne", 115);
        s += x86_jmp_id(114);
        s += x86_deflabel_id(119);
        s += x86("cmp", "eax", (long)6)  + x86_jcc_id("jne", 120);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", (long)0)  + x86_jcc_id("jne", 114);
        s += x86_jmp_id(115);
        s += x86_deflabel_id(120);
        s += x86("cmp", "eax", (long)1)  + x86_jcc_id("jne", 114);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", (long)0)  + x86_jcc_id("jne", 114);
        s += x86("mov", "rdx", "[r8 + 8]");
        s += x86("test", "rdx", "rdx")   + x86_jcc_id("je", 114);
        s += x86_jmp_id(115);
    } else if (kk == 2) {
        s += x86("cmp", "eax", (long)14) + x86_jcc_id("je", 115);
        s += x86("cmp", "eax", (long)6)  + x86_jcc_id("je", 114);
        s += x86("cmp", "eax", (long)1)  + x86_jcc_id("jne", 114);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", (long)0)  + x86_jcc_id("jne", 114);
        s += x86("mov", "rdx", "[r8 + 8]");
        s += x86("test", "rdx", "rdx")   + x86_jcc_id("je", 114);
        s += x86_jmp_id(116);
    } else {
        s += x86("cmp", "eax", (long)14) + x86_jcc_id("je", 115);
        s += x86("cmp", "eax", (long)6)  + x86_jcc_id("jne", 114);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", (long)0)  + x86_jcc_id("jne", 114);
        s += x86_movabs_r64("rdx", (uint64_t)kival);
        s += x86("mov", "rsi", "[r8 + 8]");
        s += x86("cmp", "rsi", "rdx")    + x86_jcc_id("jne", 115);
        s += x86_jmp_id(114);
    }
    s += x86_deflabel_id(114);
    s += x86("mov32", "eax", (long)6);
    s += x86("mov32", "edx", (long)1);
    s += x86_jmp_id(117);
    s += x86_deflabel_id(115);
    s += x86("mov32", "eax", (long)99);
    s += x86("mov32", "edx", (long)0);
    s += x86_jmp_id(117);
    s += x86_deflabel_id(116);
    s += x86("mov32", "esi", (long)3);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(117);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_result_slot(IR_t * nd) {
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn = _.op_sval ? _.op_sval : "";
    int nargs = (int) _.op_ival;
    /* ZD-7 (c): ZD arm for bare IR_CALL (builtins via rt_call_arr, non-registered procs excluded by zd_wl_kind).
     * Alpha already carved K=16 for the result cell (op_fc_bytes=16 set by zd_plan).
     * Args are read from predecessor ZD cells via ZOPQ(i,0/8), written into arg scratch on stack, then rt_call_arr called.
     * rsi = &args[0] via x86_reg_disp32_lea64 DIRECTLY -- bypassing x86() parser which adds +op_zdepth to RSP operands.
     * Result stored to ZRES(0/8); omega on DT_FAIL; gamma + beta_trampoline (det leaf, no re-entry). */
    if (_.op_zres) {
        std::string s = x86_alpha()
                      + x86("comment", std::string("BOX IR_CALL ZD-7 ") + fn + "(...) -> rt_call_arr [ZD: args from ZOPQ, result to ZRES]");
        if (nargs > 0) {
            s += x86("sub", "rsp", (long)(nargs * 16));
            for (int i = 0; i < nargs; i++) {
                s += x86("mov", "r10", (std::string("[rsp + ") + std::to_string(_.op_zread[i] + nargs * 16 + 0) + "]").c_str());
                s += x86("mov", "r11", (std::string("[rsp + ") + std::to_string(_.op_zread[i] + nargs * 16 + 8) + "]").c_str());
                s += x86("mov", (std::string("[rsp + ") + std::to_string(i * 16 + 0) + "]").c_str(), "r10");
                s += x86("mov", (std::string("[rsp + ") + std::to_string(i * 16 + 8) + "]").c_str(), "r11");
            }
        }
        /* ZD-PL-A (s163): THE ZD ARM IS A STORAGE FLAVOR, NOT A DISPATCH ROUTE.  Until this rung the ZD arm above early-returned to a hard-wired rt_call_arr by-name call, so an ARMED call could never reach the dop/sink dispatch at the legacy arm below (measured s163 by line: ZD arm returns 498, dop_direct_fp first consulted 507, the four PL-SINK arms 553-559).  That conflation is FREE in SNOBOL4 -- rt_call_arr IS its dispatch -- and expensive in Prolog, whose entire data plane rides dop: emitted nrev.s counts 65 call rt_pl_dop_* against 2 rt_call_arr, so admitting IR_CALL_BUILTIN_PROLOG into zd_wl_kind while this arm chose its own route would have discarded PL-SINK-1/2/4/8 + PL-REGAIN-5 and stayed GREEN on every correctness gate (rt_call_arr is correct, only slow) -- a silently-green perf regression, the worst class.  The gate consulted here is the SAME dop_direct_fp the legacy arm consults, so the dispatch DECISION is spelled once; only the STORAGE differs (args from this box's own rsp scratch built out of the ZOPQ predecessor cells rather than FRQ(argbase), result to ZRES rather than FRQ(resoff)).  Same law as ONE MEDIUM, INVISIBLE, applied to storage instead of medium.  INERT FOR SNOBOL4 BY CONSTRUCTION: dop_direct_fp's table is 100% Prolog $-builtins, so a SNOBOL4 callee never matches, zdfp stays 0, and the rt_call_arr block below is reached verbatim -- which is the positive control this rung is verified against (SNOBOL4 .s byte-identical).  nargs==0 is EXCLUDED deliberately: the only 0-arity dop is $trail_mark, and with no sub rsp there is no scratch array to point rdi at, so it keeps the by-name path until its own rung ($mkc is ar=-1/narg>=1 and is covered).  The INLINE sinks (sink_unify2_str et al) still live only on the legacy arm because they address operands through FRQ; lifting them needs the addressing parameterized, which is ZD-PL-A slice 2, not this slice. */
        const char * zdsym = 0; void * zdfp = (nargs > 0) ? dop_direct_fp(fn, (int64_t)nargs, &zdsym) : (void *)0;
        if (zdfp) {
            s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf under ZD: ") + zdsym + " (no by-name dispatch)").c_str());
            s += x86_reg_disp32_lea64("rdi", "rsp", 0);
            s += x86("mov32", "esi", (long)nargs);
            s += x86("call", zdsym, (uint64_t)(uintptr_t)zdfp);
        } else {
        {
            std::string fl = std::string(".Lrkfnzd") + std::to_string(g_flat_node_id++);
            s += x86("directive", ".section .rodata");
            s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
            s += x86("directive", ".section .text");
            s += x86("directive", ".intel_syntax noprefix");
            s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        }
        /* lea rsi, [rsp + 0]: use encoder directly -- x86("lea","rsi","qword ptr [rsp + 0]") adds +op_zdepth via RSP operand path */
        if (nargs > 0) s += x86_reg_disp32_lea64("rsi", "rsp", 0);
        else           s += x86("xor", "esi", "esi");
        s += x86("mov32", "edx", (long)nargs);
        s += x86("call", "rt_call_arr", (uint64_t)(uintptr_t)(void *)rt_call_arr);
        }
        if (nargs > 0) s += x86("add", "rsp", (long)(nargs * 16));
        s += x86("cmp", "eax", (long)99);
        s += x86_omega("je");
        s += x86("note", ZRESN()) + x86("mov", ZRES(0), "rax");
        s += x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx");
        s += x86_gamma();
        s += x86_beta_trampoline();
        return s;
    }
    int resoff = bcfn_result_slot(pBB);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_fn: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && nargs > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_fn: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = resoff + 16;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    std::string s = x86_alpha()
                  + x86("comment", std::string("BOX IR_CALL ") + fn + "(...) -> rt_call_arr [operand-marshal, FAIL->ω]");
    const char * dsym = 0; void * dfp = dop_direct_fp(fn, (int64_t)nargs, &dsym);
    /* PL-REGAIN-5 (2026-07-19): const head-unify — when one $unify side is an emit-time LIT (int / atom-as-LIT_STRING, the lower_prolog shapes), the const rides in a REGISTER and only the other side is
     * marshaled (to args[0], where the ci/cs leaf expects it).  Eligibility is emit-time-static and rides UNDER the dop hatches (SCRIP_NO_DOP / SCRIP_DOP_SKIP=unify disable it with the leaf).  The RO
     * string uses the LIT_S arm's in-band seal idiom verbatim (both-media); the skipped arg's seal ids (cui*2, cui*2+1) are free exactly because its LIT marshal is skipped.  Producer boxes still emit;
     * only the site-side copy and the DESCR-pair operand die. */
    int cui = -1; long long cival = 0; const char * csval = 0;
    if (dfp && nargs == 2 && !strcmp(fn, "$unify") && !getenv("SCRIP_NO_CU")) {
        for (int i = 0; i < 2 && cui < 0; i++) {
            IR_t * lf = (subs && subs[i]) ? subs[i]->entry : ir_call_arg(pBB, i);
            if (!lf) continue;
            if (lf->op == IR_LIT_INTEGER) { cui = i; cival = (long long)IR_LIT(lf).ival; }
            else if (lf->op == IR_LIT_STRING && IR_LIT(lf).sval) { cui = i; csval = IR_LIT(lf).sval; }
        }
    }
    int ix_kk = 0; long long ix_kival = 0;
    if (dfp && nargs == 3 && !strcmp(fn, "$ix_g")) {
        IR_t * a1 = (subs && subs[1]) ? subs[1]->entry : ir_call_arg(pBB, 1);
        IR_t * a2 = (subs && subs[2]) ? subs[2]->entry : ir_call_arg(pBB, 2);
        if (a1 && a1->op == IR_LIT_INTEGER) {
            int kk = (int)(((long long)IR_LIT(a1).ival) & 0xFF);
            if (kk == 3) ix_kk = 3;
            else if (kk == 2) ix_kk = 2;
            else if (kk == 1 && a2 && a2->op == IR_LIT_INTEGER) { ix_kk = 1; ix_kival = (long long)IR_LIT(a2).ival; }
        }
    }
    if (cui >= 0) {
        int vi = 1 - cui;
        s += marshal_call_arg((subs && subs[vi]) ? subs[vi]->entry : ir_call_arg(pBB, vi), (subs && subs[vi]) ? subs[vi] : NULL, argbase, _.node, vi);
    } else {
        for (int i = 0; i < nargs; i++)
            s += marshal_call_arg((subs && subs[i]) ? subs[i]->entry : ir_call_arg(pBB, i), (subs && subs[i]) ? subs[i] : NULL, argbase + i * 16, _.node, i);
    }
    if (dfp && cui >= 0) {
        s += x86("comment", (std::string("PL-REGAIN-5 const head-unify leaf: ") + (csval ? "rt_pl_dop_unify_cs" : "rt_pl_dop_unify_ci") + " (const in reg, one-operand marshal)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        if (csval) {
            s += x86("mov", "rsi", ROQ(cui * 2));
            s += x86_jmp_id(cui * 2 + 1);
            s += x86_ro_seal_str(cui * 2, csval);
            s += x86_deflabel_id(cui * 2 + 1);
            s += x86("call", "rt_pl_dop_unify_cs", (uint64_t)(uintptr_t)(void *)rt_pl_dop_unify_cs);
        } else {
            s += x86_movabs_r64("rsi", (uint64_t)cival);
            s += x86("call", "rt_pl_dop_unify_ci", (uint64_t)(uintptr_t)(void *)rt_pl_dop_unify_ci);
        }
    } else if (dfp && nargs == 2 && !strcmp(fn, "$unify") && !getenv("SCRIP_NO_SINK")) {
        s += sink_unify2_str(argbase, (uint64_t)(uintptr_t)dfp, dsym);
    } else if (dfp && nargs == 3 && !strcmp(fn, "$unify_lst") && !getenv("SCRIP_NO_SINK")) {
        s += sink_unify_lst_str(argbase, (uint64_t)(uintptr_t)dfp, dsym);
    } else if (dfp && nargs == 0 && !strcmp(fn, "$trail_mark") && !getenv("SCRIP_NO_SINK") && !getenv("SCRIP_NO_SINK8")) {
        s += sink_trail_mark_str(argbase, (uint64_t)(uintptr_t)dfp, dsym);
    } else if (dfp && nargs == 3 && !strcmp(fn, "$ix_g") && ix_kk > 0 && !getenv("SCRIP_NO_SINK") && !getenv("SCRIP_NO_SINK4")) {
        s += sink_ix_g_str(argbase, (uint64_t)(uintptr_t)dfp, dsym, ix_kk, ix_kival);
    } else if (dfp) {
        s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf: ") + dsym + " (no by-name dispatch)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        s += x86("mov32", "esi", (long)nargs);
        s += x86("call", dsym, (uint64_t)(uintptr_t)dfp);
    } else {
        std::string fl = std::string(".Lrkfn") + std::to_string(g_flat_node_id++);
        s += x86("directive", ".section .rodata");
        s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
        s += x86("directive", ".section .text");
        s += x86("directive", ".intel_syntax noprefix");
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        s += x86("lea", "rsi", FRQ(argbase));
        s += x86("mov32", "edx", (long)nargs);
        s += x86("call", "rt_call_arr", (uint64_t)(uintptr_t)(void *)rt_call_arr);
    }
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "eax", (long)99);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86_omega();
    return s;
}

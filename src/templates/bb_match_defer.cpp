#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" long  rt_defer_open     (const char *varname, int ival_flag);
extern "C" long  rt_defer_step     (DESCR_t fret);
extern "C" int   rt_defer_close    (int cur_delta);
extern "C" void *rt_frame_prep     (void *fb, long fbytes);
extern "C" void *rt_defer_get_pat_fn(const char *varname, int ival_flag);
extern "C" void *rt_zls_alloc      (long bytes);
extern "C" long  rt_fn_frame_bytes (void *fn);
extern "C" void  rt_zls_release    (void *fb);
extern "C" const char *g_dcap_top;
#include "x86_asm.h"
static inline int dscr() { return _.x86_scratch_off; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    return ( ((_.bb_child_fn != (void *)0) || (_.bb_child_lbl && _.bb_child_lbl[0]))
        ? ( x86("comment", "IR_MATCH_DEFER inlined frozen head (FZ-5b)")
          + x86_alpha()
          + x86("lea",  "rax", "[rip + __]", (uint64_t)(uintptr_t)_.bb_child_fn, _.bb_child_lbl ? _.bb_child_lbl : "") )
        : ( x86("comment", "IR_MATCH_DEFER")
          + x86_alpha()
          + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
          + x86("xor",  "esi", "esi")
          + x86_align_enter()
          + x86("call", "rt_defer_get_pat_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *, int))rt_defer_get_pat_fn)
          + x86_align_leave() ) )
         + x86("xor",  "ecx", "ecx")
         + x86("mov",  FRQ(dscr()), "rcx")
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + x86("mov",  FRQ(dscr()), "rax")
         + x86_align_enter()
         + x86("mov",  "rdi", FRQ(dscr()))
         + x86("call", "rt_fn_frame_bytes", (uint64_t)(uintptr_t)(void *)(long (*)(void *))rt_fn_frame_bytes)
         + x86("mov",  "rdi", "rax")
         + x86("call", "rt_zls_alloc", (uint64_t)(uintptr_t)(void *)(void *(*)(long))rt_zls_alloc)
         + x86_align_leave()
         + x86("mov",  FRQ(dscr() + 8), "rax")
         + x86("mov",  "rcx", FRQ(dscr()))
         + x86("mov",  "rdi", "rax")
         + x86("xor",  "esi", "esi")
         + x86("call", "rcx")
         + x86("cmp",  "eax", (long)1)
         + x86("je",   "L1")
         + x86("mov",  "rdi", FRQ(dscr() + 8))
         + x86_align_enter()
         + x86("call", "rt_zls_release", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release)
         + x86_align_leave()
         + x86("xor",  "eax", "eax")
         + x86("mov",  FRQ(dscr()), "rax")
         + x86_omega()
         + x86("def",  "L1")
         + x86_gamma()
         + x86("def",  "L0")
         /* NCB-1c M1 (2026-07-11): the *X / DT_X transfer is EMITTED — open returns fbytes (a call is owed),
          * the loop performs it with the NCB-1b window, step says whether a second is owed (the DT_X round),
          * close does the string compare against Σ at the cursor.
          * ⛔ r14/r15/r13 ARE SAVED ACROSS THE TRANSFER (r14d = match cursor, r15d = matcher scratch): an
          * xa_flat callee preserves only its frame reg (+ optional display reg), so an emitted BB→BB call does
          * NOT honor SysV callee-saved for r13/r14/r15.  The old C trampoline hid this by accident of GCC's own
          * register allocation.  Any transfer with a LIVE matcher cursor must save them itself — M3 and NCB-2's
          * generator arms included. */
         + x86_xfer_enter()
         /* rbp-dcap mirror-out: the callout pump below transfers into proc bodies that may run their own
          * matches — their heads load g_dcap_top and must see the live top (match-family boxes only; the
          * shared x86_frame_sink stays clean because non-SNOBOL graphs carry a non-cursor rbp). */
         + x86("mov",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_dcap_top, "g_dcap_top")
         + x86("mov",  RDQ("rcx", 0), "rbp")
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("xor",  "esi", "esi")
         + x86_anchor_enter()
         + x86("call", "rt_defer_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int))rt_defer_open)
         + x86("def",  "L2")
         + x86("test", "rax", "rax")
         + x86("je",   "L3")
         + x86_frame_sink()
         + x86_frame_base("rdi")
         + x86("mov",  "rsi", "rax")
         + x86("call", "rt_frame_prep", (uint64_t)(uintptr_t)(void *)(void *(*)(void *, long))rt_frame_prep)
         + x86_frame_base("rdi")
         + x86("xor", "esi", "esi")
         + x86("call", "rax")
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86_frame_unsink()
         + x86("call", "rt_defer_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_defer_step)
         + x86("jmp",  "L2")
         + x86("def",  "L3")
         + x86_anchor_leave()
         + x86_xfer_leave()
         + x86("mov",  "edi", "r14d")
         + x86_align_enter()
         + x86("call", "rt_defer_close", (uint64_t)(uintptr_t)(void *)(int (*)(int))rt_defer_close)
         + x86_align_leave()
         + x86("test", "eax", "eax")
         + x86_omega("js")
         + x86("mov",  "r14d", "eax")
         + x86_gamma()
         /* NCB-2/SZ-1 (FINDING-2026-07-10 blob-β-resume): β re-enters the STORED blob's own β via its esi=1
          * prologue dispatch — "the element right of the pattern failed" seen from inside the blob, so its
          * interior generator (ARBNO) extends from saved state in the kept-alive frame.  jz→ω guards the
          * callout/exhausted activations (fn slot zeroed at α and on release).  eax==1 rejoins γ at L1. */
         + x86_beta()
         + x86("mov",  "rcx", FRQ(dscr()))
         + x86("test", "rcx", "rcx")
         + x86_omega("jz")
         + x86("mov",  "rdi", FRQ(dscr() + 8))
         + x86("mov32", "esi", (long)1)
         + x86("call", "rcx")
         + x86("cmp",  "eax", (long)1)
         + x86("je",   "L1")
         + x86("mov",  "rdi", FRQ(dscr() + 8))
         + x86_align_enter()
         + x86("call", "rt_zls_release", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release)
         + x86_align_leave()
         + x86("xor",  "eax", "eax")
         + x86("mov",  FRQ(dscr()), "rax")
         + x86_omega();
}

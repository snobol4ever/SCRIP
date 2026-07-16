#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" long  rt_defer_open     (const char *varname, int ival_flag);
extern "C" long  rt_defer_step     (DESCR_t fret);
extern "C" int   rt_defer_close    (int cur_delta);
extern "C" void *rt_proc_open_fn   (void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
extern "C" uint64_t g_patstk_sp;
extern "C" uint64_t g_pat_main_rsp;
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
extern "C" void *rt_defer_get_pat_fn(const char *varname, int ival_flag);
extern "C" const char *g_dcap_top;
extern "C" uint64_t g_rspd_save, g_rspd_g4, g_rspd_g5, g_rspd_s2, g_rspd_g6, g_rspd_beta;
#include "x86_asm.h"
static inline int dswap() { return ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat; }
static inline int rspd()  { static int v = -1; if (v < 0) v = getenv("SCRIP_RSPDIFF") ? 1 : 0; return v; }
static inline std::string rspd_snap(uint64_t *cell, const char *nm) { return IF(rspd(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)cell,nm) + x86("mov",RDQ("rcx",0),"rsp")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    /* ZS-2 (Lon s58, FINDING-2026-07-14 §5-CORRECTED): the *P transfer is a JUMP OUT and a JUMP BACK — no
     * call/ret, no eax code, no caller-side allocation.  α resolves the blob fn (a C LOOKUP, not the transfer),
     * wires the OUTSIDE γ (rcx) and OUTSIDE ω (rdx) landing labels, and jmps to blob_α.  The blob is a NEW
     * ACTIVATION: it self-allocates its own rsp frame with a 32B wire header, γ-SUSPENDS it (jmp back through
     * the γ wire, frame retained for β resumes), ω-unwinds it (add rsp,K; jmp back through the ω wire).  The
     * OUTSIDE backtrack edge is uniformly `jmp qword [rsp+0]` — the LIFO law has rsp at the suspended
     * activation's own base, whose [+0] wire holds its resume landing.  Recursion (*LIST) nests activations on
     * the stream; the s58 T2 static-slot clobber is unrepresentable — there are no slots.  The callout pump
     * (fn NULL: *X / DT_E-to-any) keeps its NCB-1c machinery and suspends a 16B one-shot cell whose [+0] wire
     * lands its own exhaust stub, so the shared β needs no discrimination and no guard. */
    return x86("comment", "IR_MATCH_DEFER (ZS-2 jmp-entry)")
         + x86_alpha()
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("xor",  "esi", "esi")
         + x86_align_enter()
         + x86("call", "rt_defer_get_pat_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *, int))rt_defer_get_pat_fn)
         + x86_align_leave()
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + IF(dswap(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_pat_main_rsp,"g_pat_main_rsp")+x86("mov",RDQ("rcx",0),"rsp")+x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_patstk_sp,"g_patstk_sp")+x86("mov","rsp",RDQ("rcx",0)))
         + IF(dswap(), rspd_snap(&g_rspd_save, "g_rspd_save"))
         + x86_lea_id("rcx", 4)
         + x86_lea_id("rdx", 5)
         + x86_jmp_reg("rax")
         + x86("def",  L(4))
         + IF(dswap(), rspd_snap(&g_rspd_g4, "g_rspd_g4"))
         + IF(dswap(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_patstk_sp,"g_patstk_sp")+x86("mov",RDQ("rcx",0),"rsp")+x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_pat_main_rsp,"g_pat_main_rsp")+x86("mov","rsp",RDQ("rcx",0)))
         + x86_gamma()
         + x86("def",  L(5))
         + IF(dswap(), rspd_snap(&g_rspd_g5, "g_rspd_g5"))
         + IF(dswap(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_patstk_sp,"g_patstk_sp")+x86("mov",RDQ("rcx",0),"rsp")+x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_pat_main_rsp,"g_pat_main_rsp")+x86("mov","rsp",RDQ("rcx",0)))
         + x86_omega()
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
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         + IF(!dswap(), x86("push","r12")+x86("sub","rsp",8L))
         + x86_lea_id("rcx",7)
         + x86_lea_id("rdx",8)
         + IF(!dswap(), x86("mov","r12","rsp"))
         + x86_jmp_reg("rax")
         + x86("def",  L(7))
         + IF(!dswap(), x86("mov","rax","rsp")+x86("mov","rax",RDQ("rax",8))+x86("mov","rdi",RDQ("rax",0))+x86("mov","rsi",RDQ("rax",8))+x86("mov","rsp","r12")+x86("add","rsp",8L)+x86("pop","r12"))
         + x86("call","rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_defer_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_defer_step)
         + x86("jmp",  "L2")
         + x86("def",  L(8))
         + IF(!dswap(), x86("mov","rsp","r12")+x86("add","rsp",8L)+x86("pop","r12"))
         + x86("call","rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
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
         /* callout success: suspend a 16B one-shot cell — [+0] wire = the exhaust stub below — so the shared β
          * re-enters it exactly like a blob activation and the exhaust falls to ω (the old fn==0 jz guard's
          * job, now wired instead of tested). */
         + x86_lea_id("rax", 6)
         + IF(dswap(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_pat_main_rsp,"g_pat_main_rsp")+x86("mov",RDQ("rcx",0),"rsp")+x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_patstk_sp,"g_patstk_sp")+x86("mov","rsp",RDQ("rcx",0)))
         + IF(dswap(), rspd_snap(&g_rspd_s2, "g_rspd_s2"))
         + x86_sub("rsp", 8)
         + x86("push", "rax")
         + IF(dswap(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_patstk_sp,"g_patstk_sp")+x86("mov",RDQ("rcx",0),"rsp")+x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_pat_main_rsp,"g_pat_main_rsp")+x86("mov","rsp",RDQ("rcx",0)))
         + x86_gamma()
         + x86("def",  L(6))
         + x86_add("rsp", 16)
         + IF(dswap(), rspd_snap(&g_rspd_g6, "g_rspd_g6"))
         + IF(dswap(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_patstk_sp,"g_patstk_sp")+x86("mov",RDQ("rcx",0),"rsp")+x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_pat_main_rsp,"g_pat_main_rsp")+x86("mov","rsp",RDQ("rcx",0)))
         + x86_omega()
         /* β: the LIFO law has rsp at the suspended activation's own base — jump through its [+0] wire (blob:
          * the chain's %s_res landing re-pins the frame reg and resumes; callout: the exhaust stub above). */
         + x86_beta()
         + IF(dswap(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_pat_main_rsp,"g_pat_main_rsp")+x86("mov",RDQ("rcx",0),"rsp")+x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)&g_patstk_sp,"g_patstk_sp")+x86("mov","rsp",RDQ("rcx",0)))
         + IF(dswap(), rspd_snap(&g_rspd_beta, "g_rspd_beta"))
         + x86_jmp_mem("rsp", 0);
}

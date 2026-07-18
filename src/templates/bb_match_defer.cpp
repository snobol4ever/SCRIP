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
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
extern "C" void *rt_defer_get_pat_fn(const char *varname, int ival_flag);
extern "C" void *dtp_fn_of(void *headv);
extern int g_gva_active;
extern "C" uint64_t g_rspd_save, g_rspd_g4, g_rspd_g5, g_rspd_s2, g_rspd_g6, g_rspd_beta;
#include "x86_asm.h"
static inline int dswap() { return ZC_FRAME == ZC_FRAME_RSP; }   /* REG-7 U5: unconditional under RSP — the interior legacy dance is non-RSP-only now (Lon FORTH ruling) */
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
     * lands its own exhaust stub, so the shared β needs no discrimination and no guard.
     * ZB-ITER-3 (s85, per the s84 plan amendment): the g_patstk ISLAND SWAP around the non-flat blob/pump
     * paths is RETIRED — the blob self-allocates BELOW the caller frontier on the ONE rsp stream and suspends
     * at γ there (the s59 pure protocol restored at the outer boundary).  Every β arrival has rsp AT the
     * newest frontier record (LIFO law: σ null-progress arrives right after the suspend; bb_match_arbno's φ
     * pop lea lands exactly on the previous iteration's record), so `jmp [rsp+0]` needs no resume slot —
     * amendment (d) measured.  Statement brackets already discard one-stream residue on both exits (S10e).
     * The dswap() !arms below are non-RSP-only as of U5 s87 (the flat_pat island is retired): under RSP the pure one-stream protocol is unconditional. */
    return x86("comment", "IR_MATCH_DEFER (ZS-2 jmp-entry)")
         + x86_alpha()
         + IF(g_gva_active && _.op_gva_k >= 0,
               x86("mov",  "rax", ABSQ(RT_GVA_VA + _.op_gva_k * 16))
             + x86("mov",  "rdx", ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
             + x86("cmp",  "eax", (long)DT_P)
             + x86("jne",  L(9))
             + x86("mov",  "rdi", "rdx")
             + x86_align_enter()
             + x86("call", "dtp_fn_of", (uint64_t)(uintptr_t)(void *)(void *(*)(void *))dtp_fn_of)
             + x86_align_leave()
             + x86("jmp",  L(10))
             + x86("def",  L(9))
             + x86("xor",  "eax", "eax")
             + x86("def",  L(10)))
         + IF(!(g_gva_active && _.op_gva_k >= 0),
               x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("xor",  "esi", "esi")
             + x86_align_enter()
             + x86("call", "rt_defer_get_pat_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *, int))rt_defer_get_pat_fn)
             + x86_align_leave())
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + rspd_snap(&g_rspd_save, "g_rspd_save")
         + x86_lea_id("rcx", 4)
         + x86_lea_id("rdx", 5)
         + x86_jmp_reg("rax")
         + x86("def",  L(4))
         + rspd_snap(&g_rspd_g4, "g_rspd_g4")
         + x86_gamma()
         + x86("def",  L(5))
         + rspd_snap(&g_rspd_g5, "g_rspd_g5")
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
         /* REG-6 (was REG-2): no mirror-out — r12 IS the live top and rides the callee-saved contract through
          * the resolve C call and into the deferred pattern's nested execution, whose own head/release keep
          * it LIFO-balanced back to this box's view. */
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("xor",  "esi", "esi")
         + x86_anchor_enter()
         + x86("call", "rt_defer_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int))rt_defer_open)
         + x86("def",  "L2")
         + x86("test", "rax", "rax")
         + x86("je",   "L3")
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         + IF(!dswap(), x86("push",x86_zr())+x86("sub","rsp",8L))
         + x86_lea_id("rcx",7)
         + x86_lea_id("rdx",8)
         + IF(!dswap(), x86("mov",x86_zr(),"rsp"))
         + x86_jmp_reg("rax")
         + x86("def",  L(7))
         + IF(!dswap(), x86("mov","rax","rsp")+x86("mov","rax",RDQ("rax",8))+x86("mov","rdi",RDQ("rax",0))+x86("mov","rsi",RDQ("rax",8))+x86("mov","rsp",x86_zr())+x86("add","rsp",8L)+x86("pop",x86_zr()))
         + x86("call","rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_defer_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_defer_step)
         + x86("jmp",  "L2")
         + x86("def",  L(8))
         + IF(!dswap(), x86("mov","rsp",x86_zr())+x86("add","rsp",8L)+x86("pop",x86_zr()))
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
         + rspd_snap(&g_rspd_s2, "g_rspd_s2")
         + x86_sub("rsp", 8)
         + x86("push", "rax")
         + x86_gamma()
         + x86("def",  L(6))
         + x86_add("rsp", 16)
         + rspd_snap(&g_rspd_g6, "g_rspd_g6")
         + x86_omega()
         /* β: the LIFO law has rsp at the suspended activation's own base — jump through its [+0] wire (blob:
          * the chain's %s_res landing re-pins the frame reg and resumes; callout: the exhaust stub above). */
         + x86_beta()
         + rspd_snap(&g_rspd_beta, "g_rspd_beta")
         + x86_jmp_mem("rsp", 0);
}

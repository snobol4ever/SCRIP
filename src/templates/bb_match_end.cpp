#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void rt_zls_release_to(void *mark);
extern "C" void rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t capgen);
extern "C" long rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj);
extern "C" long rt_dcap_step(DESCR_t fret);
/* CAS-SENTINEL-CLEAN: g_patstk_sp extern removed */
extern "C" void rt_dcap_end_ok_close(void);
extern "C" void *rt_proc_open_fn(void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
extern "C" long zvo_owner_dout(int cur_head);
#include "x86_asm.h"
#define rfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0)
#define hfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0)   /* M-2: mirrors bb_match_begin's hfc() -- true when MATCH_BEGIN allocated the 80B hfc sentinel+PATCTX region */
#define stfh() (_.flat_stmt_frame)
static const char * HKQ(int k) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rbp + %d]", -48 + 8 * k); return b[i]; }   /* SUBJ-ARM-2: the head's statement-bracket rbp housekeeping slots (bb_match_begin.cpp twin, same k map: 0=deep-rbp 1=r13 2=r14 3=r15 4=capgen) -- RELEASE runs inside the SAME bracket, so [rbp-48+8k] is depth-free at the post-unwind read exactly as at the head's alpha write */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string release_pump() {
    return std::string()
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("mov",  "rsi", "r12")   /* CAS-R12-UNIFY: top IS r12 -- cell read deleted.  CAS-MARKER: mark is recovered by scanning down to HEAD's tag-0 marker (r10 walks a COPY; r12 must stay at top -- the pump consumes the entries above the marker); one config-blind mechanism serves every basis */
         + x86("mov",  "r10", "rsi")
         + x86("def",  L(5))
         + x86("sub",  "r10", (long)24)
         + x86("mov",  "rax", RDQ("r10", 0))
         + x86("test", "rax", "rax")
         + x86("jne",  L(5))
         + x86("lea",  "rdi", RDQ("r10", 24))
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         + x86("def",  L(1))
         + x86("test", "rax", "rax")
         + x86("je",   L(2))
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         /* ZW-0 stage 2: island push/restore arms deleted -- unreachable under ZC_FRAME_RSP default */
         + bb_glue_pass_wires(3, 4)   /* GLUE-SYM (s22x): dormant legacy anchor hoisted above the glue; byte-identical at the ZC_FRAME_RSP default */
         + x86("def",  L(3))
         /* ZW-0 stage 2: island rsp/zr dance deleted */
         + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(4))
         /* ZW-0 stage 2: island rsp/zr dance deleted */
         + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(2))
         + x86("call", "rt_dcap_end_ok_close", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_end_ok_close)
         + x86_anchor_leave()
         + x86_xfer_leave()
         + x86("note", "cas_mark") + x86("sub", "r12", (long)24)   /* ⭐ W-1c.3 NO-SCAN L(6): post-pump LIFO arithmetic -- pump has consumed every entry via rt_dcap_step; nested matches inside pumped assignments push balanced markers so r12 returns at top callee-saved; one sub lands on the marker, assignment IS the wholesale pop. L(5) above is the structural scan (range-locator before pump runs, count runtime-variable); it is the only real scan left in this template. */
         + x86("note", HKN(1)) + x86("mov", "r13", stfh() ? HKQ(1) : FRQ(_.op_off + 48))   /* M-2 BUG-6 FIX: PATCTX-DESCENT sub rsp,80 deleted. After CAS restore rsp=cas_rsp_mark=pre-32B-carve rsp (MATCH_BEGIN stores cas_rsp_mark BEFORE x86_zclaim(32), and saves r13/r14/r15/rbp also pre-carve at [rsp+48/56/64/40]). No descent needed: FRQ(op_off+48) resolves to [rsp+48] = the exact save address. The prior 80B bracket was left over from a reverted HFC-CLAIM REORDER (9368fac6) that would have done sub rsp,80 BEFORE the saves; that reorder was superseded but the MATCH_END descent/ascent bracket was never removed, causing the 32R crash population. */   /* PATCTX restore on success -- AFTER the pump, which still needs the INNER Σ (rt_dcap_end_ok_open's rdx) and may itself run nested matches that push/pop their own saves LIFO.  The end cursor was already stashed at +24 before r14 is overwritten.  M-2: with cas_rsp_mark=pre-sub, sub rsp,80 above re-establishes the slot base. */
         + x86("note", HKN(2)) + x86("mov", "r14", stfh() ? HKQ(2) : FRQ(_.op_off + 56))
         + x86("note", HKN(3)) + x86("mov", "r15", stfh() ? HKQ(3) : FRQ(_.op_off + 64))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)   /* re-sync the C-side Σ/Σlen mirror; CAPGEN-ERAD: arg3 dropped, restore no longer writes g_cap_gen */
         + IF(_.op_dval == 0.0 && _.flat_deep_arrival, x86("note", HKN(0)) + x86("mov", "rbp", stfh() ? HKQ(0) : FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): paired with head's gated +40 save.  HEAD-PIN (s22z): under the pin the restore rides the terminal cut instead.  M-2 BUG-6 NOTE: !op_tail exclusion removed — PAIR(2) RBPRAWD reads (bb_match_arbno.cpp PAIR(2)) fire BEFORE MATCH_END α; by the time release_pump runs, PAIR(2) is complete and HEAD-PIN rbp is safe to restore.  Tail path does NOT restore at ARBNO omega (L(2) only runs on full exhaust, not on first-match-success). */
         /* M-2 BUG-6 FIX: PATCTX-ASCENT add rsp,80 deleted (paired with DESCENT deletion above). After PATCTX restores rsp remains at pre-32B-carve level; STATEMENT_END's add rsp,16 releases the subject var cell correctly. */
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_end() {
    x86_begin();
    return !PLATFORM_X86 ? std::string()
         : _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_END: head slot not resolved (operand[0] missing or unowned)")
         : _.op_tail && rfc()
         ? x86("comment", "IR_MATCH_END (CAS-MARKER-CARRY tail: scan to the head's tag-0 sentinel, recover patstk (+16) and the rsp mark (+8) off it, one-mov unwind -- depth-free on every success-path depth, where the old RSP(op_fc_disp) reloads under-counted the live leaf cells the non-popping γ spine leaves (the 041 class: [rsp+16] read the assign_save cell, rsp := 0x7fff00000000).  Marker NOT popped here -- the pump walks the pend entries above it and its own L(6) scan pops the lot)")
         + x86_alpha()
         + x86("mov", "r10", "r12")   /* CAS-R12-UNIFY: seed the recovery scan from r12 (the one authority); r10 walks a COPY -- marker NOT popped here, the pump's L(6) scan pops the lot */
         + x86("def", L(8))
         + x86("sub", "r10", (long)24)
         + x86("mov", "rax", RDQ("r10", 0))
         + x86("test", "rax", "rax")
         + x86("jne", L(8))
         + x86("mov", "rsp", RDQ("r10", 8))   /* CAS-SENTINEL-CLEAN: patstk restore from [r10+16] removed; rsp restore from [r10+8] kept */
         + release_pump()
         : x86("comment", "IR_MATCH_END")
         + x86_alpha()
         + IF(x86_zc_frame() == ZC_FRAME_RSP, x86("mov", "r10", "r12")   /* ⭐ W-1c.3 Part B: rfc/non-rfc fork DELETED -- both arms now scan via r10 (L(9)); the non-rfc slot read at FRQ(op_off+8) is removed (slot no longer written at alpha since the patstk slot-save was deleted from bb_match_begin).  r10 survives to the unwind below: rfc reads [r10+8] for rsp; non-rfc uses x86_zls2_release_to_call(op_off+16). */
                                               + x86("def", L(9))
                                               + x86("sub", "r10", (long)24)
                                               + x86("mov", "rax", RDQ("r10", 0))
                                               + x86("test", "rax", "rax")
                                               + x86("jne", L(9)))   /* CAS-SENTINEL-CLEAN: patstk restore from [r10+16] removed */
         + (_.op_dval != 0.0
                ? IF(rfc(), x86("mov", "eax", RDD("rsp", (int)_.op_fc_disp))
                          + (x86_zc_frame() == ZC_FRAME_RSP ? x86("mov", RDD("rsp", (int)(_.op_off + _.op_fc_disp + 32)), "eax")
                                                      : x86("mov", FR(_.op_off), "eax")))
                + (x86_zc_frame() == ZC_FRAME_RSP && rfc() ? x86("mov", RSP((int)(_.op_off + 24 + _.op_fc_disp + 32)), "r14")
                                                     : x86("mov", FRQ(_.op_off + 24), "r14"))
                : std::string())
         /* ZW-0 stage 2: island rt_zls_release_to arm deleted -- unreachable under ZC_FRAME_RSP default */
         + (rfc() ? x86("mov", "rsp", RDQ("r10", 8))   /* CAS-MARKER-CARRY unwind: depth-free; marker NOT popped -- the pump walks the pend entries above it and its L(6) scan pops the lot */
            : x86_zls2_release_to_call(_.op_off + 16))
         + x86_align_leave()
         + release_pump();
}

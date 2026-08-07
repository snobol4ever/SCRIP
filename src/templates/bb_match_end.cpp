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
#include "x86_asm.h"
#define rfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0)
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
         + x86("note", HKN(1)) + x86("mov", "r13", stfh() ? HKQ(1) : FRQ(_.op_off + 48))   /* PATCTX restore on success -- AFTER the pump, which still needs the INNER Σ (rt_dcap_end_ok_open's rdx) and may itself run nested matches that push/pop their own saves LIFO.  The end cursor was already stashed at +24 before r14 is overwritten. */
         + x86("note", HKN(2)) + x86("mov", "r14", stfh() ? HKQ(2) : FRQ(_.op_off + 56))
         + x86("note", HKN(3)) + x86("mov", "r15", stfh() ? HKQ(3) : FRQ(_.op_off + 64))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", stfh() ? HKQ(4) : FRQ(_.op_off + 72))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)   /* re-sync the C-side Σ/Σlen mirror */
         + IF(_.op_dval == 0.0 && _.flat_deep_arrival && !_.op_stmt_pin, x86("note", HKN(0)) + x86("mov", "rbp", stfh() ? HKQ(0) : FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): paired with head's gated +40 save.  HEAD-PIN (s22z): under the pin the restore rides the terminal cut instead -- see bb_match_begin's twin gate for the measured reason. */
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_end() {
    x86_begin();
    return !PLATFORM_X86 ? std::string()
         : _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_END: head slot not resolved (operand[0] missing or unowned)")
         : _.op_zw2
         ? x86("comment", "IR_MATCH_END (MECHANISM-2 W-1: fixed negative rbp offsets [rbp-16]..[rbp-64] match match_begin header; blob cells via FR; whack=mov rsp,rbp;pop rbp)")
         + x86_alpha()
         + IF(_.op_dval != 0.0, x86("note", "end_δ") + x86("mov", RDQ("rbp", -8), "r14"))   /* ⭐ W-1: end_δ spare in pad slot [rbp-8] (safe; depth-immune) */
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("mov", "rsi", "r12")   /* M-1 FIX (CAS-R12-UNIFY): r12 IS the live CAS top (COND γ-pushes advance r12 but never update [RT_DCAP_TOP]; cell holds MATCH_BEGIN's value = one-above-sentinel; reading cell gave empty range, dropping all blob-interior captures silently) */
         + x86("mov", "r10", "rsi")
         + x86("def", L(5))
         + x86("sub", "r10", (long)24)
         + x86("mov", "rax", RDQ("r10", 0))
         + x86("test", "rax", "rax")
         + x86("jne", L(5))
         + x86("lea", "rdi", RDQ("r10", 24))
         + x86("mov", "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         + x86("def", L(1))
         + x86("test", "rax", "rax")
         + x86("je",  L(2))
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         + bb_glue_pass_wires(3, 4)
         + x86("def", L(3))
         + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov", "rdi", "rax") + x86("mov", "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp", L(1))
         + x86("def", L(4))
         + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
         + x86("mov", "rdi", "rax") + x86("mov", "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp", L(1))
         + x86("def", L(2))
         + x86("call", "rt_dcap_end_ok_close", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_end_ok_close)
         + x86_anchor_leave()
         + x86_xfer_leave()
         + x86("mov", "r10", ABSQ(RT_DCAP_TOP)) + x86("def", L(6)) + x86("sub", "r10", (long)24) + x86("mov", "rax", RDQ("r10", 0)) + x86("test", "rax", "rax") + x86("jne", L(6)) + x86("mov", ABSQ(RT_DCAP_TOP), "r10")
         /* ⭐ MECH2-R12-FIX: rbp-restore DELETED.  rbp is the pinned mech-2 frame base (set by match_begin's push+mov, never borrowed by ARBNO element view in this arm).  Header reads [rbp-N] proceed directly without a reload. */
         + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))   /* ⭐ W-1: restore from fixed header [rbp-16..rbp-40] matching match_begin saves */
         + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))
         + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))
         + x86("mov", "rdi", "r13") + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", RDQ("rbp", -40))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + IF(_.op_dval != 0.0, x86("note", "start_δ") + x86("mov", "eax", RDD("rbp", -48))   /* start_δ at [rbp-48] */
                              + x86("note", "end_δ")   + x86("mov", "r11", RDQ("rbp", -8)))   /* end_δ from pad slot */
         + x86("note", "mech2_whack") + x86("mov", "rsp", "rbp")   /* ⭐ MECH2-R12-FIX: was `mov rsp,r12`; rbp==α−8 is the pinned frame base; pop rbp restores old_rbp and rsp→α */
         + x86("pop", "rbp")
         + IF(_.op_dval != 0.0, x86("note", "match_start") + x86("mov", RDD("rbp", 8 + _.op_off), "eax")   /* post-whack: rbp=old, rsp=α; FRQ addresses caller claim */
                              + x86("note", "match_end")   + x86("mov", FRQ(_.op_off + 24), "r11"))
         + x86_gamma()
         /* ⭐ MECH2-R12-FIX: rbp-restore DELETED on ω path. Same law: rbp pinned by match_begin, never clobbered. */
         + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))   /* ω path: same fixed offsets */
         + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))
         + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))
         + x86("mov", "rdi", "r13") + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", RDQ("rbp", -40))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + x86("note", "mech2_whack") + x86("mov", "rsp", "rbp")   /* ⭐ MECH2-R12-FIX: was `mov rsp,r12`; rbp==α−8 pinned frame base; pop rbp restores and rsp→α */
         + x86("pop", "rbp")
         + x86_omega()
         : _.op_zw
         ? x86("comment", "IR_MATCH_END (ZW-15: γ = apply-walk [rbp-40]→top then THE WHACK.  ZW-15: rbp=claim_base (lea rbp,[rbp+8] in begin); old_rbp at [rbp-8]; cells [rbp-16]..[rbp-64]; cas_base at [rbp-40]; whack=lea rsp,[rbp-8];pop rbp restores rsp=claim_base,rbp=old_rbp.  Blob FRQ reads [rbp+blob_off]=[claim_base+blob_off] correct for any nblob_real -- blob-clause veto retired.)")
         + x86_alpha()
         + IF(_.op_dval != 0.0, x86("note", "end_δ") + x86("mov", RDQ("rbp", -48), "r14"))   /* ZW-15: was -40; end_δ parks in dead frame slot before C calls clobber r14 */
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("note", "cas_base") + x86("mov",  "rdi", RDQ("rbp", -40))   /* ZW-15: was -32 */
         + x86("note", "cas_top")  + x86("mov",  "rsi", "r12")   /* ZW-3: r12 is the live top -- all COND γ-pushes since BEGIN landed here; no cell read needed */
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         + x86("def",  L(1))
         + x86("test", "rax", "rax")
         + x86("je",   L(2))
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         + bb_glue_pass_wires(3, 4)
         + x86("def",  L(3))
         + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(4))
         + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(2))
         + x86("call", "rt_dcap_end_ok_close", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_end_ok_close)
         + x86_anchor_leave()
         + x86_xfer_leave()
         + x86("note", "cas_base") + x86("mov", "r12", RDQ("rbp", -40)) + x86("mov", ABSQ(RT_DCAP_TOP), "r12")   /* ZW-3 / ZW-15: r12←base discards applied records (was -32) */
         + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))   /* ZW-15: was -8 */
         + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))   /* ZW-15: was -16 */
         + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))   /* ZW-15: was -24 */
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", RDQ("rbp", -64))   /* ZW-15: was -56 */
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + IF(_.op_dval != 0.0, x86("note", "start_δ") + x86("mov", "r10", RDQ("rbp", -56))   /* ZW-15: was -48 */
                              + x86("note", "end_δ")   + x86("mov", "r11", RDQ("rbp", -48)))   /* ZW-15: was -40 */
         + x86("note", "whack") + x86("lea", "rsp", "qword ptr [rbp# + -8]")   /* ZW-15: old_rbp at [rbp-8]; lea rsp,[rbp-8] → rsp=claim_base-8; pop rbp → restores old_rbp, rsp=claim_base */
         + x86("pop", "rbp")
         + IF(_.op_dval != 0.0, x86("mov", "eax", "r10d")
                              + x86("note", "match_start") + x86("mov", FR(_.op_off), "eax")
                              + x86("note", "match_end")   + x86("mov", FRQ(_.op_off + 24), "r11"))
         + x86_gamma()
         + x86("note", "cas_base") + x86("mov", "r12", RDQ("rbp", -40)) + x86("mov", ABSQ(RT_DCAP_TOP), "r12")   /* ZW-3 ω / ZW-15: r12←base bulk-discards (was -32) */
         + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))   /* ZW-15: was -8 */
         + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))   /* ZW-15: was -16 */
         + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))   /* ZW-15: was -24 */
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", RDQ("rbp", -64))   /* ZW-15: was -56 */
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + x86("note", "whack") + x86("lea", "rsp", "qword ptr [rbp# + -8]")   /* ZW-15: same as γ whack */
         + x86("pop", "rbp")
         + x86_omega()
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

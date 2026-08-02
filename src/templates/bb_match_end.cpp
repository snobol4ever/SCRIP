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
extern "C" uint64_t g_patstk_sp;
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
         + x86("mov",  "rsi", ABSQ(RT_CAS_TOP))   /* CAS-MARKER: top; mark is recovered by scanning down to HEAD's tag-0 marker -- the flat +32 slot and its zc_frame fork are deleted; one config-blind mechanism serves every basis */
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
         + x86("mov", "r10", ABSQ(RT_CAS_TOP)) + x86("def", L(6)) + x86("sub", "r10", (long)24) + x86("mov", "rax", RDQ("r10", 0)) + x86("test", "rax", "rax") + x86("jne", L(6)) + x86("mov", ABSQ(RT_CAS_TOP), "r10")   /* CAS-MARKER: success path re-scans (the pump's C calls clobber r10; nested matches inside pumped assignments push balanced markers, so top returns) and pops entries + marker wholesale */
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
         : _.op_zw
         ? x86("comment", "IR_MATCH_END (ZW-12: γ = apply-walk [rbp-32]→top then THE WHACK.  The pump receives cas_base straight from the frame -- rdi=[rbp-32], rsi=cell top, rdx=Σ -- so BOTH tag-0 marker scans die; the applied records are popped wholesale by top←cas_base; the quintet restores ride the frame; and mov rsp,rbp; pop rbp frees every member/producer cell below the frame in one move, landing rsp EXACTLY at the head-α claim base -- which is why the model staged this node's op_udout post-whack, and why the REPLACE handoff (match start → flat +0, end δ → flat +24) is emitted AFTER the whack through the unchanged offset machinery.  end/start park in dead frame slots (-40 anchor is spent, -48 start_δ holds the winning attempt) across the C calls.)")
         + x86_alpha()
         + IF(_.op_dval != 0.0, x86("note", "end_δ") + x86("mov", RDQ("rbp", -40), "r14"))
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("note", "cas_base") + x86("mov",  "rdi", RDQ("rbp", -32))
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
         + x86("note", "cas_base") + x86("mov", "r12", RDQ("rbp", -32)) + x86("mov", ABSQ(RT_CAS_TOP), "r12")   /* ZW-3: r12←base discards applied records; write cell for any C code after the whack */
         + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -8))
         + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -16))
         + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -24))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", RDQ("rbp", -56))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + IF(_.op_dval != 0.0, x86("note", "start_δ") + x86("mov", "r10", RDQ("rbp", -48))
                              + x86("note", "end_δ")   + x86("mov", "r11", RDQ("rbp", -40)))
         + x86("note", "whack") + x86("mov", "rsp", "rbp")
         + x86("pop", "rbp")
         + IF(_.op_dval != 0.0, x86("mov", "eax", "r10d")
                              + x86("note", "match_start") + x86("mov", FR(_.op_off), "eax")
                              + x86("note", "match_end")   + x86("mov", FRQ(_.op_off + 24), "r11"))
         + x86_gamma()
         + x86("note", "cas_base") + x86("mov", "r12", RDQ("rbp", -32)) + x86("mov", ABSQ(RT_CAS_TOP), "r12")   /* ZW-3 ω: r12←base bulk-discards all COND records from the failed attempt; write cell for C-transit after the whack */
         + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -8))
         + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -16))
         + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -24))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", RDQ("rbp", -56))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + x86("note", "whack") + x86("mov", "rsp", "rbp")
         + x86("pop", "rbp")
         + x86_omega()
         : _.op_tail && rfc()
         ? x86("comment", "IR_MATCH_END (CAS-MARKER-CARRY tail: scan to the head's tag-0 sentinel, recover patstk (+16) and the rsp mark (+8) off it, one-mov unwind -- depth-free on every success-path depth, where the old RSP(op_fc_disp) reloads under-counted the live leaf cells the non-popping γ spine leaves (the 041 class: [rsp+16] read the assign_save cell, rsp := 0x7fff00000000).  Marker NOT popped here -- the pump walks the pend entries above it and its own L(6) scan pops the lot)")
         + x86_alpha()
         + x86("mov", "r10", ABSQ(RT_CAS_TOP))
         + x86("def", L(8))
         + x86("sub", "r10", (long)24)
         + x86("mov", "rax", RDQ("r10", 0))
         + x86("test", "rax", "rax")
         + x86("jne", L(8))
         + x86("mov", "rax", RDQ("r10", 16))
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
         + x86("mov", RDQ("rcx", 0), "rax")
         + x86("mov", "rsp", RDQ("r10", 8))
         + release_pump()
         : x86("comment", "IR_MATCH_END")
         + x86_alpha()
         + IF(x86_zc_frame() == ZC_FRAME_RSP, (rfc() ? x86("mov", "r10", ABSQ(RT_CAS_TOP))   /* CAS-MARKER-CARRY (s22x): scan to the head's tag-0 sentinel; patstk rides +16, the rsp mark +8.  The old RSP(op_fc_disp) spellings assumed fc_disp counted every live cell between head and release -- it misses the ZW-1 alpha carves the non-popping γ spine leaves live (041: fc_disp=0 read the head cell 32 low, loading a half-written leaf cell into rsp).  r10 survives to the unwind below (the dval arm touches only rax/r14). */
                                                     + x86("def", L(9))
                                                     + x86("sub", "r10", (long)24)
                                                     + x86("mov", "rax", RDQ("r10", 0))
                                                     + x86("test", "rax", "rax")
                                                     + x86("jne", L(9))
                                                     + x86("mov", "rax", RDQ("r10", 16))
                                               : x86("mov", "rax", FRQ(_.op_off + 8)))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", RDQ("rcx", 0), "rax"))
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

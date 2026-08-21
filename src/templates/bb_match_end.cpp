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
extern "C" void rt_dcap_end_ok_close(void);
extern "C" void *rt_proc_open_fn(void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
extern "C" long zvo_owner_dout(int cur_head);
extern "C" long rt_match_end_all(const char *mark, const char *top, const char *subj, const uint64_t *outer);
#include "x86_asm.h"
#define rfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0)
#define hfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int one_end(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ONE_END"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int oscap_l(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_OS_CAP"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int has_replace_l(void) { if (!g_emit_cfg) return 0; for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _nd = g_emit_cfg->all[_i]; if (_nd && (_nd->op == IR_MATCH_REPLACE || _nd->op == IR_MATCH_FENCE0 || _nd->op == IR_MATCH_FENCE1 || _nd->op == IR_MATCH_ABORT || _nd->op == IR_MATCH_ARBNO)) return 1; } return 0; }
#define stfh() (_.flat_stmt_frame || (oscap_l() && _.flat_deep_arrival && !_.flat_jmp_entry && !_.flat_lcl_proc && !_.zframe_graph && !_.flat_pat && !_.flat_gen && !has_replace_l()))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * HKQ(int k) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp# + %d]", 16 + 8 * k); return b[i]; }
static const char * HKM() { return "qword ptr [rsp# + 8]"; }
static const char * HKD() { return "dword ptr [rsp# + 0]"; }
static int cap_name_strict(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_CAP_NAME_STRICT"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string mend_bank_cursors() {
    return IF(({ static int _bg = -1; if (_bg < 0) { const char * e = getenv("SCRIP_DEFER_BETA_GUARD"); _bg = (e && *e == '0') ? 0 : 1; } _bg; }) && emit_match_rbp(),
                   x86("note", "mbc_restore")
                 + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb")
                 + x86("mov", "rax", RDQ("rbp", -48))
                 + x86("mov", RDQ("rcx", 248), "rax"))
         + ((emit_match_rbp() && _.op_dval != 0.0)
         ? x86("note", "repl_start") + x86("mov", "eax", RDD("rbp", -40))
         + x86("note", "repl_start") + x86("mov", RDD("rbp", -48), "eax")
         + x86("note", "repl_end")   + x86("mov", RDQ("rbp", -56), "r14")
         : std::string());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string release_pump_legacy() {
    return std::string()
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("mov",  "rsi", "r12")
         + (emit_match_rbp()
             ? x86("note", "cas_mark") + x86("mov", "rdi", RDQ("rbp", -8))
             : x86("mov",  "r8", "rsi")
             + x86("def",  L(5))
             + x86("sub",  "r8", (long)24)
             + x86("mov",  "rax", RDQ("r8", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(5))
             + x86("lea",  "rdi", RDQ("r8", 24)))
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         + x86("def",  L(1))
         + x86("test", "rax", "rax")
         + x86("je",   L(2))
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
         + (emit_match_rbp()
             ? x86("note", "cas_mark") + x86("mov", "r12", RDQ("rbp", -8))
             + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))
             + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))
             + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))
             : x86("note", "cas_mark") + x86("def", L(10)) + x86("sub", "r12", (long)24) + x86("mov", "rax", RDQ("r12", 0)) + x86("test", "rax", "rax") + x86("jne", L(10))
             + x86("note", HKN(1)) + x86("mov", "r13", stfh() ? HKQ(1) : FRQ(_.op_off + 48))
             + x86("note", HKN(2)) + x86("mov", "r14", stfh() ? HKQ(2) : FRQ(_.op_off + 56))
             + x86("note", HKN(3)) + x86("mov", "r15", stfh() ? HKQ(3) : FRQ(_.op_off + 64)))
         + IF(emit_match_rbp() && _.op_dval != 0.0,
               x86("note", "repl_start") + x86("mov", "eax", RDD("rbp", -48))
             + x86("mov", RDD("r12", 0), "eax")
             + x86("note", "repl_end")   + x86("mov", "rax", RDQ("rbp", -56))
             + x86("mov", RDQ("r12", 8), "rax")
             + x86("add", "r12", (long)16))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + IF(emit_match_rbp(), x86("note", "frame_whack") + x86("mov", "rsp", "rbp") + x86("pop", "rbp"))
         + IF(_.op_dval == 0.0 && _.flat_deep_arrival, x86("note", HKN(0)) + std::string(""))
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string release_pump_one() {
    return std::string()
         + x86_xfer_enter()
         + x86_anchor_enter()
         + (emit_match_rbp()
             ? x86("note", "cas_mark") + x86("mov", "rdi", RDQ("rbp", -8))
             + x86("note", HKN(3)) + x86("mov", "rax", RDQ("rbp", -32))
             + x86("note", HKN(1)) + x86("mov", "rcx", RDQ("rbp", -16))
             : x86("mov",  "r8", "r12")
             + x86("def",  L(5))
             + x86("sub",  "r8", (long)24)
             + x86("mov",  "rax", RDQ("r8", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(5))
             + x86("lea",  "rdi", RDQ("r8", 24))
             + x86("note", HKN(3)) + x86("mov", "rax", stfh() ? HKQ(3) : FRQ(_.op_off + 64))
             + x86("note", HKN(1)) + x86("mov", "rcx", stfh() ? HKQ(1) : FRQ(_.op_off + 48)))
         + x86("push", "rax")
         + x86("push", "rcx")
         + x86("lea",  "rcx", RDQ("rsp", 0))
         + x86("mov",  "rsi", "r12")
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_match_end_all", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *, const uint64_t *))rt_match_end_all)
         + IF(cap_name_strict(),
               x86("comment", "SN4-CAP-NAME-STRICT: rax != 0 = a deferred capture target resolved to a VALUE, not a NAME -- the terminus fails instead of committing an indirect assignment (oracle: sbl retreats).  The stub undoes exactly what this box's alpha established (the two pushes, anchor, xfer) and jumps omega, which lower_snobol4.c wires to head -- the same continuation a failing rightmost pattern element takes, so retry-or-exhaust is decided by head as usual.")
             + x86("test", "rax", "rax")
             + x86("je",   L(13))
             + x86("add",  "rsp", (long)16)
             + x86_anchor_leave()
             + x86_xfer_leave()
             + x86_omega()
             + x86("def",  L(13)))
         + x86("add",  "rsp", (long)16)
         + x86_anchor_leave()
         + x86_xfer_leave()
         + (emit_match_rbp()
             ? x86("note", "cas_mark") + x86("mov", "r12", RDQ("rbp", -8))
             + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))
             + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))
             + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))
             : x86("note", "cas_mark") + x86("def", L(10)) + x86("sub", "r12", (long)24) + x86("mov", "rax", RDQ("r12", 0)) + x86("test", "rax", "rax") + x86("jne", L(10))
             + x86("note", HKN(1)) + x86("mov", "r13", stfh() ? HKQ(1) : FRQ(_.op_off + 48))
             + x86("note", HKN(2)) + x86("mov", "r14", stfh() ? HKQ(2) : FRQ(_.op_off + 56))
             + x86("note", HKN(3)) + x86("mov", "r15", stfh() ? HKQ(3) : FRQ(_.op_off + 64)))
         + IF(emit_match_rbp() && _.op_dval != 0.0,
               x86("note", "repl_start") + x86("mov", "eax", RDD("rbp", -48))
             + x86("mov", RDD("r12", 0), "eax")
             + x86("note", "repl_end")   + x86("mov", "rax", RDQ("rbp", -56))
             + x86("mov", RDQ("r12", 8), "rax")
             + x86("add", "r12", (long)16))
         + IF(emit_match_rbp(), x86("note", "frame_whack") + x86("mov", "rsp", "rbp") + x86("pop", "rbp"))
         + IF(_.op_dval == 0.0 && _.flat_deep_arrival, x86("note", HKN(0)) + std::string(""))
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string release_pump() { return one_end() ? release_pump_one() : release_pump_legacy(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_end() {
    x86_begin();
    if (getenv("SCRIP_MEND_ADDR_DIAG")) fprintf(stderr, "[MEND] op_off=%d op_fc_disp=%d op_dval=%g rfc=%d zc_frame=%d op_tail=%d\n", _.op_off, _.op_fc_disp, _.op_dval, rfc() ? 1 : 0, (int)x86_zc_frame(), _.op_tail);
    return !PLATFORM_X86 ? std::string()
         : _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_END: head slot not resolved (operand[0] missing or unowned)")
         : _.op_tail && rfc()
         ? x86("comment", "IR_MATCH_END (CAS-MARKER-CARRY tail: scan to the head's tag-0 sentinel, recover patstk (+16) and the rsp mark (+8) off it, one-mov unwind -- depth-free on every success-path depth, where the old RSP(op_fc_disp) reloads under-counted the live leaf cells the non-popping γ spine leaves (the 041 class: [rsp+16] read the assign_save cell, rsp := 0x7fff00000000).  Marker NOT popped here -- the pump walks the pend entries above it and its own L(6) scan pops the lot)")
         + x86_alpha() + mend_bank_cursors()
         + (emit_match_rbp()
             ? std::string()
             : x86("mov", "r8", "r12")
             + x86("def", L(8))
             + x86("sub", "r8", (long)24)
             + x86("mov", "rax", RDQ("r8", 0))
             + x86("test", "rax", "rax")
             + x86("jne", L(8))
             + IF(emit_arbno_rbp_unwind(), x86_arbno_rbp_unwind("r8", 8, 11, 12))
             + x86("mov", "rsp", RDQ("r8", 8)))
         + release_pump()
         : x86("comment", "IR_MATCH_END")
         + x86_alpha() + mend_bank_cursors()
         + IF(x86_zc_frame() == ZC_FRAME_RSP && !emit_match_rbp(), x86("mov", "r8", "r12")
                                               + x86("def", L(9))
                                               + x86("sub", "r8", (long)24)
                                               + x86("mov", "rax", RDQ("r8", 0))
                                               + x86("test", "rax", "rax")
                                               + x86("jne", L(9)))
         + (_.op_dval != 0.0 && !emit_match_rbp()
                ? IF(rfc(), x86("mov", "eax", RDD("rsp", (int)_.op_fc_disp))
                          + (x86_zc_frame() == ZC_FRAME_RSP ? x86("mov", RDD("rsp", (int)(_.op_off + _.op_fc_disp + (emit_match_rbp() ? 64 : 32))), "eax")
                                                      : x86("mov", stfh() ? HKD() : FR(_.op_off), "eax")))
                + (x86_zc_frame() == ZC_FRAME_RSP && rfc() ? x86("mov", RSP((int)(_.op_off + 24 + _.op_fc_disp + (emit_match_rbp() ? 64 : 32))), "r14")
                                                     : x86("mov", FRQ(_.op_off + 24), "r14"))
                : std::string())
         + (emit_match_rbp() ? std::string()
            : x86_zc_frame() == ZC_FRAME_RSP ? IF(emit_arbno_rbp_unwind(), x86_arbno_rbp_unwind("r8", 8, 11, 12)) + x86("mov", "rsp", RDQ("r8", 8))
            : (rfc() ? x86("mov", "rsp", RDQ("r8", 8))
               : x86_zls2_release_to_call(stfh() ? HKM() : FRQ(_.op_off + 16))))
         + x86_align_leave()
         + release_pump();
}

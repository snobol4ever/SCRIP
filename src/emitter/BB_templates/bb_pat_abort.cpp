/* bb_pat_abort.cpp — BB template for ABORT.  CPP return-String spike (GOAL-HEADQUARTERS ⚡ CPP).
   One file per opcode. Invariant #10: no grouping with other opcodes (distinct emit shape).
   SHAPE: text backends build ONE std::string by concatenation (the future Snocone form, where `+`
   becomes `.`); the C-linkage entry point writes it once via emit_text_n (NUL-safe → binary OK).
   The MEDIUM_BINARY arm and the JVM/NET layer-2 helpers (jvm_ and net_ families) perform FILE/relocation
   side-effects, so they stay imperative and the wrapper emits nothing extra for them. */
#include <string>
#include "emit_str.h"
#include "emit_str_builders.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_abort_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); (void)nid;
    if (PLATFORM_X86) {
        bin = { {1, 5, 6}, {_.lbl_ω_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return IF(MEDIUM_MACRO_DEF,
               s_comment("# no macro form — ABORT"))
             + IF(MEDIUM_BINARY,
               bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX ABORT()")
                 + s_2asm("jmp", _.lbl_ω)
                 + s_L2asm(std::string(_.lbl_β) + ":", "jmp", _.lbl_ω));
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("abort")
             + s_directive(".inner class public static final abort_exception inner bb/bb_abort$AbortException outer bb/bb_abort")
             + jvm_init_ms_only_str("abort")
             + s_directive(".method public \316\261()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 2")
             + s_directive(".limit locals 1")
             + s_2asm("new", "bb/bb_abort$AbortException")
             + s_1asm("dup")
             + s_2asm("invokespecial", "bb/bb_abort$AbortException/<init>()V")
             + s_1asm("athrow")
             + s_directive(".end method")
             + s_directive(".method public \316\262()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 2")
             + s_directive(".limit locals 1")
             + s_2asm("new", "bb/bb_abort$AbortException")
             + s_1asm("dup")
             + s_2asm("invokespecial", "bb/bb_abort$AbortException/<init>()V")
             + s_1asm("athrow")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { let self = { succ: null, fail: null,\n", pBB->ival, nid)
             + "\316\261() { self.fail.\316\261(); return null; },\n\316\262() { self.fail.\316\261(); return null; }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int sid = 0;
        return net_class_hdr_str(sid, nid)
             + net_ctor_none_str(sid, nid)
             + net_α_hdr_str()
             + s_2asm(".maxstack", "1")
             + net_fail_ret_str()
             + s_1asm("}")
             + net_β_hdr_str()
             + s_2asm(".maxstack", "1")
             + net_fail_ret_str()
             + s_1asm("}")
             + s_1asm("}")
             + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor()", sid, nid));
    }
    if (PLATFORM_WASM) { return std::string("          (call $bb_abort_new)\n"); }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_abort(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_abort_str(pBB, bin), bin);
}

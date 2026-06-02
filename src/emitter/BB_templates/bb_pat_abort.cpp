#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_abort_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX ABORT()  [x86() self-encoding]"))
             + x86("jmp",  PORT_OMEGA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
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
        int nid = _.nid;
        return emit_fmt("function make_pat_%d_%d(ms) { let self = { succ: null, fail: null,\n", (int)_.op_ival, nid)
             + "\316\261() { self.fail.\316\261(); return null; },\n\316\262() { self.fail.\316\261(); return null; }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int sid = 0; int nid = _.nid;
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_abort(void) { bb_emit_x86(bb_pat_abort_str()); }

#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline long lenN() { return (long)(int)_.op_ival; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_len_str() {
    int nid = _.nid; int sid = 0;
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(emit_fmt("# BOX LEN(%ld)  [REG-2 δ=r14 Δ=r15, x86() self-encoding]", lenN())))
             + x86("mov", "eax", "r14d")
             + x86("add", "eax", lenN())
             + x86("cmp", "eax", "r15d")
             + x86("jg",  PORT_OMEGA)
             + x86("add", "r14d", lenN())
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
/*--------------------------------------------------------------------------------------------------------------------*/
    if (PLATFORM_JVM) {
        std::string tag_s = emit_fmt("len_%d_%d", sid, nid);
        int n = (int)_.op_ival;
        std::string r = jvm_class_hdr_str("len")
            + s_directive(".field private final n I")
            + s_directive(".field private final dyn Ljava/util/function/IntSupplier;")
            + jvm_init_ms_int_str("len", "n") + jvm_val_helper_str("len")
            + s_directive(".method public α()Lbb/bb_box$Spec;")
            + s_directive(".limit stack 5")
            + s_directive(".limit locals 2")
            + s_1asm("aload_0")
            + s_2asm("getfield", "bb/bb_len/ms Lbb/bb_box$MatchState;")
            + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
            + s_1asm("aload_0")
            + s_2asm("invokevirtual", "bb/bb_len/val()I")
            + s_1asm("iadd")
            + s_1asm("aload_0")
            + s_2asm("getfield", "bb/bb_len/ms Lbb/bb_box$MatchState;")
            + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
            + s_1asm("    if_icmpgt " + tag_s + "_ω")
            + s_1asm("aload_0")
            + s_2asm("getfield", "bb/bb_len/ms Lbb/bb_box$MatchState;")
            + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
            + s_1asm("istore_1")
            + s_1asm("aload_0")
            + s_2asm("getfield", "bb/bb_len/ms Lbb/bb_box$MatchState;")
            + s_1asm("dup")
            + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
            + s_1asm("aload_0")
            + s_2asm("invokevirtual", "bb/bb_len/val()I")
            + s_1asm("iadd")
            + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
            + s_2asm("new", "bb/bb_box$Spec")
            + s_1asm("dup")
            + s_1asm("iload_1")
            + s_1asm("aload_0")
            + s_2asm("invokevirtual", "bb/bb_len/val()I")
            + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
            + s_1asm("areturn")
            + s_1asm(tag_s + "_ω:")
            + s_1asm("aconst_null")
            + s_1asm("areturn")
            + s_directive(".end method")
            + s_directive(".method public β()Lbb/bb_box$Spec;")
            + s_directive(".limit stack 4")
            + s_directive(".limit locals 1")
            + s_1asm("aload_0")
            + s_2asm("getfield", "bb/bb_len/ms Lbb/bb_box$MatchState;")
            + s_1asm("dup")
            + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
            + s_1asm("aload_0")
            + s_2asm("invokevirtual", "bb/bb_len/val()I")
            + s_1asm("isub")
            + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
            + s_1asm("aconst_null")
            + s_1asm("areturn")
            + s_directive(".end method");
        return r;
    }
    if (PLATFORM_JS) {
        int64_t n = _.op_ival;
        return emit_fmt("function make_pat_%d_%d(ms) { const n = %ld; let self = { succ: null, fail: null,\n", _.op_ival, nid, n)
             + "α() { if (ms.delta + n > ms.omega) { self.fail.α(); return; } const r = ms.sigma.slice(ms.delta, ms.delta + n); ms.delta += n; self.succ.α(); return r; },\n"
             "β() { ms.delta -= n; self.fail.α(); }\n"
             "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int n = (int)_.op_ival;
        std::string r = net_class_hdr_str(sid, nid)
            + s_directive(".field private int32 _n")
            + s_directive(".method public specialname rtspecialname instance void .ctor(int32 n) cil managed")
            + s_1asm("{")
            + s_2asm(".maxstack", "2")
            + s_1asm("ldarg.0")
            + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
            + s_1asm("ldarg.0")
            + s_1asm("ldarg.1")
            + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_n", sid, nid))
            + s_1asm("ret")
            + s_1asm("}")
            + net_α_hdr_str()
            + s_2asm(".maxstack", "4")
            + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
            + net_cursor_load_str() + s_1asm("ldarg.0")
            + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
            + s_1asm("add")
            + s_1asm("ldarg.1") + net_ms_length_str() + s_1asm(emit_fmt("    bgt        LEN_%d_%d_FAIL", sid, nid))
            + net_cursor_load_str() + s_1asm("ldarg.0")
            + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
            + net_spec_of_str() + s_1asm("stloc.0")
            + s_1asm("ldarg.1")
            + s_1asm("ldarg.1") + net_cursor_load_str()
            + s_1asm("ldarg.0")
            + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
            + s_1asm("add")
            + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
            + s_1asm("ldloc.0")
            + s_1asm("ret")
            + s_1asm(emit_fmt("  LEN_%d_%d_FAIL:", sid, nid)) + net_fail_ret_str() + s_1asm("}")
            + net_β_hdr_str()
            + s_2asm(".maxstack", "3")
            + s_1asm("ldarg.1")
            + s_1asm("ldarg.1") + net_cursor_load_str()
            + s_1asm("ldarg.0")
            + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
            + s_1asm("sub")
            + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
            + net_fail_ret_str() + s_1asm("}")
            + s_1asm("}")
            + net_push_i4_str(n) + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor(int32)", sid, nid));
        return r;
    }
    if (PLATFORM_WASM) { return std::string("          (call $bb_len_new)\n"); }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_len(void) { bb_emit_x86(bb_pat_len_str()); }

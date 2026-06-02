#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
int bb_slot_claim(int bytes);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * cset_chars() { return _.op_sval ? _.op_sval : ""; }
static inline const char * cset_label() { return emit_intern_str(cset_chars()); }
static inline uint64_t     cset_addr()  { return (uint64_t)(uintptr_t)(const void *)cset_chars(); }
static inline uint64_t     strchr_ptr() { const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
static inline int          zoff()  { return _.x86_scratch_off; }
static inline int          zooff() { return _.x86_scratch_off + 4; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_span_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX SPAN()  [REG-4 Σ=r13 δ=r14 Δ=r15, ζ-frame z/zo, x86() self-encoding]"))
             + x86("mov",    FR(zoff()), (long)0)
             + x86("def",    L(0))
             + x86("mov",    "eax", "r14d")
             + x86("add",    "eax", FR(zoff()))
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    L(1))
             + x86("movsxd", "rcx", "eax")
             + x86("movzx",  "esi", "[r13+rcx]")
             + x86("lea",    "rdi", "[rip + __]", cset_addr(), cset_label())
             + x86("push",   "r10")
             + x86("call",   "strchr", strchr_ptr())
             + x86("pop",    "r10")
             + x86("test",   "rax", "rax")
             + x86("je",     L(1))
             + x86("add",    FR(zoff()), (long)1)
             + x86("jmp",    L(0))
             + x86("def",    L(1))
             + x86("mov",    "eax", FR(zoff()))
             + x86("test",   "eax", "eax")
             + x86("jle",    PORT_OMEGA)
             + x86("mov",    "edx", "r14d")
             + x86("mov",    FR(zooff()), "edx")
             + x86("add",    "edx", "eax")
             + x86("mov",    "r14d", "edx")
             + x86("jmp",    PORT_GAMMA)
             + x86("def",    PORT_BETA)
             + x86("mov",    "eax", FR(zoff()))
             + x86("sub",    "eax", (long)1)
             + x86("cmp",    "eax", (long)1)
             + x86("jl",     PORT_OMEGA)
             + x86("mov",    FR(zoff()), "eax")
             + x86("mov",    "edx", FR(zooff()))
             + x86("add",    "edx", "eax")
             + x86("mov",    "r14d", "edx")
             + x86("jmp",    PORT_GAMMA);
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("span")
             + s_directive(".field private final chars Ljava/lang/String;")
             + s_directive(".field private matched_len I")
             + jvm_init_ms_str_str("span", "chars")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6")
             + s_directive(".limit locals 3")
             + s_1asm("aload_0") + s_1asm("iconst_0")
             + s_2asm("putfield", "bb/bb_span/matched_len I")
             + s_L1asm(emit_fmt("span_%d_%d", 0, _.nid) + "_loop:", "aload_0")
             + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iadd") + s_1asm("istore_1") + s_1asm("iload_1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
             + s_1asm("if_icmpge " + emit_fmt("span_%d_%d", 0, _.nid) + "_done")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/chars Ljava/lang/String;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/sigma Ljava/lang/String;")
             + s_1asm("iload_1")
             + s_2asm("invokevirtual", "java/lang/String/charAt(I)C")
             + s_2asm("invokevirtual", "java/lang/String/indexOf(I)I")
             + s_1asm("iflt " + emit_fmt("span_%d_%d", 0, _.nid) + "_done")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_span/matched_len I")
             + s_1asm("goto " + emit_fmt("span_%d_%d", 0, _.nid) + "_loop")
             + s_L1asm(emit_fmt("span_%d_%d", 0, _.nid) + "_done:", "aload_0")
             + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("ifle " + emit_fmt("span_%d_%d", 0, _.nid) + "_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("istore_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("iload_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(emit_fmt("span_%d_%d", 0, _.nid) + "_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("isub") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const chars = ", (int)_.op_ival, _.nid)
             + js_escape_string_str(_.op_sval)
             + "; let delta = 0; let self = { succ: null, fail: null,\n"
               "α() { delta = 0; while (ms.delta + delta < ms.omega && chars.indexOf(ms.sigma[ms.delta + delta]) >= 0) delta++; if (delta <= 0) { self.fail.α(); return"
                   "; } const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n"
               "β() { ms.delta -= delta; self.fail.α(); }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_class_hdr_str(0, _.nid)
             + s_directive(".field private string _chars")
             + s_directive(".field private int32  _count")
             + s_directive(".method public specialname rtspecialname instance void .ctor(string chars) cil managed")
             + s_1asm("{") + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
             + s_1asm("ldarg.0") + s_1asm("ldarg.1") + s_1asm("dup")
             + s_1asm(emit_fmt("brtrue     SP_%d_%d_NN", 0, _.nid))
             + s_1asm("pop") + s_2asm("ldstr", "\"\"")
             + s_directive(emit_fmt("  SP_%d_%d_NN:", 0, _.nid))
             + s_1asm(emit_fmt("stfld      string pat_%d_%d::_chars", 0, _.nid))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "3")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_directive(emit_fmt("  SP_%d_%d_LOOP:", 0, _.nid))
             + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("add")
             + s_1asm(emit_fmt("ldfld      string pat_%d_%d::_chars", 0, _.nid))
             + s_2asm("callvirt", "instance bool [boxes]Snobol4.Runtime.Boxes.MatchState::CharInSet(int32, string)")
             + s_1asm(emit_fmt("brfalse    SP_%d_%d_DONE", 0, _.nid))
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm(emit_fmt("br         SP_%d_%d_LOOP", 0, _.nid))
             + s_directive(emit_fmt("  SP_%d_%d_DONE:", 0, _.nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("ble        SP_%d_%d_FAIL", 0, _.nid))
             + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + net_spec_of_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0") + s_1asm("ret")
             + s_directive(emit_fmt("  SP_%d_%d_FAIL:", 0, _.nid))
             + net_fail_ret_str() + s_1asm("}")
             + net_β_hdr_str() + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("sub")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + net_fail_ret_str() + s_1asm("}") + s_1asm("}")
             + net_escape_ldstr_str(_.op_sval ? _.op_sval : "")
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(string)", 0, _.nid));
    }
    if (PLATFORM_WASM) { return "          (call $bb_span_new)\n"; }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_span(void) {
    x86_begin();
    _.x86_scratch_off = bb_slot_claim(16);
    bb_emit_x86(bb_pat_span_str());
}

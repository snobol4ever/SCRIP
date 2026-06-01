/* bb_lit.cpp — BB template for literal string match. Converted to return-string (ER wave). */
#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* LIT operand accessors — pure functions of g_emit (_) and the current node. No template locals. */
static inline const char * lit()        { return (_.node && _.node->sval) ? _.node->sval : ""; }
static inline long         litlen()     { return (long)strlen(lit()); }
static inline const char * litlabel()   { return emit_intern_str(lit()); }
static inline uint64_t     litaddr()    { return (uint64_t)(uintptr_t)lit(); }
static inline uint64_t     memcmpaddr() { return (uint64_t)(uintptr_t)memcmp; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_lit_str(IR_t * pBB) {
    int nid = bb_node_id(pBB); int sid = 0;
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(emit_fmt("# BOX LIT(%s)  [REG-1 Σ=r13 δ=r14 Δ=r15]",
                                      litlen() > 24 ? emit_fmt("'%.24s...'", lit()).c_str()
                                                    : emit_fmt("'%s'", lit()).c_str())))
             + x86("mov", "eax", "r14d")
             + x86("add", "eax", litlen())
             + x86("cmp", "eax", "r15d")
             + x86("jg", PORT_OMEGA)
             + x86("movsxd", "rcx", "r14d")
             + x86("lea", "rdi", "[r13 + rcx]")
             + x86("lea", "rsi", "[rip + __]", litaddr(), litlabel())
             + x86("mov", "rdx", litlen())
             + x86("push", "r10")
             + x86("call", "memcmp", memcmpaddr())
             + x86("pop", "r10")
             + x86("test", "eax", "eax")
             + x86("jne", PORT_OMEGA)
             + x86("add", "r14d", litlen())
             + x86("mov", "[r10]", "r14d")
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("sub", "r14d", litlen())
             + x86("mov", "[r10]", "r14d")
             + x86("jmp", PORT_OMEGA);
    if (PLATFORM_JVM) {
        std::string tag_s = emit_fmt("lit_%d_%d", sid, nid);
        return jvm_class_hdr_str("lit")
             + s_directive(".field private final lit Ljava/lang/String;")
             + s_directive(".field private final len I")
             + s_directive(".method public <init>(Lbb/bb_box$MatchState;Ljava/lang/String;)V")
             + s_directive(".limit stack 3") + s_directive(".limit locals 3")
             + s_1asm("aload_0") + s_1asm("aload_1")
             + s_2asm("invokespecial", "bb/bb_box/<init>(Lbb/bb_box$MatchState;)V")
             + s_1asm("aload_0") + s_1asm("aload_2") + s_2asm("putfield", "bb/bb_lit/lit Ljava/lang/String;")
             + s_1asm("aload_0") + s_1asm("aload_2")
             + s_2asm("invokevirtual", "java/lang/String/length()I")
             + s_2asm("putfield", "bb/bb_lit/len I") + s_1asm("return")
             + s_directive(".end method")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6") + s_directive(".limit locals 2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/len I") + s_1asm("iadd")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
             + s_1asm("if_icmpgt " + tag_s + "_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/sigma Ljava/lang/String;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/lit Ljava/lang/String;")
             + s_1asm("iconst_0") + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/len I")
             + s_2asm("invokevirtual", "java/lang/String/regionMatches(ILjava/lang/String;II)Z")
             + s_1asm("ifeq " + tag_s + "_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("istore_1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/len I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("iload_1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/len I")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(tag_s + "_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_lit/len I")
             + s_1asm("isub") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        const char *sval = pBB->sval;
        return emit_fmt("function make_pat_%d_%d(ms) { const lit = ", pBB->ival, nid)
             + js_escape_string_str(sval)
             + "; const len = lit.length; let self = { succ: null, fail: null,\n"
               "α() { if (ms.delta + len > ms.omega || ms.sigma.slice(ms.delta, ms.delta + len) !== lit) { self.fail.α(); return; } ms.delta += len; self.succ.α(); },\n"
               "β() { ms.delta -= len; self.fail.α(); }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        const char *lit = pBB->sval ? pBB->sval : "";
        return net_class_hdr_str(sid, nid)
             + s_directive(".field private string _lit")
             + s_directive(".field private int32  _len")
             + s_directive(".method public specialname rtspecialname instance void .ctor(string lit) cil managed")
             + s_1asm("{") + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
             + s_1asm("ldarg.0") + s_1asm("ldarg.1") + s_1asm("dup")
             + s_1asm(emit_fmt("brtrue     LIT_%d_%d_NN", sid, nid))
             + s_1asm("pop") + s_2asm("ldstr", "\"\"")
             + s_directive(emit_fmt("  LIT_%d_%d_NN:", sid, nid))
             + s_1asm(emit_fmt("stfld      string pat_%d_%d::_lit", sid, nid))
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      string pat_%d_%d::_lit", sid, nid))
             + s_2asm("callvirt", "instance int32 [mscorlib]System.String::get_Length()")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_len", sid, nid))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "4")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + net_cursor_load_str()
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_len", sid, nid))
             + s_1asm("add") + s_1asm("ldarg.1") + net_ms_length_str()
             + s_1asm(emit_fmt("bgt        LIT_%d_%d_A_FAIL", sid, nid))
             + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm(emit_fmt("ldfld      string pat_%d_%d::_lit", sid, nid))
             + s_2asm("callvirt", "instance bool [boxes]Snobol4.Runtime.Boxes.MatchState::MatchesAt(int32, string)")
             + s_1asm(emit_fmt("brfalse    LIT_%d_%d_A_FAIL", sid, nid))
             + net_cursor_load_str()
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_len", sid, nid))
             + net_spec_of_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_len", sid, nid))
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0") + s_1asm("ret")
             + s_directive(emit_fmt("  LIT_%d_%d_A_FAIL:", sid, nid))
             + net_fail_ret_str() + s_1asm("}")
             + net_β_hdr_str() + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_len", sid, nid))
             + s_1asm("sub")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + net_fail_ret_str() + s_1asm("}") + s_1asm("}")
             + net_escape_ldstr_str(lit)
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(string)", sid, nid));
    }
    if (PLATFORM_WASM) { return "          (call $bb_lit_new)\n"; }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_lit(IR_t * pBB) {
    bb_emit_x86(bb_lit_str(pBB));
    if (MEDIUM_TEXT) g_emit_pos += 7;
}

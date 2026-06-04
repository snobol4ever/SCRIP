#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
void *rt_cs_new(const char *chars);
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * cset_chars() { return _.op_sval ? _.op_sval : ""; }
static inline const char * cset_label() { const char * l = emit_intern_str(cset_chars()); if (l) return l;
                                          static char b[24]; strtab_label(b, sizeof b, cset_chars()); return b; }
static inline uint64_t     cset_addr()  { return (uint64_t)(uintptr_t)(const void *)cset_chars(); }
static inline uint64_t     strchr_ptr() { const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_any_str() {
    int nid = _.nid; (void)nid;
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX ANY()  [REG-2 Σ=r13 δ=r14 Δ=r15, x86() self-encoding]"))
             + x86("mov",    "eax", "r14d")
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    PORT_OMEGA)
             + x86("movsxd", "rcx", "r14d")
             + x86("movzx",  "esi", "[r13+rcx]")
             + x86("lea",    "rdi", "[rip + __]", cset_addr(), cset_label())
             + x86("push",   "r10")
             + x86("call",   "strchr", strchr_ptr())
             + x86("pop",    "r10")
             + x86("test",   "rax", "rax")
             + x86("je",  PORT_OMEGA)
             + x86("add",    "r14d", (long)1)
             + x86("jmp",    PORT_GAMMA)
             + x86("def",    PORT_BETA)
             + x86("sub",    "r14d", (long)1)
             + x86("jmp",    PORT_OMEGA);
    }
/*--------------------------------------------------------------------------------------------------------------------*/
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("any")
             + s_directive(".field private final chars Ljava/lang/String;")
             + jvm_init_ms_str_str("any", "chars")
             + s_directive(".method public \316\261()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5")
             + s_directive(".limit locals 2")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_any/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_any/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
             + s_2asm("if_icmpge", emit_fmt("any_%d_%d_ω", 0, _.nid))
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_any/chars Ljava/lang/String;")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_any/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/sigma Ljava/lang/String;")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_any/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("invokevirtual", "java/lang/String/charAt(I)C")
             + s_2asm("invokevirtual", "java/lang/String/indexOf(I)I")
             + s_2asm("iflt", emit_fmt("any_%d_%d_ω", 0, _.nid))
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_any/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("istore_1")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_any/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("iconst_1")
             + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec")
             + s_1asm("dup")
             + s_1asm("iload_1")
             + s_1asm("iconst_1")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
             + s_1asm("areturn")
             + s_L1asm(emit_fmt("any_%d_%d_ω", 0, _.nid) + ":", "")
             + s_1asm("aconst_null")
             + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public \316\262()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4")
             + s_directive(".limit locals 1")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_any/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("iconst_1")
             + s_1asm("isub")
             + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aconst_null")
             + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const chars = ", _.op_ival, _.nid)
             + js_escape_string_str(_.op_sval)
             + "; let self = { succ: null, fail: null,\n"
                 "α() { if (ms.delta >= ms.omega || chars.indexOf(ms.sigma[ms.delta]) < 0) { self.fail.α(); return"
                     "; } const r = ms.sigma.slice(ms.delta, ms.delta + 1); ms.delta++; self.succ.α(); return r; },\n"
                 "β() { ms.delta--; self.fail.α(); }\n"
                 "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_charset_class_str(0, _.nid, "ANY")
             + net_α_hdr_str()
             + s_2asm(".maxstack", "3")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm(emit_fmt("    ldfld      string pat_%d_%d::_chars", 0, _.nid))
             + s_2asm("callvirt", "instance bool [boxes]Snobol4.Runtime.Boxes.MatchState::CharInSet(int32, string)")
             + s_1asm(emit_fmt("    brfalse    ANY_%d_%d_A_FAIL", 0, _.nid))
             + net_cursor_load_str() + s_1asm("ldc.i4.1") + net_spec_of_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.1")
             + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldc.i4.1")
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0")
             + s_1asm("ret")
             + s_1asm(emit_fmt("  ANY_%d_%d_A_FAIL:", 0, _.nid))
             + net_fail_ret_str() + s_1asm("}") + net_β_hdr_str()
             + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.1")
             + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldc.i4.1")
             + s_1asm("sub")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor") + net_fail_ret_str() + s_1asm("}")
             + s_1asm("}")
             + net_escape_ldstr_str(_.op_sval ? _.op_sval : "") + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor(string)", 0, _.nid));
    }
    if (PLATFORM_WASM) { return std::string("          (call $bb_any_new)\n"); }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_any(void) { bb_emit_x86(bb_pat_any_str()); }

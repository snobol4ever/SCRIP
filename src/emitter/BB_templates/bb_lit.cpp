/* bb_lit.cpp — BB template for literal string match. Converted to return-string (ER wave). */
#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_lit_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); int sid = 0;
    if (PLATFORM_X86) {
        const char *lit = pBB->sval ? pBB->sval : "";
        const char *lit_label = emit_intern_str(lit);
        int len = (int)strlen(lit);
        bin = { {22, 89, 105, 109, 121},
                {_.lbl_ω_p, _.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                {false, false, false, true, false} };
        (void)lit_label;
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — LIT"))
             + IF(MEDIUM_BINARY,
                   bytes(3, "\x41\x8B\x02")
                 + bytes(1, "\x05") + u32le((uint32_t)len)
                 + bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN)
                 + bytes(2, "\x3B\x01")
                 + bytes(2, "\x0F\x8F") + u32le(0)
                 + bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGMA)
                 + bytes(3, "\x48\x8B\x01")
                 + bytes(3, "\x49\x63\x0A")
                 + bytes(4, "\x48\x8D\x04\x08")
                 + bytes(3, "\x48\x89\xC7")
                 + bytes(2, "\x48\xBE") + u64le((uint64_t)(uintptr_t)lit)
                 + bytes(2, "\x48\xBA") + u64le((uint64_t)(uint32_t)len)
                 + bytes(2, "\x41\x52")
                 + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)memcmp)
                 + bytes(2, "\xFF\xD0")
                 + bytes(2, "\x41\x5A")
                 + bytes(2, "\x85\xC0")
                 + bytes(2, "\x0F\x85") + u32le(0)
                 + bytes(3, "\x41\x8B\x02")
                 + bytes(1, "\x05") + u32le((uint32_t)len)
                 + bytes(3, "\x41\x89\x02")
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(3, "\x41\x8B\x02")
                 + bytes(1, "\x2D") + u32le((uint32_t)len)
                 + bytes(3, "\x41\x89\x02")
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                   + s_comment(emit_fmt("# BOX LIT(%s)", len > 24 ? emit_fmt("'%.24s...'", lit).c_str() : emit_fmt("'%s'", lit).c_str()))
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("add", emit_fmt("eax, %u", (uint32_t)len))
                 + s_2asm("lea", "rcx, [rip + \xCE\xA3" "len]")
                 + s_2asm("cmp", "eax, [rcx]")
                 + s_2asm("jg", _.lbl_ω)
                 + s_2asm("lea", "rcx, [rip + Σ]")
                 + s_2asm("mov", "rax, [rcx]")
                 + s_2asm("movsxd", "rcx, dword ptr [r10]")
                 + s_2asm("lea", "rax, [rax+rcx]")
                 + s_2asm("mov", "rdi, rax")
                 + s_2asm("lea", emit_fmt("rcx, [rip + %s]", lit_label ? lit_label : "??"))
                 + s_2asm("mov", "rsi, rcx")
                 + s_2asm("mov", emit_fmt("rdx, %d", len))
                 + s_2asm("push", "r10")
                 + s_2asm("call", "memcmp@PLT")
                 + s_2asm("pop", "r10")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("jne", _.lbl_ω)
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("add", emit_fmt("eax, %d", len))
                 + s_2asm("mov", "[r10], eax")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(std::string(_.lbl_β) + ":")
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("sub", emit_fmt("eax, %d", len))
                 + s_2asm("mov", "[r10], eax")
                 + s_2asm("jmp", _.lbl_ω));
    }
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
extern "C" void bb_lit(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_lit_str(pBB, bin), bin);
    if (MEDIUM_TEXT) g_emit_pos += 7;
}

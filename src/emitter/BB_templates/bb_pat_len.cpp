/* bb_pat_len.cpp — BB template for LEN.  CPP return-String (GOAL-HEADQUARTERS CPP rung).
   One file per opcode. Invariant #10: no grouping with other opcodes (distinct emit shape).
   x86 + JS arms return concatenation (data value n + labels re-injected); JVM/NET arms keep the
   imperative jvm_/net_ helpers (x86-only directive → stubs). Built from the frozen baseline shape. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_len_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); int sid = 0;
    if (PLATFORM_X86) {
        bin = { {22, 38, 42, 43},
                {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                {false, false, true, false} };
        return IF(MEDIUM_MACRO_DEF,
                   s_comment("# no macro form — LEN"))
             + IF(MEDIUM_BINARY,
                   bytes(3, "\x41\x8B\x02")
                 + bytes(1, "\x05") + u32le((uint32_t)(int)pBB->ival)
                 + bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN)
                 + bytes(2, "\x3B\x01")
                 + bytes(2, "\x0F\x8F") + u32le(0)
                 + bytes(3, "\x41\x8B\x02")
                 + bytes(1, "\x05") + u32le((uint32_t)(int)pBB->ival)
                 + bytes(3, "\x41\x89\x02")
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                   + s_1asm(emit_fmt("# BOX LEN(%d)", (int)pBB->ival))
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("lea", "rax, [rip + Δ]") + s_2asm("mov", "eax, dword ptr [rax]") + s_1asm(emit_fmt("add eax, %d", (int)pBB->ival))
                 + s_2asm("lea", "rcx, [rip + Σlen]") + s_2asm("cmp", "eax, dword ptr [rcx]") + s_1asm(emit_fmt("jg %s", _.lbl_ω))
                 + s_2asm("lea", "rax, [rip + Δ]") + s_2asm("mov", "ecx, dword ptr [rax]") + s_1asm(emit_fmt("add ecx, %d", (int)pBB->ival)) + s_2asm("mov", "dword ptr [rax], ecx")
                 + s_1asm(emit_fmt("jmp %s", _.lbl_γ))
                 + s_1asm(emit_fmt("%s: jmp %s", _.lbl_β, _.lbl_ω)));
    }
    if (PLATFORM_JVM) {
        std::string tag_s = emit_fmt("len_%d_%d", sid, nid);
        int n = (int)pBB->ival;
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
        int64_t n = pBB->ival;
        return emit_fmt("function make_pat_%d_%d(ms) { const n = %ld; let self = { succ: null, fail: null,\n", pBB->ival, nid, n)
             + "α() { if (ms.delta + n > ms.omega) { self.fail.α(); return; } const r = ms.sigma.slice(ms.delta, ms.delta + n); ms.delta += n; self.succ.α(); return r; },\n"
             "β() { ms.delta -= n; self.fail.α(); }\n"
             "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int n = (int)pBB->ival;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_len(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_len_str(pBB, bin), bin);
}

/* bb_pat_rem.cpp — BB template for REM.  CPP return-String (GOAL-HEADQUARTERS CPP rung).
   One file per opcode. Invariant #10: no grouping with other opcodes (distinct emit shape). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_rem_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); (void)nid;
    if (PLATFORM_X86) {
        bin = { {16, 20, 21}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return IF(MEDIUM_MACRO_DEF,
               s_comment("# no macro form — REM"))
             + IF(MEDIUM_BINARY,
               bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN)
                 + bytes(2, "\x8B\x01")
                 + bytes(3, "\x41\x89\x02")
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX REM()") + s_directive(".intel_syntax noprefix")
                 + s_2asm("lea", "rax, [rip + \u03a3len]") + s_2asm("mov", "ecx, dword ptr [rax]")
                 + s_2asm("lea", "rax, [rip + \u0394]") + s_2asm("mov", "dword ptr [rax], ecx")
                 + s_1asm(emit_fmt("jmp %s", _.lbl_γ))
                 + s_1asm(emit_fmt("%s: jmp %s", _.lbl_β, _.lbl_ω)));
    }
    if (PLATFORM_JVM) {
        int sid = 0;
        return jvm_class_hdr_str("rem")
             + jvm_init_ms_only_str("rem")
             + s_directive(".method public \316\261()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6")
             + s_directive(".limit locals 2")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_rem/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("istore_1")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_rem/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_box$MatchState/\317\211 I")
             + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec")
             + s_1asm("dup")
             + s_1asm("iload_1")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_rem/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("iload_1")
             + s_1asm("isub")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
             + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public \316\262()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 1")
             + s_directive(".limit locals 1")
             + s_1asm("aconst_null")
             + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { let self = { succ: null, fail: null,\n", pBB->ival, nid)
             + "\316\261() { const r = ms.sigma.slice(ms.delta, ms.omega); ms.delta = ms.omega; self.succ.\316\261(); return r; },\n"
             "\316\262() { self.fail.\316\261(); }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int sid = 0;
        return net_class_hdr_str(sid, nid)
             + net_ctor_none_str(sid, nid)
             + net_α_hdr_str()
             + s_2asm(".maxstack", "3")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + net_cursor_load_str()
             + s_1asm("ldarg.1")
             + net_ms_length_str()
             + net_spec_of_str()
             + s_1asm("stloc.0")
             + s_1asm("ldarg.1")
             + s_1asm("ldarg.1")
             + net_ms_length_str()
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0")
             + s_1asm("ret")
             + s_1asm("}")
             + net_β_hdr_str()
             + s_2asm(".maxstack", "1")
             + net_fail_ret_str()
             + s_1asm("}")
             + s_1asm("}")
             + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor()", sid, nid));
    }
    if (PLATFORM_WASM) { return std::string("          (call $bb_rem_new)\n"); }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_rem(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_rem_str(pBB, bin), bin);
}

/* bb_pat_arb.cpp — BB template for ARB.  CPP return-String (GOAL-HEADQUARTERS CPP rung).
   One file per opcode. Invariant #10: no grouping with other opcodes (distinct emit shape).
   x86 arm returns concatenation; JVM/JS/NET/WASM arms keep the imperative helpers
   (x86-only directive → stubs). Built from the frozen baseline shape. */
#include <deque>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
extern int g_flat_node_id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_arb_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        bin = { {}, {}, {} };
        return IF(MEDIUM_MACRO_DEF,
               s_comment("# no macro form — ARB"))
             + IF(MEDIUM_BINARY, [&]() -> std::string {
                   /* ARB BINARY arm (SBL-ARB-2). Two process-lifetime deque-int scratch slots:
                      z_slot (match count), zo_slot (origin Δ). Both baked as imm64 — GC-safe.
                      Layout: 89 bytes. Sites: {32→γ, 36→β(def), 77→ω, 85→γ}.
                      α: z=0; *zo=Δ; jmp γ.
                      β: z++; eax=zo+z; if eax>Σlen → ω; Δ=eax; jmp γ. */
                   static std::deque<int> _pool;
                   _pool.emplace_back(0); int *z_slot = &_pool.back();
                   _pool.emplace_back(0); int *zo_slot = &_pool.back();
                   uint64_t za = (uint64_t)(uintptr_t)z_slot;
                   uint64_t zoa = (uint64_t)(uintptr_t)zo_slot;
                   std::string b;
                   b += bytes(2,"\x48\xB9") + u64le(za);    /* [0]  movabs rcx, z_slot */
                   b += bytes(6,"\xC7\x01\x00\x00\x00\x00");  /* [10] mov dword [rcx], 0 */
                   b += bytes(3,"\x41\x8B\x02");              /* [16] mov eax, [r10] */
                   b += bytes(2,"\x48\xB9") + u64le(zoa);   /* [19] movabs rcx, zo_slot */
                   b += bytes(2,"\x89\x01");                  /* [29] mov [rcx], eax */
                   b += bytes(1,"\xE9") + u32le(0);           /* [31] jmp γ  site[0]@32 */
                   /* lbl_β at [36] */
                   b += bytes(2,"\x48\xB9") + u64le(za);    /* [36] movabs rcx, z_slot */
                   b += bytes(3,"\x83\x01\x01");              /* [46] add dword [rcx], 1 */
                   b += bytes(2,"\x8B\x01");                  /* [49] mov eax, [rcx] */
                   b += bytes(2,"\x48\xB9") + u64le(zoa);   /* [51] movabs rcx, zo_slot */
                   b += bytes(2,"\x03\x01");                  /* [61] add eax, [rcx] */
                   b += bytes(2,"\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN); /* [63] movabs rcx, &Σlen */
                   b += bytes(2,"\x3B\x01");                  /* [73] cmp eax, [rcx] */
                   b += bytes(2,"\x0F\x8F") + u32le(0);      /* [75] jg ω  site[2]@77 */
                   b += bytes(3,"\x41\x89\x02");              /* [81] mov [r10], eax */
                   b += bytes(1,"\xE9") + u32le(0);           /* [84] jmp γ  site[3]@85 */
                   /* end [89] */
                   bin = { {32, 36, 77, 85},
                           {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                           {false, true, false, false} };
                   return b;
               }())
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX ARB()")
                 + s_directive(".section .data")
                 + s_directive(emit_fmt(".Larb%d_z", g_emit.bb_cs_id) + ": .long 0")
                 + s_directive(".long 0")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("lea rax,", "[rip + \xce\x94]")
                 + s_2asm("mov ecx,", "dword ptr [rax]")
                 + s_2asm("mov", "dword ptr [rip + " + emit_fmt(".Larb%d_z", g_emit.bb_cs_id) + " + 0], 0")
                 + s_2asm("lea rax,", "[rip + " + emit_fmt(".Larb%d_z", g_emit.bb_cs_id) + " + 4]")
                 + s_2asm("mov dword ptr [rax],", "ecx")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "lea rax,", "[rip + " + emit_fmt(".Larb%d_z", g_emit.bb_cs_id) + " + 0]")
                 + s_2asm("mov ecx,", "dword ptr [rax]")
                 + s_1asm("inc ecx")
                 + s_2asm("mov dword ptr [rax],", "ecx")
                 + s_2asm("lea rax,", "[rip + " + emit_fmt(".Larb%d_z", g_emit.bb_cs_id) + " + 4]")
                 + s_2asm("mov edx,", "dword ptr [rax]")
                 + s_2asm("add edx,", "ecx")
                 + s_2asm("lea rax,", "[rip + \xce\xa3len]")
                 + s_2asm("cmp edx,", "dword ptr [rax]")
                 + s_2asm("jg", _.lbl_ω)
                 + s_2asm("lea rax,", "[rip + \xce\x94]")
                 + s_2asm("mov dword ptr [rax],", "edx")
                 + s_2asm("jmp", _.lbl_γ));
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("arb")
             + s_directive(".field private arb_count I")
             + s_directive(".field private arb_start I")
             + jvm_init_ms_only_str("arb")
             + s_directive(".method public \316\261()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_1asm("iconst_0")
             + s_2asm("putfield", "bb/bb_arb/arb_count I")
             + s_1asm("aload_0") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("putfield", "bb/bb_arb/arb_start I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("iconst_0")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
             + s_1asm("areturn") + s_directive(".end method")
             + s_directive(".method public \316\262()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_arb/arb_count I")
             + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_arb/arb_count I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_start I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_count I")
             + s_1asm("iadd") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/\317\211 I")
             + s_1asm("    if_icmpgt " + emit_fmt("arb_%d_%d", 0, bb_node_id(pBB)) + "_\317\211")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_start I")
             + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_count I")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_count I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("areturn")
             + s_1asm(emit_fmt("arb_%d_%d", 0, bb_node_id(pBB)) + "_\317\211:")
             + s_1asm("aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { let delta = 0; let self = { succ: null, fail: null,\n", pBB->ival, bb_node_id(pBB))
             + "α() { delta = ms.omega - ms.delta; const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n"
             "β() { if (delta <= 0) { self.fail.α(); return; } delta--; ms.delta--; const r = ms.sigma.slice(ms.delta, ms.delta + delta + 1); return r; }\n"
             "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_class_hdr_str(0, bb_node_id(pBB))
             + s_directive(".field private int32 _count")
             + s_directive(".field private int32 _start")
             + net_ctor_none_str(0, bb_node_id(pBB))
             + net_α_hdr_str() + s_2asm(".maxstack", "2")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0") + s_1asm("ldarg.1")
             + s_2asm("ldfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_start", 0, bb_node_id(pBB)))
             + net_cursor_load_str() + net_spec_zw_str() + s_1asm("ret")
             + s_1asm("}") + net_β_hdr_str() + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_start", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add") + s_1asm("ldarg.1") + net_ms_length_str()
             + s_1asm(emit_fmt("    bgt        ARB_%d_%d_FAIL", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.1") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_start", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_start", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + net_spec_of_str() + s_1asm("ret")
             + s_1asm(emit_fmt("  ARB_%d_%d_FAIL:", 0, bb_node_id(pBB))) + net_fail_ret_str() + s_1asm("}")
             + s_1asm("}")
             + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor()", 0, bb_node_id(pBB)));
    }
    if (PLATFORM_WASM) { return std::string("          (call $bb_arb_new)\n"); }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_arb(BB_t * pBB) {
    if (MEDIUM_TEXT) g_emit.bb_cs_id = g_flat_node_id++;
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_arb_str(pBB, bin), bin);
}

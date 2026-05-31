/* bb_pat_tab.cpp — BB template for TAB and RTAB.
   TAB and RTAB share emit shape in every backend (single ival2 flag).
   Invariant #10: grouping permitted — both opcodes have the same shape globally. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_tab_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); int sid = 0;
    /* SBL-POS-RPOS-FLAG (2026-05-29 Opus 4.7): RTAB is distinguished by sval=="r" per
     * lower_pat_dcg.c (TT_RTAB / XRTB branches). Same fix as bb_pat_pos: `ival != 0`
     * conflated offset with kind. RTAB(0) was being emitted as TAB(0). */
    int rtab = (pBB->sval && pBB->sval[0] == 'r');
    if (PLATFORM_X86) {
        /* SBL-TAB-RTAB-FIX (2026-05-29 Opus 4.7): three bugs fixed in this hunk.
         *  (1) Sites for TAB were {9, 23, 28, 29} — site[0]=9 pointed at the LAST byte of the
         *      jg opcode `0F 8F` at offsets 8-9, so the patcher overwrote the `8F` opcode byte
         *      with the low byte of rel32 → SIGSEGV on any TAB(N) at runtime. Same off-by-one
         *      pattern as bb_pat_pos.cpp pre-`61ae501e`. Convention: sites point at the FIRST
         *      byte of rel32 (or where the label-define resolves), per bb_emit_asm_result in
         *      emit_str.cpp:70-77.
         *  (2) Sites for RTAB were {25, 32, 37, 38} — same off-by-one for sites 0 and 1.
         *  (3) RTAB had a SEMANTIC bug at offset 30: the success-path "writeback" was
         *      `89 C1` (mov ecx, eax) — a non-effective instruction that overwrote ecx (which
         *      held Σlen-N) with eax (which held Δ). The TEXT arm shows the intent: mov [r10],
         *      ecx (3 bytes: `41 89 0A`) to set Δ ← Σlen-N. The wrong instruction wrote NOTHING
         *      to Δ, leaving the cursor unchanged on success. Affected any RTAB use where the
         *      cursor needs to advance to Σlen-N. With the writeback corrected (+1 byte), the
         *      trailing jmp γ and β-define labels shift by +1 too, so RTAB sites become
         *      {26, 34, 38, 39} instead of just {26, 33, 37, 38}.
         * TAB layout (32 bytes):
         *   [0-2]   41 8B 02              mov eax, [r10]
         *   [3-7]   3D + u32le(imm)       cmp eax, imm32
         *   [8-9]   0F 8F                 jg opcode (2 bytes)
         *   [10-13] rel32                 → ω         site 10
         *   [14-18] B8 + u32le(imm)       mov eax, imm32
         *   [19-21] 41 89 02              mov [r10], eax
         *   [22]    E9                    jmp opcode
         *   [23-26] rel32                 → γ         site 23
         *   [27]    E9                    jmp opcode  β label-define here, site 27 (is_def)
         *   [28-31] rel32                 → ω         site 28
         * RTAB layout (43 bytes — was 42, +1 for writeback fix):
         *   [0-1]   48 B9                 movabs rcx, imm64 prefix
         *   [2-9]   u64le(&Σlen)
         *   [10-11] 8B 01                 mov eax, [rcx]    ; eax = Σlen
         *   [12-16] 2D + u32le(imm)       sub eax, imm32    ; eax = Σlen - N
         *   [17-18] 89 C1                 mov ecx, eax      ; ecx = Σlen - N (preserve)
         *   [19-21] 41 8B 02              mov eax, [r10]    ; eax = Δ
         *   [22-23] 39 C8                 cmp eax, ecx
         *   [24-25] 0F 8F                 jg opcode
         *   [26-29] rel32                 → ω         site 26
         *   [30-32] 41 89 0A              mov [r10], ecx    ; Δ ← Σlen-N  (FIX: was 89 C1)
         *   [33]    E9                    jmp opcode
         *   [34-37] rel32                 → γ         site 34
         *   [38]    E9                    jmp opcode  β label-define here, site 38 (is_def)
         *   [39-42] rel32                 → ω         site 39
         */
        bin = rtab
            ? bb_bin_t{ {26, 34, 38, 39}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, true, false} }
            : bb_bin_t{ {10, 23, 27, 28}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, true, false} };
        return IF(MEDIUM_MACRO_DEF, s_comment(rtab ? "# no macro form — RTAB" : "# no macro form — TAB"))
             + IF(MEDIUM_BINARY, rtab
                    ? bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN)
                     + bytes(2, "\x8B\x01")
                     + bytes(1, "\x2D") + u32le((uint32_t)(int)pBB->ival)
                     + bytes(2, "\x89\xC1")
                     + bytes(3, "\x41\x8B\x02")
                     + bytes(2, "\x39\xC8")
                     + bytes(2, "\x0F\x8F") + u32le(0)
                     + bytes(3, "\x41\x89\x0A")
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0)
                    : bytes(3, "\x41\x8B\x02")
                     + bytes(1, "\x3D") + u32le((uint32_t)(int)pBB->ival)
                     + bytes(2, "\x0F\x8F") + u32le(0)
                     + bytes(1, "\xB8") + u32le((uint32_t)(int)pBB->ival)
                     + bytes(3, "\x41\x89\x02")
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                   + s_1asm(emit_fmt("# BOX %s(%d)", rtab ? "RTAB" : "TAB", (int)pBB->ival))
                 + s_directive(".intel_syntax noprefix")
                 + (rtab
                    ? s_2asm("lea", "rax, [rip + \xce\xa3len]")
                     + s_2asm("mov", "ecx, dword ptr [rax]")
                     + s_1asm(emit_fmt("sub ecx, %d", (int)pBB->ival))
                     + s_2asm("lea", "rax, [rip + \xce\x94]")
                     + s_2asm("cmp", "ecx, dword ptr [rax]")
                     + s_1asm(emit_fmt("jl %s", _.lbl_ω))
                     + s_2asm("mov", "dword ptr [rax], ecx")
                     + s_1asm(emit_fmt("jmp %s", _.lbl_γ))
                    : s_2asm("lea", "rax, [rip + \xce\x94]")
                     + s_2asm("mov", "ecx, dword ptr [rax]")
                     + s_1asm(emit_fmt("cmp ecx, %d", (int)pBB->ival))
                     + s_1asm(emit_fmt("jg %s", _.lbl_ω))
                     + s_1asm(emit_fmt("mov dword ptr [rax], %d", (int)pBB->ival))
                     + s_1asm(emit_fmt("jmp %s", _.lbl_γ)))
                 + s_1asm(emit_fmt("%s: jmp %s", _.lbl_β, _.lbl_ω)));
    }
    if (PLATFORM_JVM) {
        const char *nm = rtab ? "rtab" : "tab";
        std::string tag_s = emit_fmt("%s_%d_%d", nm, sid, nid);
        std::string tag_fail = tag_s + "_ω";
        std::string result = jvm_class_hdr_str(nm)
                           + s_directive(".field private final n I")
                           + s_directive(".field private final dyn Ljava/util/function/IntSupplier;")
                           + s_directive(".field private advance I")
                           + jvm_init_ms_int_str(nm, "n")
                           + jvm_val_helper_str(nm)
                           + s_directive(".method public α()Lbb/bb_box$Spec;")
                           + s_directive(".limit stack 5")
                           + s_1asm(emit_fmt("    .limit locals %d", rtab ? 4 : 3));
        if (rtab) {
            result += s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
                    + s_1asm("aload_0")
                    + s_2asm("invokevirtual", "bb/bb_rtab/val()I")
                    + s_1asm("isub") + s_1asm("istore_1")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("iload_1")
                    + s_1asm("    if_icmpgt " + tag_fail)
                    + s_1asm("iload_1") + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("isub") + s_1asm("istore_2")
                    + s_1asm("aload_0") + s_1asm("iload_2")
                    + s_2asm("putfield", "bb/bb_rtab/advance I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("istore_3")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_1asm("iload_1")
                    + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
                    + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup")
                    + s_1asm("iload_3") + s_1asm("iload_2")
                    + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
                    + std::string(" ") + s_L1asm(tag_fail + ":", "")
                    + s_1asm("aconst_null") + s_1asm("areturn")
                    + s_directive(".end method")
                    + s_directive(".method public β()Lbb/bb_box$Spec;")
                    + s_directive(".limit stack 4") + s_directive(".limit locals 1")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_1asm("dup")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/advance I")
                    + s_1asm("isub")
                    + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aconst_null") + s_1asm("areturn")
                    + s_directive(".end method");
        } else {
            result += s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aload_0")
                    + s_2asm("invokevirtual", "bb/bb_tab/val()I")
                    + s_1asm("    if_icmpgt " + tag_fail)
                    + s_1asm("aload_0")
                    + s_2asm("invokevirtual", "bb/bb_tab/val()I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("isub") + s_1asm("istore_1")
                    + s_1asm("aload_0") + s_1asm("iload_1")
                    + s_2asm("putfield", "bb/bb_tab/advance I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("istore_2")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_1asm("aload_0")
                    + s_2asm("invokevirtual", "bb/bb_tab/val()I")
                    + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
                    + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup")
                    + s_1asm("iload_2") + s_1asm("iload_1")
                    + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
                    + std::string(" ") + s_L1asm(tag_fail + ":", "")
                    + s_1asm("aconst_null") + s_1asm("areturn")
                    + s_directive(".end method")
                    + s_directive(".method public β()Lbb/bb_box$Spec;")
                    + s_directive(".limit stack 4") + s_directive(".limit locals 1")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_1asm("dup")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/advance I")
                    + s_1asm("isub")
                    + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aconst_null") + s_1asm("areturn")
                    + s_directive(".end method");
        }
        return result;
    }
    if (PLATFORM_JS) {
        int64_t n = pBB->ival;
        std::string alpha = rtab
            ? emit_fmt("α() { const tgt = ms.omega - n; if (ms.delta > tgt) { self.fail.α(); return"
                "; } delta = tgt - ms.delta; const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n")
            : emit_fmt("α() { if (ms.delta > n || ms.delta > ms.omega) { self.fail.α(); return; } delta = n - ms.delta"
                "; if (ms.delta + delta > ms.omega) delta = ms.omega - ms.delta; const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n");
        return emit_fmt("function make_pat_%d_%d(ms) { const n = %ld; let delta = 0; let self = { succ: null, fail: null,\n", pBB->ival, nid, n)
             + alpha
             + "β() { ms.delta -= delta; self.fail.α(); }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int n = (int)pBB->ival;
        const char *lbl = rtab ? "RTAB" : "TAB";
        std::string r = net_class_hdr_str(sid, nid)
            + s_directive(".field private int32 _n")
            + s_directive(".field private int32 _advance")
            + s_directive(".method public specialname rtspecialname instance void .ctor(int32 n) cil managed")
            + s_1asm("{") + s_2asm(".maxstack", "2")
            + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
            + s_1asm("ldarg.0") + s_1asm("ldarg.1")
            + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_n", sid, nid))
            + s_1asm("ret") + s_1asm("}");
        if (rtab) {
            r += net_α_hdr_str() + s_2asm(".maxstack", "4")
               + s_2asm(".locals", "init (int32 V_target, valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
               + s_1asm("ldarg.1") + net_ms_length_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
               + s_1asm("sub") + s_1asm("stloc.0")
               + net_cursor_load_str() + s_1asm("ldloc.0")
               + s_1asm(emit_fmt("    bgt        %s_%d_%d_FAIL", lbl, sid, nid))
               + s_1asm("ldarg.0")
               + s_1asm("ldloc.0") + net_cursor_load_str() + s_1asm("sub")
               + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_advance", sid, nid))
               + net_cursor_load_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_advance", sid, nid))
               + net_spec_of_str() + s_1asm("stloc.1")
               + s_1asm("ldarg.1") + s_1asm("ldloc.0")
               + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
               + s_1asm("ldloc.1") + s_1asm("ret");
        } else {
            r += net_α_hdr_str() + s_2asm(".maxstack", "3")
               + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
               + net_cursor_load_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
               + s_1asm(emit_fmt("    bgt        %s_%d_%d_FAIL", lbl, sid, nid))
               + s_1asm("ldarg.0") + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
               + net_cursor_load_str() + s_1asm("sub")
               + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_advance", sid, nid))
               + net_cursor_load_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_advance", sid, nid))
               + net_spec_of_str() + s_1asm("stloc.0")
               + s_1asm("ldarg.1") + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
               + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
               + s_1asm("ldloc.0") + s_1asm("ret");
        }
        r += s_1asm(emit_fmt("  %s_%d_%d_FAIL:", lbl, sid, nid)) + net_fail_ret_str() + s_1asm("}")
           + net_β_hdr_str() + s_2asm(".maxstack", "3")
           + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
           + s_1asm("ldarg.0")
           + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_advance", sid, nid))
           + s_1asm("sub")
           + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
           + net_fail_ret_str() + s_1asm("}") + s_1asm("}")
           + net_push_i4_str(n)
           + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor(int32)", sid, nid));
        return r;
    }
    if (PLATFORM_WASM) {
        return emit_fmt("          (call $bb_%s_new)\n", rtab ? "rtab" : "tab");
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_tab(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_tab_str(pBB, bin), bin);
}

/* bb_pat_pos.cpp -- BB template for POS and RPOS.
   POS and RPOS share emit shape in every backend (single ival2 flag).
   Invariant #10: grouping permitted -- both opcodes have the same shape globally. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_pos_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); int sid = 0;
    /* SBL-POS-RPOS-FLAG (2026-05-29 Opus 4.7): RPOS is distinguished by sval=="r" per
     * lower_pat_dcg.c (TT_RPOS / XRPSI branches). The prior `ival != 0` heuristic conflated
     * "non-zero offset" with "is RPOS", which misclassified RPOS(0) as POS(0) and POS(N>0) as
     * RPOS(N>0). Only `sval` is authoritative. Without this fix, RPOS(0) (a very common idiom
     * — "at end of subject") generated POS(0) bytes, so any anchored pattern using RPOS(0)
     * after the cursor advanced past 0 silently fails to match. Affected mode-3 native AND
     * mode-4 compile (TEXT arm uses the same `rpos` variable). */
    int rpos = (pBB->sval && pBB->sval[0] == 'r');
    if (PLATFORM_X86) {
        /* SBL-POS-PATCH-OFFSET (Opus 4.7, 2026-05-28): sites must point at the FIRST BYTE OF THE rel32
         * (or the byte position where a label-define resolves), per the bb_emit_asm_result convention
         * (emit_str.cpp:70-77): the patcher streams bytes until pos == site, then either defines a label
         * at that position or writes a 4-byte rel32 there. Validated against bb_pat_any.cpp (sites
         * {17, 72, 86, 90, 100} — annotated layout shows each site is the byte AFTER the opcode bytes).
         * Prior values {9,15,20,21} / {25,31,36,37} were off by +1 on sites 1/3/4 (author used "last byte
         * of 0F xx opcode" for site 1 and shifted 3/4 to compensate), causing the patcher to overwrite
         * the 0F 85 jne opcode byte at runtime (corrupting to 0F 32 = rdmsr → SIGSEGV on rung 044).
         * POS bytes (24 total):
         *   [0-2]   41 8B 02              mov eax, [r10]
         *   [3-7]   3D + u32le(imm)       cmp eax, imm32
         *   [8-9]   0F 85                 jne opcode (2 bytes)
         *   [10-13] rel32                 → ω         site 10
         *   [14]    E9                    jmp opcode
         *   [15-18] rel32                 → γ         site 15
         *   [19]    E9                    jmp opcode  β label-define here, site 19 (is_def)
         *   [20-23] rel32                 → ω         site 20
         * RPOS bytes (40 total): same shape, offset by 16 bytes of Σlen-load preamble.
         *   [24-25] 0F 85 jne; [26-29] rel32 → ω    site 26
         *   [30]    E9 jmp;     [31-34] rel32 → γ   site 31
         *   [35]    E9 jmp;     β-define at 35     site 35 (is_def)
         *   [36-39] rel32 → ω                       site 36
         */
        bin = rpos
            ? bb_bin_t{ {26, 31, 35, 36}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, true, false} }
            : bb_bin_t{ {10, 15, 19, 20}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, true, false} };
        return IF(MEDIUM_MACRO_DEF, s_comment(rpos ? "# no macro form — RPOS" : "# no macro form — POS"))
             + IF(MEDIUM_BINARY, rpos
                    ? bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN)
                     + bytes(2, "\x8B\x01")
                     + bytes(1, "\x2D") + u32le((uint32_t)(int)pBB->ival)
                     + bytes(2, "\x89\xC1")
                     + bytes(3, "\x41\x8B\x02")
                     + bytes(2, "\x39\xC8")
                     + bytes(2, "\x0F\x85") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0)
                    : bytes(3, "\x41\x8B\x02")
                     + bytes(1, "\x3D") + u32le((uint32_t)(int)pBB->ival)
                     + bytes(2, "\x0F\x85") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT, s_1asm(emit_fmt("%s:", _.lbl_α))
                 + (rpos
                    ? s_comment(emit_fmt("# BOX RPOS(%d)", (int)pBB->ival))
                     + s_2asm("lea rcx,", emit_fmt("[rip + \xce\xa3len]"))
                     + s_2asm("mov", "eax, dword ptr [rcx]")
                     + s_2asm("sub", emit_fmt("eax, %u", (unsigned)(int)pBB->ival))
                     + s_2asm("mov", "ecx, eax")
                     + s_2asm("mov", "eax, [r10]")
                     + s_2asm("cmp", "eax, ecx")
                    : s_comment(emit_fmt("# BOX POS(%d)", (int)pBB->ival))
                     + s_2asm("mov", "eax, [r10]")
                     + s_2asm("cmp", emit_fmt("eax, %u", (unsigned)(int)pBB->ival)))
                 + s_2asm("jne", _.lbl_ω)
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    if (PLATFORM_JVM) {
        const char *nm = rpos ? "rpos" : "pos";
        std::string tag_s = emit_fmt("%s_%d_%d", nm, sid, nid);
        std::string r = jvm_class_hdr_str(nm)
            + s_directive(".field private final n I")
            + s_directive(".field private final dyn Ljava/util/function/IntSupplier;")
            + jvm_init_ms_int_str(nm, "n") + jvm_val_helper_str(nm)
            + s_directive(".method public α()Lbb/bb_box$Spec;")
            + s_directive(".limit stack 5")
            + s_directive(".limit locals 1");
        if (rpos) {
            r += s_1asm("aload_0") + s_2asm("getfield", "bb/bb_rpos/ms Lbb/bb_box$MatchState;")
               + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("aload_0")
               + s_2asm("getfield", "bb/bb_rpos/ms Lbb/bb_box$MatchState;")
               + s_2asm("getfield", "bb/bb_box$MatchState/ω I") + s_1asm("aload_0")
               + s_2asm("invokevirtual", "bb/bb_rpos/val()I") + s_1asm("isub")
               + s_1asm("    if_icmpne " + tag_s + "_ω");
        } else {
            r += s_1asm("aload_0") + s_2asm("getfield", "bb/bb_pos/ms Lbb/bb_box$MatchState;")
               + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("aload_0")
               + s_2asm("invokevirtual", "bb/bb_pos/val()I")
               + s_1asm("    if_icmpne " + tag_s + "_ω");
        }
        r += s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("aload_0")
           + s_1asm(emit_fmt("    getfield bb/bb_%s/ms Lbb/bb_box$MatchState;", nm))
           + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("iconst_0")
           + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
           + s_1asm(tag_s + "_ω:") + s_1asm("aconst_null") + s_1asm("areturn")
           + s_directive(".end method") + s_directive(".method public β()Lbb/bb_box$Spec;")
           + s_directive(".limit stack 1") + s_directive(".limit locals 1")
           + s_1asm("aconst_null") + s_1asm("areturn") + s_directive(".end method");
        return r;
    }
    if (PLATFORM_JS) {
        int64_t n = pBB->ival;
        return emit_fmt("function make_pat_%d_%d(ms) { const n = %ld; let self = { succ: null, fail: null,\n", pBB->ival, nid, n)
             + (rpos ? std::string("α() { if (ms.delta !== ms.omega - n) { self.fail.α(); return; } self.succ.α(); return ''; },\n")
                     : std::string("α() { if (ms.delta !== n) { self.fail.α(); return; } self.succ.α(); return ''; },\n"))
             + "β() { self.fail.α(); }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int n = (int)pBB->ival;
        const char *lbl = rpos ? "RPOS" : "POS";
        std::string r = net_class_hdr_str(sid, nid)
            + s_directive(".field private int32 _n")
            + s_directive(".method public specialname rtspecialname instance void .ctor(int32 n) cil managed")
            + s_1asm("{") + s_2asm(".maxstack", "2")
            + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
            + s_1asm("ldarg.0") + s_1asm("ldarg.1")
            + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_n", sid, nid))
            + s_1asm("ret") + s_1asm("}")
            + net_α_hdr_str() + s_2asm(".maxstack", "3") + net_cursor_load_str();
        if (rpos) {
            r += s_1asm("ldarg.1") + net_ms_length_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid))
               + s_1asm("sub");
        } else {
            r += s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, nid));
        }
        r += s_1asm(emit_fmt("    bne.un     %s_%d_%d_FAIL", lbl, sid, nid))
          + net_cursor_load_str() + net_spec_zw_str() + s_1asm("ret")
          + s_1asm(emit_fmt("  %s_%d_%d_FAIL:", lbl, sid, nid)) + net_fail_ret_str() + s_1asm("}")
          + net_β_hdr_str() + s_2asm(".maxstack", "1") + net_fail_ret_str() + s_1asm("}")
          + s_1asm("}")
          + net_push_i4_str(n) + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor(int32)", sid, nid));
        return r;
    }
    if (PLATFORM_WASM) {
        return emit_fmt("          (call $bb_%s_new)\n", rpos ? "rpos" : "pos");
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_pos(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_pos_str(pBB, bin), bin);
}

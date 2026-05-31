/* bb_pat_break.cpp — BB template for BREAK. Converted to return-string (ER wave). */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_break_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char *cs = pBB->sval ? pBB->sval : "";
        const char *cs_label = emit_intern_str(cs);
        std::string z   = emit_fmt(".Lbrk%d_z", g_emit.bb_cs_id);
        std::string lp  = emit_fmt(".Lbrk%d_loop", g_emit.bb_cs_id);
        std::string dn  = emit_fmt(".Lbrk%d_done", g_emit.bb_cs_id);
        std::string lp2 = emit_fmt(".Lbrk%d_xloop", g_emit.bb_cs_id);
        std::string dn2 = emit_fmt(".Lbrk%d_xdone", g_emit.bb_cs_id);
        int is_breakx   = (pBB->ival == 1);
        /* SBL-BREAK-2 (Opus 4.7, 2026-05-28): plain-BREAK BINARY arm filled (178 bytes).
         * Mirrors the plain-BREAK (`!is_breakx`) TEXT semantics. The TEXT arm uses two .data slots
         * (z, z_orig); under MEDIUM_BINARY brokered blobs have no ELF .data, so the per-node count
         * lives in the GC-allocated rt_cs_t pointed to by g_emit.bb_cs_zeta — its `delta` field sits
         * at byte offset 8 (rt_cs_t = {const char *chars; int delta;}, verified sizeof=16). The count
         * MUST persist across the α→γ→(backtrack)→β boundary, which a register cannot do; [zeta+8] can.
         * z_orig is unnecessary in the binary form because Δ ([r10]) is read fresh each loop iteration
         * and is not mutated until the final `[r10] += z`; β recovers via `[r10] -= z`.
         * Internal jumps (jge/jnz→done, jmp→loop back-edge) have constant displacements in a fixed-size
         * blob, emitted as literal rel32 bytes; only γ(150)/β-def(154)/ω(174) use the sites table.
         * Reached only via runtime-PATND scan (rt_match_variant → exec_stmt → bb_build_*). BREAKX
         * (is_breakx, pBB->ival==1) BINARY arm is the 302-byte α-scan + β-rescan form — SBL-BREAKX-2.
         *   off  bytes                       asm
         *    0   48 B9 [zeta u64]            movabs rcx, &zeta
         *   10   C7 41 08 00 00 00 00        mov dword [rcx+8], 0          ; z = 0
         *  loop(17):
         *   17   48 B8 [SIGMA u64]           movabs rax, &Σ
         *   27   4C 8B 18                    mov  r11, [rax]               ; r11 = subject ptr
         *   30   41 8B 02                    mov  eax, [r10]               ; eax = Δ
         *   33   48 B9 [zeta u64]            movabs rcx, &zeta
         *   43   03 41 08                    add  eax, [rcx+8]             ; eax = Δ + z
         *   46   48 B9 [SIGLEN u64]          movabs rcx, &Σlen
         *   56   3B 01                       cmp  eax, [rcx]
         *   58   0F 8D 42 00 00 00           jge  done (+66 → 130)
         *   64   48 63 C8                    movsxd rcx, eax
         *   67   41 0F B6 34 0B              movzx esi, byte [r11+rcx]
         *   72   48 BF [cs u64]              movabs rdi, &cset
         *   82   41 52                       push r10
         *   84   41 53                       push r11
         *   86   48 B8 [strchr u64]          movabs rax, &strchr
         *   96   FF D0                       call rax
         *   98   41 5B                       pop  r11
         *  100   41 5A                       pop  r10
         *  102   48 85 C0                    test rax, rax
         *  105   0F 85 13 00 00 00           jnz  done (+19 → 130)        ; char IN set → break here
         *  111   48 B9 [zeta u64]            movabs rcx, &zeta
         *  121   83 41 08 01                 add dword [rcx+8], 1         ; z++
         *  125   E9 8F FF FF FF              jmp  loop (-113 → 17)
         *  done(130):
         *  130   41 8B 02                    mov  eax, [r10]
         *  133   48 B9 [zeta u64]            movabs rcx, &zeta
         *  143   03 41 08                    add  eax, [rcx+8]            ; Δ += z
         *  146   41 89 02                    mov  [r10], eax
         *  149   E9 [rel32]                  jmp  γ                       ; site 150 → γ
         *  154   (lbl_β define)                                          ; site 154 → β (is_def)
         *  154   41 8B 02                    mov  eax, [r10]
         *  157   48 B9 [zeta u64]            movabs rcx, &zeta
         *  167   2B 41 08                    sub  eax, [rcx+8]            ; Δ -= z (undo)
         *  170   41 89 02                    mov  [r10], eax
         *  173   E9 [rel32]                  jmp  ω                       ; site 174 → ω
         *  178   (end)
         */
        uint64_t strchr_addr;
        { const char *(*fp)(const char *, int) = strchr; strchr_addr = (uint64_t)(uintptr_t)(void *)fp; }
        uint64_t cs_addr   = (uint64_t)(uintptr_t)(const void *)cs;
        uint64_t zeta_addr = (uint64_t)(uintptr_t)g_emit.bb_cs_zeta;
        if (!is_breakx) {
            bin = { {150, 154, 174, 179},
                    {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_ω_p},
                    {false, true, false, false} };
        } else {
            /* SBL-BREAKX-2 (Opus 4.8, 2026-05-29): real 302-byte BINARY arm. α scans to first
             * cset char (Δ += z, jmp γ); β recovers z_orig = Δ - z, steps past (z++), rescans to
             * the NEXT cset char (jmp γ on found, jmp ω on exhausted). z lives in [zeta+8]; z_orig
             * is recovered arithmetically (no second slot). Sites: γ(139)/ω(144)/β-DEF(148)/γ(293)/
             * ω(298), all assembled+verified via `as`. Mirrors bb_exec.c IR_PAT_BREAK ival==1 arm. */
            bin = { {139, 144, 148, 293, 298},
                    {_.lbl_γ_p, _.lbl_ω_p, _.lbl_β_p, _.lbl_γ_p, _.lbl_ω_p},
                    {false, false, true, false, false} };
        }
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — BREAK"))
             + IF(MEDIUM_BINARY,
                   is_breakx
                 ? (  bytes(2, "\x48\xB9") + u64le(zeta_addr)
                    + bytes(9, "\xC7\x41\x08\x00\x00\x00\x00\x48\xB8") + u64le(TEMPLATE_ADDR_SIGMA)
                    + bytes(8, "\x4C\x8B\x18\x41\x8B\x02\x48\xB9") + u64le(zeta_addr)
                    + bytes(5, "\x03\x41\x08\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN)
                    + bytes(14, "\x3B\x01\x7D\x53\x48\x63\xC8\x41\x0F\xB6\x34\x0B\x48\xBF") + u64le(cs_addr)
                    + bytes(6, "\x41\x52\x41\x53\x48\xB8") + u64le(strchr_addr)
                    + bytes(13, "\xFF\xD0\x41\x5B\x41\x5A\x48\x85\xC0\x75\x10\x48\xB9") + u64le(zeta_addr)
                    + bytes(11, "\x83\x41\x08\x01\xEB\x9A\x41\x8B\x02\x48\xB9") + u64le(zeta_addr)
                    + bytes(7, "\x03\x41\x08\x41\x89\x02\xE9") + u32le(0)
                    + bytes(1, "\xE9") + u32le(0)
                    + bytes(5, "\x41\x8B\x02\x48\xB9") + u64le(zeta_addr)
                    + bytes(12, "\x2B\x41\x08\x41\x89\x02\x83\x41\x08\x01\x48\xB8") + u64le(TEMPLATE_ADDR_SIGMA)
                    + bytes(8, "\x4C\x8B\x18\x41\x8B\x02\x48\xB9") + u64le(zeta_addr)
                    + bytes(5, "\x03\x41\x08\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN)
                    + bytes(14, "\x3B\x01\x7D\x53\x48\x63\xC8\x41\x0F\xB6\x34\x0B\x48\xBF") + u64le(cs_addr)
                    + bytes(6, "\x41\x52\x41\x53\x48\xB8") + u64le(strchr_addr)
                    + bytes(13, "\xFF\xD0\x41\x5B\x41\x5A\x48\x85\xC0\x75\x10\x48\xB9") + u64le(zeta_addr)
                    + bytes(11, "\x83\x41\x08\x01\xEB\x9A\x41\x8B\x02\x48\xB9") + u64le(zeta_addr)
                    + bytes(7, "\x03\x41\x08\x41\x89\x02\xE9") + u32le(0)
                    + bytes(1, "\xE9") + u32le(0))
                 : (  bytes(2, "\x48\xB9") + u64le(zeta_addr)
                    + bytes(7, "\xC7\x41\x08\x00\x00\x00\x00")
                    + bytes(2, "\x48\xB8") + u64le(TEMPLATE_ADDR_SIGMA)
                    + bytes(3, "\x4C\x8B\x18")
                    + bytes(3, "\x41\x8B\x02")
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)
                    + bytes(3, "\x03\x41\x08")
                    + bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN)
                    + bytes(2, "\x3B\x01")
                    + bytes(2, "\x0F\x8D") + u32le(114)
                    + bytes(3, "\x48\x63\xC8")
                    + bytes(5, "\x41\x0F\xB6\x34\x0B")
                    + bytes(2, "\x48\xBF") + u64le(cs_addr)
                    + bytes(2, "\x41\x52")
                    + bytes(2, "\x41\x53")
                    + bytes(2, "\x48\xB8") + u64le(strchr_addr)
                    + bytes(2, "\xFF\xD0")
                    + bytes(2, "\x41\x5B")
                    + bytes(2, "\x41\x5A")
                    + bytes(3, "\x48\x85\xC0")
                    + bytes(2, "\x0F\x85") + u32le(19)
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)
                    + bytes(4, "\x83\x41\x08\x01")
                    + bytes(1, "\xE9") + u32le((uint32_t)(int32_t)(-113))
                    + bytes(3, "\x41\x8B\x02")
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)
                    + bytes(3, "\x03\x41\x08")
                    + bytes(3, "\x41\x89\x02")
                    + bytes(1, "\xE9") + u32le(0)
                    + bytes(3, "\x41\x8B\x02")
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)
                    + bytes(3, "\x2B\x41\x08")
                    + bytes(3, "\x41\x89\x02")
                    + bytes(1, "\xE9") + u32le(0)
                    + bytes(1, "\xE9") + u32le(0)))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(is_breakx ? "# BOX BREAKX()" : "# BOX BREAK()")
                 + s_directive(".section .data")
                 + s_directive(z + ": .long 0")
                 + s_directive(z + "_orig: .long 0")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("mov", "dword ptr [rip + " + z + "_orig], eax")
                 + s_2asm("mov", "dword ptr [rip + " + z + "], 0")
                 + s_2asm("lea", "r11, [rip + \xCE\xA3]")
                 + s_2asm("mov", "r11, [r11]")
                 + s_1asm(emit_fmt("%s:", lp.c_str()))
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("add", "eax, dword ptr [rip + " + z + "]")
                 + s_2asm("lea", "rcx, [rip + \xCE\xA3" "len]")
                 + s_2asm("cmp", "eax, [rcx]")
                 + s_2asm("jge", _.lbl_ω)
                 + s_2asm("movsxd", "rcx, eax")
                 + s_2asm("movzx", "esi, byte ptr [r11+rcx]")
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", cs_label ? cs_label : "??"))
                 + s_2asm("push", "r10")
                 + s_2asm("push", "r11")
                 + s_2asm("call", "strchr@PLT")
                 + s_2asm("pop", "r11")
                 + s_2asm("pop", "r10")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("jnz", dn.c_str())
                 + s_2asm("add", "dword ptr [rip + " + z + "], 1")
                 + s_2asm("jmp", lp.c_str())
                 + s_1asm(emit_fmt("%s:", dn.c_str()))
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("add", "eax, dword ptr [rip + " + z + "]")
                 + s_2asm("mov", "[r10], eax")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(emit_fmt("%s:", _.lbl_β))
                 + (is_breakx
                    ? (  s_comment("# BREAKX β: step past break char, rescan to next")
                       + s_2asm("mov", "eax, dword ptr [rip + " + z + "_orig]")
                       + s_2asm("mov", "[r10], eax")
                       + s_2asm("mov", "ecx, dword ptr [rip + " + z + "]")
                       + s_2asm("add", "ecx, 1")
                       + s_2asm("mov", "dword ptr [rip + " + z + "], ecx")
                       + s_2asm("lea", "r11, [rip + \xCE\xA3]")
                       + s_2asm("mov", "r11, [r11]")
                       + s_1asm(emit_fmt("%s:", lp2.c_str()))
                       + s_2asm("mov", "eax, dword ptr [rip + " + z + "_orig]")
                       + s_2asm("add", "eax, dword ptr [rip + " + z + "]")
                       + s_2asm("lea", "rdx, [rip + \xCE\xA3" "len]")
                       + s_2asm("cmp", "eax, [rdx]")
                       + s_2asm("jge", _.lbl_ω)
                       + s_2asm("movsxd", "rcx, eax")
                       + s_2asm("movzx", "esi, byte ptr [r11+rcx]")
                       + s_2asm("lea", emit_fmt("rdi, [rip + %s]", cs_label ? cs_label : "??"))
                       + s_2asm("push", "r10")
                       + s_2asm("push", "r11")
                       + s_2asm("call", "strchr@PLT")
                       + s_2asm("pop", "r11")
                       + s_2asm("pop", "r10")
                       + s_2asm("test", "rax, rax")
                       + s_2asm("jnz", dn2.c_str())
                       + s_2asm("add", "dword ptr [rip + " + z + "], 1")
                       + s_2asm("jmp", lp2.c_str())
                       + s_1asm(emit_fmt("%s:", dn2.c_str()))
                       + s_2asm("mov", "eax, dword ptr [rip + " + z + "_orig]")
                       + s_2asm("add", "eax, dword ptr [rip + " + z + "]")
                       + s_2asm("mov", "[r10], eax")
                       + s_2asm("jmp", _.lbl_γ))
                    : (  s_2asm("mov", "eax, [r10]")
                       + s_2asm("sub", "eax, dword ptr [rip + " + z + "]")
                       + s_2asm("mov", "[r10], eax")
                       + s_2asm("jmp", _.lbl_ω))));
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("brk")
             + s_directive(".field private final chars Ljava/lang/String;")
             + s_directive(".field private matched_len I")
             + jvm_init_ms_str_str("brk", "chars")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6") + s_directive(".limit locals 3")
             + s_1asm("aload_0") + s_1asm("iconst_0")
             + s_2asm("putfield", "bb/bb_brk/matched_len I")
             + s_L1asm(emit_fmt("brk_%d_%d", 0, bb_node_id(pBB)) + "_loop:", "aload_0")
             + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_1asm("iadd") + s_1asm("istore_1") + s_1asm("iload_1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
             + s_1asm("if_icmpge " + emit_fmt("brk_%d_%d", 0, bb_node_id(pBB)) + "_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/chars Ljava/lang/String;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/sigma Ljava/lang/String;")
             + s_1asm("iload_1")
             + s_2asm("invokevirtual", "java/lang/String/charAt(I)C")
             + s_2asm("invokevirtual", "java/lang/String/indexOf(I)I")
             + s_1asm("ifge " + emit_fmt("brk_%d_%d", 0, bb_node_id(pBB)) + "_found")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_brk/matched_len I")
             + s_1asm("goto " + emit_fmt("brk_%d_%d", 0, bb_node_id(pBB)) + "_loop")
             + s_L1asm(emit_fmt("brk_%d_%d", 0, bb_node_id(pBB)) + "_found:", "aload_0")
             + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("istore_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("iload_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(emit_fmt("brk_%d_%d", 0, bb_node_id(pBB)) + "_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_1asm("isub") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const chars = ", pBB->ival, bb_node_id(pBB))
             + js_escape_string_str(pBB->sval)
             + "; let delta = 0; let self = { succ: null, fail: null,\n"
               "α() { delta = 0; while (ms.delta + delta < ms.omega && chars.indexOf(ms.sigma[ms.delta + delta]) < 0) delta++; const r = ms.sigma.slice(ms.delta"
                   ", ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n"
               "β() { ms.delta -= delta; self.fail.α(); }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_class_hdr_str(0, bb_node_id(pBB))
             + s_directive(".field private string _chars")
             + s_directive(".field private int32  _count")
             + s_directive(".method public specialname rtspecialname instance void .ctor(string chars) cil managed")
             + s_1asm("{") + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
             + s_1asm("ldarg.0") + s_1asm("ldarg.1") + s_1asm("dup")
             + s_1asm(emit_fmt("brtrue     BRK_%d_%d_NN", 0, bb_node_id(pBB)))
             + s_1asm("pop") + s_2asm("ldstr", "\"\"")
             + s_directive(emit_fmt("  BRK_%d_%d_NN:", 0, bb_node_id(pBB)))
             + s_1asm(emit_fmt("stfld      string pat_%d_%d::_chars", 0, bb_node_id(pBB)))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "4")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_directive(emit_fmt("  BRK_%d_%d_LOOP:", 0, bb_node_id(pBB)))
             + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add") + s_1asm("ldarg.1") + net_ms_length_str()
             + s_1asm(emit_fmt("bge        BRK_%d_%d_EOS", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.1") + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      string pat_%d_%d::_chars", 0, bb_node_id(pBB)))
             + s_2asm("callvirt", "instance bool [boxes]Snobol4.Runtime.Boxes.MatchState::CharInSet(int32, string)")
             + s_1asm(emit_fmt("brtrue     BRK_%d_%d_FOUND", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm(emit_fmt("br         BRK_%d_%d_LOOP", 0, bb_node_id(pBB)))
             + s_directive(emit_fmt("  BRK_%d_%d_EOS:", 0, bb_node_id(pBB))) + net_fail_ret_str()
             + s_directive(emit_fmt("  BRK_%d_%d_FOUND:", 0, bb_node_id(pBB)))
             + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + net_spec_of_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0") + s_1asm("ret") + s_1asm("}")
             + net_β_hdr_str() + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("sub")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + net_fail_ret_str() + s_1asm("}") + s_1asm("}")
             + net_escape_ldstr_str(pBB->sval ? pBB->sval : "")
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(string)", 0, bb_node_id(pBB)));
    }
    if (PLATFORM_WASM) { return "          (call $bb_break_new)\n"; }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_break(IR_t * pBB) {
    if (MEDIUM_TEXT)   g_emit.bb_cs_id = g_flat_node_id++;
    if (MEDIUM_BINARY) g_emit.bb_cs_zeta = rt_cs_new(pBB->sval ? pBB->sval : "");
    bb_bin_t bin;
    std::string out = bb_pat_break_str(pBB, bin);
    if (!out.empty()) bb_emit_asm_result(out, bin);
}

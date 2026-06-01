/* bb_pat_notany.cpp -- BB template for NOTANY.  CPP return-String (GOAL-HEADQUARTERS CPP rung).
   One file per opcode. Invariant #10: no grouping with other opcodes (distinct emit shape across backends). */
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
static std::string bb_pat_notany_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char *cs = pBB->sval ? pBB->sval : "";
        const char *cs_label = emit_intern_str(cs);
        /* REG-2 (GOAL-SNOBOL4-BB REG ladder, 2026-06-01): cursor δ=R14d, subject base Σ=R13, length Δ=R15d
         * (ratified regs, bb_regs.h; established by BB_MATCH α per REG-0). The legacy cursor-cell and the
         * &Σ / &Σlen movabs bakes are GONE. NOTANY(S) matches exactly ONE subject character that does NOT
         * appear in S (SPITBOL Manual ch.18: "NOTANY(S) matches a subject character only if it does not
         * appear in S"); on success δ += 1, on β it undoes (δ -= 1) and fails. Byte-identical to the ANY
         * REG-2 arm EXCEPT offset 50: ANY emits je ω (0F 84 — fail when char NOT in set); NOTANY emits jne ω
         * (0F 85 — fail when char IS in set). r10 is caller-saved → push/pop around call; r13/r14/r15 are
         * callee-saved and survive. LITERAL byte map, hand-coded offsets (FACT RULE TWO LITERAL FORMS — no
         * b.size()); 74 bytes; sites {8,52,61,65,70}:
         *   0  : 44 89 F0                 mov eax, r14d            ; eax = δ
         *   3  : 44 39 F8                 cmp eax, r15d
         *   6  : 0F 8D + ω_rel32          jge ω                    ; rel32 @8
         *   12 : 49 63 CE                 movsxd rcx, r14d
         *   15 : 41 0F B6 74 0D 00        movzx esi, byte [r13+rcx]
         *   21 : 48 BF + cs(8)            movabs rdi, &cset
         *   31 : 41 52                    push r10
         *   33 : 48 B8 + strchr(8)        movabs rax, &strchr
         *   43 : FF D0                    call rax
         *   45 : 41 5A                    pop r10
         *   47 : 48 85 C0                 test rax, rax
         *   50 : 0F 85 + ω_rel32          jne ω                    ; rel32 @52  (NOTANY: char IS in set ⇒ fail)
         *   56 : 41 83 C6 01              add r14d, 1
         *   60 : E9 + γ_rel32             jmp γ                    ; rel32 @61
         *   65 : 41 83 EE 01              β: sub r14d, 1           ; [β-def @65]
         *   69 : E9 + ω_rel32             jmp ω                    ; rel32 @70
         *   74 : end
         */
        uint64_t strchr_addr;
        { const char *(*fp)(const char *, int) = strchr; strchr_addr = (uint64_t)(uintptr_t)(void *)fp; }
        uint64_t cs_addr = (uint64_t)(uintptr_t)(const void *)cs;
        bin = { {8, 52, 61, 65, 70},
                {_.lbl_ω_p, _.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                {false, false, false, true, false} };
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NOTANY"))
             + IF(MEDIUM_BINARY,
                   bytes(3, "\x44\x89\xF0")
                 + bytes(3, "\x44\x39\xF8")
                 + bytes(2, "\x0F\x8D") + u32le(0)
                 + bytes(3, "\x49\x63\xCE")
                 + bytes(6, "\x41\x0F\xB6\x74\x0D\x00")
                 + bytes(2, "\x48\xBF") + u64le(cs_addr)
                 + bytes(2, "\x41\x52")
                 + bytes(2, "\x48\xB8") + u64le(strchr_addr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(2, "\x41\x5A")
                 + bytes(3, "\x48\x85\xC0")
                 + bytes(2, "\x0F\x85") + u32le(0)
                 + bytes(4, "\x41\x83\xC6\x01")
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(4, "\x41\x83\xEE\x01")
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX NOTANY()  [REG-2 Σ=r13 δ=r14 Δ=r15]")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov", "eax, r14d")
                 + s_2asm("cmp", "eax, r15d")
                 + s_2asm("jge", _.lbl_ω)
                 + s_2asm("movsxd", "rcx, r14d")
                 + s_2asm("movzx", "esi, byte ptr [r13+rcx]")
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", cs_label ? cs_label : "??"))
                 + s_2asm("push", "r10")
                 + s_2asm("call", "strchr@PLT")
                 + s_2asm("pop", "r10")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("jne", _.lbl_ω)
                 + s_2asm("add", "r14d, 1")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(emit_fmt("%s:", _.lbl_β))
                 + s_2asm("sub", "r14d, 1")
                 + s_2asm("jmp", _.lbl_ω));
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("notany")
             + s_directive(".field private final chars Ljava/lang/String;")
             + jvm_init_ms_str_str("notany", "chars")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5")
             + s_directive(".limit locals 2")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_notany/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_notany/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
             + s_1asm("if_icmpge " + emit_fmt("notany_%d_%d", 0, bb_node_id(pBB)) + "_ω")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_notany/chars Ljava/lang/String;")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_notany/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/sigma Ljava/lang/String;")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_notany/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("invokevirtual", "java/lang/String/charAt(I)C")
             + s_2asm("invokevirtual", "java/lang/String/indexOf(I)I")
             + s_1asm(std::string("ifge") + " " + emit_fmt("notany_%d_%d", 0, bb_node_id(pBB)) + "_ω")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_notany/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("istore_1")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_notany/ms Lbb/bb_box$MatchState;")
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
             + s_L1asm(emit_fmt("notany_%d_%d", 0, bb_node_id(pBB)) + "_ω:", "aconst_null")
             + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4")
             + s_directive(".limit locals 1")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_notany/ms Lbb/bb_box$MatchState;")
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
        return emit_fmt("function make_pat_%d_%d(ms) { const chars = ", pBB->ival, bb_node_id(pBB))
             + js_escape_string_str(pBB->sval)
             + "; let self = { succ: null, fail: null,\n"
               "α() { if (ms.delta >= ms.omega || chars.indexOf(ms.sigma[ms.delta]) >= 0) { self.fail.α(); return"
                   "; } const r = ms.sigma.slice(ms.delta, ms.delta + 1); ms.delta++; self.succ.α(); return r; },\n"
               "β() { ms.delta--; self.fail.α(); }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_charset_class_str(0, bb_node_id(pBB), "NOTANY")
             + net_α_hdr_str()
             + s_2asm(".maxstack", "3")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm(emit_fmt("ldfld      string pat_%d_%d::_chars", 0, bb_node_id(pBB)))
             + s_2asm("callvirt", "instance bool [boxes]Snobol4.Runtime.Boxes.MatchState::CharInSet(int32, string)")
             + s_1asm(emit_fmt("brtrue     NOTANY_%d_%d_A_FAIL", 0, bb_node_id(pBB)))
             + net_cursor_load_str() + s_1asm("ldc.i4.1") + net_spec_of_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0") + s_1asm("ret")
             + s_directive(emit_fmt("  NOTANY_%d_%d_A_FAIL:", 0, bb_node_id(pBB)))
             + net_fail_ret_str() + s_1asm("}")
             + net_β_hdr_str()
             + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldc.i4.1") + s_1asm("sub")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + net_fail_ret_str() + s_1asm("}")
             + s_1asm("}")
             + net_escape_ldstr_str(pBB->sval ? pBB->sval : "")
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(string)", 0, bb_node_id(pBB)));
    }
    if (PLATFORM_WASM) { return "          (call $bb_notany_new)\n"; }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_notany(IR_t * pBB) {
    if (MEDIUM_TEXT)   g_emit.bb_cs_id = g_flat_node_id++;
    if (MEDIUM_BINARY) g_emit.bb_cs_zeta = rt_cs_new(pBB->sval ? pBB->sval : "");
    bb_bin_t bin;
    std::string out = bb_pat_notany_str(pBB, bin);
    if (!out.empty()) bb_emit_asm_result(out, bin);
}

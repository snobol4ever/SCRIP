/* bb_pat_span.cpp — BB template for SPAN. Converted to return-string (ER wave). */
#include <string>
#include <deque>
#include <cstring>
#include "emit_str.h"
extern "C" {
void *rt_cs_new(const char *chars);
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_span_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char *cs = pBB->sval ? pBB->sval : "";
        const char *cs_label = emit_intern_str(cs);
        std::string z   = emit_fmt(".Lspan%d_z", g_emit.bb_cs_id);
        std::string lp  = emit_fmt(".Lspan%d_loop", g_emit.bb_cs_id);
        std::string dn  = emit_fmt(".Lspan%d_done", g_emit.bb_cs_id);
        bin = { {}, {}, {} };
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — SPAN"))
             + IF(MEDIUM_BINARY, [&]() -> std::string {
                   /* REG-2 (GOAL-SNOBOL4-BB REG ladder, 2026-06-01): cursor δ=R14d, subject base Σ=R13,
                      length Δ=R15d (ratified regs; established by BB_MATCH α per REG-0). The &Σ / &Σlen movabs
                      bakes are GONE, the cursor-cell reads/writes become r14d, and Σ=r13 is used DIRECTLY in
                      the loop's indexed byte load (so the old r11 base-copy and its push/pop r11 also vanish).
                      SPAN(S) matches ONE OR MORE subject chars from the set in S, longest possible (SPITBOL
                      Manual ch.18: "SPAN must match at least one subject character, and will match the longest
                      subject string possible"); on β it gives back one char at a time (z -= 1) and fails when
                      z would drop below 1. Two process-lifetime deque-int scratch slots remain (NOT a value
                      stack — per-box local match state, the REG-4/5 ζ-slot migration is a later rung):
                      z_slot = matched len @active, zo_slot = origin δ for the β undo. strchr(cs,ch) != NULL ⇒
                      char in set. r10 is caller-saved → push/pop around the call; r13/r14/r15 are callee-saved
                      and survive. LITERAL byte map, hand-coded offsets (FACT RULE TWO LITERAL FORMS — no
                      byte-counting function). Internal rel32 deltas (LITERAL): jge done = +62, je done = +18,
                      jmp loop = -86. External sites {118 ω, 143 γ, 147 β-def, 167 ω, 191 γ}. 195 bytes:
                        0   : 48 B9 + z(8)            movabs rcx, z_slot
                        10  : C7 01 00 00 00 00       mov dword[rcx], 0          ; z = 0
                        loop@16:
                        16  : 44 89 F0                mov eax, r14d              ; eax = δ
                        19  : 48 B9 + z(8)            movabs rcx, z_slot
                        29  : 03 01                   add eax, [rcx]             ; eax = δ + z
                        31  : 44 39 F8                cmp eax, r15d              ; cmp (δ+z), Δ
                        34  : 0F 8D + (+62)           jge done                   ; (δ+z) >= Δ → stop
                        40  : 48 63 C8                movsxd rcx, eax
                        43  : 41 0F B6 74 0D 00       movzx esi, byte [r13+rcx]  ; esi = Σ[δ+z]
                        49  : 48 BF + cs(8)           movabs rdi, &cset
                        59  : 41 52                   push r10
                        61  : 48 B8 + strchr(8)       movabs rax, &strchr
                        71  : FF D0                   call rax
                        73  : 41 5A                   pop r10
                        75  : 48 85 C0                test rax, rax
                        78  : 0F 84 + (+18)           je done                    ; not in set → stop
                        84  : 48 B9 + z(8)            movabs rcx, z_slot
                        94  : 83 01 01                add dword[rcx], 1          ; z += 1
                        97  : E9 + (-86)              jmp loop
                        done@102:
                        102 : 48 B9 + z(8)            movabs rcx, z_slot
                        112 : 8B 01                   mov eax, [rcx]             ; eax = z
                        114 : 85 C0                   test eax, eax
                        116 : 0F 8E + ω_rel32         jle ω                      ; site 118 (z==0 → fail)
                        122 : 44 89 F2                mov edx, r14d              ; edx = δ
                        125 : 48 B9 + zo(8)           movabs rcx, zo_slot
                        135 : 89 11                   mov [rcx], edx             ; zo = δ (origin)
                        137 : 01 C2                   add edx, eax               ; edx = δ + z
                        139 : 41 89 D6                mov r14d, edx              ; δ = δ + z
                        142 : E9 + γ_rel32            jmp γ                      ; site 143
                        β@147:
                        147 : 48 B9 + z(8)            movabs rcx, z_slot
                        157 : 8B 01                   mov eax, [rcx]             ; eax = z
                        159 : 83 E8 01                sub eax, 1                 ; z - 1
                        162 : 83 F8 01                cmp eax, 1
                        165 : 0F 8C + ω_rel32         jl ω                       ; site 167 (z-1 < 1 → fail)
                        171 : 89 01                   mov [rcx], eax             ; z = z-1
                        173 : 48 B9 + zo(8)           movabs rcx, zo_slot
                        183 : 8B 11                   mov edx, [rcx]             ; edx = zo
                        185 : 01 C2                   add edx, eax               ; edx = zo + (z-1)
                        187 : 41 89 D6                mov r14d, edx              ; δ = zo + (z-1)
                        190 : E9 + γ_rel32            jmp γ                      ; site 191
                        195 : end                                                                            */
                   static std::deque<int> _pool;
                   _pool.emplace_back(0); int *z_slot  = &_pool.back();
                   _pool.emplace_back(0); int *zo_slot = &_pool.back();
                   uint64_t za  = (uint64_t)(uintptr_t)z_slot;
                   uint64_t zoa = (uint64_t)(uintptr_t)zo_slot;
                   uint64_t csa = (uint64_t)(uintptr_t)(const void *)(pBB->sval ? pBB->sval : "");
                   uint64_t strchr_addr;
                   { const char *(*fp)(const char *, int) = strchr; strchr_addr = (uint64_t)(uintptr_t)(void *)fp; }
                   std::string b;
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [0]   movabs rcx, z_slot */
                   b += bytes(6,"\xC7\x01\x00\x00\x00\x00");              /* [10]  mov dword[rcx], 0 */
                   b += bytes(3,"\x44\x89\xF0");                          /* [16]  mov eax, r14d */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [19]  movabs rcx, z_slot */
                   b += bytes(2,"\x03\x01");                              /* [29]  add eax, [rcx] */
                   b += bytes(3,"\x44\x39\xF8");                          /* [31]  cmp eax, r15d */
                   b += bytes(2,"\x0F\x8D") + u32le(62u);                 /* [34]  jge done (+62→102) */
                   b += bytes(3,"\x48\x63\xC8");                          /* [40]  movsxd rcx, eax */
                   b += bytes(6,"\x41\x0F\xB6\x74\x0D\x00");              /* [43]  movzx esi, byte[r13+rcx] */
                   b += bytes(2,"\x48\xBF") + u64le(csa);                 /* [49]  movabs rdi, cs_addr */
                   b += bytes(2,"\x41\x52");                              /* [59]  push r10 */
                   b += bytes(2,"\x48\xB8") + u64le(strchr_addr);         /* [61]  movabs rax, strchr */
                   b += bytes(2,"\xFF\xD0");                              /* [71]  call rax */
                   b += bytes(2,"\x41\x5A");                              /* [73]  pop r10 */
                   b += bytes(3,"\x48\x85\xC0");                          /* [75]  test rax, rax */
                   b += bytes(2,"\x0F\x84") + u32le(18u);                 /* [78]  je done (+18→102) */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [84]  movabs rcx, z_slot */
                   b += bytes(3,"\x83\x01\x01");                          /* [94]  add dword[rcx], 1 */
                   b += bytes(1,"\xE9") + u32le((uint32_t)(int32_t)-86);  /* [97]  jmp loop (-86→16) */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [102] movabs rcx, z_slot */
                   b += bytes(2,"\x8B\x01");                              /* [112] mov eax, [rcx] */
                   b += bytes(2,"\x85\xC0");                              /* [114] test eax, eax */
                   b += bytes(2,"\x0F\x8E") + u32le(0);                   /* [116] jle ω  site[0]@118 */
                   b += bytes(3,"\x44\x89\xF2");                          /* [122] mov edx, r14d */
                   b += bytes(2,"\x48\xB9") + u64le(zoa);                 /* [125] movabs rcx, zo_slot */
                   b += bytes(2,"\x89\x11");                              /* [135] mov [rcx], edx */
                   b += bytes(2,"\x01\xC2");                              /* [137] add edx, eax */
                   b += bytes(3,"\x41\x89\xD6");                          /* [139] mov r14d, edx */
                   b += bytes(1,"\xE9") + u32le(0);                       /* [142] jmp γ  site[1]@143 */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [147] movabs rcx, z_slot */
                   b += bytes(2,"\x8B\x01");                              /* [157] mov eax, [rcx] */
                   b += bytes(3,"\x83\xE8\x01");                          /* [159] sub eax, 1 */
                   b += bytes(3,"\x83\xF8\x01");                          /* [162] cmp eax, 1 */
                   b += bytes(2,"\x0F\x8C") + u32le(0);                   /* [165] jl ω  site[2]@167 */
                   b += bytes(2,"\x89\x01");                              /* [171] mov [rcx], eax */
                   b += bytes(2,"\x48\xB9") + u64le(zoa);                 /* [173] movabs rcx, zo_slot */
                   b += bytes(2,"\x8B\x11");                              /* [183] mov edx, [rcx] */
                   b += bytes(2,"\x01\xC2");                              /* [185] add edx, eax */
                   b += bytes(3,"\x41\x89\xD6");                          /* [187] mov r14d, edx */
                   b += bytes(1,"\xE9") + u32le(0);                       /* [190] jmp γ  site[3]@191 */
                   bin = { {118, 143, 147, 167, 191},
                           {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                           {false, false, true, false, false} };
                   return b;
               }())
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX SPAN()  [REG-2 Σ=r13 δ=r14 Δ=r15]")
                 + s_directive(".section .data")
                 + s_directive(z + ": .long 0")
                 + s_directive(".long 0")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov", "dword ptr [rip + " + z + " + 0], 0")
                 + s_1asm(emit_fmt("%s:", lp.c_str()))
                 + s_2asm("mov", "eax, r14d")
                 + s_2asm("add", "eax, dword ptr [rip + " + z + " + 0]")
                 + s_2asm("cmp", "eax, r15d")
                 + s_2asm("jge", dn.c_str())
                 + s_2asm("movsxd", "rcx, eax")
                 + s_2asm("movzx", "esi, byte ptr [r13+rcx]")
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", cs_label ? cs_label : "??"))
                 + s_2asm("push", "r10")
                 + s_2asm("call", "strchr@PLT")
                 + s_2asm("pop", "r10")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("jz", dn.c_str())
                 + s_2asm("add", "dword ptr [rip + " + z + " + 0], 1")
                 + s_2asm("jmp", lp.c_str())
                 + s_1asm(emit_fmt("%s:", dn.c_str()))
                 + s_2asm("mov", "ecx, dword ptr [rip + " + z + " + 0]")
                 + s_2asm("cmp", "ecx, 0")
                 + s_2asm("jle", _.lbl_ω)
                 + s_2asm("mov", "eax, r14d")
                 + s_2asm("mov", "dword ptr [rip + " + z + " + 4], eax")
                 + s_2asm("add", "eax, ecx")
                 + s_2asm("mov", "r14d, eax")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(emit_fmt("%s:", _.lbl_β))
                 + s_2asm("mov", "eax, dword ptr [rip + " + z + " + 4]")
                 + s_2asm("mov", "ecx, dword ptr [rip + " + z + " + 0]")
                 + s_2asm("sub", "ecx, 1")
                 + s_2asm("cmp", "ecx, 1")
                 + s_2asm("jl", _.lbl_ω)
                 + s_2asm("mov", "dword ptr [rip + " + z + " + 0], ecx")
                 + s_2asm("add", "eax, ecx")
                 + s_2asm("mov", "r14d, eax")
                 + s_2asm("jmp", _.lbl_γ));
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("span")
             + s_directive(".field private final chars Ljava/lang/String;")
             + s_directive(".field private matched_len I")
             + jvm_init_ms_str_str("span", "chars")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6")
             + s_directive(".limit locals 3")
             + s_1asm("aload_0") + s_1asm("iconst_0")
             + s_2asm("putfield", "bb/bb_span/matched_len I")
             + s_L1asm(emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_loop:", "aload_0")
             + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iadd") + s_1asm("istore_1") + s_1asm("iload_1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
             + s_1asm("if_icmpge " + emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_done")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/chars Ljava/lang/String;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/sigma Ljava/lang/String;")
             + s_1asm("iload_1")
             + s_2asm("invokevirtual", "java/lang/String/charAt(I)C")
             + s_2asm("invokevirtual", "java/lang/String/indexOf(I)I")
             + s_1asm("iflt " + emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_done")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_span/matched_len I")
             + s_1asm("goto " + emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_loop")
             + s_L1asm(emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_done:", "aload_0")
             + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("ifle " + emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("istore_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("iload_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("isub") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const chars = ", pBB->ival, bb_node_id(pBB))
             + js_escape_string_str(pBB->sval)
             + "; let delta = 0; let self = { succ: null, fail: null,\n"
               "α() { delta = 0; while (ms.delta + delta < ms.omega && chars.indexOf(ms.sigma[ms.delta + delta]) >= 0) delta++; if (delta <= 0) { self.fail.α(); return"
                   "; } const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n"
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
             + s_1asm(emit_fmt("brtrue     SP_%d_%d_NN", 0, bb_node_id(pBB)))
             + s_1asm("pop") + s_2asm("ldstr", "\"\"")
             + s_directive(emit_fmt("  SP_%d_%d_NN:", 0, bb_node_id(pBB)))
             + s_1asm(emit_fmt("stfld      string pat_%d_%d::_chars", 0, bb_node_id(pBB)))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "3")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_directive(emit_fmt("  SP_%d_%d_LOOP:", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add")
             + s_1asm(emit_fmt("ldfld      string pat_%d_%d::_chars", 0, bb_node_id(pBB)))
             + s_2asm("callvirt", "instance bool [boxes]Snobol4.Runtime.Boxes.MatchState::CharInSet(int32, string)")
             + s_1asm(emit_fmt("brfalse    SP_%d_%d_DONE", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm(emit_fmt("br         SP_%d_%d_LOOP", 0, bb_node_id(pBB)))
             + s_directive(emit_fmt("  SP_%d_%d_DONE:", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("ble        SP_%d_%d_FAIL", 0, bb_node_id(pBB)))
             + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + net_spec_of_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0") + s_1asm("ret")
             + s_directive(emit_fmt("  SP_%d_%d_FAIL:", 0, bb_node_id(pBB)))
             + net_fail_ret_str() + s_1asm("}")
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
    if (PLATFORM_WASM) { return "          (call $bb_span_new)\n"; }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_span(IR_t * pBB) {
    if (MEDIUM_TEXT)   g_emit.bb_cs_id = g_flat_node_id++;
    if (MEDIUM_BINARY) g_emit.bb_cs_zeta = rt_cs_new(pBB->sval ? pBB->sval : "");
    bb_bin_t bin;
    std::string out = bb_pat_span_str(pBB, bin);
    if (!out.empty()) bb_emit_asm_result(out, bin);
}

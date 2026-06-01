/* bb_pat_break.cpp — BB template for BREAK/BREAKX.  REG-2 (2026-06-01): Σ=R13/δ=R14d/Δ=R15d. */
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
        /* REG-2 (GOAL-SNOBOL4-BB REG ladder, 2026-06-01): Σ=R13/δ=R14d/Δ=R15d (ratified registers,
         * established by BB_MATCH α per REG-0). Legacy subject-model (movabs &Σ, [r10], movabs &Σlen)
         * is GONE; r11 scratch + push/pop r11 removed. r10 push/pop remains (caller-saved around strchr).
         * z counter lives in [zeta+8] (BINARY) / .data slot (TEXT); persists across α→γ→β boundary.
         *
         * PLAIN BREAK (153B, sites {125,129,149}) — assembled + objdump-verified:
         *   off  bytes                       asm
         *    0   48 B9 [zeta u64]            movabs rcx, &zeta         ; [0]  10B
         *   10   C7 41 08 00 00 00 00        mov dword [rcx+8], 0      ; [10]  7B  z=0
         *  loop(17):
         *   17   44 89 F0                    mov eax, r14d             ; [17]  3B  eax=δ
         *   20   48 B9 [zeta u64]            movabs rcx, &zeta         ; [20] 10B
         *   30   03 41 08                    add eax, [rcx+8]          ; [30]  3B  eax=δ+z
         *   33   44 39 F8                    cmp eax, r15d             ; [33]  3B  cmp (δ+z),Δ
         *   36   0F 8D 3F 00 00 00           jge done (+63 → 105)      ; [36]  6B  internal
         *   42   48 63 C8                    movsxd rcx, eax           ; [42]  3B
         *   45   41 0F B6 74 0D 00           movzx esi, [r13+rcx+0]    ; [45]  6B  Σ[δ+z]
         *   51   48 BF [cs u64]              movabs rdi, &cset         ; [51] 10B
         *   61   41 52                       push r10                  ; [61]  2B
         *   63   48 B8 [strchr u64]          movabs rax, &strchr       ; [63] 10B
         *   73   FF D0                       call rax                  ; [73]  2B
         *   75   41 5A                       pop r10                   ; [75]  2B
         *   77   48 85 C0                    test rax, rax             ; [77]  3B
         *   80   0F 85 13 00 00 00           jnz done (+19 → 105)      ; [80]  6B  internal
         *   86   48 B9 [zeta u64]            movabs rcx, &zeta         ; [86] 10B
         *   96   83 41 08 01                 add dword [rcx+8], 1      ; [96]  4B  z++
         *  100   E9 A8 FF FF FF              jmp loop (-88 → 17)       ; [100]  5B  internal
         *  done(105):
         *  105   48 B9 [zeta u64]            movabs rcx, &zeta         ; [105] 10B
         *  115   44 89 F0                    mov eax, r14d             ; [115]  3B
         *  118   03 41 08                    add eax, [rcx+8]          ; [118]  3B  eax=δ+z
         *  121   41 89 C6                    mov r14d, eax             ; [121]  3B  δ+=z
         *  124   E9 [rel32]                  jmp γ                     ; [124]  5B  SITE@125
         *  129   (lbl_β defined here)
         *  129   48 B9 [zeta u64]            movabs rcx, &zeta         ; [129] 10B
         *  139   44 89 F0                    mov eax, r14d             ; [139]  3B
         *  142   2B 41 08                    sub eax, [rcx+8]          ; [142]  3B  eax=δ-z (undo)
         *  145   41 89 C6                    mov r14d, eax             ; [145]  3B  δ-=z
         *  148   E9 [rel32]                  jmp ω                     ; [148]  5B  SITE@149
         *  153   (end)
         *
         * BREAKX (290B, sites {125,130,134,265,286}) — assembled + objdump-verified:
         * α scans to first cset char (jge ω_α if none found; jmp γ_α on found).
         * β: z_orig = r14d - old_z saved to [zeta+12] (padding in 16B rt_cs_t); z++; rescan
         *    from z_orig for NEXT cset char. r13/r14d/r15d identical semantics to α loop.
         * Internal jumps (identical across both loops): jge +87, jnz +19, jmp loop -88.
         *   α loop: off 36 jge +87→omega_a(129), off 80 jnz +19→done_a(105), off 100 jmp -88→loop1(17)
         *   β loop: off 192 jge +87→omega_b(285), off 236 jnz +19→done_b(247), off 256 jmp -88→loop2(173)
         *   α done(105):  δ+=z → jmp γ_α  SITE@125; omega_a(129): jmp ω_α SITE@130; β_def(134)
         *   β done(247): δ=z_orig+z → jmp γ_β SITE@265; omega_b(285): δ=z_orig → jmp ω_β SITE@286
         */
        uint64_t strchr_addr;
        { const char *(*fp)(const char *, int) = strchr; strchr_addr = (uint64_t)(uintptr_t)(void *)fp; }
        uint64_t cs_addr   = (uint64_t)(uintptr_t)(const void *)cs;
        uint64_t zeta_addr = (uint64_t)(uintptr_t)g_emit.bb_cs_zeta;
        if (!is_breakx) {
            bin = { {125, 129, 149},
                    {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                    {false, true, false} };
        } else {
            bin = { {125, 130, 134, 265, 286},
                    {_.lbl_γ_p, _.lbl_ω_p, _.lbl_β_p, _.lbl_γ_p, _.lbl_ω_p},
                    {false, false, true, false, false} };
        }
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — BREAK"))
             + IF(MEDIUM_BINARY,
                   is_breakx
                 ? (  bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [0]   movabs rcx,&zeta */
                    + bytes(7, "\xC7\x41\x08\x00\x00\x00\x00")                      /* [10]  mov [rcx+8],0 */
                    + bytes(3, "\x44\x89\xF0")                                       /* [17]  mov eax,r14d */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [20]  movabs rcx,&zeta */
                    + bytes(3, "\x03\x41\x08")                                       /* [30]  add eax,[rcx+8] */
                    + bytes(3, "\x44\x39\xF8")                                       /* [33]  cmp eax,r15d */
                    + bytes(2, "\x0F\x8D") + u32le(87)                              /* [36]  jge +87->omega_a */
                    + bytes(3, "\x48\x63\xC8")                                       /* [42]  movsxd rcx,eax */
                    + bytes(6, "\x41\x0F\xB6\x74\x0D\x00")                          /* [45]  movzx esi,[r13+rcx+0] */
                    + bytes(2, "\x48\xBF") + u64le(cs_addr)                         /* [51]  movabs rdi,&cset */
                    + bytes(2, "\x41\x52")                                           /* [61]  push r10 */
                    + bytes(2, "\x48\xB8") + u64le(strchr_addr)                     /* [63]  movabs rax,&strchr */
                    + bytes(2, "\xFF\xD0")                                           /* [73]  call rax */
                    + bytes(2, "\x41\x5A")                                           /* [75]  pop r10 */
                    + bytes(3, "\x48\x85\xC0")                                       /* [77]  test rax,rax */
                    + bytes(2, "\x0F\x85") + u32le(19)                              /* [80]  jnz +19->done_a */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [86]  movabs rcx,&zeta */
                    + bytes(4, "\x83\x41\x08\x01")                                  /* [96]  add [rcx+8],1 */
                    + bytes(1, "\xE9") + u32le((uint32_t)(int32_t)-88)              /* [100] jmp -88->loop1 */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [105] movabs rcx,&zeta  done_a */
                    + bytes(3, "\x44\x89\xF0")                                       /* [115] mov eax,r14d */
                    + bytes(3, "\x03\x41\x08")                                       /* [118] add eax,[rcx+8] */
                    + bytes(3, "\x41\x89\xC6")                                       /* [121] mov r14d,eax */
                    + bytes(1, "\xE9") + u32le(0)                                   /* [124] jmp γ_α  SITE@125 */
                    + bytes(1, "\xE9") + u32le(0)                                   /* [129] jmp ω_α  SITE@130 (omega_a) */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [134] movabs rcx,&zeta  beta_def */
                    + bytes(3, "\x44\x89\xF0")                                       /* [144] mov eax,r14d */
                    + bytes(3, "\x2B\x41\x08")                                       /* [147] sub eax,[rcx+8] */
                    + bytes(3, "\x89\x41\x0C")                                       /* [150] mov [rcx+12],eax */
                    + bytes(4, "\x83\x41\x08\x01")                                  /* [153] add [rcx+8],1 */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [157] movabs rcx,&zeta  loop2 */
                    + bytes(3, "\x8B\x41\x0C")                                       /* [167] mov eax,[rcx+12] */
                    + bytes(3, "\x03\x41\x08")                                       /* [171] add eax,[rcx+8] */
                    + bytes(3, "\x44\x39\xF8")                                       /* [174] cmp eax,r15d */
                    + bytes(2, "\x0F\x8D") + u32le(87)                              /* [177] jge +87->omega_b */
                    + bytes(3, "\x48\x63\xC8")                                       /* [183] movsxd rcx,eax */
                    + bytes(6, "\x41\x0F\xB6\x74\x0D\x00")                          /* [186] movzx esi,[r13+rcx+0] */
                    + bytes(2, "\x48\xBF") + u64le(cs_addr)                         /* [192] movabs rdi,&cset */
                    + bytes(2, "\x41\x52")                                           /* [202] push r10 */
                    + bytes(2, "\x48\xB8") + u64le(strchr_addr)                     /* [204] movabs rax,&strchr */
                    + bytes(2, "\xFF\xD0")                                           /* [214] call rax */
                    + bytes(2, "\x41\x5A")                                           /* [216] pop r10 */
                    + bytes(3, "\x48\x85\xC0")                                       /* [218] test rax,rax */
                    + bytes(2, "\x0F\x85") + u32le(19)                              /* [221] jnz +19->done_b */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [227] movabs rcx,&zeta */
                    + bytes(4, "\x83\x41\x08\x01")                                  /* [237] add [rcx+8],1 */
                    + bytes(1, "\xE9") + u32le((uint32_t)(int32_t)-88)              /* [241] jmp -88->loop2 */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [246] movabs rcx,&zeta  done_b */
                    + bytes(3, "\x8B\x41\x0C")                                       /* [256] mov eax,[rcx+12] */
                    + bytes(3, "\x03\x41\x08")                                       /* [259] add eax,[rcx+8] */
                    + bytes(3, "\x41\x89\xC6")                                       /* [262] mov r14d,eax */
                    + bytes(1, "\xE9") + u32le(0)                                   /* [265] jmp γ_β  SITE@266? */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [270] movabs rcx,&zeta  omega_b */
                    + bytes(3, "\x8B\x41\x0C")                                       /* [280] mov eax,[rcx+12] */
                    + bytes(3, "\x41\x89\xC6")                                       /* [283] mov r14d,eax */
                    + bytes(1, "\xE9") + u32le(0))                                  /* [286] jmp ω_β  SITE@287? */
                 : (  bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [0]   movabs rcx,&zeta */
                    + bytes(7, "\xC7\x41\x08\x00\x00\x00\x00")                      /* [10]  mov [rcx+8],0 */
                    + bytes(3, "\x44\x89\xF0")                                       /* [17]  mov eax,r14d */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [20]  movabs rcx,&zeta */
                    + bytes(3, "\x03\x41\x08")                                       /* [30]  add eax,[rcx+8] */
                    + bytes(3, "\x44\x39\xF8")                                       /* [33]  cmp eax,r15d */
                    + bytes(2, "\x0F\x8D") + u32le(63)                              /* [36]  jge +63->done */
                    + bytes(3, "\x48\x63\xC8")                                       /* [42]  movsxd rcx,eax */
                    + bytes(6, "\x41\x0F\xB6\x74\x0D\x00")                          /* [45]  movzx esi,[r13+rcx+0] */
                    + bytes(2, "\x48\xBF") + u64le(cs_addr)                         /* [51]  movabs rdi,&cset */
                    + bytes(2, "\x41\x52")                                           /* [61]  push r10 */
                    + bytes(2, "\x48\xB8") + u64le(strchr_addr)                     /* [63]  movabs rax,&strchr */
                    + bytes(2, "\xFF\xD0")                                           /* [73]  call rax */
                    + bytes(2, "\x41\x5A")                                           /* [75]  pop r10 */
                    + bytes(3, "\x48\x85\xC0")                                       /* [77]  test rax,rax */
                    + bytes(2, "\x0F\x85") + u32le(19)                              /* [80]  jnz +19->done */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [86]  movabs rcx,&zeta */
                    + bytes(4, "\x83\x41\x08\x01")                                  /* [96]  add [rcx+8],1 */
                    + bytes(1, "\xE9") + u32le((uint32_t)(int32_t)-88)              /* [100] jmp -88->loop */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [105] movabs rcx,&zeta  done */
                    + bytes(3, "\x44\x89\xF0")                                       /* [115] mov eax,r14d */
                    + bytes(3, "\x03\x41\x08")                                       /* [118] add eax,[rcx+8] */
                    + bytes(3, "\x41\x89\xC6")                                       /* [121] mov r14d,eax */
                    + bytes(1, "\xE9") + u32le(0)                                   /* [124] jmp γ  SITE@125 */
                    + bytes(2, "\x48\xB9") + u64le(zeta_addr)                       /* [129] movabs rcx,&zeta  β_def */
                    + bytes(3, "\x44\x89\xF0")                                       /* [139] mov eax,r14d */
                    + bytes(3, "\x2B\x41\x08")                                       /* [142] sub eax,[rcx+8] */
                    + bytes(3, "\x41\x89\xC6")                                       /* [145] mov r14d,eax */
                    + bytes(1, "\xE9") + u32le(0)))                                 /* [148] jmp ω  SITE@149 */
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(is_breakx ? "# BOX BREAKX()  [REG-2 Σ=r13 δ=r14 Δ=r15]" : "# BOX BREAK()  [REG-2 Σ=r13 δ=r14 Δ=r15]")
                 + s_directive(".section .data")
                 + s_directive(z + ": .long 0")
                 + (is_breakx ? s_directive(z + "_orig: .long 0") : std::string())
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov", "dword ptr [rip + " + z + "], 0")
                 + s_1asm(emit_fmt("%s:", lp.c_str()))
                 + s_2asm("mov", "eax, r14d")
                 + s_2asm("add", "eax, dword ptr [rip + " + z + "]")
                 + s_2asm("cmp", "eax, r15d")
                 + s_2asm("jge", (is_breakx ? _.lbl_ω : _.lbl_ω))
                 + s_2asm("movsxd", "rcx, eax")
                 + s_2asm("movzx", "esi, byte ptr [r13+rcx]")
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", cs_label ? cs_label : "??"))
                 + s_2asm("push", "r10")
                 + s_2asm("call", "strchr@PLT")
                 + s_2asm("pop", "r10")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("jnz", dn.c_str())
                 + s_2asm("add", "dword ptr [rip + " + z + "], 1")
                 + s_2asm("jmp", lp.c_str())
                 + s_1asm(emit_fmt("%s:", dn.c_str()))
                 + s_2asm("mov", "eax, r14d")
                 + s_2asm("add", "eax, dword ptr [rip + " + z + "]")
                 + s_2asm("mov", "r14d, eax")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(emit_fmt("%s:", _.lbl_β))
                 + (is_breakx
                    ? (  s_comment("# BREAKX β: step past break char, rescan to next")
                       + s_2asm("mov", "eax, r14d")
                       + s_2asm("sub", "eax, dword ptr [rip + " + z + "]")
                       + s_2asm("mov", "dword ptr [rip + " + z + "_orig], eax")
                       + s_2asm("add", "dword ptr [rip + " + z + "], 1")
                       + s_1asm(emit_fmt("%s:", lp2.c_str()))
                       + s_2asm("mov", "eax, dword ptr [rip + " + z + "_orig]")
                       + s_2asm("add", "eax, dword ptr [rip + " + z + "]")
                       + s_2asm("cmp", "eax, r15d")
                       + s_2asm("jge", _.lbl_ω)
                       + s_2asm("movsxd", "rcx, eax")
                       + s_2asm("movzx", "esi, byte ptr [r13+rcx]")
                       + s_2asm("lea", emit_fmt("rdi, [rip + %s]", cs_label ? cs_label : "??"))
                       + s_2asm("push", "r10")
                       + s_2asm("call", "strchr@PLT")
                       + s_2asm("pop", "r10")
                       + s_2asm("test", "rax, rax")
                       + s_2asm("jnz", dn2.c_str())
                       + s_2asm("add", "dword ptr [rip + " + z + "], 1")
                       + s_2asm("jmp", lp2.c_str())
                       + s_1asm(emit_fmt("%s:", dn2.c_str()))
                       + s_2asm("mov", "eax, dword ptr [rip + " + z + "_orig]")
                       + s_2asm("add", "eax, dword ptr [rip + " + z + "]")
                       + s_2asm("mov", "r14d, eax")
                       + s_2asm("jmp", _.lbl_γ))
                    : (  s_2asm("mov", "eax, r14d")
                       + s_2asm("sub", "eax, dword ptr [rip + " + z + "]")
                       + s_2asm("mov", "r14d, eax")
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

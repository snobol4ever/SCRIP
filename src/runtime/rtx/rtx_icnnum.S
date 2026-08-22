/* rtx_icnnum.S -- RTX-6-ICN: rt_coerce_num2_d, with its static callee rt_parse_num_d ABSORBED.
 *
 * CONTRACT: .github/ARCH-ICON-RTX.md. Ladder: .github/GOAL-ICON-RTX.md. Ledger: .github/RTX-CLAIMS.md.
 * Symbol checked out OUT:ICON-RTX:s211. Gate: SCRIP_RTX_ICNNUM (eighth family gate -- a ledger event).
 *
 * WHY THIS ARM, AND NOT THE ONE THE C READS FIRST (step 0(g), extended s211):
 * bb_coerce_numeric.cpp:18-31 ALREADY INLINES the DT_I+DT_I and DT_R cases and reaches gamma without
 * calling anything. The emitted `call rt_coerce_num2_d@PLT` at line 37 sits on the L(0) arm ONLY.
 * Measured s211 on two authored workloads: pure-integer arithmetic reaches this symbol ZERO times;
 * string->numeric reaches it 60,000 times with 120,000 parse entries, live arms STR_INT and DT_I,
 * and STR_REAL / SNUL / FAIL all zero. So the callee's textually-first fast arms are DEAD from Icon
 * and the ONLY live string arm is the one that calls libc strtoll. This port replaces that strtoll
 * with an inline decimal scan; it does not merely shave -O0 frame ceremony.
 *
 * SOUNDNESS -- THE ONE PROPERTY THE WHOLE FILE RESTS ON:
 * the asm decides "this is definitely a simple decimal integer" or "let C do it". It NEVER decides a
 * hard case. Every bail is a bare `jmp c_rt_coerce_num2_d` with rdi/rsi/rdx/rcx UNTOUCHED, so the C
 * body reruns from the original arguments. Nothing is stored through rdx before the last two
 * instructions, so a bail can never leave a half-written result. Consequences, deliberately:
 *   - tabs/newlines around the digits    -> bail (C's loop skips only ' ', strtoll skips isspace)
 *   - >18 digits                          -> bail (C inherits strtoll's saturation; we do not model it)
 *   - no digits, "inf", "nan", "0x1p3"    -> bail (C's strtod arm may still accept these)
 *   - trailing junk after the digits      -> bail (C fails it via the ep/q trailing-space test)
 *   - `other` is a string                 -> handled: same scan, realness answer only
 *   - self is any tag but I/R/SNUL/S      -> bail (C's error path + core_runtime_error stays in C)
 * SCRATCH IS xmm, NOT THE STACK, precisely so that "bail is a bare jmp valid at any point" holds.
 *
 * Registers: rdi self, rsi other, rdx out, rcx codes -- all four preserved for the bail path.
 * Working set per ARCH section 2: rax r8 r9 r10 r11 + xmm0-2. Blob pins rbx r12-r15 ___ untouched.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(icnnum)

/* Scan SRC's string field for an optionally-signed decimal integer, whole-string, <=18 digits.
 * On success ACC holds the value and control falls through; otherwise jumps to .Lbail.
 * Scratch registers are caller-chosen; each is passed in every width it is used at, because
 * token-pasting a 64-bit name does NOT yield its 32/8-bit alias (rax##d is not eax). */
#define SCAN_SIMPLE_INT(SRC, ACC, ACC32, PTR, CNT32, SGN8, SGN32, DIG, DIG32, SFX)                                            \
    mov PTR, qword ptr [SRC + 8];                                                                     \
    test PTR, PTR;                                                                                    \
    jz .Lbail;                                                                                        \
    xor ACC32, ACC32;                                                                               \
    xor CNT32, CNT32;                                                                               \
    xor SGN32, SGN32;                                                                               \
.Lsp##SFX:                                                                                            \
    cmp byte ptr [PTR], 32;                                                                           \
    jne .Lspe##SFX;                                                                                   \
    inc PTR;                                                                                          \
    jmp .Lsp##SFX;                                                                                    \
.Lspe##SFX:                                                                                           \
    cmp byte ptr [PTR], 0;                                                                            \
    je .Lok##SFX;                                                                                     \
    cmp byte ptr [PTR], 45;                                                                           \
    jne .Lplus##SFX;                                                                                  \
    mov SGN8, 1;                                                                                    \
    inc PTR;                                                                                          \
    jmp .Ldig##SFX;                                                                                   \
.Lplus##SFX:                                                                                          \
    cmp byte ptr [PTR], 43;                                                                           \
    jne .Ldig##SFX;                                                                                   \
    inc PTR;                                                                                          \
.Ldig##SFX:                                                                                           \
    movzx DIG32, byte ptr [PTR];                                                                     \
    sub DIG32, 48;                                                                                   \
    cmp DIG32, 9;                                                                                    \
    ja .Ldige##SFX;                                                                                   \
    lea ACC, [ACC + ACC*4];                                                                           \
    add ACC, ACC;                                                                                     \
    add ACC, DIG;                                                                                     \
    inc PTR;                                                                                          \
    inc CNT32;                                                                                       \
    cmp CNT32, 18;                                                                                   \
    ja .Lbail;                                                                                        \
    jmp .Ldig##SFX;                                                                                   \
.Ldige##SFX:                                                                                          \
    test CNT32, CNT32;                                                                              \
    jz .Lbail;                                                                                        \
.Lts##SFX:                                                                                            \
    cmp byte ptr [PTR], 32;                                                                           \
    jne .Ltse##SFX;                                                                                   \
    inc PTR;                                                                                          \
    jmp .Lts##SFX;                                                                                    \
.Ltse##SFX:                                                                                           \
    cmp byte ptr [PTR], 0;                                                                            \
    jne .Lbail;                                                                                       \
    test SGN8, SGN8;                                                                              \
    jz .Lok##SFX;                                                                                     \
    neg ACC;                                                                                          \
.Lok##SFX:

RTX_FUNC(rt_coerce_num2_d)
    RTX_GATE(icnnum, c_rt_coerce_num2_d)
    mov eax, dword ptr [rdi]
    cmp al, DT_I
    je .Lself_i
    cmp al, DT_R
    je .Lself_r
    cmp al, DT_SNUL
    je .Lself_snul
    cmp al, DT_S
    jne .Lbail
    SCAN_SIMPLE_INT(rdi, rax, eax, r11, r9d, r10b, r10d, r8, r8d, S)
    mov r8, rax
    xor r9d, r9d
    jmp .Lself_done
.Lself_i:
    mov r8, qword ptr [rdi + 8]
    xor r9d, r9d
    jmp .Lself_done
.Lself_snul:
    xor r8d, r8d
    xor r9d, r9d
    jmp .Lself_done
.Lself_r:
    movq xmm0, qword ptr [rdi + 8]
    mov r9d, 1
.Lself_done:
    movq xmm1, r8
    movq xmm2, r9
    mov eax, dword ptr [rsi]
    cmp al, DT_R
    je .Lother_real
    cmp al, DT_S
    jne .Lother_int
    SCAN_SIMPLE_INT(rsi, rax, eax, r11, r9d, r10b, r10d, r8, r8d, O)
.Lother_int:
    movq r8, xmm1
    movq r9, xmm2
    xor r10d, r10d
    jmp .Lcombine
.Lother_real:
    movq r8, xmm1
    movq r9, xmm2
    mov r10d, 1
.Lcombine:
    test r9b, r9b
    jnz .Lstore_real
    test r10b, r10b
    jnz .Lint_to_real
    mov dword ptr [rdx], DT_I | (MOD_OP_RT_COERCE_NUM2_D << 8)   /* out-param mint, stamped (row descr-stamp-asm-mints) */
    mov dword ptr [rdx + 4], 0
    mov qword ptr [rdx + 8], r8
    ret
.Lint_to_real:
    cvtsi2sd xmm0, r8
.Lstore_real:
    mov dword ptr [rdx], DT_R | (MOD_OP_RT_COERCE_NUM2_D << 8)   /* out-param mint, stamped (row descr-stamp-asm-mints) */
    mov dword ptr [rdx + 4], 0
    movq qword ptr [rdx + 8], xmm0
    ret
.Lbail:
    jmp c_rt_coerce_num2_d
RTX_ENDF(rt_coerce_num2_d)

.section .note.GNU-stack,"",@progbits

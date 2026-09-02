#include "rtx_abi.inc"
RTX_GATE_DEF(icnnum)
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
    mov dword ptr [rdx], DT_I | (MOD_OP_RT_COERCE_NUM2_D << 8)
    mov dword ptr [rdx + 4], 0
    mov qword ptr [rdx + 8], r8
    ret
.Lint_to_real:
    cvtsi2sd xmm0, r8
.Lstore_real:
    mov dword ptr [rdx], DT_R | (MOD_OP_RT_COERCE_NUM2_D << 8)
    mov dword ptr [rdx + 4], 0
    movq qword ptr [rdx + 8], xmm0
    ret
.Lbail:
    jmp c_rt_coerce_num2_d
RTX_ENDF(rt_coerce_num2_d)
.section .note.GNU-stack,"",@progbits

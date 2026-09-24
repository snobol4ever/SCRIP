#include "rtx_abi.inc"
RTX_GATE_DEF(icnrel)
#define OP_LE    6
#define OP_GE    8
#define OP_EQ    9
#define OP_NE   10
#define OP_SLT  12
#define OP_SLE  13
#define OP_SGT  14
#define OP_SGE  15
#define OP_SEQ  16
#define OP_SNE  17
#define OP_EQV  22
#define OP_NEQV 23
RTX_FUNC(rt_jct_relop)
    RTX_GATE(icnrel, .Lbail)
    cmp     r8d, OP_EQV
    je      .Leqv
    cmp     r8d, OP_NEQV
    je      .Leqv
    cmp     r8d, OP_SLT
    jb      .Lbail
    cmp     r8d, OP_SNE
    ja      .Lbail
    cmp     dil, DT_S
    jne     .Lbail
    cmp     dl, DT_S
    jne     .Lbail
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    je      .Lbail
    mov     rax, rdx
    shr     rax, 32
    cmp     eax, -1
    je      .Lbail
    test    rsi, rsi
    jz      .Lbail
    test    rcx, rcx
    jz      .Lbail
    cmp     byte ptr [rsi], 3
    je      .Lbail
    cmp     byte ptr [rcx], 3
    je      .Lbail
    shr     rdi, 32
    shr     rdx, 32
    mov     rax, rdi
    cmp     rax, rdx
    cmova   rax, rdx
    cmp     rdi, rdx
    seta    dil
    setb    dl
    sub     dil, dl
    movsx   edi, dil
    mov     rdx, rax
.Lstrloop:
    test    rdx, rdx
    jz      .Lstrtail
    movzx   eax, byte ptr [rsi]
    cmp     al, byte ptr [rcx]
    jne     .Lstrdiff
    inc     rsi
    inc     rcx
    dec     rdx
    jmp     .Lstrloop
.Lstrtail:
    test    edi, edi
    js      .Lless
    jnz     .Lstrgt
.Lstreq:
    xor     eax, eax
    cmp     r8d, OP_SEQ
    je      .Lret1
    cmp     r8d, OP_SLE
    je      .Lret1
    cmp     r8d, OP_SGE
    je      .Lret1
    ret
.Lstrdiff:
    jb      .Lless
.Lstrgt:
    xor     eax, eax
    cmp     r8d, OP_SGT
    je      .Lret1
    cmp     r8d, OP_SGE
    je      .Lret1
    cmp     r8d, OP_SNE
    je      .Lret1
    ret
.Lless:
    xor     eax, eax
    cmp     r8d, OP_SLT
    je      .Lret1
    cmp     r8d, OP_SLE
    je      .Lret1
    cmp     r8d, OP_SNE
    je      .Lret1
    ret
.Leqv:
    cmp     edi, edx
    jne     .Lbail
    cmp     rsi, rcx
    je      .Leqv_equal
    cmp     dil, DT_I
    jne     .Lbail
    xor     eax, eax
    cmp     r8d, OP_NEQV
    je      .Lret1
    ret
.Leqv_equal:
    cmp     dil, DT_S
    jne     .Leqv_settled
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    je      .Lbail
.Leqv_settled:
    xor     eax, eax
    cmp     r8d, OP_EQV
    je      .Lret1
    ret
.Lret1:
    mov     eax, 1
    ret
.Lbail:
    RTX_CTAIL(c_rt_jct_relop)
RTX_ENDF(rt_jct_relop)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_FUNC(rt_str_coerce)
    RTX_GATE(icnrel, .Lsco_c)
    cmp     dil, DT_S
    jne     .Lsco_c
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    je      .Lsco_c
    mov     rax, rdi
    mov     rdx, rsi
    ret
.Lsco_c:
    RTX_CTAIL(c_rt_str_coerce)
RTX_ENDF(rt_str_coerce)
.section .note.GNU-stack,"",@progbits

#ifndef RTX_ABI_INC
#define RTX_ABI_INC
.intel_syntax noprefix
#include "descr_tags.inc"
#define RTX_NAME(name) .pushsection rtx_entry_names,"a",@progbits; .asciz #name; .popsection
#define RTX_FUNC(name) .text; .globl name; .type name,@function; .align 16; name: ; .cfi_startproc; endbr64; RTX_NAME(name)
#define RTX_ENTRY(name) .globl name; .type name,@function; name: ; endbr64; RTX_NAME(name)
#define RTX_ENDF(name) .cfi_endproc; .size name, .-name
#define RTX_PUSH(x) push x; .cfi_adjust_cfa_offset 8
#define RTX_POP(x) pop x; .cfi_adjust_cfa_offset -8
#define RTX_PUSHS(x) push x; .cfi_adjust_cfa_offset 8; .cfi_rel_offset x, 0
#define RTX_POPS(x) pop x; .cfi_adjust_cfa_offset -8; .cfi_restore x
#define RTX_SUB_RSP(n) sub rsp, n; .cfi_adjust_cfa_offset n
#define RTX_ADD_RSP(n) add rsp, n; .cfi_adjust_cfa_offset -(n)
#define RTX_CFA(n) .cfi_def_cfa_offset n
#define RTX_CFA_DYN(d, k) .cfi_escape 0x0f, 7, 0x77, 0x80 | ((d) & 0x7f), (d) >> 7, 0x06, 0x23, 0x80 | ((k) & 0x7f), (k) >> 7
#define RTX_PUSH_DYN(x, d, k) push x; RTX_CFA_DYN(d, k)
#define RTX_POP_DYN(x, d, k) pop x; RTX_CFA_DYN(d, k)
#define RTX_SUB_RSP_DYN(n, d, k) sub rsp, n; RTX_CFA_DYN(d, k)
#define RTX_ADD_RSP_DYN(n, d, k) add rsp, n; RTX_CFA_DYN(d, k)
#define RTX_GVA_R9 mov r9, qword ptr [rip + rtccb@GOTPCREL]; mov r9, qword ptr [r9 + 48]
#define RTX_CCALL(fn) RTX_PUSH(r8); RTX_PUSH(r9); RTX_PUSH(r10); RTX_PUSH(r11); call fn; RTX_POP(r11); RTX_POP(r10); RTX_POP(r9); RTX_POP(r8)
#define RTX_CTAIL(fn) RTX_PUSH(r8); RTX_PUSH(r9); RTX_PUSH(r10); RTX_PUSH(r11); RTX_SUB_RSP(8); call fn; RTX_ADD_RSP(8); RTX_POP(r11); RTX_POP(r10); RTX_POP(r9); RTX_POP(r8); ret
#define RTX_CTAIL_GVA(fn) RTX_PUSH(r8); RTX_PUSH(r9); RTX_PUSH(r10); RTX_PUSH(r11); RTX_SUB_RSP(8); call fn; RTX_ADD_RSP(8); RTX_POP(r11); RTX_POP(r10); RTX_POP(r9); RTX_POP(r8); RTX_GVA_R9; ret
#define RTX_DECLINE mov eax, RTX_NOT_HANDLED; ret
#define RTX_CALL(fn) call qword ptr [rip + fn@GOTPCREL]
#define RTX_JMP(fn) jmp qword ptr [rip + fn@GOTPCREL]
#define RTX_GATE_DEF(fam) .data; .align 1; .globl rtx_gate_##fam; .hidden rtx_gate_##fam; .type rtx_gate_##fam,@object; .size rtx_gate_##fam,1; rtx_gate_##fam: .byte 0; .text
#define RTX_GATE(fam, stub) cmp byte ptr [rip + rtx_gate_##fam], 0; je stub
#define RTX_CALL_ALIGN(k) RTX_PUSH(rsp); RTX_PUSH(qword ptr [rsp]); and rsp, -16; RTX_CFA_DYN(8, k)
#define RTX_CALL_UNALIGN(k) mov rsp, [rsp + 8]; .cfi_def_cfa rsp, k
#endif
#include "../rt/rt_diag.h"

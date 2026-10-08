#ifndef RTX_ABI_INC
#define RTX_ABI_INC
.intel_syntax noprefix
#include "descr_tags.inc"
#define RTX_NAME(name) .pushsection rtx_entry_names,"a",@progbits; .asciz #name; .popsection
#define RTX_FUNC(name) .text; .globl name; .type name,@function; .align 16; name: ; endbr64; RTX_NAME(name)
#define RTX_ENTRY(name) .globl name; .type name,@function; name: ; endbr64; RTX_NAME(name)
#define RTX_ENDF(name) .size name, .-name
#define RTX_GVA_R9 mov r9, qword ptr [rip + rtccb@GOTPCREL]; mov r9, qword ptr [r9 + 48]
#define RTX_CCALL(fn) push r8; push r9; push r10; push r11; call fn; pop r11; pop r10; pop r9; pop r8
#define RTX_CTAIL(fn) push r8; push r9; push r10; push r11; sub rsp, 8; call fn; add rsp, 8; pop r11; pop r10; pop r9; pop r8; ret
#define RTX_CTAIL_GVA(fn) push r8; push r9; push r10; push r11; sub rsp, 8; call fn; add rsp, 8; pop r11; pop r10; pop r9; pop r8; RTX_GVA_R9; ret
#define RTX_CALL(fn) call qword ptr [rip + fn@GOTPCREL]
#define RTX_JMP(fn) jmp qword ptr [rip + fn@GOTPCREL]
#define RTX_GATE_DEF(fam) .data; .align 1; .globl rtx_gate_##fam; .hidden rtx_gate_##fam; .type rtx_gate_##fam,@object; .size rtx_gate_##fam,1; rtx_gate_##fam: .byte 0; .text
#define RTX_GATE(fam, stub) cmp byte ptr [rip + rtx_gate_##fam], 0; je stub
#define RTX_CALL_ALIGN push rsp; push qword ptr [rsp]; and rsp, -16
#define RTX_CALL_UNALIGN mov rsp, [rsp + 8]
#endif
#include "../rt/rt_diag.h"

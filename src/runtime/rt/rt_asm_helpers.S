/* rt_asm_helpers.S — hand-written x86-64 fast paths for the SMALL, HOTTEST runtime leaves (Lon directive: asm for small helpers, C for large; -O0/-O1 only, no -O2 in this container).            */
/* SysV AMD64 ABI, matched byte-for-byte to the SCRIP emitter's own DESCR_t marshaling (verified vs emitted caller in a mode-4 .s: arg DESCR_t in RDI:RSI = {v(lo32),slen(hi32)} : payload, return   */
/* DESCR_t in RAX:RDX).  descr.h layout: v @0 (4B), slen @4 (4B), payload @8 (8B); tags come from descr_tags.inc (NEVER literals -- s230); NAMETRAP = {v=DT_N,slen=2,p=VCELL*}; VCELL_t.cellp @0.  The three HOT deref shapes are inlined here  */
/* (slen==1 pointer cell, slen==0 shared-NV name, slen==2 nametrap with a direct cellp); every COLD shape (nametrap table lookup, varref substring, malformed) tail-jumps to the full C rt_deref_slow.  */
/* RTX-5a (s200): THIS FILE IS NOW UNDER THE RTX KILL-SWITCH CONTRACT (ARCH-SNOBOL4-RTX.md section 4).  It predates the RTX ladder and was invisible to it: ARCH section 7's step-0 existence grep filtered      */
/* --include=*.c/*.h/*.cpp and so could not see .S files at all, which is why RTX-5's rung listed rt_deref as a PORT TARGET when it has been hand-written asm here all along (sixth phantom class, s200).      */
/* GATE = SCRIP_RTX_LEAF, byte rtx_gate_leaf, initialised in rtx/rtx_init.c alongside the other four families.  Gate 0 => tail-jump to the untouched C body, whose signature is identical, so the args are     */
/* already in register and its return goes straight to our caller -- that tail jump IS the byte-identity proof path (same idiom as RTX_GATE in rtx_abi.inc, hand-written here because this file is AT&T        */
/* syntax while rtx_abi.inc's macros are Intel; ARCH ruling 1 says Intel project-wide, so this file's syntax is a second way it sits outside the contract -- recorded, not fixed, as changing it would rewrite  */
/* working code for no behavioural gain).  BOTH fallbacks are COMPLETE implementations, not cold-shape handlers: rt_deref_slow (pattern_match.c:1173) covers all four shapes the asm inlines plus the cold      */
/* table/varref cases, and to_int_slow (core/core.c:1991) covers DT_I as well as the coercions -- verified s200, which is what makes a two-instruction gate sufficient here.                                    */
#include "descr_tags.inc"   /* s230: was hand-encoding DT_N/DT_FAIL/DT_I as $9/$99/$6 -- the TAG-3 renumber turned all three
                             * silently wrong (m3 276->218). This file is AT&T so it cannot include rtx_abi.inc (.intel_syntax);
                             * descr_tags.inc is syntax-neutral for exactly that reason. NEVER re-spell a tag number here.   */
    .data
    .align 1
    .globl rtx_gate_leaf
    .hidden rtx_gate_leaf
    .type  rtx_gate_leaf, @object
    .size  rtx_gate_leaf, 1
rtx_gate_leaf:
    .byte  0
    .text
    .globl rt_deref
    .type  rt_deref, @function
rt_deref:
    cmpb   $0, rtx_gate_leaf(%rip)
    jne    .Lrd_asm
    jmp    rt_deref_slow@PLT
.Lrd_asm:
    movl   %edi, %eax
    cmpl   $DT_N, %eax
    jne    .Lrd_retd
    movq   %rdi, %rax
    shrq   $32, %rax
    cmpl   $1, %eax
    je     .Lrd_ptr
    cmpl   $2, %eax
    je     .Lrd_ntrap
    testl  %eax, %eax
    je     .Lrd_name0
.Lrd_retd:
    movq   %rdi, %rax
    movq   %rsi, %rdx
    ret
.Lrd_ptr:
    testq  %rsi, %rsi
    jz     .Lrd_retd
    movq   8(%rsi), %rdx
    movq   (%rsi), %rax
    ret
.Lrd_name0:
    testq  %rsi, %rsi
    jz     .Lrd_retd
    cmpb   $0, (%rsi)
    je     .Lrd_retd
    movq   %rsi, %rdi
    jmp    NV_GET_fn@PLT
.Lrd_ntrap:
    testq  %rsi, %rsi
    jz     .Lrd_fail
    movq   (%rsi), %rax
    testq  %rax, %rax
    jz     .Lrd_slow
    movq   8(%rax), %rdx
    movq   (%rax), %rax
    ret
.Lrd_fail:
    movl   $DT_FAIL, %eax
    xorl   %edx, %edx
    ret
.Lrd_slow:
    jmp    rt_deref_slow@PLT
    .size  rt_deref, .-rt_deref
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* int64_t to_int(DESCR_t v): arg RDI:RSI = {v,slen}:payload, return int64_t in RAX.  Hot case DT_I returns v.i (RSI) with two instructions; DT_R/DT_S/DT_K/error delegate to the full C to_int_slow. */
    .globl to_int
    .type  to_int, @function
to_int:
    cmpb   $0, rtx_gate_leaf(%rip)
    jne    .Lti_asm
    jmp    to_int_slow@PLT
.Lti_asm:
    cmpl   $DT_I, %edi
    jne    .Lti_slow
    movq   %rsi, %rax
    ret
.Lti_slow:
    jmp    to_int_slow@PLT
    .size  to_int, .-to_int
    .section .note.GNU-stack,"",@progbits

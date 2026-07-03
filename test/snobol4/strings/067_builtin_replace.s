  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  pop rbp
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
main_α_body:
xchain0_n0_α:
# IR_LIT_STRING
bb1_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_ω
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
xchain0_n1_α:
# IR_LIT_STRING
bb2_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_ω
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "aeiou"
xchain0_n2_α:
# IR_LIT_STRING
bb3_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_ω
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "AEIOU"
xchain0_n3_α:
bb4_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+96] -> [r12+48]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 56], rax
# marshal arg1 = producer-box slot [r12+112] -> [r12+64]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 72], rax
# marshal arg2 = producer-box slot [r12+128] -> [r12+80]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 88], rax
  .section .rodata
  .Lrkfn5: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn5]
 lea rsi, [r12 + 48]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je main_ω
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_ω
xchain0_n4_α:
# IR_ASSIGN global
bb5_α:
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp main_γ
 xchain0_n4_β:
 jmp main_ω
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret

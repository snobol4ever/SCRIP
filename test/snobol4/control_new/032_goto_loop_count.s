  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "N"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
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
# IR_LIT_INTEGER
bb1_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad 0
xchain0_n1_α:
# IR_ASSIGN gva
bb2_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
xchain0_n2_α:
# IR_VAR gva
bb3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n6_α
xchain0_n3_α:
# IR_LIT_INTEGER
bb4_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n6_α
.Lx4_0:
 .quad 1
xchain0_n4_α:
bb5_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 80]
 cmp eax, 100
 je .Lx5_0
 mov eax, dword ptr [r12 + 96]
 cmp eax, 100
 je .Lx5_0
 mov eax, dword ptr [r12 + 80]
 cmp eax, 6
 jne .Lx5_2
 mov eax, dword ptr [r12 + 96]
 cmp eax, 6
 jne .Lx5_2
.Lx5_1:
 mov rax, qword ptr [r12 + 88]
 mov rcx, qword ptr [r12 + 104]
 add rax, rcx
 mov qword ptr [r12 + 64], 6
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n5_α
.Lx5_0:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 mov r8d, 0
 lea r9, [r12 + 64]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx5_3
.Lx5_2:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n6_α
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
.Lx5_3:
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
xchain0_n5_α:
# IR_ASSIGN gva
bb6_α:
 mov rax, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
xchain0_n6_α:
# IR_VAR gva
bb7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n2_α
xchain0_n7_α:
# IR_LIT_INTEGER
bb8_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n2_α
.Lx8_0:
 .quad 5
xchain0_n8_α:
bb9_α:
# BOX IR_CALL GT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+160] -> [r12+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [r12+176] -> [r12+144]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lrkfn10: .string "GT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [r12 + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n2_α
xchain0_n9_α:
# IR_VAR gva
bb10_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp main_ω
xchain0_n10_α:
# IR_ASSIGN global
bb11_α:
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov rdi, qword ptr [rip + .Lx12_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_ω
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
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

  .intel_syntax noprefix
  .text
swap_α:
#=======================================================================================================================
    .global swap_α
    .global swap_β
    .global swap_γ
    .global swap_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
swap_α_body:
snoch0_n0_α:
# IR_VAR
bb1_α:
 mov rdi, qword ptr [rip + .Lx2_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "a"
snoch0_n1_α:
bb2_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_VAR
bb3_α:
 mov rdi, qword ptr [rip + .Lx5_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "b"
snoch0_n3_α:
bb4_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_VAR
bb5_α:
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "tmp"
snoch0_n5_α:
bb6_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S0]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
bb7_α:
# IR_ASSIGN_CONCAT 3 parts
 mov dword ptr [r12 + 48], 1
 lea rax, [rip + .S1]
 mov qword ptr [r12 + 56], rax
 mov dword ptr [r12 + 64], 0
 lea rax, [rip + .S4]
 mov qword ptr [r12 + 72], rax
 mov dword ptr [r12 + 80], 1
 lea rax, [rip + .S2]
 mov qword ptr [r12 + 88], rax
 lea rdi, [rip + .S3]
 lea rsi, [r12 + 48]
 mov edx, 3
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_gvar_assign_concat_parts@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n7_α
snoch0_n7_α:
snoch0_n7_β:
jmp swap_γ
jmp swap_γ
swap_β:
jmp swap_ω
swap_γ:
mov eax, 1
xor edx, edx
pop r12
ret
swap_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
  .section .rodata
  .Lpn0: .string "swap"
  .Lpp0_0: .string "a"
  .Lpp0_1: .string "b"
  .Lpp0_2: .string "tmp"
  .Lpnames0:
  .quad .Lpp0_0
  .quad .Lpp0_1
  .quad .Lpp0_2
  .quad 0
  .section .text
  .intel_syntax noprefix
proc_startup:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + .Lpnames0]
  mov edx, 3
  call rt_proc_register@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + swap_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lpn0]
  mov esi, 96
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "tmp"
  .Lgvan1: .string "a"
  .Lgvan2: .string "b"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .bss
  .align 16
__gva: .space 48, 0
  .section .text
  .intel_syntax noprefix
  .section .rodata
  .Lprocn0: .string "swap"
  .align 8
__proc_names:
  .quad .Lprocn0
  .section .bss
  .align 8
__proc: .space 8, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  call proc_startup
  lea rdi, [rip + __proc]
  lea rsi, [rip + __proc_names]
  mov edx, 1
  call rt_proc_table_fill@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 3
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call flat_α
  xor eax, eax
  pop rbp
  ret
flat_α:
#=======================================================================================================================
    .global flat_α
    .global flat_β
    .global flat_γ
    .global flat_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
flat_α_body:
snoch11_n0_α:
bb8_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S5]
 call rt_proc_define@PLT
 jmp snoch11_n1_α
 snoch11_n0_β:
 jmp snoch11_n1_α
snoch11_n1_α:
# IR_LIT_S
bb9_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg13_done
 xgvarg13_β:
 jmp flat_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "hello"
xgvarg13_done:
# IR_LIT_S
bb10_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg15_done
 xgvarg15_β:
 jmp flat_γ
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "world"
xgvarg15_done:
bb11_α:
# BOX IR_CALL swap(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+64]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 72], rax
# marshal arg1 = producer-box slot [r12+32] -> [r12+80]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 40]
 mov qword ptr [r12 + 88], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je flat_γ
 jmp flat_γ
snoch11_n1_β:
 jmp flat_γ
flat_β:
jmp flat_ω
flat_γ:
mov eax, 1
xor edx, edx
pop r12
ret
flat_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
.section .rodata
.S0: .string "tmp"
.S1: .string "a"
.S2: .string "b"
.S3: .string "OUTPUT"
.S4: .string " "
.S5: .string "swap(a,b)tmp"
.text

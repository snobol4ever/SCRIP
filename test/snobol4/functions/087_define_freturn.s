  .intel_syntax noprefix
  .text
ispos_α:
#=======================================================================================================================
    .global ispos_α
    .global ispos_β
    .global ispos_γ
    .global ispos_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
ispos_α_body:
snoch0_n0_α:
# IR_VAR
bb1_α:
 mov rdi, qword ptr [rip + .Lx3_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "x"
xgvarg1_done:
# IR_LIT_I
bb2_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg4_done
 xgvarg4_β:
 jmp snoch0_n2_α
.Lx5_0:
 .quad 0
xgvarg4_done:
bb3_α:
# BOX IR_CALL GT(...) inline integer relop [four-port, FAIL->ω]
   lea rdi, [rip + .S0]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 64], rax
 mov rcx, 0
 mov qword ptr [r12 + 48], 0
 mov qword ptr [r12 + 56], 0
 mov rax, qword ptr [r12 + 64]
 cmp rax, rcx
 jle snoch0_n2_α
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
snoch0_n1_β:
jmp ispos_γ
jmp ispos_γ
snoch0_n2_α:
snoch0_n2_β:
jmp ispos_ω
jmp ispos_ω
ispos_β:
jmp ispos_ω
ispos_γ:
mov eax, 1
xor edx, edx
pop r12
ret
ispos_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
  .section .rodata
  .Lpn0: .string "ispos"
  .Lpp0_0: .string "x"
  .Lpnames0:
  .quad .Lpp0_0
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
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + ispos_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lpn0]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lprocn0: .string "ispos"
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
snoch8_n0_α:
bb4_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S1]
 call rt_proc_define@PLT
 jmp snoch8_n1_α
 snoch8_n0_β:
 jmp snoch8_n1_α
snoch8_n1_α:
# IR_LIT_I
bb5_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch8_n3_α
.Lx11_0:
 .quad 5
xgvarg10_done:
bb6_α:
# BOX IR_CALL ispos(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch8_n3_α
 jmp snoch8_n2_α
snoch8_n1_β:
 jmp snoch8_n3_α
snoch8_n2_α:
# IR_LIT_scalar
bb7_α:
 jmp snoch8_n4_α
 snoch8_n2_β:
 jmp snoch8_n5_α
snoch8_n3_α:
# IR_LIT_scalar
bb8_α:
 jmp snoch8_n6_α
 snoch8_n3_β:
 jmp snoch8_n5_α
snoch8_n4_α:
bb9_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch8_n5_α
 snoch8_n4_β:
 jmp snoch8_n5_α
snoch8_n5_α:
xargsub17_n0_α:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 72], rax
 jmp xargsub17_n1_α
 xargsub17_n0_β:
 jmp snoch8_n8_α
.Lx18_0:
 .quad 3
xargsub17_n1_α:
bb11_α:
# IR_UNOP_GVAR_SLOT
 mov rax, 3
 neg rax
 mov qword ptr [r12 + 80], rax
 jmp xgvarg16_done
 xargsub17_n1_β:
 jmp snoch8_n8_α
xgvarg16_done:
bb12_α:
# BOX IR_CALL ispos(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+80] -> [r12+104]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 104], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 112], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 104]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 88], rax
 mov qword ptr [r12 + 96], rdx
 cmp eax, 99
 je snoch8_n8_α
 jmp snoch8_n7_α
snoch8_n5_β:
 jmp snoch8_n8_α
snoch8_n6_α:
bb13_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp snoch8_n5_α
 snoch8_n6_β:
 jmp snoch8_n5_α
snoch8_n7_α:
# IR_LIT_scalar
bb14_α:
 jmp snoch8_n9_α
 snoch8_n7_β:
 jmp flat_γ
snoch8_n8_α:
# IR_LIT_scalar
bb15_α:
 jmp snoch8_n10_α
 snoch8_n8_β:
 jmp flat_γ
snoch8_n9_α:
bb16_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch8_n9_β:
 jmp flat_γ
snoch8_n10_α:
bb17_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch8_n10_β:
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
.S0: .string "x"
.S1: .string "ispos(x)"
.S2: .string "OUTPUT"
.S3: .string "positive"
.S4: .string "wrong"
.S5: .string "not positive"
.text

  .intel_syntax noprefix
  .text
fib_α:
#=======================================================================================================================
    .global fib_α
    .global fib_β
    .global fib_γ
    .global fib_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
fib_α_body:
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
 .string "n"
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
 .quad 1
xgvarg4_done:
bb3_α:
# BOX IR_CALL LE(...) inline integer relop [four-port, FAIL->ω]
   lea rdi, [rip + .S0]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 64], rax
 mov rcx, 1
 mov qword ptr [r12 + 48], 0
 mov qword ptr [r12 + 56], 0
 mov rax, qword ptr [r12 + 64]
 cmp rax, rcx
 jg snoch0_n2_α
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_VAR
bb4_α:
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp snoch0_n4_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "n"
snoch0_n2_α:
xgvarg10_done:
bb5_α:
# BOX IR_CALL fib(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+112]
   lea rdi, [rip + .S0]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 128], rax
 mov rcx, 1
 mov rax, qword ptr [r12 + 128]
 sub rax, rcx
 mov qword ptr [r12 + 112], 6
 mov qword ptr [r12 + 120], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 112]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n5_α
snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb6_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S0]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
snoch0_n4_β:
jmp fib_γ
jmp fib_γ
snoch0_n5_α:
xgvarg13_done:
bb7_α:
# BOX IR_CALL fib(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+160]
   lea rdi, [rip + .S0]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 176], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 176]
 sub rax, rcx
 mov qword ptr [r12 + 160], 6
 mov qword ptr [r12 + 168], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 160]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n6_α
snoch0_n5_β:
 jmp snoch0_n4_α
snoch0_n6_α:
bb8_α:
# IR_BINOP_GVAR_ARITH (dynamic operands: DESCR-in DESCR-out via rt_num_arith)
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je snoch0_n4_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n4_α
snoch0_n7_α:
bb9_α:
# IR_ASSIGN
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n4_α
 snoch0_n7_β:
 jmp snoch0_n4_α
fib_β:
jmp fib_ω
fib_γ:
mov eax, 1
xor edx, edx
pop r12
ret
fib_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
  .section .rodata
  .Lpn0: .string "fib"
  .Lpp0_0: .string "n"
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
  lea rsi, [rip + fib_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lpn0]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "fib"
  .Lgvan1: .string "n"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
  .section .text
  .intel_syntax noprefix
  .section .rodata
  .Lprocn0: .string "fib"
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
  mov edx, 2
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
snoch18_n0_α:
bb10_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S2]
 call rt_proc_define@PLT
 jmp snoch18_n1_α
 snoch18_n0_β:
 jmp snoch18_n1_α
snoch18_n1_α:
# IR_LIT_I
bb11_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg20_done
 xgvarg20_β:
 jmp snoch18_n3_α
.Lx21_0:
 .quad 0
xgvarg20_done:
bb12_α:
# BOX IR_CALL fib(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
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
 je snoch18_n3_α
 jmp snoch18_n2_α
snoch18_n1_β:
 jmp snoch18_n3_α
snoch18_n2_α:
bb13_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S3]
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 call rt_gvar_assign_descr@PLT
 jmp snoch18_n3_α
 snoch18_n2_β:
 jmp snoch18_n3_α
snoch18_n3_α:
# IR_LIT_I
bb14_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg24_done
 xgvarg24_β:
 jmp snoch18_n5_α
.Lx25_0:
 .quad 1
xgvarg24_done:
bb15_α:
# BOX IR_CALL fib(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch18_n5_α
 jmp snoch18_n4_α
snoch18_n3_β:
 jmp snoch18_n5_α
snoch18_n4_α:
bb16_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S3]
 mov rsi, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 call rt_gvar_assign_descr@PLT
 jmp snoch18_n5_α
 snoch18_n4_β:
 jmp snoch18_n5_α
snoch18_n5_α:
# IR_LIT_I
bb17_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg28_done
 xgvarg28_β:
 jmp snoch18_n7_α
.Lx29_0:
 .quad 6
xgvarg28_done:
bb18_α:
# BOX IR_CALL fib(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+144]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 152], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 144]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch18_n7_α
 jmp snoch18_n6_α
snoch18_n5_β:
 jmp snoch18_n7_α
snoch18_n6_α:
bb19_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S3]
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 call rt_gvar_assign_descr@PLT
 jmp snoch18_n7_α
 snoch18_n6_β:
 jmp snoch18_n7_α
snoch18_n7_α:
# IR_LIT_I
bb20_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg32_done
 xgvarg32_β:
 jmp flat_γ
.Lx33_0:
 .quad 10
xgvarg32_done:
bb21_α:
# BOX IR_CALL fib(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+192]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 200], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 192]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch18_n8_α
snoch18_n7_β:
 jmp flat_γ
snoch18_n8_α:
bb22_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S3]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch18_n8_β:
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
.S0: .string "n"
.S1: .string "fib"
.S2: .string "fib(n)"
.S3: .string "OUTPUT"
.text

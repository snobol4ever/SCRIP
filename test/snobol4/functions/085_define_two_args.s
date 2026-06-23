  .intel_syntax noprefix
  .text
add_α:
#=======================================================================================================================
    .global add_α
    .global add_β
    .global add_γ
    .global add_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
add_α_body:
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
# IR_VAR
bb2_α:
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp snoch0_n2_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "b"
snoch0_n2_α:
snoch0_n2_β:
jmp add_γ
jmp add_γ
snoch0_n3_α:
bb3_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 mov rdx, 0
 call rt_gvar_arith@PLT
 mov qword ptr [r12 + 32], rax
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n2_α
snoch0_n4_α:
bb4_α:
# IR_ASSIGN
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 32]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n2_α
 snoch0_n4_β:
 jmp snoch0_n2_α
add_β:
jmp add_ω
add_γ:
mov eax, 1
xor edx, edx
pop r12
ret
add_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
  .section .rodata
  .Lpn0: .string "add"
  .Lpp0_0: .string "a"
  .Lpp0_1: .string "b"
  .Lpnames0:
  .quad .Lpp0_0
  .quad .Lpp0_1
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
  mov edx, 2
  call rt_proc_register@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + add_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lpn0]
  mov esi, 40
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .globl main
main:
  push rbp
  mov rbp, rsp
  call proc_startup
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
bb5_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S3]
 call rt_proc_define@PLT
 jmp snoch8_n1_α
 snoch8_n0_β:
 jmp snoch8_n1_α
snoch8_n1_α:
# IR_LIT_I
bb6_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch8_n3_α
.Lx11_0:
 .quad 3
xgvarg10_done:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch8_n3_α
.Lx13_0:
 .quad 4
xgvarg12_done:
bb8_α:
# BOX IR_CALL add(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
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
  .section .rodata
  .Lprocfn15: .string "add"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn15]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je snoch8_n3_α
 jmp snoch8_n2_α
snoch8_n1_β:
 jmp snoch8_n3_α
snoch8_n2_α:
bb9_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S4]
 mov rsi, qword ptr [r12 + 48]
 mov rdx, qword ptr [r12 + 56]
 call rt_gvar_assign_descr@PLT
 jmp snoch8_n3_α
 snoch8_n2_β:
 jmp snoch8_n3_α
snoch8_n3_α:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg17_done
 xgvarg17_β:
 jmp flat_γ
.Lx18_0:
 .quad 10
xgvarg17_done:
# IR_LIT_I
bb11_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg19_done
 xgvarg19_β:
 jmp flat_γ
.Lx20_0:
 .quad 32
xgvarg19_done:
bb12_α:
# BOX IR_CALL add(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+96] -> [r12+144]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 152], rax
# marshal arg1 = producer-box slot [r12+112] -> [r12+160]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lprocfn22: .string "add"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn22]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch8_n4_α
snoch8_n3_β:
 jmp flat_γ
snoch8_n4_α:
bb13_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S4]
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch8_n4_β:
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
.S0: .string "a"
.S1: .string "b"
.S2: .string "add"
.S3: .string "add(a,b)"
.S4: .string "OUTPUT"
.text

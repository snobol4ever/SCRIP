  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
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
snoch0_n0_α:
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S0]
 movabs rsi, 0
 call rt_gvar_assign_int@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_VAR
bb3_α:
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "N"
snoch0_n3_α:
# IR_LIT_scalar
bb4_α:
 jmp snoch0_n5_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_VAR
bb5_α:
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xgvarg6_done
 xgvarg6_β:
 jmp snoch0_n2_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "N"
xgvarg6_done:
# IR_LIT_I
bb6_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg9_done
 xgvarg9_β:
 jmp snoch0_n2_α
.Lx10_0:
 .quad 5
xgvarg9_done:
bb7_α:
# BOX IR_CALL GT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn12: .string "GT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n6_α
snoch0_n4_β:
 jmp snoch0_n2_α
snoch0_n5_α:
bb8_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S0]
 call rt_gvar_get_int@PLT
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 96], rax
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp snoch0_n4_α
snoch0_n6_α:
# IR_VAR
bb9_α:
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 104], rax
 mov qword ptr [r12 + 112], rdx
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp flat_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "N"
snoch0_n7_α:
bb10_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 96]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n4_α
 snoch0_n7_β:
 jmp snoch0_n4_α
snoch0_n8_α:
bb11_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S0]
 call rt_gvar_assign_var@PLT
 jmp flat_γ
 snoch0_n8_β:
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
.S0: .string "N"
.S1: .string "OUTPUT"
.text

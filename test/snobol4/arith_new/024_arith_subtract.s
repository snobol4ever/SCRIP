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
 jmp flat_γ
snoch0_n1_α:
# IR_LIT_scalar
bb2_α:
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
bb3_α:
# IR_BINOP_GVAR_ARITH
 mov rax, 10
 mov rcx, 3
 sub rax, rcx
 mov qword ptr [r12 + 0], rax
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp flat_γ
snoch0_n3_α:
bb4_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 0]
 call rt_gvar_assign_int@PLT
 jmp flat_γ
 snoch0_n3_β:
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
.S0: .string "OUTPUT"
.text

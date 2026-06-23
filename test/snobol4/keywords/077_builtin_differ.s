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
# IR_LIT_S
bb1_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "abc"
xgvarg1_done:
# IR_LIT_S
bb2_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp snoch0_n2_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "xyz"
xgvarg3_done:
bb3_α:
# BOX IR_CALL DIFFER(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn6: .string "DIFFER"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb4_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
# IR_LIT_scalar
bb5_α:
 jmp snoch0_n4_α
 snoch0_n2_β:
 jmp flat_γ
snoch0_n3_α:
bb6_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
bb7_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n4_β:
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
.S1: .string "different"
.S2: .string "same"
.text

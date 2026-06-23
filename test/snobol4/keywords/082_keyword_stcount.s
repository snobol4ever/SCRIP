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
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb4_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S0]
 movabs rsi, 2
 call rt_gvar_assign_int@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_KEYWORD_read
bb5_α:
 mov rdi, qword ptr [rip + .Lx6_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xgvarg5_done
 xgvarg5_β:
 jmp snoch0_n6_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "STNO"
xgvarg5_done:
# IR_LIT_I
bb6_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg7_done
 xgvarg7_β:
 jmp snoch0_n6_α
.Lx8_0:
 .quad 1
xgvarg7_done:
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
  .Lbynamefn10: .string "GT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n5_α
snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
# IR_LIT_scalar
bb8_α:
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
bb10_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
bb11_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
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
.S0: .string "X"
.S1: .string "OUTPUT"
.S2: .string "stno ok"
.S3: .string "wrong"
.text

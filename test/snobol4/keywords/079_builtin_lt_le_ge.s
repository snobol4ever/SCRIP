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
# IR_LIT_I
bb1_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp flat_γ
.Lx2_0:
 .quad 3
xgvarg1_done:
# IR_LIT_I
bb2_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp flat_γ
.Lx4_0:
 .quad 5
xgvarg3_done:
bb3_α:
# BOX IR_CALL LT(...) inline integer relop [four-port, FAIL->ω]
 movabs rax, 3
 mov qword ptr [r12 + 64], rax
 mov rcx, 5
 mov qword ptr [r12 + 48], 0
 mov qword ptr [r12 + 56], 0
 mov rax, qword ptr [r12 + 64]
 cmp rax, rcx
 jge flat_γ
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp flat_γ
snoch0_n1_α:
# IR_LIT_scalar
bb4_α:
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
bb5_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_LIT_I
bb6_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 88], rax
 jmp xgvarg9_done
 xgvarg9_β:
 jmp flat_γ
.Lx10_0:
 .quad 5
xgvarg9_done:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg11_done
 xgvarg11_β:
 jmp flat_γ
.Lx12_0:
 .quad 5
xgvarg11_done:
bb8_α:
# BOX IR_CALL LE(...) inline integer relop [four-port, FAIL->ω]
 movabs rax, 5
 mov qword ptr [r12 + 128], rax
 mov rcx, 5
 mov qword ptr [r12 + 112], 0
 mov qword ptr [r12 + 120], 0
 mov rax, qword ptr [r12 + 128]
 cmp rax, rcx
 jg flat_γ
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
bb10_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_LIT_I
bb11_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg17_done
 xgvarg17_β:
 jmp flat_γ
.Lx18_0:
 .quad 7
xgvarg17_done:
# IR_LIT_I
bb12_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg19_done
 xgvarg19_β:
 jmp flat_γ
.Lx20_0:
 .quad 5
xgvarg19_done:
bb13_α:
# BOX IR_CALL GE(...) inline integer relop [four-port, FAIL->ω]
 movabs rax, 7
 mov qword ptr [r12 + 192], rax
 mov rcx, 5
 mov qword ptr [r12 + 176], 0
 mov qword ptr [r12 + 184], 0
 mov rax, qword ptr [r12 + 192]
 cmp rax, rcx
 jl flat_γ
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb14_α:
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
bb15_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
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
.S0: .string "OUTPUT"
.S1: .string "3 < 5"
.S2: .string "5 <= 5"
.S3: .string "7 >= 5"
.text

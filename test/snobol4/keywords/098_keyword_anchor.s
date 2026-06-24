  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
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
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
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
snoch0_n0_α:
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN
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
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_VAR gva
bb5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
# IR_SCAN
bb6_α:
 lea rdi, [rip + .S1]
 mov rsi, 0
 lea rdx, [rip + .S3]
 mov rcx, 0
 mov r8, 0
 call rt_scan_lit@PLT
 test eax, eax
 je snoch0_n6_α
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_LIT_scalar
bb7_α:
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb8_α:
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp snoch0_n10_α
snoch0_n8_α:
bb9_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
bb10_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n10_α
snoch0_n10_α:
# IR_VAR gva
bb11_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n12_α
snoch0_n11_α:
# IR_SCAN
bb12_α:
 lea rdi, [rip + .S1]
 mov rsi, 0
 lea rdx, [rip + .S7]
 mov rcx, 0
 mov r8, 0
 call rt_scan_lit@PLT
 test eax, eax
 je snoch0_n12_α
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
# IR_LIT_scalar
bb13_α:
 jmp snoch0_n14_α
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
# IR_LIT_scalar
bb14_α:
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
bb15_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n14_β:
 jmp flat_γ
snoch0_n15_α:
bb16_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n15_β:
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
.S0: .string "ANCHOR"
.S1: .string "X"
.S2: .string "hello world"
.S3: .string "hello"
.S4: .string "OUTPUT"
.S5: .string "wrong"
.S6: .string "anchored match ok"
.S7: .string "world"
.S8: .string "anchor prevented mid-string match"
.S9: .string "should not reach"
.text

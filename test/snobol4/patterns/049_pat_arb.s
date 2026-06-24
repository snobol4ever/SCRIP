  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "V"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
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
snoch0_n0_α:
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_VAR gva
bb3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
# IR_SUBJECT
bb4_α:
 lea rdi, [rip + .S0]
 lea rsi, [r12 + 16]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan5_sγ
 xscan5_sβ:
 jmp snoch0_n4_α
xscan5_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb5_α:
 mov r13, qword ptr [r12 + 16]
 mov r15d, dword ptr [r12 + 24]
 mov dword ptr [r12 + 32], 0
 lea r10, [r12 + 40]
 jmp smatch8_retry
 snoch0_n3_β:
 jmp xscan5_dfail
smatch8_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 32]
# IR_MATCH_CAPTURE_SAVE
bb7_α:
 mov dword ptr [r12 + 48], r14d
# IR_MATCH_ARB
bb8_α:
 mov dword ptr [r12 + 64], 0
 mov eax, r14d
 mov dword ptr [r12 + 68], eax
 jmp xcap12_γ
 xcat11_left_β:
 add dword ptr [r12 + 64], 1
 mov eax, dword ptr [r12 + 68]
 add eax, dword ptr [r12 + 64]
 cmp eax, r15d
 jg xcat11_ω
 mov r14d, eax
 jmp xcap12_γ
xcap12_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S2]
 mov esi, dword ptr [r12 + 48]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xcat11_γ
xcat11_γ:
# IR_MATCH_LEN
bb10_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xcat11_right_ω
 add r14d, 1
 jmp xscan5_dok
 xcat11_right_β:
 jmp xcat11_right_ω
xcat11_right_ω:
 jmp xcat11_left_β
smatch8_elemb:
 jmp xcat11_right_β
xcat11_ω:
 jmp smatch8_adv
smatch8_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 32], 1
 mov eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jg xscan5_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan5_dfail
 jmp smatch8_retry
xscan5_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n5_α
xscan5_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n4_α
snoch0_n4_α:
# IR_LIT_scalar
bb13_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_VAR gva
bb14_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 72], rax
 mov qword ptr [r12 + 80], rdx
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
bb15_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S3]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
bb16_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S3]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_var@PLT
 jmp flat_γ
 snoch0_n7_β:
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
.S1: .string "aXb"
.S2: .string "V"
.S3: .string "OUTPUT"
.S4: .string "fail"
.text

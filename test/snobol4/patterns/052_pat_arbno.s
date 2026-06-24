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
flat_c0_α:
#=======================================================================================================================
    .global flat_c0_α
    .global flat_c0_β
    .global flat_c0_γ
    .global flat_c0_ω
flat_c0_α_body:
# IR_LIT
bb1_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg flat_c0_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov rdx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne flat_c0_ω
 add r14d, 1
 jmp flat_c0_γ
 flat_c0_β:
 sub r14d, 1
 jmp flat_c0_ω
flat_c0_γ:
 jmp flat_c0_wγ
flat_c0_ω:
 jmp flat_c0_wω
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
snoch1_n0_α:
# IR_LIT_scalar
bb2_α:
 jmp snoch1_n1_α
 snoch1_n0_β:
 jmp snoch1_n2_α
snoch1_n1_α:
bb3_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp snoch1_n2_α
 snoch1_n1_β:
 jmp snoch1_n2_α
snoch1_n2_α:
# IR_VAR gva
bb4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch1_n3_α
 snoch1_n2_β:
 jmp snoch1_n4_α
snoch1_n3_α:
# IR_SUBJECT
bb5_α:
 lea rdi, [rip + .S1]
 lea rsi, [r12 + 16]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan6_sγ
 xscan6_sβ:
 jmp snoch1_n4_α
xscan6_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb6_α:
 mov r13, qword ptr [r12 + 16]
 mov r15d, dword ptr [r12 + 24]
 mov dword ptr [r12 + 32], 0
 lea r10, [r12 + 40]
 jmp smatch9_retry
 snoch1_n3_β:
 jmp xscan6_dfail
smatch9_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 32]
# IR_MATCH_POS
bb8_α:
 cmp r14d, 0
 jne xcat12_ω
 jmp xcat12_γ
 xcat12_left_β:
 jmp xcat12_ω
xcat12_γ:
# IR_MATCH_CAPTURE_SAVE
bb9_α:
 mov dword ptr [r12 + 48], r14d
# IR_MATCH_ARBNO
bb10_α:
 mov dword ptr [r12 + 64], r14d
 jmp xcap14_γ
 xcat12_mid1_β:
 mov dword ptr [r12 + 68], r14d
 jmp flat_c0_α
flat_c0_wγ:
 mov eax, dword ptr [r12 + 68]
 cmp r14d, eax
 je .Lx17_0
 jmp xcap14_γ
flat_c0_wω:
.Lx17_0:
 mov r14d, dword ptr [r12 + 64]
 jmp xcat12_left_β
xcap14_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S3]
 mov esi, dword ptr [r12 + 48]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xcat12_mid1_γ
xcat12_mid1_γ:
# IR_MATCH_RPOS
bb12_α:
 mov ecx, r15d
 sub ecx, 0
 cmp r14d, ecx
 jne xcat12_mid1_β
 jmp xscan6_dok
 xcat12_mid2_β:
 jmp xcat12_mid1_β
xcat12_right_ω:
 jmp xcat12_left_β
smatch9_elemb:
 jmp xcat12_mid2_β
xcat12_ω:
 jmp smatch9_adv
smatch9_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 32], 1
 mov eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jg xscan6_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan6_dfail
 jmp smatch9_retry
xscan6_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
jmp snoch1_n5_α
xscan6_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch1_n4_α
snoch1_n4_α:
# IR_LIT_scalar
bb15_α:
 jmp snoch1_n6_α
 snoch1_n4_β:
 jmp flat_γ
snoch1_n5_α:
# IR_VAR gva
bb16_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 72], rax
 mov qword ptr [r12 + 80], rdx
 jmp snoch1_n7_α
 snoch1_n5_β:
 jmp flat_γ
snoch1_n6_α:
bb17_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch1_n6_β:
 jmp flat_γ
snoch1_n7_α:
bb18_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_var@PLT
 jmp flat_γ
 snoch1_n7_β:
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
.S1: .string "X"
.S2: .string "aaa"
.S3: .string "V"
.S4: .string "OUTPUT"
.S5: .string "fail"
.text

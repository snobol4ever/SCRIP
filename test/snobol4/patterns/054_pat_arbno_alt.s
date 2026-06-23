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
 jg ialt0_a0_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov rdx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne ialt0_a0_ω
 add r14d, 1
 jmp flat_c0_γ
 ialt0_a0_β:
 sub r14d, 1
 jmp ialt0_a0_ω
ialt0_a0_ω:
# IR_LIT
bb2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg ialt0_a1_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov rdx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne ialt0_a1_ω
 add r14d, 1
 jmp flat_c0_γ
 ialt0_a1_β:
 sub r14d, 1
 jmp ialt0_a1_ω
ialt0_a1_ω:
bb3_α:
# IR_FAIL
 jmp flat_c0_ω
 ialt0_a2_β:
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
snoch4_n0_α:
# IR_LIT_scalar
bb4_α:
 jmp snoch4_n1_α
 snoch4_n0_β:
 jmp snoch4_n2_α
snoch4_n1_α:
bb5_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch4_n2_α
 snoch4_n1_β:
 jmp snoch4_n2_α
snoch4_n2_α:
# IR_VAR
bb6_α:
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch4_n3_α
 snoch4_n2_β:
 jmp snoch4_n4_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "X"
snoch4_n3_α:
# IR_SUBJECT
bb7_α:
 lea rdi, [rip + .S2]
 lea rsi, [r12 + 16]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan9_sγ
 xscan9_sβ:
 jmp snoch4_n4_α
xscan9_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb8_α:
 mov r13, qword ptr [r12 + 16]
 mov r15d, dword ptr [r12 + 24]
 mov dword ptr [r12 + 32], 0
 lea r10, [r12 + 40]
 jmp smatch12_retry
 snoch4_n3_β:
 jmp xscan9_dfail
smatch12_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 32]
# IR_MATCH_POS
bb10_α:
 cmp r14d, 0
 jne xcat15_ω
 jmp xcat15_γ
 xcat15_left_β:
 jmp xcat15_ω
xcat15_γ:
# IR_MATCH_CAPTURE_SAVE
bb11_α:
 mov dword ptr [r12 + 48], r14d
# IR_MATCH_ARBNO
bb12_α:
 mov dword ptr [r12 + 64], r14d
 jmp xcap17_γ
 xcat15_mid1_β:
 mov dword ptr [r12 + 68], r14d
 jmp flat_c0_α
flat_c0_wγ:
 mov eax, dword ptr [r12 + 68]
 cmp r14d, eax
 je .Lx20_0
 jmp xcap17_γ
flat_c0_wω:
.Lx20_0:
 mov r14d, dword ptr [r12 + 64]
 jmp xcat15_left_β
xcap17_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S4]
 mov esi, dword ptr [r12 + 48]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xcat15_mid1_γ
xcat15_mid1_γ:
# IR_MATCH_RPOS
bb14_α:
 mov ecx, r15d
 sub ecx, 0
 cmp r14d, ecx
 jne xcat15_mid1_β
 jmp xscan9_dok
 xcat15_mid2_β:
 jmp xcat15_mid1_β
xcat15_right_ω:
 jmp xcat15_left_β
smatch12_elemb:
 jmp xcat15_mid2_β
xcat15_ω:
 jmp smatch12_adv
smatch12_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 32], 1
 mov eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jg xscan9_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan9_dfail
 jmp smatch12_retry
xscan9_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
jmp snoch4_n5_α
xscan9_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch4_n4_α
snoch4_n4_α:
# IR_LIT_scalar
bb17_α:
 jmp snoch4_n6_α
 snoch4_n4_β:
 jmp flat_γ
snoch4_n5_α:
# IR_VAR
bb18_α:
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 72], rax
 mov qword ptr [r12 + 80], rdx
 jmp snoch4_n7_α
 snoch4_n5_β:
 jmp flat_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "V"
snoch4_n6_α:
bb19_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S5]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch4_n6_β:
 jmp flat_γ
snoch4_n7_α:
bb20_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S5]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_var@PLT
 jmp flat_γ
 snoch4_n7_β:
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
.S2: .string "X"
.S3: .string "abba"
.S4: .string "V"
.S5: .string "OUTPUT"
.S6: .string "no match"
.text

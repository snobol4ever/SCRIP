  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "LINE"
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
flat_c0_α:
#=======================================================================================================================
    .global flat_c0_α
    .global flat_c0_β
    .global flat_c0_γ
    .global flat_c0_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
flat_c0_α_body:
# IR_LIT
bb1_α:
 mov eax, r14d
 add eax, 5
 cmp eax, r15d
 jg xcat0_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov rdx, 5
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xcat0_ω
 add r14d, 5
 jmp xcat0_γ
 xcat0_left_β:
 sub r14d, 5
 jmp xcat0_ω
xcat0_γ:
# IR_MATCH_CAPTURE_SAVE
bb2_α:
 mov dword ptr [r12 + 0], r14d
# IR_MATCH_ARB
bb3_α:
 mov dword ptr [r12 + 16], 0
 mov eax, r14d
 mov dword ptr [r12 + 20], eax
 jmp xcap2_γ
 xcat0_mid1_β:
 add dword ptr [r12 + 16], 1
 mov eax, dword ptr [r12 + 20]
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jg xcat0_left_β
 mov r14d, eax
 jmp xcap2_γ
xcap2_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S1]
 mov esi, dword ptr [r12 + 0]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xcat0_mid1_γ
xcat0_mid1_γ:
# IR_LIT
bb5_α:
 mov eax, r14d
 add eax, 4
 cmp eax, r15d
 jg alt6_c0_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov rdx, 4
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne alt6_c0_ω
 add r14d, 4
 jmp flat_c0_γ
 alt6_c0_β:
 sub r14d, 4
 jmp alt6_c0_ω
alt6_c0_ω:
# IR_LIT
bb6_α:
 mov eax, r14d
 add eax, 3
 cmp eax, r15d
 jg alt6_c1_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S3]
 mov rdx, 3
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne alt6_c1_ω
 add r14d, 3
 jmp flat_c0_γ
 alt6_c1_β:
 sub r14d, 3
 jmp alt6_c1_ω
alt6_c1_ω:
 jmp xcat0_mid1_β
xcat0_mid2_β:
 jmp alt6_c0_β
xcat0_right_ω:
 jmp xcat0_left_β
flat_c0_β:
 jmp xcat0_mid2_β
xcat0_ω:
 jmp flat_c0_ω
flat_c0_γ:
mov eax, 1
xor edx, edx
pop r12
ret
flat_c0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
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
snoch11_n0_α:
# IR_REF_INVARIANT frozen-pattern store
bb9_α:
 lea rdi, [rip + .S4]
 lea rsi, [rip + flat_c0_α]
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_gvar_assign_pat@PLT
 mov rsp, rbx
 pop rbx
 jmp snoch11_n1_α
snoch11_n0_β:
 jmp snoch11_n1_α
snoch11_n1_α:
# IR_VAR
bb10_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch11_n2_α
 snoch11_n1_β:
 jmp flat_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "INPUT"
snoch11_n2_α:
bb11_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S5]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_var@PLT
 jmp snoch11_n3_α
 snoch11_n2_β:
 jmp flat_γ
snoch11_n3_α:
# IR_VAR gva
bb12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch11_n4_α
 snoch11_n3_β:
 jmp snoch11_n1_α
snoch11_n4_α:
# IR_SUBJECT
bb13_α:
 lea rdi, [rip + .S5]
 lea rsi, [r12 + 32]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan18_sγ
 xscan18_sβ:
 jmp snoch11_n1_α
xscan18_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb14_α:
 mov r13, qword ptr [r12 + 32]
 mov r15d, dword ptr [r12 + 40]
 mov dword ptr [r12 + 48], 0
 lea r10, [r12 + 56]
 jmp smatch21_retry
 snoch11_n4_β:
 jmp xscan18_dfail
smatch21_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 48]
# IR_MATCH_DEFER
bb16_α:
 lea rdi, [rip + .S4]
 mov esi, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbx
 pop rbx
 test rax, rax
 jz .Lx24_0
 push rax
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_frame@PLT
 mov rsp, rbx
 pop rbx
 pop rcx
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 jne smatch21_adv
 jmp xscan18_dok
.Lx24_0:
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbx
 pop rbx
 test eax, eax
 js smatch21_adv
 mov r14d, eax
 jmp xscan18_dok
 smatch21_elemb:
 jmp smatch21_adv
smatch21_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 48], 1
 mov eax, dword ptr [r12 + 48]
 cmp eax, r15d
 jg xscan18_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan18_dfail
 jmp smatch21_retry
xscan18_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
jmp snoch11_n1_α
xscan18_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch11_n1_α
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
.S0: .string " the "
.S1: .string "OUTPUT"
.S2: .string " of "
.S3: .string " a "
.S4: .string "PAT"
.S5: .string "LINE"
.S6: .string "INPUT"
.text

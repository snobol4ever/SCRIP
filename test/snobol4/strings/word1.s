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
# IR_VAR
bb1_α:
 mov rdi, qword ptr [rip + .Lx2_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp flat_γ
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "INPUT"
snoch0_n1_α:
bb2_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
# IR_VAR
bb3_α:
 mov rdi, qword ptr [rip + .Lx5_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n0_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "LINE"
snoch0_n3_α:
# IR_SUBJECT
bb4_α:
 lea rdi, [rip + .S0]
 lea rsi, [r12 + 32]
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
 jmp snoch0_n0_α
xscan6_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb5_α:
 mov r13, qword ptr [r12 + 32]
 mov r15d, dword ptr [r12 + 40]
 mov dword ptr [r12 + 48], 0
 lea r10, [r12 + 56]
 jmp smatch9_retry
 snoch0_n3_β:
 jmp xscan6_dfail
smatch9_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 48]
# IR_MATCH_DEFER
bb7_α:
 lea rdi, [rip + .S2]
 mov esi, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbx
 pop rbx
 test rax, rax
 jz .Lx12_0
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
 jne smatch9_adv
 jmp xscan6_dok
.Lx12_0:
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbx
 pop rbx
 test eax, eax
 js smatch9_adv
 mov r14d, eax
 jmp xscan6_dok
 smatch9_elemb:
 jmp smatch9_adv
smatch9_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 48], 1
 mov eax, dword ptr [r12 + 48]
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
jmp snoch0_n0_α
xscan6_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n0_α
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
.S0: .string "LINE"
.S1: .string "INPUT"
.S2: .string "PAT"
.text

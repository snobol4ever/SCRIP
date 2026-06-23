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
bb1_α:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
 snoch0_n0_β:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
snoch0_n1_α:
bb2_α:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
 snoch0_n1_β:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
snoch0_n2_α:
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n4_α
 snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb4_α:
 lea rdi, [rip + .S1]
 call rt_bomb@PLT
 ud2
 snoch0_n3_β:
 lea rdi, [rip + .S1]
 call rt_bomb@PLT
 ud2
snoch0_n4_α:
bb5_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
# IR_VAR
bb6_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 72], rax
 mov qword ptr [r12 + 80], rdx
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp snoch0_n8_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "X"
snoch0_n6_α:
bb7_α:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
 snoch0_n6_β:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
snoch0_n7_α:
# IR_SUBJECT
bb8_α:
 lea rdi, [rip + .S2]
 lea rsi, [r12 + 112]
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
 jmp snoch0_n8_α
xscan9_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb9_α:
 mov r13, qword ptr [r12 + 112]
 mov r15d, dword ptr [r12 + 120]
 mov dword ptr [r12 + 128], 0
 lea r10, [r12 + 136]
 jmp smatch12_retry
 snoch0_n7_β:
 jmp xscan9_dfail
smatch12_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 128]
# IR_MATCH_CAPTURE_SAVE
bb11_α:
 mov dword ptr [r12 + 144], r14d
# IR_MATCH_DEFER
bb12_α:
 lea rdi, [rip + .S5]
 mov esi, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbx
 pop rbx
 test rax, rax
 jz .Lx17_0
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
 jne smatch12_adv
 jmp xcap15_γ
.Lx17_0:
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbx
 pop rbx
 test eax, eax
 js smatch12_adv
 mov r14d, eax
 jmp xcap15_γ
 smatch12_elemb:
 jmp smatch12_adv
xcap15_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S4]
 mov esi, dword ptr [r12 + 144]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xscan9_dok
smatch12_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 128], 1
 mov eax, dword ptr [r12 + 128]
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
jmp snoch0_n10_α
xscan9_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n8_α
snoch0_n8_α:
# IR_LIT_scalar
bb15_α:
 jmp snoch0_n11_α
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
bb16_α:
 lea rdi, [rip + .S1]
 call rt_bomb@PLT
 ud2
 snoch0_n9_β:
 lea rdi, [rip + .S1]
 call rt_bomb@PLT
 ud2
snoch0_n10_α:
# IR_VAR
bb17_α:
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 184], rax
 mov qword ptr [r12 + 192], rdx
 jmp snoch0_n13_α
 snoch0_n10_β:
 jmp flat_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "V"
snoch0_n11_α:
bb18_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S6]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
bb19_α:
 lea rdi, [rip + .S1]
 call rt_bomb@PLT
 ud2
 snoch0_n12_β:
 lea rdi, [rip + .S1]
 call rt_bomb@PLT
 ud2
snoch0_n13_α:
bb20_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S6]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_var@PLT
 jmp flat_γ
 snoch0_n13_β:
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
.S0: .string "bb_pattern_lit: DT_P builder pending (DDS)"
.S1: .string "bb_pattern_alt: DT_P builder pending (DDS)"
.S2: .string "X"
.S3: .string "b"
.S4: .string "V"
.S5: .string "P"
.S6: .string "OUTPUT"
.S7: .string "no match"
.text

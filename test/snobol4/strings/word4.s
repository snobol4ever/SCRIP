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
# IR_MATCH_POS
bb1_α:
 cmp r14d, 0
 jne xcat0_ω
 jmp xcat0_γ
 xcat0_left_β:
 jmp xcat0_ω
xcat0_γ:
# IR_MATCH_CAPTURE_SAVE
bb2_α:
 mov dword ptr [r12 + 0], r14d
# IR_MATCH_BREAK
bb3_α:
 mov dword ptr [r12 + 16], 0
.Lx5_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jge xcat0_left_β
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx5_1
 add dword ptr [r12 + 16], 1
 jmp .Lx5_0
.Lx5_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 16]
 mov r14d, eax
 jmp xcap2_γ
 xcat0_mid1_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 16]
 mov r14d, eax
 jmp xcat0_left_β
xcap2_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S0]
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
 add eax, 1
 cmp eax, r15d
 jg xcat0_mid1_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov rdx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xcat0_mid1_β
 add r14d, 1
 jmp xcat0_mid2_γ
 xcat0_mid2_β:
 sub r14d, 1
 jmp xcat0_mid1_β
xcat0_mid2_γ:
# IR_MATCH_SPAN
bb6_α:
 mov dword ptr [r12 + 20], 0
.Lx9_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 20]
 cmp eax, r15d
 jge .Lx9_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx9_1
 add dword ptr [r12 + 20], 1
 jmp .Lx9_0
.Lx9_1:
 mov eax, dword ptr [r12 + 20]
 test eax, eax
 jle xcat0_mid2_β
 mov edx, r14d
 mov dword ptr [r12 + 24], edx
 add edx, eax
 mov r14d, edx
 jmp xcat0_mid3_γ
 xcat0_mid3_β:
 xcat0_mid3_β:
 mov r14d, dword ptr [r12 + 24]
 jmp xcat0_mid2_β
xcat0_mid3_γ:
# IR_MATCH_CAPTURE_SAVE
bb7_α:
 mov dword ptr [r12 + 28], r14d
# IR_MATCH_BREAKX
bb8_α:
 mov dword ptr [r12 + 44], 0
.Lx13_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 44]
 cmp eax, r15d
 jge xcat0_mid3_β
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx13_1
 add dword ptr [r12 + 44], 1
 jmp .Lx13_0
.Lx13_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 44]
 mov r14d, eax
 jmp xcap10_γ
 xcat0_mid4_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 44]
 mov dword ptr [r12 + 48], eax
 add dword ptr [r12 + 44], 1
.Lx13_2:
 mov eax, dword ptr [r12 + 48]
 add eax, dword ptr [r12 + 44]
 cmp eax, r15d
 jge .Lx13_4
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx13_3
 add dword ptr [r12 + 44], 1
 jmp .Lx13_2
.Lx13_3:
 mov eax, dword ptr [r12 + 48]
 add eax, dword ptr [r12 + 44]
 mov r14d, eax
 jmp xcap10_γ
.Lx13_4:
 mov r14d, dword ptr [r12 + 48]
 jmp xcat0_mid3_β
xcap10_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S2]
 mov esi, dword ptr [r12 + 28]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xcat0_mid4_γ
xcat0_mid4_γ:
# IR_LIT
bb10_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xcat0_mid4_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov rdx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xcat0_mid4_β
 add r14d, 1
 jmp xcat0_mid5_γ
 xcat0_mid5_β:
 sub r14d, 1
 jmp xcat0_mid4_β
xcat0_mid5_γ:
# IR_MATCH_SPAN
bb11_α:
 mov dword ptr [r12 + 52], 0
.Lx17_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 52]
 cmp eax, r15d
 jge .Lx17_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je .Lx17_1
 add dword ptr [r12 + 52], 1
 jmp .Lx17_0
.Lx17_1:
 mov eax, dword ptr [r12 + 52]
 test eax, eax
 jle xcat0_mid5_β
 mov edx, r14d
 mov dword ptr [r12 + 56], edx
 add edx, eax
 mov r14d, edx
 jmp xcat0_mid6_γ
 xcat0_mid6_β:
 xcat0_mid6_β:
 mov r14d, dword ptr [r12 + 56]
 jmp xcat0_mid5_β
xcat0_mid6_γ:
# IR_MATCH_CAPTURE_SAVE
bb12_α:
 mov dword ptr [r12 + 60], r14d
# IR_MATCH_REM
bb13_α:
 mov r14d, r15d
 jmp xcap18_γ
 xcat0_mid7_β:
 jmp xcat0_mid6_β
xcap18_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S4]
 mov esi, dword ptr [r12 + 60]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp flat_c0_γ
xcat0_right_ω:
 jmp xcat0_left_β
flat_c0_β:
 jmp xcat0_mid7_β
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
snoch23_n0_α:
# IR_REF_INVARIANT frozen-pattern store
bb16_α:
 lea rdi, [rip + .S5]
 lea rsi, [rip + flat_c0_α]
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_gvar_assign_pat@PLT
 mov rsp, rbx
 pop rbx
 jmp snoch23_n1_α
snoch23_n0_β:
 jmp snoch23_n1_α
snoch23_n1_α:
# IR_VAR
bb17_α:
 mov rdi, qword ptr [rip + .Lx26_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch23_n2_α
 snoch23_n1_β:
 jmp flat_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "INPUT"
snoch23_n2_α:
bb18_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S6]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_var@PLT
 jmp snoch23_n3_α
 snoch23_n2_β:
 jmp flat_γ
snoch23_n3_α:
# IR_VAR gva
bb19_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch23_n4_α
 snoch23_n3_β:
 jmp snoch23_n1_α
snoch23_n4_α:
# IR_SUBJECT
bb20_α:
 lea rdi, [rip + .S6]
 lea rsi, [r12 + 32]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan30_sγ
 xscan30_sβ:
 jmp snoch23_n1_α
xscan30_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb21_α:
 mov r13, qword ptr [r12 + 32]
 mov r15d, dword ptr [r12 + 40]
 mov dword ptr [r12 + 48], 0
 lea r10, [r12 + 56]
 jmp smatch33_retry
 snoch23_n4_β:
 jmp xscan30_dfail
smatch33_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 48]
# IR_MATCH_DEFER
bb23_α:
 lea rdi, [rip + .S5]
 mov esi, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbx
 pop rbx
 test rax, rax
 jz .Lx36_0
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
 jne smatch33_adv
 jmp xscan30_dok
.Lx36_0:
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbx
 pop rbx
 test eax, eax
 js smatch33_adv
 mov r14d, eax
 jmp xscan30_dok
 smatch33_elemb:
 jmp smatch33_adv
smatch33_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 48], 1
 mov eax, dword ptr [r12 + 48]
 cmp eax, r15d
 jg xscan30_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan30_dfail
 jmp smatch33_retry
xscan30_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
jmp snoch23_n5_α
xscan30_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch23_n1_α
snoch23_n5_α:
bb25_α:
# IR_ASSIGN_CONCAT 5 parts
 mov dword ptr [r12 + 64], 1
 lea rax, [rip + .S2]
 mov qword ptr [r12 + 72], rax
 mov dword ptr [r12 + 80], 0
 lea rax, [rip + .S9]
 mov qword ptr [r12 + 88], rax
 mov dword ptr [r12 + 96], 1
 lea rax, [rip + .S4]
 mov qword ptr [r12 + 104], rax
 mov dword ptr [r12 + 112], 0
 lea rax, [rip + .S10]
 mov qword ptr [r12 + 120], rax
 mov dword ptr [r12 + 128], 1
 lea rax, [rip + .S0]
 mov qword ptr [r12 + 136], rax
 lea rdi, [rip + .S8]
 lea rsi, [r12 + 64]
 mov edx, 5
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_gvar_assign_concat_parts@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp snoch23_n1_α
 snoch23_n5_β:
 jmp snoch23_n1_α
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
.S0: .string "WHEN"
.S1: .string " "
.S2: .string "WHO"
.S3: .string " :"
.S4: .string "WHAT"
.S5: .string "PAT"
.S6: .string "LINE"
.S7: .string "INPUT"
.S8: .string "OUTPUT"
.S9: .string " invented the "
.S10: .string " in "
.text

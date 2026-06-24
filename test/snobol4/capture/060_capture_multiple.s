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
# IR_MATCH_BREAK
bb8_α:
 mov dword ptr [r12 + 64], 0
.Lx15_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 64]
 cmp eax, r15d
 jge xcat11_ω
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx15_1
 add dword ptr [r12 + 64], 1
 jmp .Lx15_0
.Lx15_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 64]
 mov r14d, eax
 jmp xcap12_γ
 xcat11_left_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 64]
 mov r14d, eax
 jmp xcat11_ω
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
 jg xcat11_left_β
 add r14d, 1
 jmp xcat11_mid1_γ
 xcat11_mid1_β:
 jmp xcat11_left_β
xcat11_mid1_γ:
# IR_MATCH_CAPTURE_SAVE
bb11_α:
 mov dword ptr [r12 + 68], r14d
# IR_MATCH_REM
bb12_α:
 mov r14d, r15d
 jmp xcap18_γ
 xcat11_mid2_β:
 jmp xcat11_mid1_β
xcap18_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S4]
 mov esi, dword ptr [r12 + 68]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xscan5_dok
xcat11_right_ω:
 jmp xcat11_left_β
smatch8_elemb:
 jmp xcat11_mid2_β
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
bb16_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
bb17_α:
# IR_ASSIGN_CONCAT 3 parts
 mov dword ptr [r12 + 84], 1
 lea rax, [rip + .S2]
 mov qword ptr [r12 + 92], rax
 mov dword ptr [r12 + 100], 0
 lea rax, [rip + .S6]
 mov qword ptr [r12 + 108], rax
 mov dword ptr [r12 + 116], 1
 lea rax, [rip + .S4]
 mov qword ptr [r12 + 124], rax
 lea rdi, [rip + .S5]
 lea rsi, [r12 + 84]
 mov edx, 3
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_gvar_assign_concat_parts@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp flat_γ
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
bb18_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S5]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
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
.S1: .string "John Smith"
.S2: .string "FIRST"
.S3: .string " "
.S4: .string "LAST"
.S5: .string "OUTPUT"
.S6: .string " / "
.S7: .string "fail"
.text

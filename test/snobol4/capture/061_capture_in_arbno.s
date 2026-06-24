  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "N"
  .Lgvan2: .string "V"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .bss
  .align 16
__gva: .space 48, 0
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
  mov edx, 3
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
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb4_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S2]
 movabs rsi, 0
 call rt_gvar_assign_int@PLT
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
 jmp flat_γ
snoch0_n5_α:
# IR_SUBJECT
bb6_α:
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
 jmp xscan7_sγ
 xscan7_sβ:
 jmp flat_γ
xscan7_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb7_α:
 mov r13, qword ptr [r12 + 16]
 mov r15d, dword ptr [r12 + 24]
 mov dword ptr [r12 + 32], 0
 lea r10, [r12 + 40]
 jmp smatch10_retry
 snoch0_n5_β:
 jmp xscan7_dfail
smatch10_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 32]
# IR_MATCH_POS
bb9_α:
 cmp r14d, 0
 jne xcat13_ω
 jmp xcat13_γ
 xcat13_left_β:
 jmp xcat13_ω
xcat13_γ:
# IR_MATCH_CAPTURE_SAVE
bb10_α:
 mov dword ptr [r12 + 48], r14d
# IR_LIT
bb11_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xcat13_right_ω
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S4]
 mov rdx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xcat13_right_ω
 add r14d, 1
 jmp xcap15_γ
 xcat13_right_β:
 sub r14d, 1
 jmp xcat13_right_ω
xcap15_γ:
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
 jmp xscan7_dok
xcat13_right_ω:
 jmp xcat13_left_β
smatch10_elemb:
 jmp xcat13_right_β
xcat13_ω:
 jmp smatch10_adv
smatch10_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 32], 1
 mov eax, dword ptr [r12 + 32]
 cmp eax, r15d
 jg xscan7_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan7_dfail
 jmp smatch10_retry
xscan7_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n6_α
xscan7_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp flat_γ
snoch0_n6_α:
# IR_VAR gva
bb15_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n8_α
snoch0_n7_α:
bb16_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S5]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
# IR_VAR gva
bb17_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n4_α
snoch0_n9_α:
# IR_LIT_scalar
bb18_α:
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n4_α
snoch0_n10_α:
bb19_α:
# IR_BINOP_GVAR_ARITH
 mov rdx, qword ptr [rbx + 16]
 cmp edx, 6
 jne .Lx28_0
 mov rax, qword ptr [rbx + 24]
 jmp .Lx28_1
.Lx28_0:
 lea rdi, [rip + .S2]
 call rt_gvar_get_int@PLT
.Lx28_1:
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 96], rax
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n4_α
snoch0_n11_α:
bb20_α:
# IR_ASSIGN
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [rbx + 16], 6
 mov qword ptr [rbx + 24], rax
 jmp snoch0_n4_α
 snoch0_n11_β:
 jmp snoch0_n4_α
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
.S1: .string "aaa"
.S2: .string "N"
.S3: .string "V"
.S4: .string "a"
.S5: .string "OUTPUT"
.text

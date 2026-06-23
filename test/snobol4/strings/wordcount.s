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
 lea rdi, [rip + .S4]
 call rt_bomb@PLT
 ud2
snoch0_n5_α:
# IR_VAR
bb6_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n7_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "INPUT"
snoch0_n6_α:
bb7_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S5]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp snoch0_n7_α
snoch0_n7_α:
# IR_VAR
bb8_α:
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp flat_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "N"
snoch0_n8_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp snoch0_n5_α
snoch0_n9_α:
bb10_α:
# IR_UNOP_GVAR_SLOT
 lea rdi, [rip + .S7]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 32], rax
 jmp snoch0_n11_α
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_SUBJECT
bb11_α:
 lea rdi, [rip + .S5]
 lea rsi, [r12 + 40]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan13_sγ
 xscan13_sβ:
 jmp snoch0_n5_α
xscan13_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb12_α:
 mov r13, qword ptr [r12 + 40]
 mov r15d, dword ptr [r12 + 48]
 mov dword ptr [r12 + 56], 0
 lea r10, [r12 + 64]
 jmp smatch16_retry
 snoch0_n10_β:
 jmp xscan13_dfail
smatch16_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 56]
# IR_MATCH_DEFER
bb14_α:
 lea rdi, [rip + .S8]
 mov esi, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbx
 pop rbx
 test rax, rax
 jz .Lx19_0
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
 jne smatch16_adv
 jmp xscan13_dok
.Lx19_0:
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbx
 pop rbx
 test eax, eax
 js smatch16_adv
 mov r14d, eax
 jmp xscan13_dok
 smatch16_elemb:
 jmp smatch16_adv
smatch16_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 56], 1
 mov eax, dword ptr [r12 + 56]
 cmp eax, r15d
 jg xscan13_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan13_dfail
 jmp smatch16_retry
xscan13_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
# SPLICE_EMPTY
 lea rdi, [rip + .S5]
 mov esi, dword ptr [r12 + 56]
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_scan_splice_empty@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n12_α
xscan13_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n5_α
snoch0_n11_α:
# IR_LIT_scalar
bb16_α:
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
# IR_VAR
bb17_α:
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 72], rax
 mov qword ptr [r12 + 80], rdx
 jmp snoch0_n14_α
 snoch0_n12_β:
 jmp snoch0_n8_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "N"
snoch0_n13_α:
# IR_LIT_S
bb18_α:
 mov qword ptr [r12 + 88], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 96], rax
 jmp xgvcat24_1d
 xgvcat24_1b:
 jmp flat_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string " words"
xgvcat24_1d:
bb19_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, 6
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 88]
 mov rcx, qword ptr [r12 + 96]
 call str_concat_d@PLT
 mov qword ptr [r12 + 104], rax
 mov qword ptr [r12 + 112], rdx
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
# IR_LIT_scalar
bb20_α:
 jmp snoch0_n16_α
 snoch0_n14_β:
 jmp snoch0_n8_α
snoch0_n15_α:
bb21_α:
# IR_ASSIGN
 lea rdi, [rip + .S9]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 112]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n15_β:
 jmp flat_γ
snoch0_n16_α:
bb22_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S7]
 call rt_gvar_get_int@PLT
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 120], rax
 jmp snoch0_n17_α
 snoch0_n16_β:
 jmp snoch0_n8_α
snoch0_n17_α:
bb23_α:
# IR_ASSIGN
 lea rdi, [rip + .S7]
 mov rsi, qword ptr [r12 + 120]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n8_α
 snoch0_n17_β:
 jmp snoch0_n8_α
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
.S0: .string "TRIM"
.S1: .string "NUMERALS"
.S2: .string "0123456789"
.S3: .string "WORD"
.S4: .string "bb_gvar_assign_concat: no parts (not flattenable)"
.S5: .string "LINE"
.S6: .string "INPUT"
.S7: .string "N"
.S8: .string "WPAT"
.S9: .string "OUTPUT"
.text

  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "NUMERALS"
  .Lgvan1: .string "WORD"
  .Lgvan2: .string "WPAT"
  .Lgvan3: .string "LINE"
  .Lgvan4: .string "N"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .section .bss
  .align 16
__gva: .space 80, 0
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
  mov edx, 5
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
# IR_LIT_scalar
bb5_α:
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
# IR_VAR gva
bb6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_VAR
bb7_α:
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp snoch0_n9_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "INPUT"
snoch0_n7_α:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvcat10_0d
 xgvcat10_0b:
 jmp snoch0_n6_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "'-"
xgvcat10_0d:
bb9_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, qword ptr [r12 + 32]
 mov rsi, qword ptr [r12 + 40]
 mov rdx, qword ptr [r12 + 0]
 mov rcx, qword ptr [r12 + 8]
 call str_concat_d@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp snoch0_n10_α
 snoch0_n7_β:
 jmp snoch0_n6_α
snoch0_n8_α:
bb10_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S3]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n11_α
 snoch0_n8_β:
 jmp snoch0_n9_α
snoch0_n9_α:
# IR_VAR gva
bb11_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp snoch0_n12_α
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_KEYWORD_read
bb12_α:
 mov rdi, qword ptr [rip + .Lx16_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp snoch0_n13_α
 snoch0_n10_β:
 jmp snoch0_n6_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "UCASE"
snoch0_n11_α:
# IR_LIT_scalar
bb13_α:
 jmp snoch0_n14_α
 snoch0_n11_β:
 jmp snoch0_n6_α
snoch0_n12_α:
bb14_α:
# IR_UNOP_GVAR_SLOT
 lea rdi, [rip + .S5]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 96], rax
 jmp snoch0_n15_α
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
bb15_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, 6
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 80]
 mov rcx, qword ptr [r12 + 88]
 call str_concat_d@PLT
 mov qword ptr [r12 + 104], rax
 mov qword ptr [r12 + 112], rdx
 jmp snoch0_n16_α
 snoch0_n13_β:
 jmp snoch0_n6_α
snoch0_n14_α:
# IR_SUBJECT
bb16_α:
 lea rdi, [rip + .S3]
 lea rsi, [r12 + 120]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan20_sγ
 xscan20_sβ:
 jmp snoch0_n6_α
xscan20_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb17_α:
 mov r13, qword ptr [r12 + 120]
 mov r15d, dword ptr [r12 + 128]
 mov dword ptr [r12 + 136], 0
 lea r10, [r12 + 144]
 jmp smatch23_retry
 snoch0_n14_β:
 jmp xscan20_dfail
smatch23_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 136]
# IR_MATCH_DEFER
bb19_α:
 lea rdi, [rip + .S6]
 mov esi, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbx
 pop rbx
 test rax, rax
 jz .Lx26_0
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
 jne smatch23_adv
 jmp xscan20_dok
.Lx26_0:
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbx
 pop rbx
 test eax, eax
 js smatch23_adv
 mov r14d, eax
 jmp xscan20_dok
 smatch23_elemb:
 jmp smatch23_adv
smatch23_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 136], 1
 mov eax, dword ptr [r12 + 136]
 cmp eax, r15d
 jg xscan20_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan20_dfail
 jmp smatch23_retry
xscan20_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
# SPLICE_EMPTY
 lea rdi, [rip + .S3]
 mov esi, dword ptr [r12 + 136]
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_scan_splice_empty@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n17_α
xscan20_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n6_α
snoch0_n15_α:
# IR_LIT_scalar
bb21_α:
 jmp snoch0_n18_α
 snoch0_n15_β:
 jmp flat_γ
snoch0_n16_α:
# IR_KEYWORD_read
bb22_α:
 mov rdi, qword ptr [rip + .Lx29_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 152], rax
 mov qword ptr [r12 + 160], rdx
 jmp snoch0_n19_α
 snoch0_n16_β:
 jmp snoch0_n6_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "LCASE"
snoch0_n17_α:
# IR_VAR gva
bb23_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 168], rax
 mov qword ptr [r12 + 176], rdx
 jmp snoch0_n20_α
 snoch0_n17_β:
 jmp snoch0_n11_α
snoch0_n18_α:
# IR_LIT_S
bb24_α:
 mov qword ptr [r12 + 184], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 192], rax
 jmp xgvcat32_1d
 xgvcat32_1b:
 jmp flat_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string " words"
xgvcat32_1d:
bb25_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, 6
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 184]
 mov rcx, qword ptr [r12 + 192]
 call str_concat_d@PLT
 mov qword ptr [r12 + 200], rax
 mov qword ptr [r12 + 208], rdx
 jmp snoch0_n21_α
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
bb26_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, 6
 mov rsi, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 152]
 mov rcx, qword ptr [r12 + 160]
 call str_concat_d@PLT
 mov qword ptr [r12 + 216], rax
 mov qword ptr [r12 + 224], rdx
 jmp snoch0_n22_α
 snoch0_n19_β:
 jmp snoch0_n6_α
snoch0_n20_α:
# IR_LIT_scalar
bb27_α:
 jmp snoch0_n23_α
 snoch0_n20_β:
 jmp snoch0_n11_α
snoch0_n21_α:
bb28_α:
# IR_ASSIGN
 lea rdi, [rip + .S7]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 208]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n21_β:
 jmp flat_γ
snoch0_n22_α:
bb29_α:
# IR_ASSIGN
 mov rax, qword ptr [r12 + 216]
 mov rcx, qword ptr [r12 + 224]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rcx
 jmp snoch0_n6_α
 snoch0_n22_β:
 jmp snoch0_n6_α
snoch0_n23_α:
bb30_α:
# IR_BINOP_GVAR_ARITH
 mov rdx, qword ptr [rbx + 64]
 cmp edx, 6
 jne .Lx42_0
 mov rax, qword ptr [rbx + 72]
 jmp .Lx42_1
.Lx42_0:
 lea rdi, [rip + .S5]
 call rt_gvar_get_int@PLT
.Lx42_1:
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 232], rax
 jmp snoch0_n24_α
 snoch0_n23_β:
 jmp snoch0_n11_α
snoch0_n24_α:
bb31_α:
# IR_ASSIGN
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [rbx + 64], 6
 mov qword ptr [rbx + 72], rax
 jmp snoch0_n11_α
 snoch0_n24_β:
 jmp snoch0_n11_α
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
.S3: .string "LINE"
.S4: .string "INPUT"
.S5: .string "N"
.S6: .string "WPAT"
.S7: .string "OUTPUT"
.S8: .string "WORD"
.text

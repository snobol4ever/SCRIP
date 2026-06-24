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
# IR_PATTERN_LEN passthrough (chain entry; builder emitted via DTP_ASSIGN)
bb2_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_VAR
bb3_α:
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp snoch0_n4_α
 snoch0_n2_β:
 jmp flat_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "INPUT"
snoch0_n3_α:
# IR_PATTERN_CAT passthrough (chain entry)
bb4_α:
 jmp snoch0_n5_α
 snoch0_n3_β:
 jmp snoch0_n2_α
snoch0_n4_α:
bb5_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
bb6_α:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
 snoch0_n5_β:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
snoch0_n6_α:
# IR_VAR gva
bb7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp snoch0_n2_α
snoch0_n7_α:
# IR_PATTERN_CAT passthrough (chain entry)
bb8_α:
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp snoch0_n2_α
snoch0_n8_α:
# IR_SUBJECT
bb9_α:
 lea rdi, [rip + .S1]
 lea rsi, [r12 + 152]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan11_sγ
 xscan11_sβ:
 jmp snoch0_n2_α
xscan11_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb10_α:
 mov r13, qword ptr [r12 + 152]
 mov r15d, dword ptr [r12 + 160]
 mov dword ptr [r12 + 168], 0
 lea r10, [r12 + 176]
 jmp smatch14_retry
 snoch0_n8_β:
 jmp xscan11_dfail
smatch14_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 168]
# IR_MATCH_DEFER
bb12_α:
 lea rdi, [rip + .S3]
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
 jne smatch14_adv
 jmp xscan11_dok
.Lx17_0:
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbx
 pop rbx
 test eax, eax
 js smatch14_adv
 mov r14d, eax
 jmp xscan11_dok
 smatch14_elemb:
 jmp smatch14_adv
smatch14_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 168], 1
 mov eax, dword ptr [r12 + 168]
 cmp eax, r15d
 jg xscan11_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan11_dfail
 jmp smatch14_retry
xscan11_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n10_α
xscan11_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n2_α
snoch0_n9_α:
bb14_α:
 lea rdi, [rip + .S4]
 call rt_bomb@PLT
 ud2
 snoch0_n9_β:
 lea rdi, [rip + .S4]
 call rt_bomb@PLT
 ud2
snoch0_n10_α:
bb15_α:
# IR_ASSIGN_CONCAT 5 parts
 mov dword ptr [r12 + 208], 1
 lea rax, [rip + .S6]
 mov qword ptr [r12 + 216], rax
 mov dword ptr [r12 + 224], 0
 lea rax, [rip + .S7]
 mov qword ptr [r12 + 232], rax
 mov dword ptr [r12 + 240], 1
 lea rax, [rip + .S8]
 mov qword ptr [r12 + 248], rax
 mov dword ptr [r12 + 256], 0
 lea rax, [rip + .S9]
 mov qword ptr [r12 + 264], rax
 mov dword ptr [r12 + 272], 1
 lea rax, [rip + .S10]
 mov qword ptr [r12 + 280], rax
 lea rdi, [rip + .S5]
 lea rsi, [r12 + 208]
 mov edx, 5
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_gvar_assign_concat_parts@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp snoch0_n2_α
 snoch0_n10_β:
 jmp snoch0_n2_α
snoch0_n11_α:
# IR_PATTERN_CAT passthrough (chain entry)
bb16_α:
 jmp snoch0_n12_α
 snoch0_n11_β:
 jmp snoch0_n2_α
snoch0_n12_α:
# IR_PATTERN_LIT passthrough (chain entry; matcher built inside CAT blob)
bb17_α:
 jmp snoch0_n13_α
 snoch0_n12_β:
 jmp snoch0_n2_α
snoch0_n13_α:
# IR_PATTERN_CAT passthrough (chain entry)
bb18_α:
 jmp snoch0_n14_α
 snoch0_n13_β:
 jmp snoch0_n2_α
snoch0_n14_α:
bb19_α:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
 snoch0_n14_β:
 lea rdi, [rip + .S0]
 call rt_bomb@PLT
 ud2
snoch0_n15_α:
# IR_PATTERN_CAT passthrough (chain entry)
bb20_α:
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n2_α
snoch0_n16_α:
bb21_α:
 lea rdi, [rip + .S11]
 call rt_bomb@PLT
 ud2
 snoch0_n16_β:
 lea rdi, [rip + .S11]
 call rt_bomb@PLT
 ud2
snoch0_n17_α:
# IR_PATTERN_CAT passthrough (chain entry)
bb22_α:
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n2_α
snoch0_n18_α:
bb23_α:
 lea rdi, [rip + .S12]
 call rt_bomb@PLT
 ud2
 snoch0_n18_β:
 lea rdi, [rip + .S12]
 call rt_bomb@PLT
 ud2
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
.S0: .string "bb_pattern_unary_i: DT_P builder pending (DDS)"
.S1: .string "LINE"
.S2: .string "INPUT"
.S3: .string "PAT"
.S4: .string "bb_pattern_arb: DT_P builder pending (DDS)"
.S5: .string "OUTPUT"
.S6: .string "WHO"
.S7: .string " invented the "
.S8: .string "WHAT"
.S9: .string " in "
.S10: .string "WHEN"
.S11: .string "bb_pattern_nullary: DT_P builder pending (DDS)"
.S12: .string "bb_pattern_cat: non-frozen stored CAT shape (invariant CATs freeze upstream; structural-variance stitch pending)"
.text

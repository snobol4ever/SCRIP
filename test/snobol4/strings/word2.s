  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 528
  mov [rsp+8], rcx
  mov [rsp+16], rdx
  mov [rsp+24], rbp
  lea rbp, [rsp+32]
  mov rdi, rbp
  mov ecx, 496
  xor eax, eax
  rep stosb
  mov qword ptr [rbp + 488], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [rbp + 480], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [rbp + 464], rax
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n0_α:
 jmp xchain0_n1_α
xchain0_n0_as:
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 jmp xchain0_n13_β
xchain0_n0_af:
 jmp proc_PAT$0_ω
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 64], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 72], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n0_af
.Lx3_0:
 .quad 0
# IR_MATCH_POS
 xchain0_n2_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain0_n0_af
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n3_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n14_α
 xchain0_n3_β:
 add rsp, 16
 jmp xchain0_n2_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n4_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S0]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n5_α
 xchain0_n4_β:
 sub r12, 24
 jmp xchain0_n14_β
# IR_LIT_INTEGER
 xchain0_n5_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n4_β
.Lx9_0:
 .quad 6
# IR_MATCH_TAB
 xchain0_n6_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov rax, 6
 cmp r14d, eax
 jle .Lx10_239
 add rsp, 16
 jmp xchain0_n4_β
.Lx10_239:
 cmp r15d, eax
 jge .Lx10_240
 add rsp, 16
 jmp xchain0_n4_β
.Lx10_240:
 mov r14d, eax
 jmp xchain0_n7_α
 xchain0_n6_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 jmp xchain0_n4_β
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n7_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n15_α
 xchain0_n7_β:
 add rsp, 16
 jmp xchain0_n6_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n8_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S1]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n9_α
 xchain0_n8_β:
 sub r12, 24
 jmp xchain0_n15_β
# IR_MATCH_LIT
 xchain0_n9_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n8_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 2
 call memcmp@PLT
 test eax, eax
 jne xchain0_n8_β
 add r14d, 2
 jmp xchain0_n10_α
 xchain0_n9_β:
 sub r14d, 2
 jmp xchain0_n8_β
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rbp + 352], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 360], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n9_β
.Lx17_0:
 .quad 24
# IR_MATCH_TAB
 xchain0_n11_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov rax, 24
 cmp r14d, eax
 jle .Lx18_239
 add rsp, 16
 jmp xchain0_n9_β
.Lx18_239:
 cmp r15d, eax
 jge .Lx18_240
 add rsp, 16
 jmp xchain0_n9_β
.Lx18_240:
 mov r14d, eax
 jmp xchain0_n12_α
 xchain0_n11_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 jmp xchain0_n9_β
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n12_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n16_α
 xchain0_n12_β:
 add rsp, 16
 jmp xchain0_n11_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n13_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S3]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n0_as
 xchain0_n13_β:
 sub r12, 24
 jmp xchain0_n16_β
# IR_MATCH_LEN
 xchain0_n14_α:
 mov eax, r14d
 add eax, 4
 cmp eax, r15d
 jg xchain0_n3_β
 add r14d, 4
 jmp xchain0_n4_α
 xchain0_n14_β:
 sub r14d, 4
 jmp xchain0_n3_β
# IR_MATCH_ARB
 xchain0_n15_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
 mov eax, r14d
 mov dword ptr [rsp + 4], eax
 jmp xchain0_n8_α
 xchain0_n15_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 4]
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx25_0
 mov r14d, eax
 jmp xchain0_n8_α
.Lx25_0:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n7_β
# IR_MATCH_REM
 xchain0_n16_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov r14d, r15d
 jmp xchain0_n13_α
 xchain0_n16_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 jmp xchain0_n12_β
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 464]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp-24]
mov rbp, [rbp-8]
jmp rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [rbp + 480]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_PAT$0_ω:
mov rax, [rbp-16]
lea rsp, [rbp + 496]
mov rbp, [rbp-8]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "PAT$0"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_PAT$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 496
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "PAT"
  .Lgvan1: .string "LINE"
  .Lgvan2: .string "WHO"
  .Lgvan3: .string "WHAT"
  .Lgvan4: .string "WHEN"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 5
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 5
  call gva_register@PLT
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 712], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain27_n0_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain27_n1_α
 xchain27_n0_β:
 jmp xchain27_n3_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "PAT$0"
 xchain27_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lrkfn30: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn30]
 lea rsi, [rbp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain27_n3_α
 jmp xchain27_n2_α
 xchain27_n1_β:
 jmp xchain27_n3_α
# IR_ASSIGN gva
 xchain27_n2_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain27_n3_α
 xchain27_n2_β:
 jmp xchain27_n3_α
# IR_VAR
 xchain27_n3_α:
 mov rdi, qword ptr [rip + .Lx32_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain27_n4_α
 xchain27_n3_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain27_n4_α:
 mov rax, qword ptr [rbp + 208]
 mov rdx, qword ptr [rbp + 216]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 jmp xchain27_n5_α
 xchain27_n4_β:
 jmp main_γ
# IR_VAR
 xchain27_n5_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain27_n6_α
 xchain27_n5_β:
 jmp xchain27_n3_α
# IR_MATCH_HEAD
 xchain27_n6_α:
 mov qword ptr [rbp + 296], rbp
 mov rdi, qword ptr [rbp + 368]
 mov rsi, qword ptr [rbp + 376]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 288], r12
 mov qword ptr [rbp + 272], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 264], rax
 mov dword ptr [rbp + 256], 0
.Lx36_0:
 mov r14d, dword ptr [rbp + 256]
 jmp xchain27_n7_α
 xchain27_n6_β:
 add dword ptr [rbp + 256], 1
 mov eax, dword ptr [rbp + 256]
 cmp eax, r15d
 jg .Lx36_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx36_1
 jmp .Lx36_0
.Lx36_1:
 mov rax, qword ptr [rbp + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 272]
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain27_n3_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain27_n7_α:
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx37_0
 lea rcx, [rip + .Lx37_4]
 lea rdx, [rip + .Lx37_5]
 jmp rax
.Lx37_4:
 jmp xchain27_n8_α
.Lx37_5:
 jmp xchain27_n6_β
.Lx37_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx37_2:
 test rax, rax
 je .Lx37_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx37_7]
 lea rdx, [rip + .Lx37_8]
 jmp rax
.Lx37_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx37_2
.Lx37_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx37_2
.Lx37_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain27_n6_β
 mov r14d, eax
 lea rax, [rip + .Lx37_6]
 sub rsp, 8
 push rax
 jmp xchain27_n8_α
.Lx37_6:
 add rsp, 16
 jmp xchain27_n6_β
 xchain27_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain27_n8_α:
 mov rax, qword ptr [rbp + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 272]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 320]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx39_1:
 test rax, rax
 je .Lx39_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx39_3]
 lea rdx, [rip + .Lx39_4]
 jmp rax
.Lx39_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx39_1
.Lx39_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx39_1
.Lx39_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain27_n9_α
# IR_VAR
 xchain27_n9_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 jmp xchain27_n10_α
 xchain27_n9_β:
 jmp xchain27_n3_α
# IR_LIT_STRING
 xchain27_n10_α:
 mov qword ptr [rbp + 576], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [rbp + 584], rax
 jmp xchain27_n11_α
 xchain27_n10_β:
 jmp xchain27_n3_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string " invented the "
 xchain27_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 544]
 mov rsi, qword ptr [rbp + 552]
 mov rdx, qword ptr [rbp + 576]
 mov rcx, qword ptr [rbp + 584]
 call str_concat_d@PLT
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
 jmp xchain27_n12_α
 xchain27_n11_β:
 jmp xchain27_n3_α
# IR_VAR
 xchain27_n12_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 608], rax
 mov qword ptr [rbp + 616], rdx
 jmp xchain27_n13_α
 xchain27_n12_β:
 jmp xchain27_n3_α
 xchain27_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 512]
 mov rsi, qword ptr [rbp + 520]
 mov rdx, qword ptr [rbp + 608]
 mov rcx, qword ptr [rbp + 616]
 call str_concat_d@PLT
 mov qword ptr [rbp + 480], rax
 mov qword ptr [rbp + 488], rdx
 jmp xchain27_n14_α
 xchain27_n13_β:
 jmp xchain27_n3_α
# IR_LIT_STRING
 xchain27_n14_α:
 mov qword ptr [rbp + 640], 1
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [rbp + 648], rax
 jmp xchain27_n15_α
 xchain27_n14_β:
 jmp xchain27_n3_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string " in "
 xchain27_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 480]
 mov rsi, qword ptr [rbp + 488]
 mov rdx, qword ptr [rbp + 640]
 mov rcx, qword ptr [rbp + 648]
 call str_concat_d@PLT
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 jmp xchain27_n16_α
 xchain27_n15_β:
 jmp xchain27_n3_α
# IR_VAR
 xchain27_n16_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 680], rdx
 jmp xchain27_n17_α
 xchain27_n16_β:
 jmp xchain27_n3_α
 xchain27_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 448]
 mov rsi, qword ptr [rbp + 456]
 mov rdx, qword ptr [rbp + 672]
 mov rcx, qword ptr [rbp + 680]
 call str_concat_d@PLT
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 jmp xchain27_n18_α
 xchain27_n17_β:
 jmp xchain27_n3_α
# IR_ASSIGN global
 xchain27_n18_α:
 mov rsi, qword ptr [rbp + 416]
 mov rdx, qword ptr [rbp + 424]
 mov rdi, qword ptr [rip + .Lx49_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp xchain27_n3_α
 xchain27_n18_β:
 jmp xchain27_n3_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 712]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 712]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "WHEN"
.S1: .string "WHO"
.S2: .string " :"
.S3: .string "WHAT"
.S4: .string "PAT"
.text

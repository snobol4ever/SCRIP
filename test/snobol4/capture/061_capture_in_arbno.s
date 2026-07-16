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
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 3
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 3
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
  mov qword ptr [rsp + 632], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "aaa"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rsp + 160], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad 0
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rsp + 160]
 mov rdx, qword ptr [rsp + 168]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp main_γ
# IR_COERCE_INTEGER
 xchain0_n6_α:
 lea rdi, [rsp + 400]
 lea rsi, [rsp + 368]
 mov rdx, 10682530
 call rt_coerce_int_d@PLT
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp main_γ
# IR_MATCH_HEAD
 xchain0_n7_α:
 mov rdi, qword ptr [rsp + 432]
 mov rsi, qword ptr [rsp + 440]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rsp + 248], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [rsp + 240], rbp
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx10_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n8_α
 xchain0_n7_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx10_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx10_1
 jmp .Lx10_0
.Lx10_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 240]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 248]
 jmp main_γ
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n8_α:
 jmp xchain0_n10_α
xchain0_n8_as:
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n12_β
xchain0_n8_af:
 jmp xchain0_n7_β
# IR_MATCH_RELEASE
 xchain0_n9_α:
 mov rax, qword ptr [rsp + 24]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 32]
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [rsp + 272]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx14_1:
 test rax, rax
 je .Lx14_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx14_3]
 lea rdx, [rip + .Lx14_4]
 jmp rax
.Lx14_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx14_1
.Lx14_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx14_1
.Lx14_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 240]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 248]
 jmp xchain0_n13_α
# IR_MATCH_POS
 xchain0_n10_α:
 mov rax, qword ptr [rsp + 408]
 cmp r14d, eax
 jne xchain0_n8_af
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n8_af
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n11_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n14_α
 xchain0_n11_β:
 add rsp, 16
 jmp xchain0_n10_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n12_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S0]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n8_as
 xchain0_n12_β:
 sub rbp, 24
 jmp xchain0_n14_β
# IR_VAR
 xchain0_n13_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 480], rax
 mov qword ptr [rsp + 488], rdx
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n16_α
# IR_MATCH_LIT
 xchain0_n14_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n11_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n11_β
 add r14d, 1
 jmp xchain0_n12_α
 xchain0_n14_β:
 sub r14d, 1
 jmp xchain0_n11_β
# IR_ASSIGN global
 xchain0_n15_α:
 mov rsi, qword ptr [rsp + 480]
 mov rdx, qword ptr [rsp + 488]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
# IR_VAR
 xchain0_n16_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 560], rax
 mov qword ptr [rsp + 568], rdx
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [rsp + 592], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rsp + 600], rax
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n4_α
.Lx25_0:
 .quad 1
 xchain0_n18_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 560]
 cmp eax, 100
 je .Lx26_0
 mov eax, dword ptr [rsp + 560]
 cmp eax, 6
 jne .Lx26_2
.Lx26_1:
 mov rax, qword ptr [rsp + 568]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 528], 6
 mov qword ptr [rsp + 536], rax
 jmp xchain0_n19_α
.Lx26_0:
 mov rdi, qword ptr [rsp + 560]
 mov rsi, qword ptr [rsp + 568]
 mov rdx, qword ptr [rsp + 592]
 mov rcx, qword ptr [rsp + 600]
 mov r8d, 0
 lea r9, [rsp + 528]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx26_3
.Lx26_2:
 mov rdi, qword ptr [rsp + 560]
 mov rsi, qword ptr [rsp + 568]
 mov rdx, qword ptr [rsp + 592]
 mov rcx, qword ptr [rsp + 600]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [rsp + 528], rax
 mov qword ptr [rsp + 536], rdx
.Lx26_3:
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n4_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [rsp + 528]
 mov rdx, qword ptr [rsp + 536]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 jmp xchain0_n4_α
 xchain0_n19_β:
 jmp xchain0_n4_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 632]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 632]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string "V"
.S1: .string "a"
.text

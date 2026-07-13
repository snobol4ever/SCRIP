  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "A"
  .Lgvan2: .string "B"
  .Lgvan3: .string "C"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 4
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
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
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 824], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 816], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abcdef"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 168], rax
 mov qword ptr [r12 + 176], rsp
 mov rdi, qword ptr [r12 + 432]
 mov rsi, qword ptr [r12 + 440]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 200], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 192], rbp
 mov dword ptr [r12 + 160], 0
.Lx5_0:
 mov r14d, dword ptr [r12 + 160]
 jmp xchain0_n5_α
 xchain0_n3_β:
 add dword ptr [r12 + 160], 1
 mov eax, dword ptr [r12 + 160]
 cmp eax, r15d
 jg .Lx5_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx5_1
 jmp .Lx5_0
.Lx5_1:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 168]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 176]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 200]
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "fail"
# IR_MATCH_SEQ_NARY
 xchain0_n5_α:
 mov dword ptr [r12 + 224], r14d
 mov dword ptr [r12 + 228], 0
 jmp xchain0_n8_α
xchain0_n5_as:
 mov eax, dword ptr [r12 + 228]
 add eax, 1
 mov dword ptr [r12 + 228], eax
 cmp eax, 1
 je xchain0_n10_α
 cmp eax, 2
 je xchain0_n12_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 mov dword ptr [r12 + 228], 3
xchain0_n5_af:
 mov eax, dword ptr [r12 + 228]
 sub eax, 1
 mov dword ptr [r12 + 228], eax
 cmp eax, 0
 je xchain0_n9_β
 cmp eax, 1
 je xchain0_n11_β
 cmp eax, 2
 je xchain0_n13_β
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n7_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 168]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 176]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 push r14
 push r15
 push r13
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [r12 + 192]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx11_1:
 test rax, rax
 je .Lx11_2
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_dcap_step@PLT
 jmp .Lx11_1
.Lx11_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 200]
 jmp xchain0_n14_α
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n8_α:
 lea rdi, [r12 + 272]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n15_α
 xchain0_n8_β:
 lea rdi, [r12 + 272]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n5_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n9_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 272]
 call rt_cap_top@PLT
 mov rsp, [rsp + 8]
 lea rcx, [rip + .S0]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n5_as
 xchain0_n9_β:
 sub rbp, 24
 jmp xchain0_n15_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n10_α:
 lea rdi, [r12 + 336]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n16_α
 xchain0_n10_β:
 lea rdi, [r12 + 336]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n5_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n11_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 336]
 call rt_cap_top@PLT
 mov rsp, [rsp + 8]
 lea rcx, [rip + .S1]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n5_as
 xchain0_n11_β:
 sub rbp, 24
 jmp xchain0_n16_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n12_α:
 lea rdi, [r12 + 400]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n17_α
 xchain0_n12_β:
 lea rdi, [r12 + 400]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n5_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n13_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 400]
 call rt_cap_top@PLT
 mov rsp, [rsp + 8]
 lea rcx, [rip + .S2]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n5_as
 xchain0_n13_β:
 sub rbp, 24
 jmp xchain0_n17_β
# IR_VAR
 xchain0_n14_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp main_γ
# IR_MATCH_LEN
 xchain0_n15_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n8_β
 add r14d, 2
 jmp xchain0_n9_α
 xchain0_n15_β:
 sub r14d, 2
 jmp xchain0_n8_β
# IR_MATCH_LEN
 xchain0_n16_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n10_β
 add r14d, 2
 jmp xchain0_n11_α
 xchain0_n16_β:
 sub r14d, 2
 jmp xchain0_n10_β
# IR_MATCH_LEN
 xchain0_n17_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n12_β
 add r14d, 2
 jmp xchain0_n13_α
 xchain0_n17_β:
 sub r14d, 2
 jmp xchain0_n12_β
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string " "
 xchain0_n19_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 656]
 mov rsi, qword ptr [r12 + 664]
 mov rdx, qword ptr [r12 + 688]
 mov rcx, qword ptr [r12 + 696]
 call str_concat_d@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp main_γ
# IR_VAR
 xchain0_n20_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp main_γ
 xchain0_n21_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 mov rdx, qword ptr [r12 + 720]
 mov rcx, qword ptr [r12 + 728]
 call str_concat_d@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string " "
 xchain0_n23_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 592]
 mov rsi, qword ptr [r12 + 600]
 mov rdx, qword ptr [r12 + 752]
 mov rcx, qword ptr [r12 + 760]
 call str_concat_d@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp main_γ
# IR_VAR
 xchain0_n24_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp main_γ
 xchain0_n25_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 560]
 mov rsi, qword ptr [r12 + 568]
 mov rdx, qword ptr [r12 + 784]
 mov rcx, qword ptr [r12 + 792]
 call str_concat_d@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n26_α:
 mov rsi, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov rdi, qword ptr [rip + .Lx36_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 824]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 816]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 824]
pop r12
ret
.section .rodata
.S0: .string "A"
.S1: .string "B"
.S2: .string "C"
.text

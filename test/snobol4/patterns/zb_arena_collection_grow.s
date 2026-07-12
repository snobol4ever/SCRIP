  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "s"
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
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  mov rsp, rbp
  pop rbp
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
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 592], rax
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
 .string "abcdefghij"
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
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 168], rax
 mov qword ptr [r12 + 176], rsp
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
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
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 168]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 176]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "fail"
# IR_MATCH_SEQ_NARY
 xchain0_n5_α:
 mov dword ptr [r12 + 208], r14d
 mov dword ptr [r12 + 212], 0
 jmp xchain0_n8_α
xchain0_n5_as:
 mov eax, dword ptr [r12 + 212]
 add eax, 1
 mov dword ptr [r12 + 212], eax
 cmp eax, 1
 je xchain0_n10_α
 cmp eax, 2
 je xchain0_n11_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 mov dword ptr [r12 + 212], 3
xchain0_n5_af:
 mov eax, dword ptr [r12 + 212]
 sub eax, 1
 mov dword ptr [r12 + 212], eax
 cmp eax, 0
 je xchain0_n9_β
 cmp eax, 1
 je xchain0_n10_β
 cmp eax, 2
 je xchain0_n12_β
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n7_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 168]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 176]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx11_1:
 test rax, rax
 je .Lx11_2
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx11_1
.Lx11_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n13_α
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n5_af
.Lx12_0:
 .quad 0
# IR_MATCH_POS
 xchain0_n9_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain0_n5_af
 jmp xchain0_n5_as
 xchain0_n9_β:
 jmp xchain0_n5_af
# IR_MATCH_ARBNO2 gen
 xchain0_n10_α:
 mov dword ptr [r12 + 288], r14d
 mov dword ptr [r12 + 292], r14d
 mov dword ptr [r12 + 296], 0
 jmp xchain0_n5_as
 xchain0_n10_β:
 mov r14d, dword ptr [r12 + 292]
 lea rdi, [r12 + 304]
 lea rsi, [r12 + 300]
 mov edx, dword ptr [r12 + 296]
 mov rcx, 80
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zcol_push@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [rax + 0], r12
 mov ecx, r14d
 mov qword ptr [rax + 8], rcx
 lea r12, [rax + -336]
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n5_af
.Lx16_0:
 .quad 0
# IR_MATCH_RPOS
 xchain0_n12_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain0_n5_af
 jmp xchain0_n5_as
 xchain0_n12_β:
 jmp xchain0_n5_af
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "grow ok"
# IR_MATCH_ALT_NARY
 xchain0_n14_α:
 mov dword ptr [r12 + 368], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 372], eax
 mov dword ptr [r12 + 376], 0
 jmp xchain0_n18_α
xchain0_n14_as:
 jmp xchain0_n16_α
 xchain0_n14_β:
 mov eax, dword ptr [r12 + 376]
 cmp eax, 0
 je xchain0_n18_β
 jmp xchain0_n19_β
xchain0_n14_af:
 add dword ptr [r12 + 376], 1
 mov r14d, dword ptr [r12 + 368]
 mov edi, dword ptr [r12 + 372]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 376]
 cmp eax, 1
 je xchain0_n19_α
 jmp xchain0_n17_β
# IR_ASSIGN global
 xchain0_n15_α:
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "OUTPUT"
# IR_MATCH_ARBNO2 ok
 xchain0_n16_α:
 mov eax, dword ptr [r12 + 344]
 mov rdx, qword ptr [r12 + 336]
 cmp r14d, eax
 je .Lx23_0
 mov r12, rdx
 mov eax, dword ptr [r12 + 296]
 add eax, 1
 mov dword ptr [r12 + 296], eax
 mov dword ptr [r12 + 292], r14d
 jmp xchain0_n5_as
.Lx23_0:
 mov r12, rdx
 jmp xchain0_n17_α
# IR_MATCH_ARBNO2 pop/exhaust
 xchain0_n17_α:
 jmp .Lx25_1
 xchain0_n17_β:
 mov rdx, qword ptr [r12 + 336]
 mov r12, rdx
 mov eax, dword ptr [r12 + 296]
 test eax, eax
 jz .Lx25_2
 sub eax, 1
 mov dword ptr [r12 + 296], eax
.Lx25_1:
 mov rax, qword ptr [r12 + 304]
 mov ecx, dword ptr [r12 + 296]
 mov rdx, 80
 imul rcx, rdx
 add rax, rcx
 lea r12, [rax + -336]
 jmp xchain0_n14_β
.Lx25_2:
 mov r14d, dword ptr [r12 + 288]
 jmp xchain0_n5_af
# IR_MATCH_LEN
 xchain0_n18_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n14_af
 add r14d, 1
 jmp xchain0_n14_as
 xchain0_n18_β:
 sub r14d, 1
 jmp xchain0_n14_af
# IR_MATCH_LEN
 xchain0_n19_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n14_af
 add r14d, 1
 jmp xchain0_n14_as
 xchain0_n19_β:
 sub r14d, 1
 jmp xchain0_n14_af
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 592]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret

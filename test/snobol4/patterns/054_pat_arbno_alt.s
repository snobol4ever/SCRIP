  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "V"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
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
  mov edx, 2
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
 mov qword ptr [r12 + 640], rax
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
 .string "abba"
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
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 168], rax
 mov qword ptr [r12 + 176], rsp
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
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
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "no match"
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
 je xchain0_n12_α
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
 je xchain0_n11_β
 cmp eax, 2
 je xchain0_n13_β
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
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
 jmp xchain0_n14_α
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
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n10_α:
 lea rdi, [r12 + 304]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n15_α
 xchain0_n10_β:
 lea rdi, [r12 + 304]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n5_af
# IR_MATCH_CAPTURE_COND
 xchain0_n11_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 304]
 call rt_cap_top@PLT
 lea rdi, [rip + .S0]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_open@PLT
 test rax, rax
 je .Lx17_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_cap_finish@PLT
.Lx17_1:
 mov rsp, rbp
 pop rbp
 jmp xchain0_n5_as
 xchain0_n11_β:
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n15_β
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [r12 + 480], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n5_af
.Lx18_0:
 .quad 0
# IR_MATCH_RPOS
 xchain0_n13_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain0_n5_af
 jmp xchain0_n5_as
 xchain0_n13_β:
 jmp xchain0_n5_af
# IR_VAR
 xchain0_n14_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
# IR_MATCH_ARBNO2 gen
 xchain0_n15_α:
 mov dword ptr [r12 + 336], r14d
 mov dword ptr [r12 + 340], r14d
 mov dword ptr [r12 + 344], 0
 jmp xchain0_n11_α
 xchain0_n15_β:
 mov r14d, dword ptr [r12 + 340]
 lea rdi, [r12 + 352]
 lea rsi, [r12 + 348]
 mov edx, dword ptr [r12 + 344]
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
 lea r12, [rax + -384]
 jmp xchain0_n17_α
# IR_ASSIGN global
 xchain0_n16_α:
 mov rsi, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
# IR_MATCH_ALT_NARY
 xchain0_n17_α:
 mov dword ptr [r12 + 416], r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_height@PLT
 mov rsp, rbp
 pop rbp
 mov dword ptr [r12 + 420], eax
 mov dword ptr [r12 + 424], 0
 jmp xchain0_n20_α
xchain0_n17_as:
 jmp xchain0_n18_α
 xchain0_n17_β:
 mov eax, dword ptr [r12 + 424]
 cmp eax, 0
 je xchain0_n20_β
 jmp xchain0_n21_β
xchain0_n17_af:
 add dword ptr [r12 + 424], 1
 mov r14d, dword ptr [r12 + 416]
 mov edi, dword ptr [r12 + 420]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_restore_to@PLT
 mov rsp, rbp
 pop rbp
 mov eax, dword ptr [r12 + 424]
 cmp eax, 1
 je xchain0_n21_α
 jmp xchain0_n19_β
# IR_MATCH_ARBNO2 ok
 xchain0_n18_α:
 mov eax, dword ptr [r12 + 392]
 mov rdx, qword ptr [r12 + 384]
 cmp r14d, eax
 je .Lx27_0
 mov r12, rdx
 mov eax, dword ptr [r12 + 344]
 add eax, 1
 mov dword ptr [r12 + 344], eax
 mov dword ptr [r12 + 340], r14d
 jmp xchain0_n11_α
.Lx27_0:
 mov r12, rdx
 jmp xchain0_n19_α
# IR_MATCH_ARBNO2 pop/exhaust
 xchain0_n19_α:
 jmp .Lx29_1
 xchain0_n19_β:
 mov rdx, qword ptr [r12 + 384]
 mov r12, rdx
 mov eax, dword ptr [r12 + 344]
 test eax, eax
 jz .Lx29_2
 sub eax, 1
 mov dword ptr [r12 + 344], eax
.Lx29_1:
 mov rax, qword ptr [r12 + 352]
 mov ecx, dword ptr [r12 + 344]
 mov rdx, 80
 imul rcx, rdx
 add rax, rcx
 lea r12, [rax + -384]
 jmp xchain0_n17_β
.Lx29_2:
 mov r14d, dword ptr [r12 + 336]
 jmp xchain0_n10_β
# IR_MATCH_LIT
 xchain0_n20_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n17_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n17_af
 add r14d, 1
 jmp xchain0_n17_as
 xchain0_n20_β:
 sub r14d, 1
 jmp xchain0_n17_af
# IR_MATCH_LIT
 xchain0_n21_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n17_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n17_af
 add r14d, 1
 jmp xchain0_n17_as
 xchain0_n21_β:
 sub r14d, 1
 jmp xchain0_n17_af
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
 mov rdi, qword ptr [r12 + 640]
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
.section .rodata
.S0: .string "V"
.S1: .string "a"
.S2: .string "b"
.text

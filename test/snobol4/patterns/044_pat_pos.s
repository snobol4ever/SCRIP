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
 mov qword ptr [r12 + 464], rax
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
 .string "hello"
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
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 168], rax
 mov qword ptr [r12 + 176], rsp
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
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
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 392], rax
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
 jmp xchain0_n7_α
 xchain0_n5_β:
 mov dword ptr [r12 + 212], 2
xchain0_n5_af:
 mov eax, dword ptr [r12 + 212]
 sub eax, 1
 mov dword ptr [r12 + 212], eax
 cmp eax, 0
 je xchain0_n9_β
 cmp eax, 1
 je xchain0_n11_β
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
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
 jmp xchain0_n12_α
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
 jmp xchain0_n13_α
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
 jmp xchain0_n13_β
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp main_γ
# IR_MATCH_LEN
 xchain0_n13_α:
 mov eax, r14d
 add eax, 3
 cmp eax, r15d
 jg xchain0_n10_β
 add r14d, 3
 jmp xchain0_n11_α
 xchain0_n13_β:
 sub r14d, 3
 jmp xchain0_n10_β
# IR_ASSIGN global
 xchain0_n14_α:
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
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
 mov rdi, qword ptr [r12 + 464]
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
.text

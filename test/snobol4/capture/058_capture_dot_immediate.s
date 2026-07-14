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
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
  call gva_register@PLT
  mov rbx, rax
  sub rsp, 65536
  mov rdi, rsp
  mov ecx, 8192
  xor eax, eax
  rep stosq
  mov rdi, rsp
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 65536
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
  mov qword ptr [r12 + 408], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 400], rax
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
 .string "hello world"
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
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 sub rsp, 32
 call rt_zls_mark@PLT
 mov qword ptr [rsp + 8], rax
 mov rax, rsp
 add rax, 32
 mov qword ptr [rsp + 16], rax
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 200], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 192], rbp
 mov dword ptr [rsp + 0], 0
.Lx5_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n5_α
 xchain0_n3_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx5_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx5_1
 jmp .Lx5_0
.Lx5_1:
 mov rdi, qword ptr [rsp + 8]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [rsp + 16]
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
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "fail"
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n5_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n7_α
 xchain0_n5_β:
 add rsp, 16
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 320]
 mov rdx, qword ptr [r12 + 328]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_MATCH_LEN
 xchain0_n7_α:
 mov eax, r14d
 add eax, 5
 cmp eax, r15d
 jg xchain0_n5_β
 add r14d, 5
 jmp xchain0_n8_α
 xchain0_n7_β:
 sub r14d, 5
 jmp xchain0_n5_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n8_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S0]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n9_α
 xchain0_n8_β:
 sub rbp, 24
 jmp xchain0_n7_β
# IR_MATCH_RELEASE
 xchain0_n9_α:
 mov rdi, qword ptr [rsp + 24]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [rsp + 32]
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
.Lx14_1:
 test rax, rax
 je .Lx14_2
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
 jmp .Lx14_1
.Lx14_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 200]
 jmp xchain0_n10_α
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov rdi, qword ptr [rip + .Lx16_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 408]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 400]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 408]
pop r12
ret
.section .rodata
.S0: .string "V"
.text

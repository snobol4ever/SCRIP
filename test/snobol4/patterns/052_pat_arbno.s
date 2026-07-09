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
 mov qword ptr [r12 + 240], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "aaa"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 56], rax
 mov qword ptr [r12 + 64], rsp
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 48], 0
.Lx5_0:
 mov r14d, dword ptr [r12 + 48]
 jmp xchain0_n5_α
 xchain0_n3_β:
 add dword ptr [r12 + 48], 1
 mov eax, dword ptr [r12 + 48]
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
 mov rdi, qword ptr [r12 + 56]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 64]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "fail"
# IR_LIT_INTEGER
 xchain0_n5_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n3_α
.Lx7_0:
 .quad 0
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "OUTPUT"
# IR_MATCH_POS
 xchain0_n7_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain0_n3_β
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n3_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n8_α:
 lea rdi, [r12 + 96]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n9_α
 xchain0_n8_β:
 lea rdi, [r12 + 96]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n3_β
# IR_MATCH_ARBNO gen
 xchain0_n9_α:
 sub rsp, 32
 mov rcx, qword ptr [r12 + 128]
 mov qword ptr [rsp + 0], rcx
 mov qword ptr [r12 + 128], rsp
 mov dword ptr [r12 + 112], r14d
 mov dword ptr [r12 + 116], r14d
 jmp xchain0_n10_α
 xchain0_n9_β:
 mov r14d, dword ptr [r12 + 116]
 mov dword ptr [r12 + 120], r14d
 jmp xchain0_n11_α
# IR_MATCH_CAPTURE_COND
 xchain0_n10_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 96]
 call rt_cap_top@PLT
 lea rdi, [rip + .S0]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n9_β
# IR_MATCH_LIT
 xchain0_n11_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n14_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n14_β
 add r14d, 1
 jmp xchain0_n13_α
 xchain0_n11_β:
 sub r14d, 1
 jmp xchain0_n14_β
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n3_α
.Lx18_0:
 .quad 0
# IR_MATCH_ARBNO ok
 xchain0_n13_α:
 mov eax, dword ptr [r12 + 120]
 cmp r14d, eax
 je .Lx20_0
 mov dword ptr [r12 + 116], r14d
 jmp xchain0_n10_α
.Lx20_0:
 jmp xchain0_n14_α
# IR_MATCH_ARBNO exhaust
 xchain0_n14_α:
 xchain0_n14_β:
 mov r14d, dword ptr [r12 + 112]
.Lx22_9:
 mov rax, qword ptr [r12 + 128]
 mov rcx, qword ptr [rax + 0]
 mov qword ptr [r12 + 128], rcx
 lea rsp, [rax + 32]
 jmp xchain0_n8_β
# IR_MATCH_RPOS
 xchain0_n15_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain0_n10_β
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n10_β
# IR_MATCH_RELEASE
 xchain0_n16_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 56]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 64]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n17_α
# IR_VAR
 xchain0_n17_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n18_α:
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp main_γ
 xchain0_n18_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
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
 mov rdi, qword ptr [r12 + 240]
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
.text

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
  .section .bss
  .align 16
__gva: .space 48, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 3
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 256], rax
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
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad 0
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_VAR gva
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_MATCH_HEAD
 xchain0_n5_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 88], rax
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [rdi + 0]
 mov qword ptr [r12 + 96], rax
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 80], 0
.Lx7_0:
 mov r14d, dword ptr [r12 + 80]
 jmp xchain0_n6_α
 xchain0_n5_β:
 add dword ptr [r12 + 80], 1
 mov eax, dword ptr [r12 + 80]
 cmp eax, r15d
 jg .Lx7_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx7_1
 jmp .Lx7_0
.Lx7_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 88]
 call rt_zls_release_to@PLT
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [rdi + 0], rax
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp main_γ
# IR_VAR gva
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_MATCH_POS
 xchain0_n7_α:
 mov rax, qword ptr [r12 + 136]
 cmp r14d, eax
 jne xchain0_n5_β
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n5_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n8_α:
 lea rdi, [r12 + 112]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n9_α
 xchain0_n8_β:
 lea rdi, [r12 + 112]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n5_β
# IR_MATCH_LIT
 xchain0_n9_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n8_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n8_β
 add r14d, 1
 jmp xchain0_n10_α
 xchain0_n9_β:
 sub r14d, 1
 jmp xchain0_n8_β
# IR_MATCH_CAPTURE_COND
 xchain0_n10_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 112]
 call rt_cap_top@PLT
 lea rdi, [rip + .S1]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n8_β
# IR_MATCH_RELEASE
 xchain0_n11_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 88]
 call rt_zls_release_to@PLT
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [rdi + 0], rax
 call rt_dcap_end_ok@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n12_α
# IR_VAR gva
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "OUTPUT"
# IR_VAR gva
 xchain0_n14_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n4_α
.Lx21_0:
 .quad 1
 xchain0_n16_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 224]
 cmp eax, 100
 je .Lx22_0
 mov eax, dword ptr [r12 + 240]
 cmp eax, 100
 je .Lx22_0
 mov eax, dword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx22_2
 mov eax, dword ptr [r12 + 240]
 cmp eax, 6
 jne .Lx22_2
.Lx22_1:
 mov rax, qword ptr [r12 + 232]
 mov rcx, qword ptr [r12 + 248]
 add rax, rcx
 mov qword ptr [r12 + 208], 6
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n17_α
.Lx22_0:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 0
 lea r9, [r12 + 208]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx22_3
.Lx22_2:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
.Lx22_3:
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n4_α
# IR_ASSIGN gva
 xchain0_n17_α:
 mov rax, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n4_α
 xchain0_n17_β:
 jmp xchain0_n4_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 256]
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
pop r12
ret
.section .rodata
.S0: .string "a"
.S1: .string "V"
.text

  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "N"
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
 mov qword ptr [r12 + 272], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad 0
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 1
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n2_α
 xchain0_n5_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 128]
 cmp eax, 100
 je .Lx6_0
 mov eax, dword ptr [r12 + 128]
 cmp eax, 6
 jne .Lx6_2
.Lx6_1:
 mov rax, qword ptr [r12 + 136]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 112], 6
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n7_α
.Lx6_0:
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 mov r8d, 0
 lea r9, [r12 + 112]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx6_3
.Lx6_2:
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
.Lx6_3:
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n2_α
.Lx7_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n7_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n4_α
 xchain0_n7_β:
 jmp xchain0_n4_α
# IR_COERCE_NUMERIC
 xchain0_n8_α:
 mov eax, dword ptr [r12 + 224]
 cmp eax, 7
 je .Lx10_1
 cmp eax, 6
 jne .Lx10_0
 mov eax, dword ptr [r12 + 208]
 cmp eax, 6
 jne .Lx10_0
.Lx10_1:
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 200], rax
 jmp .Lx10_2
.Lx10_0:
 lea rdi, [r12 + 224]
 lea rsi, [r12 + 208]
 lea rdx, [r12 + 192]
 mov rcx, 111
 call rt_coerce_num2_d@PLT
.Lx10_2:
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n2_α
# IR_COERCE_NUMERIC
 xchain0_n9_α:
 mov eax, dword ptr [r12 + 208]
 cmp eax, 7
 je .Lx12_1
 cmp eax, 6
 jne .Lx12_0
 mov eax, dword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx12_0
.Lx12_1:
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 184], rax
 jmp .Lx12_2
.Lx12_0:
 lea rdi, [r12 + 208]
 lea rsi, [r12 + 224]
 lea rdx, [r12 + 176]
 mov rcx, 112
 call rt_coerce_num2_d@PLT
.Lx12_2:
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n2_α
# IR_CMP_TEST
 xchain0_n10_α:
 lea rdi, [r12 + 192]
 lea rsi, [r12 + 176]
 call rt_cmp_d@PLT
 test eax, eax
 jle xchain0_n2_α
 mov qword ptr [r12 + 160], 0
 mov qword ptr [r12 + 168], 0
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n12_α:
 mov rsi, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 mov rdi, qword ptr [rip + .Lx16_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp main_γ
 xchain0_n12_β:
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
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 272]
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

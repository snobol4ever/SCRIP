  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "N"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
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
  mov qword ptr [rsp + 472], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rbp + 112], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 120], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad 0
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rbp + 224], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 232], rax
 jmp xchain0_n5_α
.Lx4_0:
 .quad 1
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
 jmp xchain0_n6_α
 xchain0_n5_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 192]
 cmp eax, 100
 je .Lx6_0
 mov eax, dword ptr [rbp + 192]
 cmp eax, 6
 jne .Lx6_2
.Lx6_1:
 mov rax, qword ptr [rbp + 200]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rbp + 160], 6
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n7_α
.Lx6_0:
 mov rdi, qword ptr [rbp + 192]
 mov rsi, qword ptr [rbp + 200]
 mov rdx, qword ptr [rbp + 224]
 mov rcx, qword ptr [rbp + 232]
 mov r8d, 0
 lea r9, [rbp + 160]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain0_n7_α
.Lx6_2:
 mov rdi, qword ptr [rbp + 192]
 mov rsi, qword ptr [rbp + 200]
 mov rdx, qword ptr [rbp + 224]
 mov rcx, qword ptr [rbp + 232]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [rbp + 160], rax
 mov qword ptr [rbp + 168], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rbp + 352], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rbp + 360], rax
 jmp xchain0_n8_α
.Lx7_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n7_α:
 mov rax, qword ptr [rbp + 160]
 mov rdx, qword ptr [rbp + 168]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain0_n4_α
# IR_COERCE_NUMERIC
 xchain0_n8_α:
 mov eax, dword ptr [rbp + 384]
 cmp eax, 7
 je .Lx10_1
 cmp eax, 6
 jne .Lx10_0
 mov eax, dword ptr [rbp + 352]
 cmp eax, 6
 jne .Lx10_0
.Lx10_1:
 mov rax, qword ptr [rbp + 384]
 mov qword ptr [rbp + 320], rax
 mov rax, qword ptr [rbp + 392]
 mov qword ptr [rbp + 328], rax
 jmp xchain0_n9_α
.Lx10_0:
 lea rdi, [rbp + 384]
 lea rsi, [rbp + 352]
 lea rdx, [rbp + 320]
 mov rcx, 111
 call rt_coerce_num2_d@PLT
 jmp xchain0_n9_α
# IR_COERCE_NUMERIC
 xchain0_n9_α:
 mov eax, dword ptr [rbp + 352]
 cmp eax, 7
 je .Lx12_1
 cmp eax, 6
 jne .Lx12_0
 mov eax, dword ptr [rbp + 384]
 cmp eax, 6
 jne .Lx12_0
.Lx12_1:
 mov rax, qword ptr [rbp + 352]
 mov qword ptr [rbp + 288], rax
 mov rax, qword ptr [rbp + 360]
 mov qword ptr [rbp + 296], rax
 jmp xchain0_n10_α
.Lx12_0:
 lea rdi, [rbp + 352]
 lea rsi, [rbp + 384]
 lea rdx, [rbp + 288]
 mov rcx, 112
 call rt_coerce_num2_d@PLT
 jmp xchain0_n10_α
# IR_CMP_TEST
 xchain0_n10_α:
 lea rdi, [rbp + 320]
 lea rsi, [rbp + 288]
 call rt_cmp_d@PLT
 test eax, eax
 jle xchain0_n2_α
 mov qword ptr [rbp + 256], 0
 mov qword ptr [rbp + 264], 0
 jmp xchain0_n11_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain0_n12_α
# IR_ASSIGN global
 xchain0_n12_α:
 mov rsi, qword ptr [rbp + 432]
 mov rdx, qword ptr [rbp + 440]
 mov rdi, qword ptr [rip + .Lx16_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
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
mov rsp, qword ptr [rbp + 472]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 472]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

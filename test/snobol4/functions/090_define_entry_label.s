  .intel_syntax noprefix
  .text
  .globl proc_bumpit_α
proc_bumpit_α:
#=======================================================================================================================
    .global proc_bumpit_α
    .global proc_bumpit_β
    .global proc_bumpit_γ
    .global proc_bumpit_ω
push r12
  mov r12, rdi
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 144], rax
 pop rsi
proc_bumpit_α_body:
# IR_VAR gva
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_bumpit_γ
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_bumpit_γ
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 48]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 64]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 48]
 cmp eax, 6
 jne .Lx3_2
 mov eax, dword ptr [r12 + 64]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [r12 + 56]
 mov rcx, qword ptr [r12 + 72]
 add rax, rcx
 mov qword ptr [r12 + 32], 6
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 mov r8d, 0
 lea r9, [r12 + 32]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_bumpit_γ
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_bumpit_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_bumpit_γ
 xchain0_n3_β:
 jmp proc_bumpit_γ
proc_bumpit_β:
jmp proc_bumpit_ω
proc_bumpit_γ:
mov eax, 1
xor edx, edx
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 144]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_bumpit_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "bumpit"
  .Lstartup_pp0_0: .string "v"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_bumpit_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "bumpit"
  .Lgvan1: .string "v"
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
  call proc_startup
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 144], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain5_n0_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp main_γ
.Lx6_0:
 .quad 41
 xchain5_n1_α:
  .section .rodata
  .Lcall6_pname: .string "bumpit"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall6_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je main_γ
 jmp xchain5_n2_α
xchain5_n1_β:
 jmp main_γ
# IR_ASSIGN global
 xchain5_n2_α:
 mov rsi, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp main_γ
 xchain5_n2_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "OUTPUT"
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
 mov rdi, qword ptr [r12 + 144]
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

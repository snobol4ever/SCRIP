  .intel_syntax noprefix
  .text
  .globl proc_add_α
proc_add_α:
#=======================================================================================================================
    .global proc_add_α
    .global proc_add_β
    .global proc_add_γ
    .global proc_add_ω
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
proc_add_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_add_γ
# IR_VAR
 xchain0_n1_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_add_γ
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
 je proc_add_γ
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_add_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_add_γ
 xchain0_n3_β:
 jmp proc_add_γ
proc_add_β:
jmp proc_add_ω
proc_add_γ:
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
proc_add_ω:
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
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "add"
  .Lstartup_pp0_0: .string "a"
  .Lstartup_pp0_1: .string "b"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad .Lstartup_pp0_1
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 2
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_add_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 2
  call rt_proc_set_nparams@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "add"
  .Lgvan1: .string "a"
  .Lgvan2: .string "b"
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
  call core_lib_init@PLT
  call proc_startup
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
 xchain5_n0_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp xchain5_n4_α
.Lx6_0:
 .quad 3
# IR_LIT_INTEGER
 xchain5_n1_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain5_n2_α
 xchain5_n1_β:
 jmp xchain5_n4_α
.Lx7_0:
 .quad 4
 xchain5_n2_α:
  .section .rodata
  .Lcall7_pname: .string "add"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 call rt_arg_stage@PLT
 mov edi, 1
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall7_pname]
 mov esi, 2
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain5_n4_α
 jmp xchain5_n3_α
xchain5_n2_β:
 jmp xchain5_n4_α
# IR_ASSIGN global
 xchain5_n3_α:
 mov rsi, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain5_n4_α
 xchain5_n3_β:
 jmp xchain5_n4_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain5_n4_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain5_n5_α
 xchain5_n4_β:
 jmp main_γ
.Lx10_0:
 .quad 10
# IR_LIT_INTEGER
 xchain5_n5_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain5_n6_α
 xchain5_n5_β:
 jmp main_γ
.Lx11_0:
 .quad 32
 xchain5_n6_α:
  .section .rodata
  .Lcall11_pname: .string "add"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 call rt_arg_stage@PLT
 mov edi, 1
 mov rsi, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall11_pname]
 mov esi, 2
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je main_γ
 jmp xchain5_n7_α
xchain5_n6_β:
 jmp main_γ
# IR_ASSIGN global
 xchain5_n7_α:
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain5_n7_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
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

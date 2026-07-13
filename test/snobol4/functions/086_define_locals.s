  .intel_syntax noprefix
  .text
  .globl proc_swap_α
proc_swap_α:
#=======================================================================================================================
    .global proc_swap_α
    .global proc_swap_β
    .global proc_swap_γ
    .global proc_swap_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 552], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 544], rax
 pop rsi
proc_swap_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp proc_swap_γ
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp proc_swap_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string " "
 xchain0_n8_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 320]
 mov rsi, qword ptr [r12 + 328]
 mov rdx, qword ptr [r12 + 352]
 mov rcx, qword ptr [r12 + 360]
 call str_concat_d@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp proc_swap_γ
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp proc_swap_γ
 xchain0_n10_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 384]
 mov rcx, qword ptr [r12 + 392]
 call str_concat_d@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp proc_swap_γ
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 mov rdi, qword ptr [rip + .Lx12_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp proc_swap_γ
 xchain0_n11_β:
 jmp proc_swap_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "OUTPUT"
proc_swap_β:
jmp proc_swap_ω
proc_swap_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 552]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 544]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_swap_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 552]
pop r12
ret
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "swap"
  .Lstartup_pp0_0: .string "a"
  .Lstartup_pp0_1: .string "b"
  .Lstartup_pp0_2: .string "tmp"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad .Lstartup_pp0_1
  .quad .Lstartup_pp0_2
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 3
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_swap_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 3
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 560
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "swap"
  .Lgvan1: .string "a"
  .Lgvan2: .string "b"
  .Lgvan3: .string "tmp"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 4
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
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
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 552], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 544], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain13_n0_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain13_n1_α
 xchain13_n0_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain13_n1_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain13_n2_α
 xchain13_n1_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "world"
 xchain13_n2_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov edi, 0
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 call rt_arg_stage@PLT
 mov edi, 1
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx17_0]
 mov esi, 2
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx17_1
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
 call rt_proc_call_epilogue@PLT
 jmp .Lx17_2
.Lx17_1:
 call rt_faildescr@PLT
.Lx17_2:
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je main_γ
 jmp main_γ
 xchain13_n2_β:
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "swap"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 552]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 544]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 552]
pop r12
ret

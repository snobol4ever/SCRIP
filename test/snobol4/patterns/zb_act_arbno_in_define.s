  .intel_syntax noprefix
  .text
  .globl proc_MATCHIT_α
proc_MATCHIT_α:
#=======================================================================================================================
    .global proc_MATCHIT_α
    .global proc_MATCHIT_β
    .global proc_MATCHIT_γ
    .global proc_MATCHIT_ω
push r12
  mov r12, rdi
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 272], rax
 pop rsi
proc_MATCHIT_α_body:
# IR_VAR gva
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_MATCH_HEAD
 xchain0_n1_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 24], rax
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [rdi + 0]
 mov qword ptr [r12 + 32], rax
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 16], 0
.Lx3_0:
 mov r14d, dword ptr [r12 + 16]
 jmp xchain0_n3_α
 xchain0_n1_β:
 add dword ptr [r12 + 16], 1
 mov eax, dword ptr [r12 + 16]
 cmp eax, r15d
 jg .Lx3_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx3_1
 jmp .Lx3_0
.Lx3_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 24]
 call rt_zls_release_to@PLT
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [rdi + 0], rax
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp proc_MATCHIT_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "fail"
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n1_α
.Lx5_0:
 .quad 0
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp proc_MATCHIT_γ
 xchain0_n4_β:
 jmp proc_MATCHIT_γ
# IR_MATCH_POS
 xchain0_n5_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain0_n1_β
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n1_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n6_α:
 lea rdi, [r12 + 64]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n7_α
 xchain0_n6_β:
 lea rdi, [r12 + 64]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n1_β
# IR_MATCH_ARBNO gen
 xchain0_n7_α:
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [rdi + 0]
 sub rax, 32
 mov qword ptr [rdi + 0], rax
 mov rcx, qword ptr [r12 + 96]
 mov qword ptr [rax + 0], rcx
 mov qword ptr [r12 + 96], rax
 mov dword ptr [r12 + 80], r14d
 mov dword ptr [r12 + 84], r14d
 jmp xchain0_n8_α
 xchain0_n7_β:
 mov r14d, dword ptr [r12 + 84]
 mov dword ptr [r12 + 88], r14d
 jmp xchain0_n9_α
# IR_MATCH_CAPTURE_COND
 xchain0_n8_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 64]
 call rt_cap_top@PLT
 lea rdi, [rip + .S0]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n7_β
# IR_MATCH_LIT
 xchain0_n9_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n12_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n12_β
 add r14d, 1
 jmp xchain0_n11_α
 xchain0_n9_β:
 sub r14d, 1
 jmp xchain0_n12_β
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 48], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 56], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n1_α
.Lx16_0:
 .quad 0
# IR_MATCH_ARBNO ok
 xchain0_n11_α:
 mov eax, dword ptr [r12 + 88]
 cmp r14d, eax
 je .Lx18_0
 mov dword ptr [r12 + 84], r14d
 jmp xchain0_n8_α
.Lx18_0:
 jmp xchain0_n12_α
# IR_MATCH_ARBNO exhaust
 xchain0_n12_α:
 xchain0_n12_β:
 mov r14d, dword ptr [r12 + 80]
.Lx20_9:
 mov rax, qword ptr [r12 + 96]
 mov rcx, qword ptr [rax + 0]
 mov qword ptr [r12 + 96], rcx
 add rax, 32
 lea rdi, [rip + g_zls2_cur]
 mov qword ptr [rdi + 0], rax
 jmp xchain0_n6_β
# IR_MATCH_RPOS
 xchain0_n13_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain0_n8_β
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n8_β
# IR_MATCH_RELEASE
 xchain0_n14_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 24]
 call rt_zls_release_to@PLT
 lea rdi, [rip + g_zls2_cur]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [rdi + 0], rax
 call rt_dcap_end_ok@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n15_α
# IR_VAR gva
 xchain0_n15_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp proc_MATCHIT_γ
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp proc_MATCHIT_γ
 xchain0_n16_β:
 jmp proc_MATCHIT_γ
proc_MATCHIT_β:
jmp proc_MATCHIT_ω
proc_MATCHIT_γ:
mov eax, 1
xor edx, edx
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 272]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_MATCHIT_ω:
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
  .Lstartup_pname0: .string "MATCHIT"
  .Lstartup_pp0_0: .string "S"
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
  lea rsi, [rip + proc_MATCHIT_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "MATCHIT"
  .Lgvan1: .string "S"
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
# IR_LIT_STRING
 xchain26_n0_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain26_n1_α
 xchain26_n0_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "aaa"
 xchain26_n1_α:
  .section .rodata
  .Lcall19_pname: .string "MATCHIT"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall19_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je main_γ
 jmp xchain26_n2_α
xchain26_n1_β:
 jmp main_γ
# IR_ASSIGN global
 xchain26_n2_α:
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp main_γ
 xchain26_n2_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
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
pop r12
ret
.section .rodata
.S0: .string "V"
.S1: .string "a"
.text

  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "V"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
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
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "aXb"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 mov qword ptr [rsp + 200], rbp
 mov rdi, qword ptr [rsp + 336]
 mov rsi, qword ptr [rsp + 344]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rsp + 192], r12
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
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
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 mov r12, qword ptr [rsp + 192]
 mov rbp, qword ptr [rsp + 200]
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 384], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 392], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "fail"
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n5_α:
 jmp xchain0_n8_α
xchain0_n5_as:
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n10_β
xchain0_n5_af:
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [rsp + 384]
 mov rdx, qword ptr [rsp + 392]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n7_α:
 mov rax, qword ptr [rsp + 40]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 48]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 224]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx11_1:
 test rax, rax
 je .Lx11_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx11_3]
 lea rdx, [rip + .Lx11_4]
 jmp rax
.Lx11_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx11_1
.Lx11_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx11_1
.Lx11_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rsp + 192]
 mov rbp, qword ptr [rsp + 200]
 jmp xchain0_n11_α
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n8_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n12_α
 xchain0_n8_β:
 add rsp, 16
 jmp xchain0_n5_af
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n9_α:
 mov eax, dword ptr [rsp + 16]
 mov rdi, r12
 lea rcx, [rip + .S0]
 mov qword ptr [rdi + 0], rcx
 mov esi, eax
 mov qword ptr [rdi + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rdi + 16], rdx
 add r12, 24
 jmp xchain0_n10_α
 xchain0_n9_β:
 sub r12, 24
 jmp xchain0_n12_β
# IR_MATCH_LEN
 xchain0_n10_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n9_β
 add r14d, 1
 jmp xchain0_n5_as
 xchain0_n10_β:
 sub r14d, 1
 jmp xchain0_n9_β
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
# IR_MATCH_ARB
 xchain0_n12_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
 mov eax, r14d
 mov dword ptr [rsp + 4], eax
 jmp xchain0_n9_α
 xchain0_n12_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 4]
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx19_0
 mov r14d, eax
 jmp xchain0_n9_α
.Lx19_0:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n8_β
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [rsp + 432]
 mov rdx, qword ptr [rsp + 440]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 jmp main_γ
 xchain0_n13_β:
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
mov rsp, qword ptr [rsp + 472]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 472]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string "V"
.text

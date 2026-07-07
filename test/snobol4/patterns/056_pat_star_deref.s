  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "PAT"
  .Lgvan1: .string "X"
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
main_α_body:
xchain0_n0_α:
# IR_LIT_STRING
bb1_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
xchain0_n1_α:
# IR_ASSIGN gva
bb2_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
xchain0_n2_α:
# IR_LIT_STRING
bb3_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "say hello world"
xchain0_n3_α:
# IR_ASSIGN gva
bb4_α:
 mov rax, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
xchain0_n4_α:
# IR_VAR gva
bb5_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
xchain0_n5_α:
# IR_MATCH_HEAD
bb6_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 88], rax
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 80], 0
.Lx7_0:
 mov r14d, dword ptr [r12 + 80]
 jmp xchain0_n7_α
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
 mov rsp, rbp
 pop rbp
 jmp xchain0_n6_α
xchain0_n6_α:
# IR_LIT_STRING
bb7_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "fail"
xchain0_n7_α:
# IR_MATCH_CAPTURE_SAVE push
bb8_α:
 lea rdi, [r12 + 96]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n9_α
 xchain0_n7_β:
 lea rdi, [r12 + 96]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n5_β
xchain0_n8_α:
# IR_ASSIGN global
bb9_α:
 mov rsi, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov rdi, qword ptr [rip + .Lx11_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp main_γ
 xchain0_n8_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "OUTPUT"
xchain0_n9_α:
# IR_MATCH_DEFER
bb10_α:
 lea rdi, [rip + .S0]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 test rax, rax
 jz .Lx12_0
 push rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 pop rcx
 push rax
 mov rdi, rax
 xor esi, esi
 call rcx
 pop rdi
 cmp eax, 1
 je .Lx12_1
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n7_β
.Lx12_1:
 jmp xchain0_n10_α
.Lx12_0:
 lea rdi, [rip + .S0]
 xor esi, esi
 mov edx, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_match@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain0_n7_β
 mov r14d, eax
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n7_β
xchain0_n10_α:
# IR_MATCH_CAPTURE_COND
bb11_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 96]
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
 jmp xchain0_n7_β
xchain0_n11_α:
# IR_MATCH_RELEASE
bb12_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 88]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n12_α
xchain0_n12_α:
# IR_VAR gva
bb13_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp main_γ
xchain0_n13_α:
# IR_ASSIGN global
bb14_α:
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov rdi, qword ptr [rip + .Lx18_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
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
.S0: .string "PAT"
.S1: .string "V"
.text

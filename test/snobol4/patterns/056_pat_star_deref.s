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
  call core_lib_init@PLT
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
 mov qword ptr [r12 + 256], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 80], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
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
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "say hello world"
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_MATCH_HEAD
 xchain0_n5_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 136], rax
 mov qword ptr [r12 + 144], rsp
 mov rdi, qword ptr [r12 + 176]
 mov rsi, qword ptr [r12 + 184]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 128], 0
.Lx7_0:
 mov r14d, dword ptr [r12 + 128]
 jmp xchain0_n7_α
 xchain0_n5_β:
 add dword ptr [r12 + 128], 1
 mov eax, dword ptr [r12 + 128]
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
 mov rdi, qword ptr [r12 + 136]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 144]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n6_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "fail"
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n7_α:
 lea rdi, [r12 + 160]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n9_α
 xchain0_n7_β:
 lea rdi, [r12 + 160]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n5_β
# IR_ASSIGN global
 xchain0_n8_α:
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov rdi, qword ptr [rip + .Lx11_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp main_γ
 xchain0_n8_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "OUTPUT"
# IR_MATCH_DEFER
 xchain0_n9_α:
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
# IR_MATCH_CAPTURE_COND
 xchain0_n10_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 160]
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
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n7_β
# IR_MATCH_RELEASE
 xchain0_n11_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 136]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 144]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n12_α
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov rdi, qword ptr [rip + .Lx18_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
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
mov rsp, rbp
pop rbp
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
mov rsp, rbp
pop rbp
pop r12
ret
.section .rodata
.S0: .string "PAT"
.S1: .string "V"
.text

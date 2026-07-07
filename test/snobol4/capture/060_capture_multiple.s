  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "LAST"
  .Lgvan2: .string "FIRST"
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
 .string "John Smith"
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
# IR_VAR gva
bb3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
xchain0_n3_α:
# IR_MATCH_HEAD
bb4_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 56], rax
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 48], 0
.Lx5_0:
 mov r14d, dword ptr [r12 + 48]
 jmp xchain0_n5_α
 xchain0_n3_β:
 add dword ptr [r12 + 48], 1
 mov eax, dword ptr [r12 + 48]
 cmp eax, r15d
 jg .Lx5_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx5_1
 jmp .Lx5_0
.Lx5_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 56]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n4_α
xchain0_n4_α:
# IR_LIT_STRING
bb5_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "fail"
xchain0_n5_α:
# IR_MATCH_CAPTURE_SAVE push
bb6_α:
 lea rdi, [r12 + 96]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n7_α
 xchain0_n5_β:
 lea rdi, [r12 + 96]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n3_β
xchain0_n6_α:
# IR_ASSIGN global
bb7_α:
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
xchain0_n7_α:
# IR_MATCH_BREAK
bb8_α:
 mov dword ptr [r12 + 112], 0
.Lx11_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 112]
 cmp eax, r15d
 jge xchain0_n5_β
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx11_1
 add dword ptr [r12 + 112], 1
 jmp .Lx11_0
.Lx11_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 112]
 mov r14d, eax
 jmp xchain0_n8_α
 xchain0_n7_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 112]
 mov r14d, eax
 jmp xchain0_n5_β
xchain0_n8_α:
# IR_MATCH_CAPTURE_COND
bb9_α:
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
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n5_β
xchain0_n9_α:
# IR_MATCH_LEN
bb10_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n8_β
 add r14d, 1
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n8_β
xchain0_n10_α:
# IR_MATCH_CAPTURE_SAVE push
bb11_α:
 lea rdi, [r12 + 64]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n11_α
 xchain0_n10_β:
 lea rdi, [r12 + 64]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n3_β
xchain0_n11_α:
# IR_MATCH_REM
bb12_α:
 mov dword ptr [r12 + 80], r14d
 mov r14d, r15d
 jmp xchain0_n12_α
 xchain0_n11_β:
 mov r14d, dword ptr [r12 + 80]
 jmp xchain0_n10_β
xchain0_n12_α:
# IR_MATCH_CAPTURE_COND
bb13_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 64]
 call rt_cap_top@PLT
 lea rdi, [rip + .S2]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n10_β
xchain0_n13_α:
# IR_MATCH_RELEASE
bb14_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 56]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n14_α
xchain0_n14_α:
# IR_VAR gva
bb15_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
xchain0_n15_α:
# IR_LIT_STRING
bb16_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string " / "
xchain0_n16_α:
bb17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call str_concat_d@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
xchain0_n17_α:
# IR_VAR gva
bb18_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp main_γ
xchain0_n18_α:
bb19_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call str_concat_d@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_γ
xchain0_n19_α:
# IR_ASSIGN global
bb20_α:
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
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
.S0: .string " "
.S1: .string "FIRST"
.S2: .string "LAST"
.text

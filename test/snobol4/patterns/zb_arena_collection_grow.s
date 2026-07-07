  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "s"
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
 .string "abcdefghij"
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
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
xchain0_n3_α:
# IR_MATCH_HEAD
bb4_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 56], rax
 mov rdi, qword ptr [r12 + 176]
 mov rsi, qword ptr [r12 + 184]
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
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "fail"
xchain0_n5_α:
# IR_LIT_INTEGER
bb6_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n3_α
.Lx7_0:
 .quad 0
xchain0_n6_α:
# IR_ASSIGN global
bb7_α:
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "OUTPUT"
xchain0_n7_α:
# IR_MATCH_POS
bb8_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain0_n3_β
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n3_β
xchain0_n8_α:
# IR_MATCH_ARBNO2 gen
bb9_α:
 mov dword ptr [r12 + 80], r14d
 mov dword ptr [r12 + 84], r14d
 mov dword ptr [r12 + 88], 0
 jmp xchain0_n9_α
 xchain0_n8_β:
 mov r14d, dword ptr [r12 + 84]
 lea rdi, [r12 + 96]
 lea rsi, [r12 + 92]
 mov edx, dword ptr [r12 + 88]
 mov rcx, 64
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zcol_push@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [rax + 0], r12
 mov ecx, r14d
 mov qword ptr [rax + 8], rcx
 lea r12, [rax + -96]
 jmp xchain0_n10_α
xchain0_n9_α:
# IR_LIT_INTEGER
bb10_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n3_α
.Lx12_0:
 .quad 0
xchain0_n10_α:
# IR_MATCH_ALT_SAVE
bb11_α:
 mov dword ptr [r12 + 112], r14d
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp qword ptr [r12 + 120]
xchain0_n11_α:
# IR_MATCH_RPOS
bb12_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain0_n8_β
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n8_β
xchain0_n12_α:
# IR_MATCH_LEN
bb13_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n14_β
 add r14d, 1
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n14_β
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
 jmp xchain0_n15_α
xchain0_n14_α:
# IR_MATCH_ALT_JOIN
bb15_α:
 lea rax, [rip + .Lx20_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n17_α
 xchain0_n14_β:
.Lx20_0:
 mov r14d, dword ptr [r12 + 112]
 jmp xchain0_n16_α
xchain0_n15_α:
# IR_LIT_STRING
bb16_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "grow ok"
xchain0_n16_α:
# IR_MATCH_LEN
bb17_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n19_β
 add r14d, 1
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n19_β
xchain0_n17_α:
# IR_MATCH_ARBNO2 ok
bb18_α:
 mov eax, dword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 96]
 cmp r14d, eax
 je .Lx24_0
 mov r12, rdx
 mov eax, dword ptr [r12 + 88]
 add eax, 1
 mov dword ptr [r12 + 88], eax
 mov dword ptr [r12 + 84], r14d
 jmp xchain0_n9_α
.Lx24_0:
 mov r12, rdx
 jmp xchain0_n20_α
xchain0_n18_α:
# IR_ASSIGN global
bb19_α:
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov rdi, qword ptr [rip + .Lx25_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp main_γ
 xchain0_n18_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "OUTPUT"
xchain0_n19_α:
# IR_MATCH_ALT_JOIN
bb20_α:
 lea rax, [rip + .Lx27_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n17_α
 xchain0_n19_β:
.Lx27_0:
 mov r14d, dword ptr [r12 + 112]
 jmp xchain0_n20_β
xchain0_n20_α:
# IR_MATCH_ARBNO2 pop/exhaust
bb21_α:
 jmp .Lx29_1
 xchain0_n20_β:
 mov rdx, qword ptr [r12 + 96]
 mov r12, rdx
 mov eax, dword ptr [r12 + 88]
 test eax, eax
 jz .Lx29_2
 sub eax, 1
 mov dword ptr [r12 + 88], eax
.Lx29_1:
 mov rax, qword ptr [r12 + 96]
 mov ecx, dword ptr [r12 + 88]
 mov rdx, 64
 imul rcx, rdx
 add rax, rcx
 lea r12, [rax + -96]
 jmp xchain0_n10_β
.Lx29_2:
 mov r14d, dword ptr [r12 + 80]
 jmp xchain0_n3_β
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

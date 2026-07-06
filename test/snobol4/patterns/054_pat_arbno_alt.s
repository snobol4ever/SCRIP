  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "V"
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
 .string "abba"
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
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n20_α
xchain0_n3_α:
# IR_MATCH_HEAD
bb4_α:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 48], 0
.Lx5_0:
 mov r14d, dword ptr [r12 + 48]
 jmp xchain0_n4_α
 xchain0_n3_β:
 add dword ptr [r12 + 48], 1
 mov eax, dword ptr [r12 + 48]
 cmp eax, r15d
 jg xchain0_n20_α
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xchain0_n20_α
 jmp .Lx5_0
xchain0_n4_α:
# IR_LIT_INTEGER
bb5_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n3_α
.Lx6_0:
 .quad 0
xchain0_n5_α:
# IR_MATCH_POS
bb6_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain0_n3_β
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n3_β
xchain0_n6_α:
# IR_MATCH_CAPTURE_SAVE push
bb7_α:
 lea rdi, [r12 + 80]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n7_α
 xchain0_n6_β:
 lea rdi, [r12 + 80]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n3_β
xchain0_n7_α:
# IR_MATCH_ARBNO2 gen
bb8_α:
 mov dword ptr [r12 + 96], r14d
 mov dword ptr [r12 + 100], r14d
 mov dword ptr [r12 + 104], 0
 jmp xchain0_n8_α
 xchain0_n7_β:
 mov r14d, dword ptr [r12 + 100]
 lea rdi, [r12 + 112]
 lea rsi, [r12 + 108]
 mov edx, dword ptr [r12 + 104]
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
 lea r12, [rax + -112]
 jmp xchain0_n9_α
xchain0_n8_α:
# IR_MATCH_CAPTURE_COND
bb9_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 80]
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
xchain0_n9_α:
# IR_MATCH_ALT_SAVE
bb10_α:
 mov dword ptr [r12 + 128], r14d
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp qword ptr [r12 + 136]
xchain0_n10_α:
# IR_LIT_INTEGER
bb11_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n3_α
.Lx16_0:
 .quad 0
xchain0_n11_α:
# IR_MATCH_LIT
bb12_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n13_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n13_β
 add r14d, 1
 jmp xchain0_n13_α
 xchain0_n11_β:
 sub r14d, 1
 jmp xchain0_n13_β
xchain0_n12_α:
# IR_MATCH_RPOS
bb13_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain0_n8_β
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n8_β
xchain0_n13_α:
# IR_MATCH_ALT_JOIN
bb14_α:
 lea rax, [rip + .Lx21_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
.Lx21_0:
 mov r14d, dword ptr [r12 + 128]
 jmp xchain0_n15_α
xchain0_n14_α:
# IR_VAR gva
bb15_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp main_γ
xchain0_n15_α:
# IR_MATCH_LIT
bb16_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n18_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n18_β
 add r14d, 1
 jmp xchain0_n18_α
 xchain0_n15_β:
 sub r14d, 1
 jmp xchain0_n18_β
xchain0_n16_α:
# IR_MATCH_ARBNO2 ok
bb17_α:
 mov eax, dword ptr [r12 + 120]
 mov rdx, qword ptr [r12 + 112]
 cmp r14d, eax
 je .Lx26_0
 mov r12, rdx
 mov eax, dword ptr [r12 + 104]
 add eax, 1
 mov dword ptr [r12 + 104], eax
 mov dword ptr [r12 + 100], r14d
 jmp xchain0_n8_α
.Lx26_0:
 mov r12, rdx
 jmp xchain0_n19_α
xchain0_n17_α:
# IR_ASSIGN global
bb18_α:
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "OUTPUT"
xchain0_n18_α:
# IR_MATCH_ALT_JOIN
bb19_α:
 lea rax, [rip + .Lx29_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n16_α
 xchain0_n18_β:
.Lx29_0:
 mov r14d, dword ptr [r12 + 128]
 jmp xchain0_n19_β
xchain0_n19_α:
# IR_MATCH_ARBNO2 pop/exhaust
bb20_α:
 jmp .Lx31_1
 xchain0_n19_β:
 mov rdx, qword ptr [r12 + 112]
 mov r12, rdx
 mov eax, dword ptr [r12 + 104]
 test eax, eax
 jz .Lx31_2
 sub eax, 1
 mov dword ptr [r12 + 104], eax
.Lx31_1:
 mov rax, qword ptr [r12 + 112]
 mov ecx, dword ptr [r12 + 104]
 mov rdx, 64
 imul rcx, rdx
 add rax, rcx
 lea r12, [rax + -112]
 jmp xchain0_n9_β
.Lx31_2:
 mov r14d, dword ptr [r12 + 96]
 jmp xchain0_n6_β
xchain0_n20_α:
# IR_LIT_STRING
bb21_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "no match"
xchain0_n21_α:
# IR_ASSIGN global
bb22_α:
 mov rsi, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp main_γ
 xchain0_n21_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
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
.S0: .string "V"
.S1: .string "a"
.S2: .string "b"
.text

  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "C"
  .Lgvan2: .string "B"
  .Lgvan3: .string "A"
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
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
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
 .string "abcdef"
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
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n23_α
xchain0_n3_α:
# IR_MATCH_HEAD
bb4_α:
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
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
 jg xchain0_n23_α
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xchain0_n23_α
 jmp .Lx5_0
xchain0_n4_α:
# IR_MATCH_CAPTURE_SAVE push
bb5_α:
 lea rdi, [r12 + 96]
 mov esi, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n5_α
 xchain0_n4_β:
 lea rdi, [r12 + 96]
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n3_β
xchain0_n5_α:
# IR_MATCH_LEN
bb6_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n4_β
 add r14d, 2
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n4_β
xchain0_n6_α:
# IR_MATCH_CAPTURE_COND
bb7_α:
 push rbx
 mov rbx, rsp
 and rsp, -16
 lea rdi, [r12 + 96]
 call rt_cap_top@PLT
 lea rdi, [rip + .S0]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n4_β
xchain0_n7_α:
# IR_MATCH_CAPTURE_SAVE push
bb8_α:
 lea rdi, [r12 + 80]
 mov esi, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n8_α
 xchain0_n7_β:
 lea rdi, [r12 + 80]
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n6_β
xchain0_n8_α:
# IR_MATCH_LEN
bb9_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n7_β
 add r14d, 2
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n7_β
xchain0_n9_α:
# IR_MATCH_CAPTURE_COND
bb10_α:
 push rbx
 mov rbx, rsp
 and rsp, -16
 lea rdi, [r12 + 80]
 call rt_cap_top@PLT
 lea rdi, [rip + .S1]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n7_β
xchain0_n10_α:
# IR_MATCH_CAPTURE_SAVE push
bb11_α:
 lea rdi, [r12 + 64]
 mov esi, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n11_α
 xchain0_n10_β:
 lea rdi, [r12 + 64]
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n9_β
xchain0_n11_α:
# IR_MATCH_LEN
bb12_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n10_β
 add r14d, 2
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n10_β
xchain0_n12_α:
# IR_MATCH_CAPTURE_COND
bb13_α:
 push rbx
 mov rbx, rsp
 and rsp, -16
 lea rdi, [r12 + 64]
 call rt_cap_top@PLT
 lea rdi, [rip + .S2]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n10_β
xchain0_n13_α:
# IR_VAR gva
bb14_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp main_γ
xchain0_n14_α:
# IR_LIT_STRING
bb15_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string " "
xchain0_n15_α:
bb16_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call str_concat_d@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_γ
xchain0_n16_α:
# IR_VAR gva
bb17_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
xchain0_n17_α:
bb18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 call str_concat_d@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp main_γ
xchain0_n18_α:
# IR_LIT_STRING
bb19_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string " "
xchain0_n19_α:
bb20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 call str_concat_d@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp main_γ
xchain0_n20_α:
# IR_VAR gva
bb21_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp main_γ
xchain0_n21_α:
bb22_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 call str_concat_d@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp main_γ
xchain0_n22_α:
# IR_ASSIGN global
bb23_α:
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp main_γ
 xchain0_n22_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "OUTPUT"
xchain0_n23_α:
# IR_LIT_STRING
bb24_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp main_γ
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "fail"
xchain0_n24_α:
# IR_ASSIGN global
bb25_α:
 mov rsi, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov rdi, qword ptr [rip + .Lx32_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
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
.S0: .string "A"
.S1: .string "B"
.S2: .string "C"
.text

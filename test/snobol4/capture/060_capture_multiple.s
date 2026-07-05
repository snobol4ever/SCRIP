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
 jmp xchain0_n17_α
xchain0_n3_α:
# IR_MATCH_HEAD
bb4_α:
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
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
 jg xchain0_n17_α
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xchain0_n17_α
 jmp .Lx5_0
xchain0_n4_α:
# IR_MATCH_CAPTURE_SAVE
bb5_α:
 mov dword ptr [r12 + 112], r14d
 jmp xchain0_n5_α
xchain0_n5_α:
# IR_MATCH_BREAK
bb6_α:
 mov dword ptr [r12 + 96], 0
.Lx8_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 96]
 cmp eax, r15d
 jge xchain0_n3_β
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx8_1
 add dword ptr [r12 + 96], 1
 jmp .Lx8_0
.Lx8_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 96]
 mov r14d, eax
 jmp xchain0_n6_α
 xchain0_n5_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 96]
 mov r14d, eax
 jmp xchain0_n3_β
xchain0_n6_α:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S0]
 mov esi, dword ptr [r12 + 112]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n7_α
xchain0_n7_α:
# IR_MATCH_LEN
bb8_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n3_β
 add r14d, 1
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n3_β
xchain0_n8_α:
# IR_MATCH_CAPTURE_SAVE
bb9_α:
 mov dword ptr [r12 + 80], r14d
 jmp xchain0_n9_α
xchain0_n9_α:
# IR_MATCH_REM
bb10_α:
 mov dword ptr [r12 + 64], r14d
 mov r14d, r15d
 jmp xchain0_n10_α
 xchain0_n9_β:
 mov r14d, dword ptr [r12 + 64]
 jmp xchain0_n3_β
xchain0_n10_α:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S2]
 mov esi, dword ptr [r12 + 80]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xchain0_n11_α
xchain0_n11_α:
# IR_VAR gva
bb12_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp main_γ
xchain0_n12_α:
# IR_LIT_STRING
bb13_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string " / "
xchain0_n13_α:
bb14_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call str_concat_d@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp main_γ
xchain0_n14_α:
# IR_VAR gva
bb15_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
xchain0_n15_α:
bb16_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call str_concat_d@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_γ
xchain0_n16_α:
# IR_ASSIGN global
bb17_α:
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "OUTPUT"
xchain0_n17_α:
# IR_LIT_STRING
bb18_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp main_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "fail"
xchain0_n18_α:
# IR_ASSIGN global
bb19_α:
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp main_γ
 xchain0_n18_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
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
.S0: .string "FIRST"
.S1: .string " "
.S2: .string "LAST"
.text

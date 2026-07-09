  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "T"
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
  call core_lib_init@PLT
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
 mov qword ptr [r12 + 528], rax
 pop rsi
main_α_body:
 xchain0_n0_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn2: .string "TABLE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn2]
 lea rsi, [r12 + 48]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR gva
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "name"
# IR_VAR gva
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n7_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n5_α:
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain0_n8_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "age"
# IR_VAR gva
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n12_α
 xchain0_n8_β:
 jmp xchain0_n4_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "Alice"
# IR_SUBSCRIPT x[i] variable
 xchain0_n9_α:
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n7_α
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "lang"
# IR_VAR gva
 xchain0_n11_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n16_α
# IR_ASSIGN_VAR write through variable
 xchain0_n12_α:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n4_α
 xchain0_n12_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n7_α
.Lx15_0:
 .quad 30
# IR_SUBSCRIPT x[i] variable
 xchain0_n14_α:
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "name"
# IR_VAR gva
 xchain0_n16_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n21_α
# IR_ASSIGN_VAR write through variable
 xchain0_n17_α:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n7_α
 xchain0_n17_β:
 jmp xchain0_n7_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n22_α
 xchain0_n18_β:
 jmp xchain0_n11_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "SNOBOL4"
# IR_SUBSCRIPT x[i] variable
 xchain0_n19_α:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n16_α
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n23_α
 xchain0_n19_β:
 jmp xchain0_n16_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n24_α
 xchain0_n20_β:
 jmp xchain0_n21_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "age"
# IR_VAR gva
 xchain0_n21_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp main_γ
# IR_ASSIGN_VAR write through variable
 xchain0_n22_α:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n11_α
 xchain0_n22_β:
 jmp xchain0_n11_α
# IR_DEREF variable -> value
 xchain0_n23_α:
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n16_α
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n26_α
 xchain0_n23_β:
 jmp xchain0_n16_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n24_α:
 mov rdi, qword ptr [r12 + 384]
 mov rsi, qword ptr [r12 + 392]
 mov rdx, qword ptr [r12 + 400]
 mov rcx, qword ptr [r12 + 408]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n21_α
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n27_α
 xchain0_n24_β:
 jmp xchain0_n21_α
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n28_α
 xchain0_n25_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "lang"
# IR_ASSIGN global
 xchain0_n26_α:
 mov rsi, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n16_α
 xchain0_n26_β:
 jmp xchain0_n16_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "OUTPUT"
# IR_DEREF variable -> value
 xchain0_n27_α:
 mov rdi, qword ptr [r12 + 416]
 mov rsi, qword ptr [r12 + 424]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n21_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n21_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n28_α:
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 mov rdx, qword ptr [r12 + 480]
 mov rcx, qword ptr [r12 + 488]
 call rt_subscript_var@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n29_α:
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov rdi, qword ptr [rip + .Lx31_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n21_α
 xchain0_n29_β:
 jmp xchain0_n21_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "OUTPUT"
# IR_DEREF variable -> value
 xchain0_n30_α:
 mov rdi, qword ptr [r12 + 496]
 mov rsi, qword ptr [r12 + 504]
 call rt_deref@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n31_α:
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp main_γ
 xchain0_n31_β:
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
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 528]
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

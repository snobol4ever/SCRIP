  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "A"
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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 560], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 5
 xchain0_n1_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+64] -> [r12+48]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn3: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR gva
 xchain0_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad 1
# IR_VAR gva
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad 3
# IR_VAR gva
 xchain0_n8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "first"
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx13_0:
 .quad 5
# IR_VAR gva
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n17_α
# IR_ASSIGN_VAR write through variable
 xchain0_n13_α:
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
 mov rdx, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n5_α
 xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "third"
# IR_SUBSCRIPT x[i] variable
 xchain0_n15_α:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx18_0:
 .quad 1
# IR_VAR gva
 xchain0_n17_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n21_α
 xchain0_n17_β:
 jmp xchain0_n22_α
# IR_ASSIGN_VAR write through variable
 xchain0_n18_α:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 208]
 mov rcx, qword ptr [r12 + 216]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n8_α
 xchain0_n18_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n23_α
 xchain0_n19_β:
 jmp xchain0_n12_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "fifth"
# IR_SUBSCRIPT x[i] variable
 xchain0_n20_α:
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
 mov rdx, qword ptr [r12 + 352]
 mov rcx, qword ptr [r12 + 360]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n24_α
 xchain0_n20_β:
 jmp xchain0_n17_α
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp xchain0_n22_α
.Lx23_0:
 .quad 3
# IR_VAR gva
 xchain0_n22_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n26_α
 xchain0_n22_β:
 jmp main_γ
# IR_ASSIGN_VAR write through variable
 xchain0_n23_α:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n12_α
 xchain0_n23_β:
 jmp xchain0_n12_α
# IR_DEREF variable -> value
 xchain0_n24_α:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n27_α
 xchain0_n24_β:
 jmp xchain0_n17_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n25_α:
 mov rdi, qword ptr [r12 + 416]
 mov rsi, qword ptr [r12 + 424]
 mov rdx, qword ptr [r12 + 432]
 mov rcx, qword ptr [r12 + 440]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n22_α
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain0_n28_α
 xchain0_n25_β:
 jmp xchain0_n22_α
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n29_α
 xchain0_n26_β:
 jmp main_γ
.Lx28_0:
 .quad 5
# IR_ASSIGN global
 xchain0_n27_α:
 mov rsi, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n17_α
 xchain0_n27_β:
 jmp xchain0_n17_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
# IR_DEREF variable -> value
 xchain0_n28_α:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n22_α
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp xchain0_n22_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n29_α:
 mov rdi, qword ptr [r12 + 496]
 mov rsi, qword ptr [r12 + 504]
 mov rdx, qword ptr [r12 + 512]
 mov rcx, qword ptr [r12 + 520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n30_α:
 mov rsi, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov rdi, qword ptr [rip + .Lx32_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n22_α
 xchain0_n30_β:
 jmp xchain0_n22_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "OUTPUT"
# IR_DEREF variable -> value
 xchain0_n31_α:
 mov rdi, qword ptr [r12 + 528]
 mov rsi, qword ptr [r12 + 536]
 call rt_deref@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov rdi, qword ptr [rip + .Lx34_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp main_γ
 xchain0_n32_β:
 jmp main_γ
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
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
 mov rdi, qword ptr [r12 + 560]
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

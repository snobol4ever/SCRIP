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
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 968], rsp
main_α_body:
 xchain0_n0_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn2: .string "TABLE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn2]
 lea rsi, [rsp + 128]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rsp + 176], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 184], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "name"
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n7_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n5_α:
 mov rdi, qword ptr [rsp + 144]
 mov rsi, qword ptr [rsp + 152]
 mov rdx, qword ptr [rsp + 176]
 mov rcx, qword ptr [rsp + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 jmp xchain0_n8_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rsp + 320], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 328], rax
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "age"
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 240], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 248], rax
 jmp xchain0_n12_α
 xchain0_n8_β:
 jmp xchain0_n4_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "Alice"
# IR_SUBSCRIPT x[i] variable
 xchain0_n9_α:
 mov rdi, qword ptr [rsp + 288]
 mov rsi, qword ptr [rsp + 296]
 mov rdx, qword ptr [rsp + 320]
 mov rcx, qword ptr [rsp + 328]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n7_α
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [rsp + 464], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "lang"
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n16_α
# IR_ASSIGN_VAR
 xchain0_n12_α:
 mov rdi, qword ptr [rsp + 208]
 mov rsi, qword ptr [rsp + 216]
 mov rdx, qword ptr [rsp + 240]
 mov rcx, qword ptr [rsp + 248]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 jmp xchain0_n4_α
 xchain0_n12_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rsp + 384], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 392], rax
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n7_α
.Lx15_0:
 .quad 30
# IR_SUBSCRIPT x[i] variable
 xchain0_n14_α:
 mov rdi, qword ptr [rsp + 432]
 mov rsi, qword ptr [rsp + 440]
 mov rdx, qword ptr [rsp + 464]
 mov rcx, qword ptr [rsp + 472]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [rsp + 624], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rsp + 632], rax
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "name"
# IR_VAR
 xchain0_n16_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 720], rax
 mov qword ptr [rsp + 728], rdx
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n21_α
# IR_ASSIGN_VAR
 xchain0_n17_α:
 mov rdi, qword ptr [rsp + 352]
 mov rsi, qword ptr [rsp + 360]
 mov rdx, qword ptr [rsp + 384]
 mov rcx, qword ptr [rsp + 392]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 jmp xchain0_n7_α
 xchain0_n17_β:
 jmp xchain0_n7_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 528], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 536], rax
 jmp xchain0_n22_α
 xchain0_n18_β:
 jmp xchain0_n11_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "SNOBOL4"
# IR_SUBSCRIPT x[i] variable
 xchain0_n19_α:
 mov rdi, qword ptr [rsp + 592]
 mov rsi, qword ptr [rsp + 600]
 mov rdx, qword ptr [rsp + 624]
 mov rcx, qword ptr [rsp + 632]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n16_α
 mov qword ptr [rsp + 656], rax
 mov qword ptr [rsp + 664], rdx
 jmp xchain0_n23_α
 xchain0_n19_β:
 jmp xchain0_n16_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rsp + 752], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 760], rax
 jmp xchain0_n24_α
 xchain0_n20_β:
 jmp xchain0_n21_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "age"
# IR_VAR
 xchain0_n21_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 848], rax
 mov qword ptr [rsp + 856], rdx
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [rsp + 496]
 mov rsi, qword ptr [rsp + 504]
 mov rdx, qword ptr [rsp + 528]
 mov rcx, qword ptr [rsp + 536]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [rsp + 560], rax
 mov qword ptr [rsp + 568], rdx
 jmp xchain0_n11_α
 xchain0_n22_β:
 jmp xchain0_n11_α
# IR_DEREF variable -> value
 xchain0_n23_α:
 mov rdi, qword ptr [rsp + 656]
 mov rsi, qword ptr [rsp + 664]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n16_α
 mov qword ptr [rsp + 688], rax
 mov qword ptr [rsp + 696], rdx
 jmp xchain0_n26_α
 xchain0_n23_β:
 jmp xchain0_n16_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n24_α:
 mov rdi, qword ptr [rsp + 720]
 mov rsi, qword ptr [rsp + 728]
 mov rdx, qword ptr [rsp + 752]
 mov rcx, qword ptr [rsp + 760]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n21_α
 mov qword ptr [rsp + 784], rax
 mov qword ptr [rsp + 792], rdx
 jmp xchain0_n27_α
 xchain0_n24_β:
 jmp xchain0_n21_α
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [rsp + 880], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [rsp + 888], rax
 jmp xchain0_n28_α
 xchain0_n25_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "lang"
# IR_ASSIGN global
 xchain0_n26_α:
 mov rsi, qword ptr [rsp + 688]
 mov rdx, qword ptr [rsp + 696]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 jmp xchain0_n16_α
 xchain0_n26_β:
 jmp xchain0_n16_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "OUTPUT"
# IR_DEREF variable -> value
 xchain0_n27_α:
 mov rdi, qword ptr [rsp + 784]
 mov rsi, qword ptr [rsp + 792]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n21_α
 mov qword ptr [rsp + 816], rax
 mov qword ptr [rsp + 824], rdx
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n21_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n28_α:
 mov rdi, qword ptr [rsp + 848]
 mov rsi, qword ptr [rsp + 856]
 mov rdx, qword ptr [rsp + 880]
 mov rcx, qword ptr [rsp + 888]
 call rt_subscript_var@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 912], rax
 mov qword ptr [rsp + 920], rdx
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n29_α:
 mov rsi, qword ptr [rsp + 816]
 mov rdx, qword ptr [rsp + 824]
 mov rdi, qword ptr [rip + .Lx31_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 jmp xchain0_n21_α
 xchain0_n29_β:
 jmp xchain0_n21_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "OUTPUT"
# IR_DEREF variable -> value
 xchain0_n30_α:
 mov rdi, qword ptr [rsp + 912]
 mov rsi, qword ptr [rsp + 920]
 call rt_deref@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 944], rax
 mov qword ptr [rsp + 952], rdx
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n31_α:
 mov rsi, qword ptr [rsp + 944]
 mov rdx, qword ptr [rsp + 952]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 832], rax
 mov qword ptr [rsp + 840], rdx
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
mov rsp, qword ptr [rsp + 968]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 968]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

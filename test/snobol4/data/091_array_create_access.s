  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "A"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
  call gva_register@PLT
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
  mov qword ptr [rsp + 1016], rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 5
 xchain0_n1_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn3: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn3]
 lea rsi, [rsp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 224], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad 1
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [rsp + 192]
 mov rsi, qword ptr [rsp + 200]
 mov rdx, qword ptr [rsp + 224]
 mov rcx, qword ptr [rsp + 232]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 256], rax
 mov qword ptr [rsp + 264], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rsp + 368], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 376], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad 3
# IR_VAR
 xchain0_n8_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 480], rax
 mov qword ptr [rsp + 488], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rsp + 288], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "first"
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [rsp + 336]
 mov rsi, qword ptr [rsp + 344]
 mov rdx, qword ptr [rsp + 368]
 mov rcx, qword ptr [rsp + 376]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [rsp + 512], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rsp + 520], rax
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx13_0:
 .quad 5
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 640], rax
 mov qword ptr [rsp + 648], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n17_α
# IR_ASSIGN_VAR
 xchain0_n13_α:
 mov rdi, qword ptr [rsp + 256]
 mov rsi, qword ptr [rsp + 264]
 mov rdx, qword ptr [rsp + 288]
 mov rcx, qword ptr [rsp + 296]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain0_n5_α
 xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rsp + 432], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 440], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "third"
# IR_SUBSCRIPT x[i] variable
 xchain0_n15_α:
 mov rdi, qword ptr [rsp + 480]
 mov rsi, qword ptr [rsp + 488]
 mov rdx, qword ptr [rsp + 512]
 mov rcx, qword ptr [rsp + 520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [rsp + 672], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 680], rax
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx18_0:
 .quad 1
# IR_VAR
 xchain0_n17_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 768], rax
 mov qword ptr [rsp + 776], rdx
 jmp xchain0_n21_α
 xchain0_n17_β:
 jmp xchain0_n22_α
# IR_ASSIGN_VAR
 xchain0_n18_α:
 mov rdi, qword ptr [rsp + 400]
 mov rsi, qword ptr [rsp + 408]
 mov rdx, qword ptr [rsp + 432]
 mov rcx, qword ptr [rsp + 440]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain0_n8_α
 xchain0_n18_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [rsp + 576], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rsp + 584], rax
 jmp xchain0_n23_α
 xchain0_n19_β:
 jmp xchain0_n12_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "fifth"
# IR_SUBSCRIPT x[i] variable
 xchain0_n20_α:
 mov rdi, qword ptr [rsp + 640]
 mov rsi, qword ptr [rsp + 648]
 mov rdx, qword ptr [rsp + 672]
 mov rcx, qword ptr [rsp + 680]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 jmp xchain0_n24_α
 xchain0_n20_β:
 jmp xchain0_n17_α
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [rsp + 800], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rsp + 808], rax
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp xchain0_n22_α
.Lx23_0:
 .quad 3
# IR_VAR
 xchain0_n22_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 896], rax
 mov qword ptr [rsp + 904], rdx
 jmp xchain0_n26_α
 xchain0_n22_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n23_α:
 mov rdi, qword ptr [rsp + 544]
 mov rsi, qword ptr [rsp + 552]
 mov rdx, qword ptr [rsp + 576]
 mov rcx, qword ptr [rsp + 584]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [rsp + 608], rax
 mov qword ptr [rsp + 616], rdx
 jmp xchain0_n12_α
 xchain0_n23_β:
 jmp xchain0_n12_α
# IR_DEREF variable -> value
 xchain0_n24_α:
 mov rdi, qword ptr [rsp + 704]
 mov rsi, qword ptr [rsp + 712]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [rsp + 736], rax
 mov qword ptr [rsp + 744], rdx
 jmp xchain0_n27_α
 xchain0_n24_β:
 jmp xchain0_n17_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n25_α:
 mov rdi, qword ptr [rsp + 768]
 mov rsi, qword ptr [rsp + 776]
 mov rdx, qword ptr [rsp + 800]
 mov rcx, qword ptr [rsp + 808]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n22_α
 mov qword ptr [rsp + 832], rax
 mov qword ptr [rsp + 840], rdx
 jmp xchain0_n28_α
 xchain0_n25_β:
 jmp xchain0_n22_α
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [rsp + 928], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rsp + 936], rax
 jmp xchain0_n29_α
 xchain0_n26_β:
 jmp main_γ
.Lx28_0:
 .quad 5
# IR_ASSIGN global
 xchain0_n27_α:
 mov rsi, qword ptr [rsp + 736]
 mov rdx, qword ptr [rsp + 744]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 624], rax
 mov qword ptr [rsp + 632], rdx
 jmp xchain0_n17_α
 xchain0_n27_β:
 jmp xchain0_n17_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
# IR_DEREF variable -> value
 xchain0_n28_α:
 mov rdi, qword ptr [rsp + 832]
 mov rsi, qword ptr [rsp + 840]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n22_α
 mov qword ptr [rsp + 864], rax
 mov qword ptr [rsp + 872], rdx
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp xchain0_n22_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n29_α:
 mov rdi, qword ptr [rsp + 896]
 mov rsi, qword ptr [rsp + 904]
 mov rdx, qword ptr [rsp + 928]
 mov rcx, qword ptr [rsp + 936]
 call rt_subscript_var@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 960], rax
 mov qword ptr [rsp + 968], rdx
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n30_α:
 mov rsi, qword ptr [rsp + 864]
 mov rdx, qword ptr [rsp + 872]
 mov rdi, qword ptr [rip + .Lx32_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 752], rax
 mov qword ptr [rsp + 760], rdx
 jmp xchain0_n22_α
 xchain0_n30_β:
 jmp xchain0_n22_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "OUTPUT"
# IR_DEREF variable -> value
 xchain0_n31_α:
 mov rdi, qword ptr [rsp + 960]
 mov rsi, qword ptr [rsp + 968]
 call rt_deref@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 992], rax
 mov qword ptr [rsp + 1000], rdx
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [rsp + 992]
 mov rdx, qword ptr [rsp + 1000]
 mov rdi, qword ptr [rip + .Lx34_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 880], rax
 mov qword ptr [rsp + 888], rdx
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
mov rsp, qword ptr [rsp + 1016]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1016]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

  .intel_syntax noprefix
  .text
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
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
  mov qword ptr [rsp + 936], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 224], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "b"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [rsp + 192], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "a"
# IR_COERCE_STRING
 xchain0_n2_α:
 lea rdi, [rsp + 224]
 lea rsi, [rsp + 160]
 mov rdx, 126
 call rt_coerce_str_d@PLT
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
# IR_COERCE_STRING
 xchain0_n3_α:
 lea rdi, [rsp + 192]
 lea rsi, [rsp + 128]
 mov rdx, 127
 call rt_coerce_str_d@PLT
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n4_α:
 lea rdi, [rsp + 160]
 lea rsi, [rsp + 128]
 call rt_cmp_d@PLT
 test eax, eax
 jle main_γ
 mov qword ptr [rsp + 96], 0
 mov qword ptr [rsp + 104], 0
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [rsp + 272], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 280], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "b > a"
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [rsp + 272]
 mov rdx, qword ptr [rsp + 280]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 256], rax
 mov qword ptr [rsp + 264], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 432], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 440], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "a"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 400], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 408], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "b"
# IR_COERCE_STRING
 xchain0_n9_α:
 lea rdi, [rsp + 432]
 lea rsi, [rsp + 368]
 mov rdx, 130
 call rt_coerce_str_d@PLT
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp main_γ
# IR_COERCE_STRING
 xchain0_n10_α:
 lea rdi, [rsp + 400]
 lea rsi, [rsp + 336]
 mov rdx, 131
 call rt_coerce_str_d@PLT
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n11_α:
 lea rdi, [rsp + 368]
 lea rsi, [rsp + 336]
 call rt_cmp_d@PLT
 test eax, eax
 jns main_γ
 mov qword ptr [rsp + 304], 0
 mov qword ptr [rsp + 312], 0
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rsp + 480], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 488], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "a < b"
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [rsp + 480]
 mov rdx, qword ptr [rsp + 488]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rsp + 640], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "cat"
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [rsp + 608], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 616], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "cat"
# IR_COERCE_STRING
 xchain0_n16_α:
 lea rdi, [rsp + 640]
 lea rsi, [rsp + 576]
 mov rdx, 122
 call rt_coerce_str_d@PLT
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
# IR_COERCE_STRING
 xchain0_n17_α:
 lea rdi, [rsp + 608]
 lea rsi, [rsp + 544]
 mov rdx, 123
 call rt_coerce_str_d@PLT
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n18_α:
 lea rdi, [rsp + 576]
 lea rsi, [rsp + 544]
 call rt_cmp_d@PLT
 test eax, eax
 jne main_γ
 mov qword ptr [rsp + 512], 0
 mov qword ptr [rsp + 520], 0
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [rsp + 688], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rsp + 696], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n21_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "cat = cat"
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [rsp + 688]
 mov rdx, qword ptr [rsp + 696]
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 672], rax
 mov qword ptr [rsp + 680], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n21_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rsp + 848], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [rsp + 856], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp main_γ
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "cat"
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [rsp + 816], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [rsp + 824], rax
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "dog"
# IR_COERCE_STRING
 xchain0_n23_α:
 lea rdi, [rsp + 848]
 lea rsi, [rsp + 784]
 mov rdx, 132
 call rt_coerce_str_d@PLT
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp main_γ
# IR_COERCE_STRING
 xchain0_n24_α:
 lea rdi, [rsp + 816]
 lea rsi, [rsp + 752]
 mov rdx, 133
 call rt_coerce_str_d@PLT
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp main_γ
# IR_CMP_TEST
 xchain0_n25_α:
 lea rdi, [rsp + 784]
 lea rsi, [rsp + 752]
 call rt_cmp_d@PLT
 test eax, eax
 je main_γ
 mov qword ptr [rsp + 720], 0
 mov qword ptr [rsp + 728], 0
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n26_α:
 mov qword ptr [rsp + 896], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rsp + 904], rax
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp main_γ
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "cat != dog"
# IR_ASSIGN global
 xchain0_n27_α:
 mov rsi, qword ptr [rsp + 896]
 mov rdx, qword ptr [rsp + 904]
 mov rdi, qword ptr [rip + .Lx40_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 880], rax
 mov qword ptr [rsp + 888], rdx
 jmp main_γ
 xchain0_n27_β:
 jmp main_γ
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 936]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 936]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

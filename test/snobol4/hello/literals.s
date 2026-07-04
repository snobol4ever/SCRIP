  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
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
 .string ""
xchain0_n1_α:
# IR_ASSIGN global
bb2_α:
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov rdi, qword ptr [rip + .Lx2_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "OUTPUT"
xchain0_n2_α:
# IR_LIT_STRING
bb3_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string ""
xchain0_n3_α:
# IR_ASSIGN global
bb4_α:
 mov rsi, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "OUTPUT"
xchain0_n4_α:
# IR_LIT_STRING
bb5_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "Hello World!"
xchain0_n5_α:
# IR_ASSIGN global
bb6_α:
 mov rsi, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "OUTPUT"
xchain0_n6_α:
# IR_LIT_INTEGER
bb7_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx7_0:
 .quad 0
xchain0_n7_α:
# IR_ASSIGN global
bb8_α:
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "OUTPUT"
xchain0_n8_α:
# IR_LIT_INTEGER
bb9_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n10_α
.Lx9_0:
 .quad 1
xchain0_n9_α:
# IR_ASSIGN global
bb10_α:
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
xchain0_n10_α:
# IR_LIT_INTEGER
bb11_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n13_α
.Lx11_0:
 .quad 1
xchain0_n11_α:
# IR_UNOP
bb12_α:
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n13_α
xchain0_n12_α:
# IR_ASSIGN global
bb13_α:
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n13_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
xchain0_n13_α:
# IR_LIT_REAL
bb14_α:
 mov qword ptr [r12 + 240], 7
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n15_α
.Lx14_0:
 .quad 4607182418800017408
xchain0_n14_α:
# IR_ASSIGN global
bb15_α:
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n15_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "OUTPUT"
xchain0_n15_α:
# IR_LIT_STRING
bb16_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n17_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "1"
xchain0_n16_α:
# IR_ASSIGN global
bb17_α:
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "OUTPUT"
xchain0_n17_α:
# IR_LIT_STRING
bb18_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "1"
xchain0_n18_α:
# IR_ASSIGN global
bb19_α:
 mov rsi, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n19_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "OUTPUT"
xchain0_n19_α:
# IR_LIT_STRING
bb20_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n21_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "1.0"
xchain0_n20_α:
# IR_ASSIGN global
bb21_α:
 mov rsi, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n21_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "OUTPUT"
xchain0_n21_α:
# IR_LIT_STRING
bb22_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n23_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "I'm here"
xchain0_n22_α:
# IR_ASSIGN global
bb23_α:
 mov rsi, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
xchain0_n23_α:
# IR_LIT_STRING
bb24_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n25_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "\"Quote of the day\""
xchain0_n24_α:
# IR_ASSIGN global
bb25_α:
 mov rsi, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov rdi, qword ptr [rip + .Lx25_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "OUTPUT"
xchain0_n25_α:
# IR_LIT_STRING
bb26_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n29_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string ""
xchain0_n26_α:
# IR_LIT_STRING
bb27_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n29_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string ""
xchain0_n27_α:
bb28_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 448]
 cmp eax, 100
 je .Lx28_0
 mov eax, dword ptr [r12 + 464]
 cmp eax, 100
 je .Lx28_0
 mov eax, dword ptr [r12 + 448]
 cmp eax, 6
 jne .Lx28_2
 mov eax, dword ptr [r12 + 464]
 cmp eax, 6
 jne .Lx28_2
.Lx28_1:
 mov rax, qword ptr [r12 + 456]
 mov rcx, qword ptr [r12 + 472]
 add rax, rcx
 mov qword ptr [r12 + 432], 6
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n28_α
.Lx28_0:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8d, 0
 lea r9, [r12 + 432]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx28_3
.Lx28_2:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
.Lx28_3:
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n29_α
xchain0_n28_α:
# IR_ASSIGN global
bb29_α:
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp xchain0_n29_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
xchain0_n29_α:
# IR_LIT_STRING
bb30_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp xchain0_n33_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string ""
xchain0_n30_α:
# IR_LIT_INTEGER
bb31_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp xchain0_n33_α
.Lx31_0:
 .quad 1
xchain0_n31_α:
bb32_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 512]
 cmp eax, 100
 je .Lx32_0
 mov eax, dword ptr [r12 + 528]
 cmp eax, 100
 je .Lx32_0
 mov eax, dword ptr [r12 + 512]
 cmp eax, 6
 jne .Lx32_2
 mov eax, dword ptr [r12 + 528]
 cmp eax, 6
 jne .Lx32_2
.Lx32_1:
 mov rax, qword ptr [r12 + 520]
 mov rcx, qword ptr [r12 + 536]
 add rax, rcx
 mov qword ptr [r12 + 496], 6
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n32_α
.Lx32_0:
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 mov r8d, 0
 lea r9, [r12 + 496]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx32_3
.Lx32_2:
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n33_α
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
.Lx32_3:
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp xchain0_n33_α
xchain0_n32_α:
# IR_ASSIGN global
bb33_α:
 mov rsi, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n33_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "OUTPUT"
xchain0_n33_α:
# IR_LIT_INTEGER
bb34_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n37_α
.Lx34_0:
 .quad 1
xchain0_n34_α:
# IR_LIT_STRING
bb35_α:
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n37_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string ""
xchain0_n35_α:
bb36_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 576]
 cmp eax, 100
 je .Lx36_0
 mov eax, dword ptr [r12 + 592]
 cmp eax, 100
 je .Lx36_0
 mov eax, dword ptr [r12 + 576]
 cmp eax, 6
 jne .Lx36_2
 mov eax, dword ptr [r12 + 592]
 cmp eax, 6
 jne .Lx36_2
.Lx36_1:
 mov rax, qword ptr [r12 + 584]
 mov rcx, qword ptr [r12 + 600]
 add rax, rcx
 mov qword ptr [r12 + 560], 6
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n36_α
.Lx36_0:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 mov rdx, qword ptr [r12 + 592]
 mov rcx, qword ptr [r12 + 600]
 mov r8d, 0
 lea r9, [r12 + 560]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx36_3
.Lx36_2:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 mov rdx, qword ptr [r12 + 592]
 mov rcx, qword ptr [r12 + 600]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n37_α
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
.Lx36_3:
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp xchain0_n37_α
xchain0_n36_α:
# IR_ASSIGN global
bb37_α:
 mov rsi, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n37_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
xchain0_n37_α:
# IR_LIT_STRING
bb38_α:
 mov qword ptr [r12 + 640], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n41_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string ""
xchain0_n38_α:
# IR_LIT_STRING
bb39_α:
 mov qword ptr [r12 + 656], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n39_α
 xchain0_n38_β:
 jmp xchain0_n41_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string ""
xchain0_n39_α:
bb40_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 640]
 mov rsi, qword ptr [r12 + 648]
 mov rdx, qword ptr [r12 + 656]
 mov rcx, qword ptr [r12 + 664]
 call str_concat_d@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n41_α
xchain0_n40_α:
# IR_ASSIGN global
bb41_α:
 mov rsi, qword ptr [r12 + 624]
 mov rdx, qword ptr [r12 + 632]
 mov rdi, qword ptr [rip + .Lx41_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp xchain0_n41_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "OUTPUT"
xchain0_n41_α:
# IR_LIT_STRING
bb42_α:
 mov qword ptr [r12 + 704], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n45_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string ""
xchain0_n42_α:
# IR_LIT_STRING
bb43_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n45_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "Z"
xchain0_n43_α:
bb44_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 704]
 mov rsi, qword ptr [r12 + 712]
 mov rdx, qword ptr [r12 + 720]
 mov rcx, qword ptr [r12 + 728]
 call str_concat_d@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n44_α
 xchain0_n43_β:
 jmp xchain0_n45_α
xchain0_n44_α:
# IR_ASSIGN global
bb45_α:
 mov rsi, qword ptr [r12 + 688]
 mov rdx, qword ptr [r12 + 696]
 mov rdi, qword ptr [rip + .Lx45_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain0_n45_α
 xchain0_n44_β:
 jmp xchain0_n45_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "OUTPUT"
xchain0_n45_α:
# IR_LIT_STRING
bb46_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n46_α
 xchain0_n45_β:
 jmp xchain0_n49_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "A"
xchain0_n46_α:
# IR_LIT_STRING
bb47_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n49_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string ""
xchain0_n47_α:
bb48_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 768]
 mov rsi, qword ptr [r12 + 776]
 mov rdx, qword ptr [r12 + 784]
 mov rcx, qword ptr [r12 + 792]
 call str_concat_d@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n49_α
xchain0_n48_α:
# IR_ASSIGN global
bb49_α:
 mov rsi, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov rdi, qword ptr [rip + .Lx49_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp xchain0_n49_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "OUTPUT"
xchain0_n49_α:
# IR_LIT_STRING
bb50_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n50_α
 xchain0_n49_β:
 jmp xchain0_n53_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "A"
xchain0_n50_α:
# IR_LIT_STRING
bb51_α:
 mov qword ptr [r12 + 848], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n51_α
 xchain0_n50_β:
 jmp xchain0_n53_α
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "Z"
xchain0_n51_α:
bb52_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 832]
 mov rsi, qword ptr [r12 + 840]
 mov rdx, qword ptr [r12 + 848]
 mov rcx, qword ptr [r12 + 856]
 call str_concat_d@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain0_n52_α
 xchain0_n51_β:
 jmp xchain0_n53_α
xchain0_n52_α:
# IR_ASSIGN global
bb53_α:
 mov rsi, qword ptr [r12 + 816]
 mov rdx, qword ptr [r12 + 824]
 mov rdi, qword ptr [rip + .Lx53_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xchain0_n53_α
 xchain0_n52_β:
 jmp xchain0_n53_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "OUTPUT"
xchain0_n53_α:
# IR_LIT_INTEGER
bb54_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n54_α
 xchain0_n53_β:
 jmp xchain0_n57_α
.Lx54_0:
 .quad 1
xchain0_n54_α:
# IR_LIT_INTEGER
bb55_α:
 mov qword ptr [r12 + 912], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n55_α
 xchain0_n54_β:
 jmp xchain0_n57_α
.Lx55_0:
 .quad 2
xchain0_n55_α:
bb56_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 896]
 cmp eax, 100
 je .Lx56_0
 mov eax, dword ptr [r12 + 912]
 cmp eax, 100
 je .Lx56_0
 mov eax, dword ptr [r12 + 896]
 cmp eax, 6
 jne .Lx56_2
 mov eax, dword ptr [r12 + 912]
 cmp eax, 6
 jne .Lx56_2
.Lx56_1:
 mov rax, qword ptr [r12 + 904]
 mov rcx, qword ptr [r12 + 920]
 add rax, rcx
 mov qword ptr [r12 + 880], 6
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n56_α
.Lx56_0:
 mov rdi, qword ptr [r12 + 896]
 mov rsi, qword ptr [r12 + 904]
 mov rdx, qword ptr [r12 + 912]
 mov rcx, qword ptr [r12 + 920]
 mov r8d, 0
 lea r9, [r12 + 880]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx56_3
.Lx56_2:
 mov rdi, qword ptr [r12 + 896]
 mov rsi, qword ptr [r12 + 904]
 mov rdx, qword ptr [r12 + 912]
 mov rcx, qword ptr [r12 + 920]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n57_α
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
.Lx56_3:
 jmp xchain0_n56_α
 xchain0_n55_β:
 jmp xchain0_n57_α
xchain0_n56_α:
# IR_ASSIGN global
bb57_α:
 mov rsi, qword ptr [r12 + 880]
 mov rdx, qword ptr [r12 + 888]
 mov rdi, qword ptr [rip + .Lx57_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n57_α
 xchain0_n56_β:
 jmp xchain0_n57_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "OUTPUT"
xchain0_n57_α:
# IR_LIT_INTEGER
bb58_α:
 mov qword ptr [r12 + 960], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n58_α
 xchain0_n57_β:
 jmp xchain0_n62_α
.Lx58_0:
 .quad 1
xchain0_n58_α:
# IR_LIT_INTEGER
bb59_α:
 mov qword ptr [r12 + 992], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp xchain0_n62_α
.Lx59_0:
 .quad 2
xchain0_n59_α:
# IR_LIT_INTEGER
bb60_α:
 mov qword ptr [r12 + 1008], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp xchain0_n62_α
.Lx60_0:
 .quad 3
xchain0_n60_α:
bb61_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 992]
 cmp eax, 100
 je .Lx61_0
 mov eax, dword ptr [r12 + 1008]
 cmp eax, 100
 je .Lx61_0
 mov eax, dword ptr [r12 + 992]
 cmp eax, 6
 jne .Lx61_2
 mov eax, dword ptr [r12 + 1008]
 cmp eax, 6
 jne .Lx61_2
.Lx61_1:
 mov rax, qword ptr [r12 + 1000]
 mov rcx, qword ptr [r12 + 1016]
 imul rax, rcx
 mov qword ptr [r12 + 976], 6
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n61_α
.Lx61_0:
 mov rdi, qword ptr [r12 + 992]
 mov rsi, qword ptr [r12 + 1000]
 mov rdx, qword ptr [r12 + 1008]
 mov rcx, qword ptr [r12 + 1016]
 mov r8d, 2
 lea r9, [r12 + 976]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx61_3
.Lx61_2:
 mov rdi, qword ptr [r12 + 992]
 mov rsi, qword ptr [r12 + 1000]
 mov rdx, qword ptr [r12 + 1008]
 mov rcx, qword ptr [r12 + 1016]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n62_α
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
.Lx61_3:
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp xchain0_n62_α
xchain0_n61_α:
bb62_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 960]
 cmp eax, 100
 je .Lx62_0
 mov eax, dword ptr [r12 + 976]
 cmp eax, 100
 je .Lx62_0
 mov eax, dword ptr [r12 + 960]
 cmp eax, 6
 jne .Lx62_2
 mov eax, dword ptr [r12 + 976]
 cmp eax, 6
 jne .Lx62_2
.Lx62_1:
 mov rax, qword ptr [r12 + 968]
 mov rcx, qword ptr [r12 + 984]
 add rax, rcx
 mov qword ptr [r12 + 944], 6
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n63_α
.Lx62_0:
 mov rdi, qword ptr [r12 + 960]
 mov rsi, qword ptr [r12 + 968]
 mov rdx, qword ptr [r12 + 976]
 mov rcx, qword ptr [r12 + 984]
 mov r8d, 0
 lea r9, [r12 + 944]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx62_3
.Lx62_2:
 mov rdi, qword ptr [r12 + 960]
 mov rsi, qword ptr [r12 + 968]
 mov rdx, qword ptr [r12 + 976]
 mov rcx, qword ptr [r12 + 984]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n62_α
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
.Lx62_3:
 jmp xchain0_n63_α
 xchain0_n61_β:
 jmp xchain0_n62_α
xchain0_n62_α:
# IR_LIT_INTEGER
bb63_α:
 mov qword ptr [r12 + 1072], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 1080], rax
 jmp xchain0_n64_α
 xchain0_n62_β:
 jmp xchain0_n67_α
.Lx63_0:
 .quad 1
xchain0_n63_α:
# IR_ASSIGN global
bb64_α:
 mov rsi, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov rdi, qword ptr [rip + .Lx64_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain0_n62_α
 xchain0_n63_β:
 jmp xchain0_n62_α
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string "OUTPUT"
xchain0_n64_α:
# IR_LIT_INTEGER
bb65_α:
 mov qword ptr [r12 + 1088], 6
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n65_α
 xchain0_n64_β:
 jmp xchain0_n67_α
.Lx65_0:
 .quad 2
xchain0_n65_α:
bb66_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1072]
 cmp eax, 100
 je .Lx66_0
 mov eax, dword ptr [r12 + 1088]
 cmp eax, 100
 je .Lx66_0
 mov eax, dword ptr [r12 + 1072]
 cmp eax, 6
 jne .Lx66_2
 mov eax, dword ptr [r12 + 1088]
 cmp eax, 6
 jne .Lx66_2
.Lx66_1:
 mov rax, qword ptr [r12 + 1080]
 mov rcx, qword ptr [r12 + 1096]
 add rax, rcx
 mov qword ptr [r12 + 1056], 6
 mov qword ptr [r12 + 1064], rax
 jmp xchain0_n66_α
.Lx66_0:
 mov rdi, qword ptr [r12 + 1072]
 mov rsi, qword ptr [r12 + 1080]
 mov rdx, qword ptr [r12 + 1088]
 mov rcx, qword ptr [r12 + 1096]
 mov r8d, 0
 lea r9, [r12 + 1056]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx66_3
.Lx66_2:
 mov rdi, qword ptr [r12 + 1072]
 mov rsi, qword ptr [r12 + 1080]
 mov rdx, qword ptr [r12 + 1088]
 mov rcx, qword ptr [r12 + 1096]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
.Lx66_3:
 jmp xchain0_n66_α
 xchain0_n65_β:
 jmp xchain0_n67_α
xchain0_n66_α:
# IR_LIT_INTEGER
bb67_α:
 mov qword ptr [r12 + 1104], 6
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [r12 + 1112], rax
 jmp xchain0_n68_α
 xchain0_n66_β:
 jmp xchain0_n67_α
.Lx67_0:
 .quad 3
xchain0_n67_α:
# IR_LIT_INTEGER
bb68_α:
 mov qword ptr [r12 + 1152], 6
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n69_α
 xchain0_n67_β:
 jmp main_γ
.Lx68_0:
 .quad 1
xchain0_n68_α:
bb69_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1056]
 cmp eax, 100
 je .Lx69_0
 mov eax, dword ptr [r12 + 1104]
 cmp eax, 100
 je .Lx69_0
 mov eax, dword ptr [r12 + 1056]
 cmp eax, 6
 jne .Lx69_2
 mov eax, dword ptr [r12 + 1104]
 cmp eax, 6
 jne .Lx69_2
.Lx69_1:
 mov rax, qword ptr [r12 + 1064]
 mov rcx, qword ptr [r12 + 1112]
 imul rax, rcx
 mov qword ptr [r12 + 1040], 6
 mov qword ptr [r12 + 1048], rax
 jmp xchain0_n70_α
.Lx69_0:
 mov rdi, qword ptr [r12 + 1056]
 mov rsi, qword ptr [r12 + 1064]
 mov rdx, qword ptr [r12 + 1104]
 mov rcx, qword ptr [r12 + 1112]
 mov r8d, 2
 lea r9, [r12 + 1040]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx69_3
.Lx69_2:
 mov rdi, qword ptr [r12 + 1056]
 mov rsi, qword ptr [r12 + 1064]
 mov rdx, qword ptr [r12 + 1104]
 mov rcx, qword ptr [r12 + 1112]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
.Lx69_3:
 jmp xchain0_n70_α
 xchain0_n68_β:
 jmp xchain0_n67_α
xchain0_n69_α:
# IR_LIT_INTEGER
bb70_α:
 mov qword ptr [r12 + 1184], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n71_α
 xchain0_n69_β:
 jmp main_γ
.Lx70_0:
 .quad 2
xchain0_n70_α:
# IR_ASSIGN global
bb71_α:
 mov rsi, qword ptr [r12 + 1040]
 mov rdx, qword ptr [r12 + 1048]
 mov rdi, qword ptr [rip + .Lx71_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain0_n67_α
 xchain0_n70_β:
 jmp xchain0_n67_α
.Lx71_0:
 .quad .Lx71_0_s
.Lx71_0_s:
 .string "OUTPUT"
xchain0_n71_α:
# IR_LIT_INTEGER
bb72_α:
 mov qword ptr [r12 + 1200], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 1208], rax
 jmp xchain0_n72_α
 xchain0_n71_β:
 jmp main_γ
.Lx72_0:
 .quad 3
xchain0_n72_α:
bb73_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1184]
 cmp eax, 100
 je .Lx73_0
 mov eax, dword ptr [r12 + 1200]
 cmp eax, 100
 je .Lx73_0
 mov eax, dword ptr [r12 + 1184]
 cmp eax, 6
 jne .Lx73_2
 mov eax, dword ptr [r12 + 1200]
 cmp eax, 6
 jne .Lx73_2
.Lx73_1:
 mov rax, qword ptr [r12 + 1192]
 mov rcx, qword ptr [r12 + 1208]
 imul rax, rcx
 mov qword ptr [r12 + 1168], 6
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n73_α
.Lx73_0:
 mov rdi, qword ptr [r12 + 1184]
 mov rsi, qword ptr [r12 + 1192]
 mov rdx, qword ptr [r12 + 1200]
 mov rcx, qword ptr [r12 + 1208]
 mov r8d, 2
 lea r9, [r12 + 1168]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx73_3
.Lx73_2:
 mov rdi, qword ptr [r12 + 1184]
 mov rsi, qword ptr [r12 + 1192]
 mov rdx, qword ptr [r12 + 1200]
 mov rcx, qword ptr [r12 + 1208]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
.Lx73_3:
 jmp xchain0_n73_α
 xchain0_n72_β:
 jmp main_γ
xchain0_n73_α:
bb74_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1152]
 cmp eax, 100
 je .Lx74_0
 mov eax, dword ptr [r12 + 1168]
 cmp eax, 100
 je .Lx74_0
 mov eax, dword ptr [r12 + 1152]
 cmp eax, 6
 jne .Lx74_2
 mov eax, dword ptr [r12 + 1168]
 cmp eax, 6
 jne .Lx74_2
.Lx74_1:
 mov rax, qword ptr [r12 + 1160]
 mov rcx, qword ptr [r12 + 1176]
 add rax, rcx
 mov qword ptr [r12 + 1136], 6
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n74_α
.Lx74_0:
 mov rdi, qword ptr [r12 + 1152]
 mov rsi, qword ptr [r12 + 1160]
 mov rdx, qword ptr [r12 + 1168]
 mov rcx, qword ptr [r12 + 1176]
 mov r8d, 0
 lea r9, [r12 + 1136]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx74_3
.Lx74_2:
 mov rdi, qword ptr [r12 + 1152]
 mov rsi, qword ptr [r12 + 1160]
 mov rdx, qword ptr [r12 + 1168]
 mov rcx, qword ptr [r12 + 1176]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
.Lx74_3:
 jmp xchain0_n74_α
 xchain0_n73_β:
 jmp main_γ
xchain0_n74_α:
# IR_ASSIGN global
bb75_α:
 mov rsi, qword ptr [r12 + 1136]
 mov rdx, qword ptr [r12 + 1144]
 mov rdi, qword ptr [rip + .Lx75_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 jmp main_γ
 xchain0_n74_β:
 jmp main_γ
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
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

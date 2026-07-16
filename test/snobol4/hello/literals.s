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
  mov qword ptr [rsp + 2136], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string ""
# IR_ASSIGN global
 xchain0_n1_α:
 mov rsi, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov rdi, qword ptr [rip + .Lx2_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rsp + 160], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string ""
# IR_ASSIGN global
 xchain0_n3_α:
 mov rsi, qword ptr [rsp + 160]
 mov rdx, qword ptr [rsp + 168]
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 208], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 216], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "Hello World!"
# IR_ASSIGN global
 xchain0_n5_α:
 mov rsi, qword ptr [rsp + 208]
 mov rdx, qword ptr [rsp + 216]
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rsp + 256], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rsp + 264], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx7_0:
 .quad 0
# IR_ASSIGN global
 xchain0_n7_α:
 mov rsi, qword ptr [rsp + 256]
 mov rdx, qword ptr [rsp + 264]
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [rsp + 304], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 312], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n10_α
.Lx9_0:
 .quad 1
# IR_ASSIGN global
 xchain0_n9_α:
 mov rsi, qword ptr [rsp + 304]
 mov rdx, qword ptr [rsp + 312]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rsp + 384], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 392], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n13_α
.Lx11_0:
 .quad 1
# IR_UNOP
 xchain0_n11_α:
 mov rdi, qword ptr [rsp + 384]
 mov rsi, qword ptr [rsp + 392]
 call rt_num_neg@PLT
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n13_α
# IR_ASSIGN global
 xchain0_n12_α:
 mov rsi, qword ptr [rsp + 352]
 mov rdx, qword ptr [rsp + 360]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n13_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
# IR_LIT_REAL
 xchain0_n13_α:
 mov qword ptr [rsp + 432], 7
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 440], rax
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n15_α
.Lx14_0:
 .quad 4607182418800017408
# IR_ASSIGN global
 xchain0_n14_α:
 mov rsi, qword ptr [rsp + 432]
 mov rdx, qword ptr [rsp + 440]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n15_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [rsp + 480], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 488], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n17_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "1"
# IR_ASSIGN global
 xchain0_n16_α:
 mov rsi, qword ptr [rsp + 480]
 mov rdx, qword ptr [rsp + 488]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [rsp + 528], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 536], rax
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "1"
# IR_ASSIGN global
 xchain0_n18_α:
 mov rsi, qword ptr [rsp + 528]
 mov rdx, qword ptr [rsp + 536]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n19_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [rsp + 576], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 584], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n21_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "1.0"
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [rsp + 576]
 mov rdx, qword ptr [rsp + 584]
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 560], rax
 mov qword ptr [rsp + 568], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n21_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rsp + 624], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 632], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n23_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "I'm here"
# IR_ASSIGN global
 xchain0_n22_α:
 mov rsi, qword ptr [rsp + 624]
 mov rdx, qword ptr [rsp + 632]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 608], rax
 mov qword ptr [rsp + 616], rdx
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [rsp + 672], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [rsp + 680], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n25_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "\"Quote of the day\""
# IR_ASSIGN global
 xchain0_n24_α:
 mov rsi, qword ptr [rsp + 672]
 mov rdx, qword ptr [rsp + 680]
 mov rdi, qword ptr [rip + .Lx25_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 656], rax
 mov qword ptr [rsp + 664], rdx
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [rsp + 752], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 760], rax
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n29_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n26_α:
 mov qword ptr [rsp + 784], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [rsp + 792], rax
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n29_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string ""
 xchain0_n27_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 752]
 cmp eax, 100
 je .Lx28_0
 mov eax, dword ptr [rsp + 784]
 cmp eax, 100
 je .Lx28_0
 mov eax, dword ptr [rsp + 752]
 cmp eax, 6
 jne .Lx28_2
 mov eax, dword ptr [rsp + 784]
 cmp eax, 6
 jne .Lx28_2
.Lx28_1:
 mov rax, qword ptr [rsp + 760]
 mov rcx, qword ptr [rsp + 792]
 add rax, rcx
 mov qword ptr [rsp + 720], 6
 mov qword ptr [rsp + 728], rax
 jmp xchain0_n28_α
.Lx28_0:
 mov rdi, qword ptr [rsp + 752]
 mov rsi, qword ptr [rsp + 760]
 mov rdx, qword ptr [rsp + 784]
 mov rcx, qword ptr [rsp + 792]
 mov r8d, 0
 lea r9, [rsp + 720]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx28_3
.Lx28_2:
 mov rdi, qword ptr [rsp + 752]
 mov rsi, qword ptr [rsp + 760]
 mov rdx, qword ptr [rsp + 784]
 mov rcx, qword ptr [rsp + 792]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [rsp + 720], rax
 mov qword ptr [rsp + 728], rdx
.Lx28_3:
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n29_α
# IR_ASSIGN global
 xchain0_n28_α:
 mov rsi, qword ptr [rsp + 720]
 mov rdx, qword ptr [rsp + 728]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp xchain0_n29_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rsp + 864], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rsp + 872], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp xchain0_n33_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n30_α:
 mov qword ptr [rsp + 896], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [rsp + 904], rax
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp xchain0_n33_α
.Lx31_0:
 .quad 1
 xchain0_n31_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 864]
 cmp eax, 100
 je .Lx32_0
 mov eax, dword ptr [rsp + 864]
 cmp eax, 6
 jne .Lx32_2
.Lx32_1:
 mov rax, qword ptr [rsp + 872]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 832], 6
 mov qword ptr [rsp + 840], rax
 jmp xchain0_n32_α
.Lx32_0:
 mov rdi, qword ptr [rsp + 864]
 mov rsi, qword ptr [rsp + 872]
 mov rdx, qword ptr [rsp + 896]
 mov rcx, qword ptr [rsp + 904]
 mov r8d, 0
 lea r9, [rsp + 832]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx32_3
.Lx32_2:
 mov rdi, qword ptr [rsp + 864]
 mov rsi, qword ptr [rsp + 872]
 mov rdx, qword ptr [rsp + 896]
 mov rcx, qword ptr [rsp + 904]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n33_α
 mov qword ptr [rsp + 832], rax
 mov qword ptr [rsp + 840], rdx
.Lx32_3:
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp xchain0_n33_α
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [rsp + 832]
 mov rdx, qword ptr [rsp + 840]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 816], rax
 mov qword ptr [rsp + 824], rdx
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n33_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n33_α:
 mov qword ptr [rsp + 976], 6
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [rsp + 984], rax
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n37_α
.Lx34_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n34_α:
 mov qword ptr [rsp + 1008], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [rsp + 1016], rax
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n37_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string ""
 xchain0_n35_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 1008]
 cmp eax, 100
 je .Lx36_0
 mov eax, dword ptr [rsp + 1008]
 cmp eax, 6
 jne .Lx36_2
.Lx36_1:
 mov rax, 1
 mov rcx, qword ptr [rsp + 1016]
 add rax, rcx
 mov qword ptr [rsp + 944], 6
 mov qword ptr [rsp + 952], rax
 jmp xchain0_n36_α
.Lx36_0:
 mov rdi, qword ptr [rsp + 976]
 mov rsi, qword ptr [rsp + 984]
 mov rdx, qword ptr [rsp + 1008]
 mov rcx, qword ptr [rsp + 1016]
 mov r8d, 0
 lea r9, [rsp + 944]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx36_3
.Lx36_2:
 mov rdi, qword ptr [rsp + 976]
 mov rsi, qword ptr [rsp + 984]
 mov rdx, qword ptr [rsp + 1008]
 mov rcx, qword ptr [rsp + 1016]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n37_α
 mov qword ptr [rsp + 944], rax
 mov qword ptr [rsp + 952], rdx
.Lx36_3:
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp xchain0_n37_α
# IR_ASSIGN global
 xchain0_n36_α:
 mov rsi, qword ptr [rsp + 944]
 mov rdx, qword ptr [rsp + 952]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 928], rax
 mov qword ptr [rsp + 936], rdx
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n37_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [rsp + 1088], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [rsp + 1096], rax
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n41_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n38_α:
 mov qword ptr [rsp + 1120], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rsp + 1128], rax
 jmp xchain0_n39_α
 xchain0_n38_β:
 jmp xchain0_n41_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string ""
 xchain0_n39_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 1088]
 mov rsi, qword ptr [rsp + 1096]
 mov rdx, qword ptr [rsp + 1120]
 mov rcx, qword ptr [rsp + 1128]
 call str_concat_d@PLT
 mov qword ptr [rsp + 1056], rax
 mov qword ptr [rsp + 1064], rdx
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n41_α
# IR_ASSIGN global
 xchain0_n40_α:
 mov rsi, qword ptr [rsp + 1056]
 mov rdx, qword ptr [rsp + 1064]
 mov rdi, qword ptr [rip + .Lx41_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1040], rax
 mov qword ptr [rsp + 1048], rdx
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp xchain0_n41_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [rsp + 1200], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [rsp + 1208], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n45_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [rsp + 1232], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [rsp + 1240], rax
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n45_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "Z"
 xchain0_n43_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 1200]
 mov rsi, qword ptr [rsp + 1208]
 mov rdx, qword ptr [rsp + 1232]
 mov rcx, qword ptr [rsp + 1240]
 call str_concat_d@PLT
 mov qword ptr [rsp + 1168], rax
 mov qword ptr [rsp + 1176], rdx
 jmp xchain0_n44_α
 xchain0_n43_β:
 jmp xchain0_n45_α
# IR_ASSIGN global
 xchain0_n44_α:
 mov rsi, qword ptr [rsp + 1168]
 mov rdx, qword ptr [rsp + 1176]
 mov rdi, qword ptr [rip + .Lx45_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1152], rax
 mov qword ptr [rsp + 1160], rdx
 jmp xchain0_n45_α
 xchain0_n44_β:
 jmp xchain0_n45_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n45_α:
 mov qword ptr [rsp + 1312], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [rsp + 1320], rax
 jmp xchain0_n46_α
 xchain0_n45_β:
 jmp xchain0_n49_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "A"
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [rsp + 1344], 1
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [rsp + 1352], rax
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n49_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string ""
 xchain0_n47_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 1312]
 mov rsi, qword ptr [rsp + 1320]
 mov rdx, qword ptr [rsp + 1344]
 mov rcx, qword ptr [rsp + 1352]
 call str_concat_d@PLT
 mov qword ptr [rsp + 1280], rax
 mov qword ptr [rsp + 1288], rdx
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n49_α
# IR_ASSIGN global
 xchain0_n48_α:
 mov rsi, qword ptr [rsp + 1280]
 mov rdx, qword ptr [rsp + 1288]
 mov rdi, qword ptr [rip + .Lx49_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1264], rax
 mov qword ptr [rsp + 1272], rdx
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp xchain0_n49_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [rsp + 1424], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [rsp + 1432], rax
 jmp xchain0_n50_α
 xchain0_n49_β:
 jmp xchain0_n53_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "A"
# IR_LIT_STRING
 xchain0_n50_α:
 mov qword ptr [rsp + 1456], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [rsp + 1464], rax
 jmp xchain0_n51_α
 xchain0_n50_β:
 jmp xchain0_n53_α
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "Z"
 xchain0_n51_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 1424]
 mov rsi, qword ptr [rsp + 1432]
 mov rdx, qword ptr [rsp + 1456]
 mov rcx, qword ptr [rsp + 1464]
 call str_concat_d@PLT
 mov qword ptr [rsp + 1392], rax
 mov qword ptr [rsp + 1400], rdx
 jmp xchain0_n52_α
 xchain0_n51_β:
 jmp xchain0_n53_α
# IR_ASSIGN global
 xchain0_n52_α:
 mov rsi, qword ptr [rsp + 1392]
 mov rdx, qword ptr [rsp + 1400]
 mov rdi, qword ptr [rip + .Lx53_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1376], rax
 mov qword ptr [rsp + 1384], rdx
 jmp xchain0_n53_α
 xchain0_n52_β:
 jmp xchain0_n53_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [rsp + 1536], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [rsp + 1544], rax
 jmp xchain0_n54_α
 xchain0_n53_β:
 jmp xchain0_n57_α
.Lx54_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n54_α:
 mov qword ptr [rsp + 1568], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [rsp + 1576], rax
 jmp xchain0_n55_α
 xchain0_n54_β:
 jmp xchain0_n57_α
.Lx55_0:
 .quad 2
 xchain0_n55_α:
# IR_BINOP_ARITH
.Lx56_1:
 mov rax, 1
 mov rcx, 2
 add rax, rcx
 mov qword ptr [rsp + 1504], 6
 mov qword ptr [rsp + 1512], rax
 jmp xchain0_n56_α
.Lx56_0:
 mov rdi, qword ptr [rsp + 1536]
 mov rsi, qword ptr [rsp + 1544]
 mov rdx, qword ptr [rsp + 1568]
 mov rcx, qword ptr [rsp + 1576]
 mov r8d, 0
 lea r9, [rsp + 1504]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx56_3
.Lx56_2:
 mov rdi, qword ptr [rsp + 1536]
 mov rsi, qword ptr [rsp + 1544]
 mov rdx, qword ptr [rsp + 1568]
 mov rcx, qword ptr [rsp + 1576]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n57_α
 mov qword ptr [rsp + 1504], rax
 mov qword ptr [rsp + 1512], rdx
.Lx56_3:
 jmp xchain0_n56_α
 xchain0_n55_β:
 jmp xchain0_n57_α
# IR_ASSIGN global
 xchain0_n56_α:
 mov rsi, qword ptr [rsp + 1504]
 mov rdx, qword ptr [rsp + 1512]
 mov rdi, qword ptr [rip + .Lx57_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1488], rax
 mov qword ptr [rsp + 1496], rdx
 jmp xchain0_n57_α
 xchain0_n56_β:
 jmp xchain0_n57_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n57_α:
 mov qword ptr [rsp + 1648], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [rsp + 1656], rax
 jmp xchain0_n58_α
 xchain0_n57_β:
 jmp xchain0_n62_α
.Lx58_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n58_α:
 mov qword ptr [rsp + 1712], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [rsp + 1720], rax
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp xchain0_n62_α
.Lx59_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n59_α:
 mov qword ptr [rsp + 1744], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [rsp + 1752], rax
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp xchain0_n62_α
.Lx60_0:
 .quad 3
 xchain0_n60_α:
# IR_BINOP_ARITH
.Lx61_1:
 mov rax, 2
 mov rcx, 3
 imul rax, rcx
 mov qword ptr [rsp + 1680], 6
 mov qword ptr [rsp + 1688], rax
 jmp xchain0_n61_α
.Lx61_0:
 mov rdi, qword ptr [rsp + 1712]
 mov rsi, qword ptr [rsp + 1720]
 mov rdx, qword ptr [rsp + 1744]
 mov rcx, qword ptr [rsp + 1752]
 mov r8d, 2
 lea r9, [rsp + 1680]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx61_3
.Lx61_2:
 mov rdi, qword ptr [rsp + 1712]
 mov rsi, qword ptr [rsp + 1720]
 mov rdx, qword ptr [rsp + 1744]
 mov rcx, qword ptr [rsp + 1752]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n62_α
 mov qword ptr [rsp + 1680], rax
 mov qword ptr [rsp + 1688], rdx
.Lx61_3:
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp xchain0_n62_α
 xchain0_n61_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 1680]
 cmp eax, 100
 je .Lx62_0
 mov eax, dword ptr [rsp + 1680]
 cmp eax, 6
 jne .Lx62_2
.Lx62_1:
 mov rax, 1
 mov rcx, qword ptr [rsp + 1688]
 add rax, rcx
 mov qword ptr [rsp + 1616], 6
 mov qword ptr [rsp + 1624], rax
 jmp xchain0_n63_α
.Lx62_0:
 mov rdi, qword ptr [rsp + 1648]
 mov rsi, qword ptr [rsp + 1656]
 mov rdx, qword ptr [rsp + 1680]
 mov rcx, qword ptr [rsp + 1688]
 mov r8d, 0
 lea r9, [rsp + 1616]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx62_3
.Lx62_2:
 mov rdi, qword ptr [rsp + 1648]
 mov rsi, qword ptr [rsp + 1656]
 mov rdx, qword ptr [rsp + 1680]
 mov rcx, qword ptr [rsp + 1688]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n62_α
 mov qword ptr [rsp + 1616], rax
 mov qword ptr [rsp + 1624], rdx
.Lx62_3:
 jmp xchain0_n63_α
 xchain0_n61_β:
 jmp xchain0_n62_α
# IR_LIT_INTEGER
 xchain0_n62_α:
 mov qword ptr [rsp + 1856], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [rsp + 1864], rax
 jmp xchain0_n64_α
 xchain0_n62_β:
 jmp xchain0_n67_α
.Lx63_0:
 .quad 1
# IR_ASSIGN global
 xchain0_n63_α:
 mov rsi, qword ptr [rsp + 1616]
 mov rdx, qword ptr [rsp + 1624]
 mov rdi, qword ptr [rip + .Lx64_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1600], rax
 mov qword ptr [rsp + 1608], rdx
 jmp xchain0_n62_α
 xchain0_n63_β:
 jmp xchain0_n62_α
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n64_α:
 mov qword ptr [rsp + 1888], 6
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [rsp + 1896], rax
 jmp xchain0_n65_α
 xchain0_n64_β:
 jmp xchain0_n67_α
.Lx65_0:
 .quad 2
 xchain0_n65_α:
# IR_BINOP_ARITH
.Lx66_1:
 mov rax, 1
 mov rcx, 2
 add rax, rcx
 mov qword ptr [rsp + 1824], 6
 mov qword ptr [rsp + 1832], rax
 jmp xchain0_n66_α
.Lx66_0:
 mov rdi, qword ptr [rsp + 1856]
 mov rsi, qword ptr [rsp + 1864]
 mov rdx, qword ptr [rsp + 1888]
 mov rcx, qword ptr [rsp + 1896]
 mov r8d, 0
 lea r9, [rsp + 1824]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx66_3
.Lx66_2:
 mov rdi, qword ptr [rsp + 1856]
 mov rsi, qword ptr [rsp + 1864]
 mov rdx, qword ptr [rsp + 1888]
 mov rcx, qword ptr [rsp + 1896]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [rsp + 1824], rax
 mov qword ptr [rsp + 1832], rdx
.Lx66_3:
 jmp xchain0_n66_α
 xchain0_n65_β:
 jmp xchain0_n67_α
# IR_LIT_INTEGER
 xchain0_n66_α:
 mov qword ptr [rsp + 1920], 6
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [rsp + 1928], rax
 jmp xchain0_n68_α
 xchain0_n66_β:
 jmp xchain0_n67_α
.Lx67_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n67_α:
 mov qword ptr [rsp + 2000], 6
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [rsp + 2008], rax
 jmp xchain0_n69_α
 xchain0_n67_β:
 jmp main_γ
.Lx68_0:
 .quad 1
 xchain0_n68_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 1824]
 cmp eax, 100
 je .Lx69_0
 mov eax, dword ptr [rsp + 1824]
 cmp eax, 6
 jne .Lx69_2
.Lx69_1:
 mov rax, qword ptr [rsp + 1832]
 mov rcx, 3
 imul rax, rcx
 mov qword ptr [rsp + 1792], 6
 mov qword ptr [rsp + 1800], rax
 jmp xchain0_n70_α
.Lx69_0:
 mov rdi, qword ptr [rsp + 1824]
 mov rsi, qword ptr [rsp + 1832]
 mov rdx, qword ptr [rsp + 1920]
 mov rcx, qword ptr [rsp + 1928]
 mov r8d, 2
 lea r9, [rsp + 1792]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx69_3
.Lx69_2:
 mov rdi, qword ptr [rsp + 1824]
 mov rsi, qword ptr [rsp + 1832]
 mov rdx, qword ptr [rsp + 1920]
 mov rcx, qword ptr [rsp + 1928]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [rsp + 1792], rax
 mov qword ptr [rsp + 1800], rdx
.Lx69_3:
 jmp xchain0_n70_α
 xchain0_n68_β:
 jmp xchain0_n67_α
# IR_LIT_INTEGER
 xchain0_n69_α:
 mov qword ptr [rsp + 2064], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [rsp + 2072], rax
 jmp xchain0_n71_α
 xchain0_n69_β:
 jmp main_γ
.Lx70_0:
 .quad 2
# IR_ASSIGN global
 xchain0_n70_α:
 mov rsi, qword ptr [rsp + 1792]
 mov rdx, qword ptr [rsp + 1800]
 mov rdi, qword ptr [rip + .Lx71_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1776], rax
 mov qword ptr [rsp + 1784], rdx
 jmp xchain0_n67_α
 xchain0_n70_β:
 jmp xchain0_n67_α
.Lx71_0:
 .quad .Lx71_0_s
.Lx71_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n71_α:
 mov qword ptr [rsp + 2096], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [rsp + 2104], rax
 jmp xchain0_n72_α
 xchain0_n71_β:
 jmp main_γ
.Lx72_0:
 .quad 3
 xchain0_n72_α:
# IR_BINOP_ARITH
.Lx73_1:
 mov rax, 2
 mov rcx, 3
 imul rax, rcx
 mov qword ptr [rsp + 2032], 6
 mov qword ptr [rsp + 2040], rax
 jmp xchain0_n73_α
.Lx73_0:
 mov rdi, qword ptr [rsp + 2064]
 mov rsi, qword ptr [rsp + 2072]
 mov rdx, qword ptr [rsp + 2096]
 mov rcx, qword ptr [rsp + 2104]
 mov r8d, 2
 lea r9, [rsp + 2032]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx73_3
.Lx73_2:
 mov rdi, qword ptr [rsp + 2064]
 mov rsi, qword ptr [rsp + 2072]
 mov rdx, qword ptr [rsp + 2096]
 mov rcx, qword ptr [rsp + 2104]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 2032], rax
 mov qword ptr [rsp + 2040], rdx
.Lx73_3:
 jmp xchain0_n73_α
 xchain0_n72_β:
 jmp main_γ
 xchain0_n73_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 2032]
 cmp eax, 100
 je .Lx74_0
 mov eax, dword ptr [rsp + 2032]
 cmp eax, 6
 jne .Lx74_2
.Lx74_1:
 mov rax, 1
 mov rcx, qword ptr [rsp + 2040]
 add rax, rcx
 mov qword ptr [rsp + 1968], 6
 mov qword ptr [rsp + 1976], rax
 jmp xchain0_n74_α
.Lx74_0:
 mov rdi, qword ptr [rsp + 2000]
 mov rsi, qword ptr [rsp + 2008]
 mov rdx, qword ptr [rsp + 2032]
 mov rcx, qword ptr [rsp + 2040]
 mov r8d, 0
 lea r9, [rsp + 1968]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx74_3
.Lx74_2:
 mov rdi, qword ptr [rsp + 2000]
 mov rsi, qword ptr [rsp + 2008]
 mov rdx, qword ptr [rsp + 2032]
 mov rcx, qword ptr [rsp + 2040]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 1968], rax
 mov qword ptr [rsp + 1976], rdx
.Lx74_3:
 jmp xchain0_n74_α
 xchain0_n73_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n74_α:
 mov rsi, qword ptr [rsp + 1968]
 mov rdx, qword ptr [rsp + 1976]
 mov rdi, qword ptr [rip + .Lx75_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1952], rax
 mov qword ptr [rsp + 1960], rdx
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
mov rsp, qword ptr [rsp + 2136]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 2136]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

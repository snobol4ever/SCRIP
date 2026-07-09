  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
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
 mov qword ptr [r12 + 928], rax
 pop rsi
main_α_body:
# IR_LIT_REAL
 xchain0_n0_α:
 mov qword ptr [r12 + 80], 7
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 4611686018427387904
# IR_LIT_REAL
 xchain0_n1_α:
 mov qword ptr [r12 + 96], 7
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 4613937818241073152
 xchain0_n2_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_REAL
 xchain0_n3_α:
 mov qword ptr [r12 + 112], 7
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 4617315517961601024
# IR_LIT_REAL
 xchain0_n4_α:
 mov qword ptr [r12 + 224], 7
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad 4613937818241073152
 xchain0_n5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [r12+112] -> [r12+48]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn7: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_REAL
 xchain0_n6_α:
 mov qword ptr [r12 + 240], 7
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx8_0:
 .quad 4607182418800017408
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 412/001: 2.0+3.0"
 xchain0_n8_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n10_α:
 mov qword ptr [r12 + 256], 7
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx12_0:
 .quad 4611686018427387904
# IR_LIT_REAL
 xchain0_n11_α:
 mov qword ptr [r12 + 368], 7
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx13_0:
 .quad 4613937818241073152
 xchain0_n12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+176]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+256] -> [r12+192]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn15: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_REAL
 xchain0_n13_α:
 mov qword ptr [r12 + 384], 7
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx16_0:
 .quad 4611686018427387904
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "FAIL 412/002: 3.0-1.0"
 xchain0_n15_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 mov rdx, qword ptr [r12 + 384]
 mov rcx, qword ptr [r12 + 392]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n17_α:
 mov qword ptr [r12 + 400], 7
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx20_0:
 .quad 4618441417868443648
# IR_LIT_REAL
 xchain0_n18_α:
 mov qword ptr [r12 + 512], 7
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad 4613937818241073152
 xchain0_n19_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+352] -> [r12+320]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 328], rax
# marshal arg1 = producer-box slot [r12+400] -> [r12+336]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn23: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 320]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_REAL
 xchain0_n20_α:
 mov qword ptr [r12 + 528], 7
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n25_α
.Lx24_0:
 .quad 4611686018427387904
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 412/003: 3.0*2.0"
 xchain0_n22_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n24_α:
 mov qword ptr [r12 + 544], 7
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx28_0:
 .quad 4609434218613702656
# IR_LIT_REAL
 xchain0_n25_α:
 mov qword ptr [r12 + 656], 7
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n32_α
.Lx29_0:
 .quad 4613937818241073152
 xchain0_n26_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+496] -> [r12+464]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 472], rax
# marshal arg1 = producer-box slot [r12+544] -> [r12+480]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn31: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn31]
 lea rsi, [r12 + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
xchain0_n26_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n32_α
.Lx32_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "FAIL 412/004: 3.0/2.0"
 xchain0_n29_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 656]
 mov rsi, qword ptr [r12 + 664]
 mov rdx, qword ptr [r12 + 672]
 mov rcx, qword ptr [r12 + 680]
 mov r8d, 18
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n32_α
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n32_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n31_α:
 mov qword ptr [r12 + 688], 7
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx36_0:
 .quad 4628293042053316608
# IR_LIT_REAL
 xchain0_n32_α:
 mov qword ptr [r12 + 800], 7
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n34_α
 xchain0_n32_β:
 jmp xchain0_n37_α
.Lx37_0:
 .quad 4607182418800017408
 xchain0_n33_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+640] -> [r12+608]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 616], rax
# marshal arg1 = producer-box slot [r12+688] -> [r12+624]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn39: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn39]
 lea rsi, [r12 + 608]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n35_α
xchain0_n33_β:
 jmp xchain0_n32_α
# IR_UNOP
 xchain0_n34_α:
 mov rdi, qword ptr [r12 + 800]
 mov rsi, qword ptr [r12 + 808]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n37_α
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n38_α
 xchain0_n35_β:
 jmp main_γ
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "FAIL 412/005: 3.0**3"
# IR_LIT_REAL
 xchain0_n36_α:
 mov qword ptr [r12 + 832], 7
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n39_α
 xchain0_n36_β:
 jmp xchain0_n37_α
.Lx42_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [r12 + 912], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "PASS 412_arith_real (6/6)"
# IR_ASSIGN gva
 xchain0_n38_α:
 mov rax, qword ptr [r12 + 720]
 mov rdx, qword ptr [r12 + 728]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp main_γ
 xchain0_n38_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n39_α:
 mov qword ptr [r12 + 848], 7
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n41_α
 xchain0_n39_β:
 jmp xchain0_n37_α
.Lx45_0:
 .quad 4607182418800017408
# IR_ASSIGN gva
 xchain0_n40_α:
 mov rax, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp main_γ
 xchain0_n40_β:
 jmp main_γ
 xchain0_n41_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 832]
 mov rsi, qword ptr [r12 + 840]
 mov rdx, qword ptr [r12 + 848]
 mov rcx, qword ptr [r12 + 856]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n37_α
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n37_α
 xchain0_n42_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+784] -> [r12+752]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 760], rax
# marshal arg1 = producer-box slot [r12+816] -> [r12+768]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn49: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn49]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je xchain0_n37_α
 jmp xchain0_n43_α
xchain0_n42_β:
 jmp xchain0_n37_α
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n44_α
 xchain0_n43_β:
 jmp main_γ
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 412/006: unary minus on real"
# IR_ASSIGN gva
 xchain0_n44_α:
 mov rax, qword ptr [r12 + 880]
 mov rdx, qword ptr [r12 + 888]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp main_γ
 xchain0_n44_β:
 jmp main_γ
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
 mov rdi, qword ptr [r12 + 928]
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

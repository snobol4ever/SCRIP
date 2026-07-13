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
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1616], rax
 pop rsi
main_α_body:
# IR_LIT_REAL
 xchain0_n0_α:
 mov qword ptr [r12 + 192], 7
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 4611686018427387904
# IR_LIT_REAL
 xchain0_n1_α:
 mov qword ptr [r12 + 224], 7
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 4613937818241073152
 xchain0_n2_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_REAL
 xchain0_n3_α:
 mov qword ptr [r12 + 256], 7
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 4617315517961601024
# IR_LIT_REAL
 xchain0_n4_α:
 mov qword ptr [r12 + 432], 7
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad 4613937818241073152
 xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+128]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn6: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_REAL
 xchain0_n6_α:
 mov qword ptr [r12 + 464], 7
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad 4607182418800017408
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "FAIL 412/001: 2.0+3.0"
 xchain0_n8_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 432]
 mov rsi, qword ptr [r12 + 440]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n10_α:
 mov qword ptr [r12 + 496], 7
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad 4611686018427387904
# IR_LIT_REAL
 xchain0_n11_α:
 mov qword ptr [r12 + 672], 7
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx12_0:
 .quad 4613937818241073152
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+352]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [zr+496] -> [zr+368]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_REAL
 xchain0_n13_α:
 mov qword ptr [r12 + 704], 7
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx14_0:
 .quad 4611686018427387904
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 412/002: 3.0-1.0"
 xchain0_n15_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 672]
 mov rsi, qword ptr [r12 + 680]
 mov rdx, qword ptr [r12 + 704]
 mov rcx, qword ptr [r12 + 712]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n17_α:
 mov qword ptr [r12 + 736], 7
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx18_0:
 .quad 4618441417868443648
# IR_LIT_REAL
 xchain0_n18_α:
 mov qword ptr [r12 + 912], 7
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n25_α
.Lx19_0:
 .quad 4613937818241073152
 xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+640] -> [zr+592]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 600], rax
# marshal arg1 = producer-box slot [zr+736] -> [zr+608]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_REAL
 xchain0_n20_α:
 mov qword ptr [r12 + 944], 7
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad 4611686018427387904
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 412/003: 3.0*2.0"
 xchain0_n22_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 912]
 mov rsi, qword ptr [r12 + 920]
 mov rdx, qword ptr [r12 + 944]
 mov rcx, qword ptr [r12 + 952]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n24_α:
 mov qword ptr [r12 + 976], 7
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx25_0:
 .quad 4609434218613702656
# IR_LIT_REAL
 xchain0_n25_α:
 mov qword ptr [r12 + 1152], 7
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n32_α
.Lx26_0:
 .quad 4613937818241073152
 xchain0_n26_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+832]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 840], rax
# marshal arg1 = producer-box slot [zr+976] -> [zr+848]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 856], rax
  .section .rodata
  .Lbynamefn27: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 832]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 1184], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n32_α
.Lx28_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 412/004: 3.0/2.0"
 xchain0_n29_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 1152]
 mov rsi, qword ptr [r12 + 1160]
 mov rdx, qword ptr [r12 + 1184]
 mov rcx, qword ptr [r12 + 1192]
 mov r8d, 18
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n32_α
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n32_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 1024]
 mov rdx, qword ptr [r12 + 1032]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n31_α:
 mov qword ptr [r12 + 1216], 7
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 1224], rax
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx32_0:
 .quad 4628293042053316608
# IR_LIT_REAL
 xchain0_n32_α:
 mov qword ptr [r12 + 1392], 7
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 1400], rax
 jmp xchain0_n34_α
 xchain0_n32_β:
 jmp xchain0_n37_α
.Lx33_0:
 .quad 4607182418800017408
 xchain0_n33_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1072]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1072], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1080], rax
# marshal arg1 = producer-box slot [zr+1216] -> [zr+1088]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1096], rax
  .section .rodata
  .Lbynamefn34: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 1072]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n35_α
 xchain0_n33_β:
 jmp xchain0_n32_α
# IR_UNOP
 xchain0_n34_α:
 mov rdi, qword ptr [r12 + 1392]
 mov rsi, qword ptr [r12 + 1400]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n37_α
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [r12 + 1264], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 1272], rax
 jmp xchain0_n38_α
 xchain0_n35_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "FAIL 412/005: 3.0**3"
# IR_LIT_REAL
 xchain0_n36_α:
 mov qword ptr [r12 + 1456], 7
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n39_α
 xchain0_n36_β:
 jmp xchain0_n37_α
.Lx37_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [r12 + 1584], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 1592], rax
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "PASS 412_arith_real (6/6)"
# IR_ASSIGN gva
 xchain0_n38_α:
 mov rax, qword ptr [r12 + 1264]
 mov rdx, qword ptr [r12 + 1272]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 jmp main_γ
 xchain0_n38_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n39_α:
 mov qword ptr [r12 + 1488], 7
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 1496], rax
 jmp xchain0_n41_α
 xchain0_n39_β:
 jmp xchain0_n37_α
.Lx40_0:
 .quad 4607182418800017408
# IR_ASSIGN gva
 xchain0_n40_α:
 mov rax, qword ptr [r12 + 1584]
 mov rdx, qword ptr [r12 + 1592]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 jmp main_γ
 xchain0_n40_β:
 jmp main_γ
 xchain0_n41_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 1456]
 mov rsi, qword ptr [r12 + 1464]
 mov rdx, qword ptr [r12 + 1488]
 mov rcx, qword ptr [r12 + 1496]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n37_α
 mov qword ptr [r12 + 1424], rax
 mov qword ptr [r12 + 1432], rdx
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n37_α
 xchain0_n42_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1360] -> [zr+1312]
 mov rax, qword ptr [r12 + 1360]
 mov qword ptr [r12 + 1312], rax
 mov rax, qword ptr [r12 + 1368]
 mov qword ptr [r12 + 1320], rax
# marshal arg1 = producer-box slot [zr+1424] -> [zr+1328]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1328], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1336], rax
  .section .rodata
  .Lbynamefn43: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 1312]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 cmp eax, 99
 je xchain0_n37_α
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n37_α
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [r12 + 1536], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 1544], rax
 jmp xchain0_n44_α
 xchain0_n43_β:
 jmp main_γ
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "FAIL 412/006: unary minus on real"
# IR_ASSIGN gva
 xchain0_n44_α:
 mov rax, qword ptr [r12 + 1536]
 mov rdx, qword ptr [r12 + 1544]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
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
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1616]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
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

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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1056], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+64] -> [r12+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [r12+80] -> [r12+48]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n7_α
.Lx5_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n7_α
.Lx6_0:
 .quad 7
 xchain0_n5_α:
# BOX IR_CALL max(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+160] -> [r12+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [r12+176] -> [r12+144]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lrkfn8: .string "max"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn8]
 lea rsi, [r12 + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n7_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n11_α
.Lx10_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad 7
 xchain0_n9_α:
# BOX IR_CALL min(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+256] -> [r12+224]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 232], rax
# marshal arg1 = producer-box slot [r12+272] -> [r12+240]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lrkfn13: .string "min"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn13]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n11_α
# IR_ASSIGN global
 xchain0_n10_α:
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov rdi, qword ptr [rip + .Lx14_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "OUTPUT"
# IR_LIT_REAL
 xchain0_n11_α:
 mov qword ptr [r12 + 352], 7
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n15_α
.Lx15_0:
 .quad 4615063718147915776
# IR_LIT_REAL
 xchain0_n12_α:
 mov qword ptr [r12 + 368], 7
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n15_α
.Lx16_0:
 .quad 4611911198408756429
 xchain0_n13_α:
# BOX IR_CALL max(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+352] -> [r12+320]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 328], rax
# marshal arg1 = producer-box slot [r12+368] -> [r12+336]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lrkfn18: .string "max"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn18]
 lea rsi, [r12 + 320]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je xchain0_n15_α
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n15_α
# IR_ASSIGN global
 xchain0_n14_α:
 mov rsi, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n15_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "OUTPUT"
# IR_LIT_REAL
 xchain0_n15_α:
 mov qword ptr [r12 + 448], 7
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n19_α
.Lx20_0:
 .quad 4615063718147915776
# IR_LIT_REAL
 xchain0_n16_α:
 mov qword ptr [r12 + 464], 7
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n19_α
.Lx21_0:
 .quad 4611911198408756429
 xchain0_n17_α:
# BOX IR_CALL min(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+448] -> [r12+416]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 424], rax
# marshal arg1 = producer-box slot [r12+464] -> [r12+432]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lrkfn23: .string "min"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn23]
 lea rsi, [r12 + 416]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
# IR_ASSIGN global
 xchain0_n18_α:
 mov rsi, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n19_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [r12 + 544], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n22_α
.Lx25_0:
 .quad 42
# IR_UNOP
 xchain0_n20_α:
 mov rdi, qword ptr [r12 + 544]
 mov rsi, qword ptr [r12 + 552]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n22_α
 xchain0_n21_α:
# BOX IR_CALL abs(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+528] -> [r12+512]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 520], rax
  .section .rodata
  .Lrkfn28: .string "abs"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn28]
 lea rsi, [r12 + 512]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain0_n22_α
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp xchain0_n22_α
# IR_LIT_INTEGER
 xchain0_n22_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n26_α
.Lx29_0:
 .quad 0
# IR_ASSIGN global
 xchain0_n23_α:
 mov rsi, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n22_α
 xchain0_n23_β:
 jmp xchain0_n22_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "OUTPUT"
 xchain0_n24_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+608] -> [r12+592]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 600], rax
  .section .rodata
  .Lbynamefn32: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 592]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n25_α
xchain0_n24_β:
 jmp xchain0_n26_α
# IR_ASSIGN global
 xchain0_n25_α:
 mov rsi, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n29_α
.Lx34_0:
 .quad 5
 xchain0_n27_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+672] -> [r12+656]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 664], rax
  .section .rodata
  .Lbynamefn36: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn36]
 lea rsi, [r12 + 656]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
xchain0_n27_β:
 jmp xchain0_n29_α
# IR_ASSIGN global
 xchain0_n28_α:
 mov rsi, qword ptr [r12 + 640]
 mov rdx, qword ptr [r12 + 648]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp xchain0_n29_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n29_α:
 mov qword ptr [r12 + 752], 6
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp xchain0_n32_α
.Lx38_0:
 .quad 3
# IR_UNOP
 xchain0_n30_α:
 mov rdi, qword ptr [r12 + 752]
 mov rsi, qword ptr [r12 + 760]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp xchain0_n32_α
 xchain0_n31_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+736] -> [r12+720]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn41: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 720]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n33_α
xchain0_n31_β:
 jmp xchain0_n32_α
# IR_LIT_INTEGER
 xchain0_n32_α:
 mov qword ptr [r12 + 832], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n34_α
 xchain0_n32_β:
 jmp xchain0_n37_α
.Lx42_0:
 .quad 12
# IR_ASSIGN global
 xchain0_n33_α:
 mov rsi, qword ptr [r12 + 704]
 mov rdx, qword ptr [r12 + 712]
 mov rdi, qword ptr [rip + .Lx43_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n32_α
 xchain0_n33_β:
 jmp xchain0_n32_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n34_α:
 mov qword ptr [r12 + 848], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n37_α
.Lx44_0:
 .quad 8
 xchain0_n35_α:
# BOX IR_CALL gcd(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+832] -> [r12+800]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 808], rax
# marshal arg1 = producer-box slot [r12+848] -> [r12+816]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn46: .string "gcd"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 800]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 cmp eax, 99
 je xchain0_n37_α
 jmp xchain0_n36_α
xchain0_n35_β:
 jmp xchain0_n37_α
# IR_ASSIGN global
 xchain0_n36_α:
 mov rsi, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov rdi, qword ptr [rip + .Lx47_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n37_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [r12 + 928], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n41_α
.Lx48_0:
 .quad 100
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [r12 + 944], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n39_α
 xchain0_n38_β:
 jmp xchain0_n41_α
.Lx49_0:
 .quad 75
 xchain0_n39_α:
# BOX IR_CALL gcd(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+928] -> [r12+896]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 896], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 904], rax
# marshal arg1 = producer-box slot [r12+944] -> [r12+912]
 mov rax, qword ptr [r12 + 944]
 mov qword ptr [r12 + 912], rax
 mov rax, qword ptr [r12 + 952]
 mov qword ptr [r12 + 920], rax
  .section .rodata
  .Lbynamefn51: .string "gcd"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn51]
 lea rsi, [r12 + 896]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n40_α
xchain0_n39_β:
 jmp xchain0_n41_α
# IR_ASSIGN global
 xchain0_n40_α:
 mov rsi, qword ptr [r12 + 880]
 mov rdx, qword ptr [r12 + 888]
 mov rdi, qword ptr [rip + .Lx52_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp xchain0_n41_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n41_α:
 mov qword ptr [r12 + 1024], 6
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp main_γ
.Lx53_0:
 .quad 4
# IR_LIT_INTEGER
 xchain0_n42_α:
 mov qword ptr [r12 + 1040], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1048], rax
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp main_γ
.Lx54_0:
 .quad 6
 xchain0_n43_α:
# BOX IR_CALL lcm(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1024] -> [r12+992]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 1000], rax
# marshal arg1 = producer-box slot [r12+1040] -> [r12+1008]
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 1016], rax
  .section .rodata
  .Lbynamefn56: .string "lcm"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn56]
 lea rsi, [r12 + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n44_α
xchain0_n43_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n44_α:
 mov rsi, qword ptr [r12 + 976]
 mov rdx, qword ptr [r12 + 984]
 mov rdi, qword ptr [rip + .Lx57_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp main_γ
 xchain0_n44_β:
 jmp main_γ
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
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
 mov rdi, qword ptr [r12 + 1056]
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

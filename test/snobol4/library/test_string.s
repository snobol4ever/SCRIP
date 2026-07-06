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
xchain0_n1_α:
# IR_LIT_INTEGER
bb2_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
xchain0_n2_α:
bb3_α:
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
xchain0_n3_α:
# IR_LIT_STRING
bb4_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "hi"
xchain0_n4_α:
# IR_LIT_INTEGER
bb5_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n8_α
.Lx6_0:
 .quad 6
xchain0_n5_α:
# IR_LIT_STRING
bb6_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n8_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "*"
xchain0_n6_α:
bb7_α:
# BOX IR_CALL pad_left(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+176] -> [r12+128]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [r12+192] -> [r12+144]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 152], rax
# marshal arg2 = producer-box slot [r12+208] -> [r12+160]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn9: .string "pad_left"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 128]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
xchain0_n6_β:
 jmp xchain0_n8_α
xchain0_n7_α:
# IR_ASSIGN global
bb8_α:
 mov rsi, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
xchain0_n8_α:
# IR_LIT_STRING
bb9_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n13_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "hi"
xchain0_n9_α:
# IR_LIT_INTEGER
bb10_α:
 mov qword ptr [r12 + 320], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n13_α
.Lx12_0:
 .quad 6
xchain0_n10_α:
# IR_LIT_STRING
bb11_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n13_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "*"
xchain0_n11_α:
bb12_α:
# BOX IR_CALL pad_right(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+304] -> [r12+256]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [r12+320] -> [r12+272]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 280], rax
# marshal arg2 = producer-box slot [r12+336] -> [r12+288]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn15: .string "pad_right"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 256]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n12_α
xchain0_n11_β:
 jmp xchain0_n13_α
xchain0_n12_α:
# IR_ASSIGN global
bb13_α:
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov rdi, qword ptr [rip + .Lx16_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n13_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "OUTPUT"
xchain0_n13_α:
# IR_LIT_STRING
bb14_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n16_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "   hello"
xchain0_n14_α:
bb15_α:
# BOX IR_CALL ltrim(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+400] -> [r12+384]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn19: .string "ltrim"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 384]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
xchain0_n14_β:
 jmp xchain0_n16_α
xchain0_n15_α:
# IR_ASSIGN global
bb16_α:
 mov rsi, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
xchain0_n16_α:
# IR_LIT_STRING
bb17_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n19_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "hello   "
xchain0_n17_α:
bb18_α:
# BOX IR_CALL rtrim(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+464] -> [r12+448]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn23: .string "rtrim"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
xchain0_n17_β:
 jmp xchain0_n19_α
xchain0_n18_α:
# IR_ASSIGN global
bb19_α:
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n19_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
xchain0_n19_α:
# IR_LIT_STRING
bb20_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n22_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "  hello  "
xchain0_n20_α:
bb21_α:
# BOX IR_CALL trimws(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+528] -> [r12+512]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 520], rax
  .section .rodata
  .Lbynamefn27: .string "trimws"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 512]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain0_n22_α
 jmp xchain0_n21_α
xchain0_n20_β:
 jmp xchain0_n22_α
xchain0_n21_α:
# IR_ASSIGN global
bb22_α:
 mov rsi, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n22_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "OUTPUT"
xchain0_n22_α:
# IR_LIT_STRING
bb23_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n26_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "hi"
xchain0_n23_α:
# IR_LIT_INTEGER
bb24_α:
 mov qword ptr [r12 + 624], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n26_α
.Lx30_0:
 .quad 3
xchain0_n24_α:
bb25_α:
# BOX IR_CALL repeat(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+608] -> [r12+576]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 584], rax
# marshal arg1 = producer-box slot [r12+624] -> [r12+592]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 600], rax
  .section .rodata
  .Lbynamefn32: .string "repeat"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 576]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n25_α
xchain0_n24_β:
 jmp xchain0_n26_α
xchain0_n25_α:
# IR_ASSIGN global
bb26_α:
 mov rsi, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "OUTPUT"
xchain0_n26_α:
# IR_LIT_STRING
bb27_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n30_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "foobar"
xchain0_n27_α:
# IR_LIT_STRING
bb28_α:
 mov qword ptr [r12 + 704], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n30_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "oba"
xchain0_n28_α:
bb29_α:
# BOX IR_CALL contains(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+688] -> [r12+656]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 664], rax
# marshal arg1 = producer-box slot [r12+704] -> [r12+672]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn37: .string "contains"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n30_α
 jmp xchain0_n29_α
xchain0_n28_β:
 jmp xchain0_n30_α
xchain0_n29_α:
# IR_LIT_STRING
bb30_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n33_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "contains ok"
xchain0_n30_α:
# IR_LIT_STRING
bb31_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n32_α
 xchain0_n30_β:
 jmp xchain0_n33_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "FAIL: contains"
xchain0_n31_α:
# IR_ASSIGN global
bb32_α:
 mov rsi, qword ptr [r12 + 736]
 mov rdx, qword ptr [r12 + 744]
 mov rdi, qword ptr [rip + .Lx40_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n33_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "OUTPUT"
xchain0_n32_α:
# IR_ASSIGN global
bb33_α:
 mov rsi, qword ptr [r12 + 768]
 mov rdx, qword ptr [r12 + 776]
 mov rdi, qword ptr [rip + .Lx41_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n33_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "OUTPUT"
xchain0_n33_α:
# IR_LIT_STRING
bb34_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n37_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "foobar"
xchain0_n34_α:
# IR_LIT_STRING
bb35_α:
 mov qword ptr [r12 + 848], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n37_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "foo"
xchain0_n35_α:
bb36_α:
# BOX IR_CALL startswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn45: .string "startswith"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn45]
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
xchain0_n36_α:
# IR_LIT_STRING
bb37_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp xchain0_n40_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "startswith ok"
xchain0_n37_α:
# IR_LIT_STRING
bb38_α:
 mov qword ptr [r12 + 912], 1
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n39_α
 xchain0_n37_β:
 jmp xchain0_n40_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "FAIL: startswith"
xchain0_n38_α:
# IR_ASSIGN global
bb39_α:
 mov rsi, qword ptr [r12 + 880]
 mov rdx, qword ptr [r12 + 888]
 mov rdi, qword ptr [rip + .Lx48_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n40_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "OUTPUT"
xchain0_n39_α:
# IR_ASSIGN global
bb40_α:
 mov rsi, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
 mov rdi, qword ptr [rip + .Lx49_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n40_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "OUTPUT"
xchain0_n40_α:
# IR_LIT_STRING
bb41_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp xchain0_n44_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "foobar"
xchain0_n41_α:
# IR_LIT_STRING
bb42_α:
 mov qword ptr [r12 + 992], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n44_α
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "bar"
xchain0_n42_α:
bb43_α:
# BOX IR_CALL endswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+976] -> [r12+944]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 944], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 952], rax
# marshal arg1 = producer-box slot [r12+992] -> [r12+960]
 mov rax, qword ptr [r12 + 992]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 1000]
 mov qword ptr [r12 + 968], rax
  .section .rodata
  .Lbynamefn53: .string "endswith"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn53]
 lea rsi, [r12 + 944]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 cmp eax, 99
 je xchain0_n44_α
 jmp xchain0_n43_α
xchain0_n42_β:
 jmp xchain0_n44_α
xchain0_n43_α:
# IR_LIT_STRING
bb44_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp xchain0_n47_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "endswith ok"
xchain0_n44_α:
# IR_LIT_STRING
bb45_α:
 mov qword ptr [r12 + 1056], 1
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 1064], rax
 jmp xchain0_n46_α
 xchain0_n44_β:
 jmp xchain0_n47_α
.Lx55_0:
 .quad .Lx55_0_s
.Lx55_0_s:
 .string "FAIL: endswith"
xchain0_n45_α:
# IR_ASSIGN global
bb46_α:
 mov rsi, qword ptr [r12 + 1024]
 mov rdx, qword ptr [r12 + 1032]
 mov rdi, qword ptr [rip + .Lx56_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xchain0_n47_α
 xchain0_n45_β:
 jmp xchain0_n47_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "OUTPUT"
xchain0_n46_α:
# IR_ASSIGN global
bb47_α:
 mov rsi, qword ptr [r12 + 1056]
 mov rdx, qword ptr [r12 + 1064]
 mov rdi, qword ptr [rip + .Lx57_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n47_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "OUTPUT"
xchain0_n47_α:
# IR_LIT_STRING
bb48_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n51_α
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "foobar"
xchain0_n48_α:
# IR_LIT_STRING
bb49_α:
 mov qword ptr [r12 + 1136], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp xchain0_n51_α
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "bar"
xchain0_n49_α:
bb50_α:
# BOX IR_CALL startswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1120] -> [r12+1088]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1096], rax
# marshal arg1 = producer-box slot [r12+1136] -> [r12+1104]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1104], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1112], rax
  .section .rodata
  .Lbynamefn61: .string "startswith"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn61]
 lea rsi, [r12 + 1088]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 cmp eax, 99
 je xchain0_n51_α
 jmp xchain0_n50_α
xchain0_n49_β:
 jmp xchain0_n51_α
xchain0_n50_α:
# IR_LIT_STRING
bb51_α:
 mov qword ptr [r12 + 1200], 1
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 1208], rax
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp xchain0_n54_α
.Lx62_0:
 .quad .Lx62_0_s
.Lx62_0_s:
 .string "FAIL: startswith matched wrong"
xchain0_n51_α:
# IR_LIT_STRING
bb52_α:
 mov qword ptr [r12 + 1168], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n53_α
 xchain0_n51_β:
 jmp xchain0_n54_α
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string "no startswith ok"
xchain0_n52_α:
# IR_ASSIGN global
bb53_α:
 mov rsi, qword ptr [r12 + 1200]
 mov rdx, qword ptr [r12 + 1208]
 mov rdi, qword ptr [rip + .Lx64_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp xchain0_n54_α
 xchain0_n52_β:
 jmp xchain0_n54_α
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string "OUTPUT"
xchain0_n53_α:
# IR_ASSIGN global
bb54_α:
 mov rsi, qword ptr [r12 + 1168]
 mov rdx, qword ptr [r12 + 1176]
 mov rdi, qword ptr [rip + .Lx65_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp xchain0_n54_α
 xchain0_n53_β:
 jmp xchain0_n54_α
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "OUTPUT"
xchain0_n54_α:
# IR_LIT_STRING
bb55_α:
 mov qword ptr [r12 + 1280], 1
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n55_α
 xchain0_n54_β:
 jmp xchain0_n58_α
.Lx66_0:
 .quad .Lx66_0_s
.Lx66_0_s:
 .string "foobar"
xchain0_n55_α:
# IR_LIT_STRING
bb56_α:
 mov qword ptr [r12 + 1296], 1
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n56_α
 xchain0_n55_β:
 jmp xchain0_n58_α
.Lx67_0:
 .quad .Lx67_0_s
.Lx67_0_s:
 .string "oba"
xchain0_n56_α:
bb57_α:
# BOX IR_CALL index(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1280] -> [r12+1248]
 mov rax, qword ptr [r12 + 1280]
 mov qword ptr [r12 + 1248], rax
 mov rax, qword ptr [r12 + 1288]
 mov qword ptr [r12 + 1256], rax
# marshal arg1 = producer-box slot [r12+1296] -> [r12+1264]
 mov rax, qword ptr [r12 + 1296]
 mov qword ptr [r12 + 1264], rax
 mov rax, qword ptr [r12 + 1304]
 mov qword ptr [r12 + 1272], rax
  .section .rodata
  .Lbynamefn69: .string "index"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn69]
 lea rsi, [r12 + 1248]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 cmp eax, 99
 je xchain0_n58_α
 jmp xchain0_n57_α
xchain0_n56_β:
 jmp xchain0_n58_α
xchain0_n57_α:
# IR_ASSIGN global
bb58_α:
 mov rsi, qword ptr [r12 + 1232]
 mov rdx, qword ptr [r12 + 1240]
 mov rdi, qword ptr [rip + .Lx70_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 jmp xchain0_n58_α
 xchain0_n57_β:
 jmp xchain0_n58_α
.Lx70_0:
 .quad .Lx70_0_s
.Lx70_0_s:
 .string "OUTPUT"
xchain0_n58_α:
# IR_LIT_STRING
bb59_α:
 mov qword ptr [r12 + 1376], 1
 mov rax, qword ptr [rip + .Lx71_0]
 mov qword ptr [r12 + 1384], rax
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp main_γ
.Lx71_0:
 .quad .Lx71_0_s
.Lx71_0_s:
 .string "foobar"
xchain0_n59_α:
# IR_LIT_STRING
bb60_α:
 mov qword ptr [r12 + 1392], 1
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 1400], rax
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp main_γ
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "xyz"
xchain0_n60_α:
bb61_α:
# BOX IR_CALL index(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1376] -> [r12+1344]
 mov rax, qword ptr [r12 + 1376]
 mov qword ptr [r12 + 1344], rax
 mov rax, qword ptr [r12 + 1384]
 mov qword ptr [r12 + 1352], rax
# marshal arg1 = producer-box slot [r12+1392] -> [r12+1360]
 mov rax, qword ptr [r12 + 1392]
 mov qword ptr [r12 + 1360], rax
 mov rax, qword ptr [r12 + 1400]
 mov qword ptr [r12 + 1368], rax
  .section .rodata
  .Lbynamefn74: .string "index"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 1344]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n61_α
xchain0_n60_β:
 jmp main_γ
xchain0_n61_α:
# IR_ASSIGN global
bb62_α:
 mov rsi, qword ptr [r12 + 1328]
 mov rdx, qword ptr [r12 + 1336]
 mov rdi, qword ptr [rip + .Lx75_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp main_γ
 xchain0_n61_β:
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

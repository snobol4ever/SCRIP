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
  mov qword ptr [rsp + 1688], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [rbp + 192]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 200]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn4]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rbp + 304], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 312], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n7_α
.Lx5_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rbp + 336], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n7_α
.Lx6_0:
 .quad 7
 xchain0_n5_α:
# BOX IR_CALL max(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+304] -> [zr+256]
 mov rax, qword ptr [rbp + 304]
 mov qword ptr [rbp + 256], rax
 mov rax, qword ptr [rbp + 312]
 mov qword ptr [rbp + 264], rax
# marshal arg1 = producer-box slot [zr+336] -> [zr+272]
 mov rax, qword ptr [rbp + 336]
 mov qword ptr [rbp + 272], rax
 mov rax, qword ptr [rbp + 344]
 mov qword ptr [rbp + 280], rax
  .section .rodata
  .Lrkfn8: .string "max"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn8]
 lea rsi, [rbp + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 240], rax
 mov qword ptr [rbp + 248], rdx
 cmp eax, 99
 je xchain0_n7_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [rbp + 240]
 mov rdx, qword ptr [rbp + 248]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rbp + 448], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 456], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n11_α
.Lx10_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [rbp + 480], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rbp + 488], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad 7
 xchain0_n9_α:
# BOX IR_CALL min(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+448] -> [zr+400]
 mov rax, qword ptr [rbp + 448]
 mov qword ptr [rbp + 400], rax
 mov rax, qword ptr [rbp + 456]
 mov qword ptr [rbp + 408], rax
# marshal arg1 = producer-box slot [zr+480] -> [zr+416]
 mov rax, qword ptr [rbp + 480]
 mov qword ptr [rbp + 416], rax
 mov rax, qword ptr [rbp + 488]
 mov qword ptr [rbp + 424], rax
  .section .rodata
  .Lrkfn13: .string "min"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn13]
 lea rsi, [rbp + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n11_α
# IR_ASSIGN global
 xchain0_n10_α:
 mov rsi, qword ptr [rbp + 384]
 mov rdx, qword ptr [rbp + 392]
 mov rdi, qword ptr [rip + .Lx14_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "OUTPUT"
# IR_LIT_REAL
 xchain0_n11_α:
 mov qword ptr [rbp + 592], 7
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 600], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n15_α
.Lx15_0:
 .quad 4615063718147915776
# IR_LIT_REAL
 xchain0_n12_α:
 mov qword ptr [rbp + 624], 7
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rbp + 632], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n15_α
.Lx16_0:
 .quad 4611911198408756429
 xchain0_n13_α:
# BOX IR_CALL max(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+592] -> [zr+544]
 mov rax, qword ptr [rbp + 592]
 mov qword ptr [rbp + 544], rax
 mov rax, qword ptr [rbp + 600]
 mov qword ptr [rbp + 552], rax
# marshal arg1 = producer-box slot [zr+624] -> [zr+560]
 mov rax, qword ptr [rbp + 624]
 mov qword ptr [rbp + 560], rax
 mov rax, qword ptr [rbp + 632]
 mov qword ptr [rbp + 568], rax
  .section .rodata
  .Lrkfn18: .string "max"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn18]
 lea rsi, [rbp + 544]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 cmp eax, 99
 je xchain0_n15_α
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n15_α
# IR_ASSIGN global
 xchain0_n14_α:
 mov rsi, qword ptr [rbp + 528]
 mov rdx, qword ptr [rbp + 536]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n15_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "OUTPUT"
# IR_LIT_REAL
 xchain0_n15_α:
 mov qword ptr [rbp + 736], 7
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rbp + 744], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n19_α
.Lx20_0:
 .quad 4615063718147915776
# IR_LIT_REAL
 xchain0_n16_α:
 mov qword ptr [rbp + 768], 7
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rbp + 776], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n19_α
.Lx21_0:
 .quad 4611911198408756429
 xchain0_n17_α:
# BOX IR_CALL min(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+736] -> [zr+688]
 mov rax, qword ptr [rbp + 736]
 mov qword ptr [rbp + 688], rax
 mov rax, qword ptr [rbp + 744]
 mov qword ptr [rbp + 696], rax
# marshal arg1 = producer-box slot [zr+768] -> [zr+704]
 mov rax, qword ptr [rbp + 768]
 mov qword ptr [rbp + 704], rax
 mov rax, qword ptr [rbp + 776]
 mov qword ptr [rbp + 712], rax
  .section .rodata
  .Lrkfn23: .string "min"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn23]
 lea rsi, [rbp + 688]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 680], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
# IR_ASSIGN global
 xchain0_n18_α:
 mov rsi, qword ptr [rbp + 672]
 mov rdx, qword ptr [rbp + 680]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n19_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rbp + 896], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rbp + 904], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n22_α
.Lx25_0:
 .quad 42
# IR_UNOP
 xchain0_n20_α:
 mov rdi, qword ptr [rbp + 896]
 mov rsi, qword ptr [rbp + 904]
 call rt_num_neg@PLT
 mov qword ptr [rbp + 864], rax
 mov qword ptr [rbp + 872], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n22_α
 xchain0_n21_α:
# BOX IR_CALL abs(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+864] -> [zr+832]
 mov rax, qword ptr [rbp + 864]
 mov qword ptr [rbp + 832], rax
 mov rax, qword ptr [rbp + 872]
 mov qword ptr [rbp + 840], rax
  .section .rodata
  .Lrkfn28: .string "abs"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn28]
 lea rsi, [rbp + 832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 816], rax
 mov qword ptr [rbp + 824], rdx
 cmp eax, 99
 je xchain0_n22_α
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp xchain0_n22_α
# IR_LIT_INTEGER
 xchain0_n22_α:
 mov qword ptr [rbp + 992], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rbp + 1000], rax
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n26_α
.Lx29_0:
 .quad 0
# IR_ASSIGN global
 xchain0_n23_α:
 mov rsi, qword ptr [rbp + 816]
 mov rdx, qword ptr [rbp + 824]
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 800], rax
 mov qword ptr [rbp + 808], rdx
 jmp xchain0_n22_α
 xchain0_n23_β:
 jmp xchain0_n22_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "OUTPUT"
 xchain0_n24_α:
# BOX CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+992] -> [zr+960]
 mov rax, qword ptr [rbp + 992]
 mov qword ptr [rbp + 960], rax
 mov rax, qword ptr [rbp + 1000]
 mov qword ptr [rbp + 968], rax
  .section .rodata
  .Lbynamefn25: .string "sign"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn25]
 lea rsi, [rbp + 960]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 944], rax
 mov qword ptr [rbp + 952], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n26_α
# IR_ASSIGN global
 xchain0_n25_α:
 mov rsi, qword ptr [rbp + 944]
 mov rdx, qword ptr [rbp + 952]
 mov rdi, qword ptr [rip + .Lx32_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 928], rax
 mov qword ptr [rbp + 936], rdx
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [rbp + 1088], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [rbp + 1096], rax
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n29_α
.Lx33_0:
 .quad 5
 xchain0_n27_α:
# BOX CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1088] -> [zr+1056]
 mov rax, qword ptr [rbp + 1088]
 mov qword ptr [rbp + 1056], rax
 mov rax, qword ptr [rbp + 1096]
 mov qword ptr [rbp + 1064], rax
  .section .rodata
  .Lbynamefn28: .string "sign"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [rbp + 1056]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1040], rax
 mov qword ptr [rbp + 1048], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n29_α
# IR_ASSIGN global
 xchain0_n28_α:
 mov rsi, qword ptr [rbp + 1040]
 mov rdx, qword ptr [rbp + 1048]
 mov rdi, qword ptr [rip + .Lx35_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1024], rax
 mov qword ptr [rbp + 1032], rdx
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp xchain0_n29_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n29_α:
 mov qword ptr [rbp + 1216], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rbp + 1224], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp xchain0_n32_α
.Lx36_0:
 .quad 3
# IR_UNOP
 xchain0_n30_α:
 mov rdi, qword ptr [rbp + 1216]
 mov rsi, qword ptr [rbp + 1224]
 call rt_num_neg@PLT
 mov qword ptr [rbp + 1184], rax
 mov qword ptr [rbp + 1192], rdx
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp xchain0_n32_α
 xchain0_n31_α:
# BOX CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1184] -> [zr+1152]
 mov rax, qword ptr [rbp + 1184]
 mov qword ptr [rbp + 1152], rax
 mov rax, qword ptr [rbp + 1192]
 mov qword ptr [rbp + 1160], rax
  .section .rodata
  .Lbynamefn32: .string "sign"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn32]
 lea rsi, [rbp + 1152]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1136], rax
 mov qword ptr [rbp + 1144], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n32_α
# IR_LIT_INTEGER
 xchain0_n32_α:
 mov qword ptr [rbp + 1328], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rbp + 1336], rax
 jmp xchain0_n34_α
 xchain0_n32_β:
 jmp xchain0_n37_α
.Lx39_0:
 .quad 12
# IR_ASSIGN global
 xchain0_n33_α:
 mov rsi, qword ptr [rbp + 1136]
 mov rdx, qword ptr [rbp + 1144]
 mov rdi, qword ptr [rip + .Lx40_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1120], rax
 mov qword ptr [rbp + 1128], rdx
 jmp xchain0_n32_α
 xchain0_n33_β:
 jmp xchain0_n32_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n34_α:
 mov qword ptr [rbp + 1360], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [rbp + 1368], rax
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n37_α
.Lx41_0:
 .quad 8
 xchain0_n35_α:
# BOX CALL gcd(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1328] -> [zr+1280]
 mov rax, qword ptr [rbp + 1328]
 mov qword ptr [rbp + 1280], rax
 mov rax, qword ptr [rbp + 1336]
 mov qword ptr [rbp + 1288], rax
# marshal arg1 = producer-box slot [zr+1360] -> [zr+1296]
 mov rax, qword ptr [rbp + 1360]
 mov qword ptr [rbp + 1296], rax
 mov rax, qword ptr [rbp + 1368]
 mov qword ptr [rbp + 1304], rax
  .section .rodata
  .Lbynamefn36: .string "gcd"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn36]
 lea rsi, [rbp + 1280]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1264], rax
 mov qword ptr [rbp + 1272], rdx
 cmp eax, 99
 je xchain0_n37_α
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp xchain0_n37_α
# IR_ASSIGN global
 xchain0_n36_α:
 mov rsi, qword ptr [rbp + 1264]
 mov rdx, qword ptr [rbp + 1272]
 mov rdi, qword ptr [rip + .Lx43_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1248], rax
 mov qword ptr [rbp + 1256], rdx
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n37_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [rbp + 1472], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [rbp + 1480], rax
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n41_α
.Lx44_0:
 .quad 100
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [rbp + 1504], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [rbp + 1512], rax
 jmp xchain0_n39_α
 xchain0_n38_β:
 jmp xchain0_n41_α
.Lx45_0:
 .quad 75
 xchain0_n39_α:
# BOX CALL gcd(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1472] -> [zr+1424]
 mov rax, qword ptr [rbp + 1472]
 mov qword ptr [rbp + 1424], rax
 mov rax, qword ptr [rbp + 1480]
 mov qword ptr [rbp + 1432], rax
# marshal arg1 = producer-box slot [zr+1504] -> [zr+1440]
 mov rax, qword ptr [rbp + 1504]
 mov qword ptr [rbp + 1440], rax
 mov rax, qword ptr [rbp + 1512]
 mov qword ptr [rbp + 1448], rax
  .section .rodata
  .Lbynamefn40: .string "gcd"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn40]
 lea rsi, [rbp + 1424]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1408], rax
 mov qword ptr [rbp + 1416], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n41_α
# IR_ASSIGN global
 xchain0_n40_α:
 mov rsi, qword ptr [rbp + 1408]
 mov rdx, qword ptr [rbp + 1416]
 mov rdi, qword ptr [rip + .Lx47_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1392], rax
 mov qword ptr [rbp + 1400], rdx
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp xchain0_n41_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n41_α:
 mov qword ptr [rbp + 1616], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [rbp + 1624], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp main_γ
.Lx48_0:
 .quad 4
# IR_LIT_INTEGER
 xchain0_n42_α:
 mov qword ptr [rbp + 1648], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [rbp + 1656], rax
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp main_γ
.Lx49_0:
 .quad 6
 xchain0_n43_α:
# BOX CALL lcm(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1616] -> [zr+1568]
 mov rax, qword ptr [rbp + 1616]
 mov qword ptr [rbp + 1568], rax
 mov rax, qword ptr [rbp + 1624]
 mov qword ptr [rbp + 1576], rax
# marshal arg1 = producer-box slot [zr+1648] -> [zr+1584]
 mov rax, qword ptr [rbp + 1648]
 mov qword ptr [rbp + 1584], rax
 mov rax, qword ptr [rbp + 1656]
 mov qword ptr [rbp + 1592], rax
  .section .rodata
  .Lbynamefn44: .string "lcm"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn44]
 lea rsi, [rbp + 1568]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1552], rax
 mov qword ptr [rbp + 1560], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n44_α
 xchain0_n43_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n44_α:
 mov rsi, qword ptr [rbp + 1552]
 mov rdx, qword ptr [rbp + 1560]
 mov rdi, qword ptr [rip + .Lx51_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 1536], rax
 mov qword ptr [rbp + 1544], rdx
 jmp main_γ
 xchain0_n44_β:
 jmp main_γ
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1688]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1688]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

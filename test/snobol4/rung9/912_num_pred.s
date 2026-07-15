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
  mov qword ptr [rsp + 3384], rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 4
 xchain0_n2_α:
# BOX CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 200]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lbynamefn3: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rsp + 240], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 248], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp main_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "FAIL 912/001: lt(5,4) should fail"
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 336], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 344], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n9_α
.Lx5_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [rsp + 240]
 mov rdx, qword ptr [rsp + 248]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp main_γ
 xchain0_n5_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rsp + 368], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rsp + 376], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx7_0:
 .quad 4
 xchain0_n7_α:
# BOX CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+288]
 mov rax, qword ptr [rsp + 336]
 mov qword ptr [rsp + 288], rax
 mov rax, qword ptr [rsp + 344]
 mov qword ptr [rsp + 296], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+304]
 mov rax, qword ptr [rsp + 368]
 mov qword ptr [rsp + 304], rax
 mov rax, qword ptr [rsp + 376]
 mov qword ptr [rsp + 312], rax
  .section .rodata
  .Lbynamefn8: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rsp + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 416], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 424], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 912/002: lt(4,4) should fail"
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [rsp + 512], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 520], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n14_α
.Lx10_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [rsp + 416]
 mov rdx, qword ptr [rsp + 424]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [rsp + 544], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 552], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n14_α
.Lx12_0:
 .quad 5
 xchain0_n12_α:
# BOX CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
 mov rax, qword ptr [rsp + 512]
 mov qword ptr [rsp + 464], rax
 mov rax, qword ptr [rsp + 520]
 mov qword ptr [rsp + 472], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+480]
 mov rax, qword ptr [rsp + 544]
 mov qword ptr [rsp + 480], rax
 mov rax, qword ptr [rsp + 552]
 mov qword ptr [rsp + 488], rax
  .section .rodata
  .Lbynamefn13: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [rsp + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rsp + 688], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 696], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n19_α
.Lx14_0:
 .quad 5
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rsp + 592], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 600], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 912/003: lt(4,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [rsp + 720], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 728], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n19_α
.Lx16_0:
 .quad 2
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rsp + 592]
 mov rdx, qword ptr [rsp + 600]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
 xchain0_n17_α:
# BOX CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+640]
 mov rax, qword ptr [rsp + 688]
 mov qword ptr [rsp + 640], rax
 mov rax, qword ptr [rsp + 696]
 mov qword ptr [rsp + 648], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+656]
 mov rax, qword ptr [rsp + 720]
 mov qword ptr [rsp + 656], rax
 mov rax, qword ptr [rsp + 728]
 mov qword ptr [rsp + 664], rax
  .section .rodata
  .Lbynamefn18: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [rsp + 640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 624], rax
 mov qword ptr [rsp + 632], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 768], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 776], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 912/004: le(5,2) should fail"
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rsp + 864], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 872], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n24_α
.Lx20_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [rsp + 768]
 mov rdx, qword ptr [rsp + 776]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 752], rax
 mov qword ptr [rsp + 760], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [rsp + 896], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 904], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n24_α
.Lx22_0:
 .quad 4
 xchain0_n22_α:
# BOX CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+864] -> [zr+816]
 mov rax, qword ptr [rsp + 864]
 mov qword ptr [rsp + 816], rax
 mov rax, qword ptr [rsp + 872]
 mov qword ptr [rsp + 824], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+832]
 mov rax, qword ptr [rsp + 896]
 mov qword ptr [rsp + 832], rax
 mov rax, qword ptr [rsp + 904]
 mov qword ptr [rsp + 840], rax
  .section .rodata
  .Lbynamefn23: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [rsp + 816]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 800], rax
 mov qword ptr [rsp + 808], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n24_α
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [rsp + 1040], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [rsp + 1048], rax
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n29_α
.Lx24_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [rsp + 944], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rsp + 952], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 912/005: le(4,4) should succeed"
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [rsp + 1072], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 1080], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n29_α
.Lx26_0:
 .quad 10
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [rsp + 944]
 mov rdx, qword ptr [rsp + 952]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 928], rax
 mov qword ptr [rsp + 936], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
 xchain0_n27_α:
# BOX CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1040] -> [zr+992]
 mov rax, qword ptr [rsp + 1040]
 mov qword ptr [rsp + 992], rax
 mov rax, qword ptr [rsp + 1048]
 mov qword ptr [rsp + 1000], rax
# marshal arg1 = producer-box slot [zr+1072] -> [zr+1008]
 mov rax, qword ptr [rsp + 1072]
 mov qword ptr [rsp + 1008], rax
 mov rax, qword ptr [rsp + 1080]
 mov qword ptr [rsp + 1016], rax
  .section .rodata
  .Lbynamefn28: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [rsp + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 976], rax
 mov qword ptr [rsp + 984], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n29_α
# IR_LIT_INTEGER
 xchain0_n28_α:
 mov qword ptr [rsp + 1216], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rsp + 1224], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp xchain0_n34_α
.Lx29_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rsp + 1120], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rsp + 1128], rax
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 912/006: le(4,10) should succeed"
# IR_LIT_INTEGER
 xchain0_n30_α:
 mov qword ptr [rsp + 1248], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [rsp + 1256], rax
 jmp xchain0_n32_α
 xchain0_n30_β:
 jmp xchain0_n34_α
.Lx31_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [rsp + 1120]
 mov rdx, qword ptr [rsp + 1128]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 1104], rax
 mov qword ptr [rsp + 1112], rdx
 jmp main_γ
 xchain0_n31_β:
 jmp main_γ
 xchain0_n32_α:
# BOX CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1168]
 mov rax, qword ptr [rsp + 1216]
 mov qword ptr [rsp + 1168], rax
 mov rax, qword ptr [rsp + 1224]
 mov qword ptr [rsp + 1176], rax
# marshal arg1 = producer-box slot [zr+1248] -> [zr+1184]
 mov rax, qword ptr [rsp + 1248]
 mov qword ptr [rsp + 1184], rax
 mov rax, qword ptr [rsp + 1256]
 mov qword ptr [rsp + 1192], rax
  .section .rodata
  .Lbynamefn33: .string "eq"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn33]
 lea rsi, [rsp + 1168]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1152], rax
 mov qword ptr [rsp + 1160], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n34_α
# IR_LIT_STRING
 xchain0_n33_α:
 mov qword ptr [rsp + 1296], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [rsp + 1304], rax
 jmp xchain0_n35_α
 xchain0_n33_β:
 jmp main_γ
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "FAIL 912/007: eq(4,5) should fail"
# IR_LIT_INTEGER
 xchain0_n34_α:
 mov qword ptr [rsp + 1392], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [rsp + 1400], rax
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n39_α
.Lx35_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n35_α:
 mov rax, qword ptr [rsp + 1296]
 mov rdx, qword ptr [rsp + 1304]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 1280], rax
 mov qword ptr [rsp + 1288], rdx
 jmp main_γ
 xchain0_n35_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n36_α:
 mov qword ptr [rsp + 1424], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [rsp + 1432], rax
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n39_α
.Lx37_0:
 .quad 5
 xchain0_n37_α:
# BOX CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1392] -> [zr+1344]
 mov rax, qword ptr [rsp + 1392]
 mov qword ptr [rsp + 1344], rax
 mov rax, qword ptr [rsp + 1400]
 mov qword ptr [rsp + 1352], rax
# marshal arg1 = producer-box slot [zr+1424] -> [zr+1360]
 mov rax, qword ptr [rsp + 1424]
 mov qword ptr [rsp + 1360], rax
 mov rax, qword ptr [rsp + 1432]
 mov qword ptr [rsp + 1368], rax
  .section .rodata
  .Lbynamefn38: .string "eq"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn38]
 lea rsi, [rsp + 1344]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1328], rax
 mov qword ptr [rsp + 1336], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [rsp + 1568], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rsp + 1576], rax
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n44_α
.Lx39_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n39_α:
 mov qword ptr [rsp + 1472], 1
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [rsp + 1480], rax
 jmp xchain0_n41_α
 xchain0_n39_β:
 jmp main_γ
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "FAIL 912/008: eq(5,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n40_α:
 mov qword ptr [rsp + 1600], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [rsp + 1608], rax
 jmp xchain0_n42_α
 xchain0_n40_β:
 jmp xchain0_n44_α
.Lx41_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n41_α:
 mov rax, qword ptr [rsp + 1472]
 mov rdx, qword ptr [rsp + 1480]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 1456], rax
 mov qword ptr [rsp + 1464], rdx
 jmp main_γ
 xchain0_n41_β:
 jmp main_γ
 xchain0_n42_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1568] -> [zr+1520]
 mov rax, qword ptr [rsp + 1568]
 mov qword ptr [rsp + 1520], rax
 mov rax, qword ptr [rsp + 1576]
 mov qword ptr [rsp + 1528], rax
# marshal arg1 = producer-box slot [zr+1600] -> [zr+1536]
 mov rax, qword ptr [rsp + 1600]
 mov qword ptr [rsp + 1536], rax
 mov rax, qword ptr [rsp + 1608]
 mov qword ptr [rsp + 1544], rax
  .section .rodata
  .Lbynamefn43: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn43]
 lea rsi, [rsp + 1520]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1504], rax
 mov qword ptr [rsp + 1512], rdx
 cmp eax, 99
 je xchain0_n44_α
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n44_α
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [rsp + 1648], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [rsp + 1656], rax
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp main_γ
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "FAIL 912/009: ne(4,4) should fail"
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [rsp + 1744], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [rsp + 1752], rax
 jmp xchain0_n46_α
 xchain0_n44_β:
 jmp xchain0_n49_α
.Lx45_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n45_α:
 mov rax, qword ptr [rsp + 1648]
 mov rdx, qword ptr [rsp + 1656]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 1632], rax
 mov qword ptr [rsp + 1640], rdx
 jmp main_γ
 xchain0_n45_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n46_α:
 mov qword ptr [rsp + 1776], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [rsp + 1784], rax
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n49_α
.Lx47_0:
 .quad 6
 xchain0_n47_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1744] -> [zr+1696]
 mov rax, qword ptr [rsp + 1744]
 mov qword ptr [rsp + 1696], rax
 mov rax, qword ptr [rsp + 1752]
 mov qword ptr [rsp + 1704], rax
# marshal arg1 = producer-box slot [zr+1776] -> [zr+1712]
 mov rax, qword ptr [rsp + 1776]
 mov qword ptr [rsp + 1712], rax
 mov rax, qword ptr [rsp + 1784]
 mov qword ptr [rsp + 1720], rax
  .section .rodata
  .Lbynamefn48: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn48]
 lea rsi, [rsp + 1696]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1680], rax
 mov qword ptr [rsp + 1688], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n49_α
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [rsp + 1920], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [rsp + 1928], rax
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n54_α
.Lx49_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [rsp + 1824], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [rsp + 1832], rax
 jmp xchain0_n51_α
 xchain0_n49_β:
 jmp main_γ
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 912/010: ne(4,6) should succeed"
# IR_LIT_INTEGER
 xchain0_n50_α:
 mov qword ptr [rsp + 1952], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [rsp + 1960], rax
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp xchain0_n54_α
.Lx51_0:
 .quad 6
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [rsp + 1824]
 mov rdx, qword ptr [rsp + 1832]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 1808], rax
 mov qword ptr [rsp + 1816], rdx
 jmp main_γ
 xchain0_n51_β:
 jmp main_γ
 xchain0_n52_α:
# BOX CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1920] -> [zr+1872]
 mov rax, qword ptr [rsp + 1920]
 mov qword ptr [rsp + 1872], rax
 mov rax, qword ptr [rsp + 1928]
 mov qword ptr [rsp + 1880], rax
# marshal arg1 = producer-box slot [zr+1952] -> [zr+1888]
 mov rax, qword ptr [rsp + 1952]
 mov qword ptr [rsp + 1888], rax
 mov rax, qword ptr [rsp + 1960]
 mov qword ptr [rsp + 1896], rax
  .section .rodata
  .Lbynamefn53: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [rsp + 1872]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1856], rax
 mov qword ptr [rsp + 1864], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n53_α
 xchain0_n52_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n53_α:
 mov qword ptr [rsp + 2000], 1
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [rsp + 2008], rax
 jmp xchain0_n55_α
 xchain0_n53_β:
 jmp main_γ
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "FAIL 912/011: gt(4,6) should fail"
# IR_LIT_INTEGER
 xchain0_n54_α:
 mov qword ptr [rsp + 2096], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [rsp + 2104], rax
 jmp xchain0_n56_α
 xchain0_n54_β:
 jmp xchain0_n59_α
.Lx55_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n55_α:
 mov rax, qword ptr [rsp + 2000]
 mov rdx, qword ptr [rsp + 2008]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 1984], rax
 mov qword ptr [rsp + 1992], rdx
 jmp main_γ
 xchain0_n55_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n56_α:
 mov qword ptr [rsp + 2128], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [rsp + 2136], rax
 jmp xchain0_n57_α
 xchain0_n56_β:
 jmp xchain0_n59_α
.Lx57_0:
 .quad 4
 xchain0_n57_α:
# BOX CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2096] -> [zr+2048]
 mov rax, qword ptr [rsp + 2096]
 mov qword ptr [rsp + 2048], rax
 mov rax, qword ptr [rsp + 2104]
 mov qword ptr [rsp + 2056], rax
# marshal arg1 = producer-box slot [zr+2128] -> [zr+2064]
 mov rax, qword ptr [rsp + 2128]
 mov qword ptr [rsp + 2064], rax
 mov rax, qword ptr [rsp + 2136]
 mov qword ptr [rsp + 2072], rax
  .section .rodata
  .Lbynamefn58: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn58]
 lea rsi, [rsp + 2048]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2032], rax
 mov qword ptr [rsp + 2040], rdx
 cmp eax, 99
 je xchain0_n59_α
 jmp xchain0_n58_α
 xchain0_n57_β:
 jmp xchain0_n59_α
# IR_LIT_STRING
 xchain0_n58_α:
 mov qword ptr [rsp + 2176], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [rsp + 2184], rax
 jmp xchain0_n60_α
 xchain0_n58_β:
 jmp main_γ
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "FAIL 912/012: gt(4,4) should fail"
# IR_LIT_INTEGER
 xchain0_n59_α:
 mov qword ptr [rsp + 2272], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [rsp + 2280], rax
 jmp xchain0_n61_α
 xchain0_n59_β:
 jmp xchain0_n64_α
.Lx60_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n60_α:
 mov rax, qword ptr [rsp + 2176]
 mov rdx, qword ptr [rsp + 2184]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 2160], rax
 mov qword ptr [rsp + 2168], rdx
 jmp main_γ
 xchain0_n60_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n61_α:
 mov qword ptr [rsp + 2304], 6
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [rsp + 2312], rax
 jmp xchain0_n62_α
 xchain0_n61_β:
 jmp xchain0_n64_α
.Lx62_0:
 .quad 2
 xchain0_n62_α:
# BOX CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2272] -> [zr+2224]
 mov rax, qword ptr [rsp + 2272]
 mov qword ptr [rsp + 2224], rax
 mov rax, qword ptr [rsp + 2280]
 mov qword ptr [rsp + 2232], rax
# marshal arg1 = producer-box slot [zr+2304] -> [zr+2240]
 mov rax, qword ptr [rsp + 2304]
 mov qword ptr [rsp + 2240], rax
 mov rax, qword ptr [rsp + 2312]
 mov qword ptr [rsp + 2248], rax
  .section .rodata
  .Lbynamefn63: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn63]
 lea rsi, [rsp + 2224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2208], rax
 mov qword ptr [rsp + 2216], rdx
 cmp eax, 99
 je xchain0_n64_α
 jmp xchain0_n63_α
 xchain0_n62_β:
 jmp xchain0_n64_α
# IR_LIT_INTEGER
 xchain0_n63_α:
 mov qword ptr [rsp + 2448], 6
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [rsp + 2456], rax
 jmp xchain0_n65_α
 xchain0_n63_β:
 jmp xchain0_n69_α
.Lx64_0:
 .quad 5
# IR_LIT_STRING
 xchain0_n64_α:
 mov qword ptr [rsp + 2352], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [rsp + 2360], rax
 jmp xchain0_n66_α
 xchain0_n64_β:
 jmp main_γ
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "FAIL 912/013: gt(5,2) should succeed"
# IR_LIT_INTEGER
 xchain0_n65_α:
 mov qword ptr [rsp + 2480], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [rsp + 2488], rax
 jmp xchain0_n67_α
 xchain0_n65_β:
 jmp xchain0_n69_α
.Lx66_0:
 .quad 7
# IR_ASSIGN gva
 xchain0_n66_α:
 mov rax, qword ptr [rsp + 2352]
 mov rdx, qword ptr [rsp + 2360]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 2336], rax
 mov qword ptr [rsp + 2344], rdx
 jmp main_γ
 xchain0_n66_β:
 jmp main_γ
 xchain0_n67_α:
# BOX CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2448] -> [zr+2400]
 mov rax, qword ptr [rsp + 2448]
 mov qword ptr [rsp + 2400], rax
 mov rax, qword ptr [rsp + 2456]
 mov qword ptr [rsp + 2408], rax
# marshal arg1 = producer-box slot [zr+2480] -> [zr+2416]
 mov rax, qword ptr [rsp + 2480]
 mov qword ptr [rsp + 2416], rax
 mov rax, qword ptr [rsp + 2488]
 mov qword ptr [rsp + 2424], rax
  .section .rodata
  .Lbynamefn68: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn68]
 lea rsi, [rsp + 2400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2384], rax
 mov qword ptr [rsp + 2392], rdx
 cmp eax, 99
 je xchain0_n69_α
 jmp xchain0_n68_α
 xchain0_n67_β:
 jmp xchain0_n69_α
# IR_LIT_STRING
 xchain0_n68_α:
 mov qword ptr [rsp + 2528], 1
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [rsp + 2536], rax
 jmp xchain0_n70_α
 xchain0_n68_β:
 jmp main_γ
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string "FAIL 912/014: ge(5,7) should fail"
# IR_LIT_INTEGER
 xchain0_n69_α:
 mov qword ptr [rsp + 2624], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [rsp + 2632], rax
 jmp xchain0_n71_α
 xchain0_n69_β:
 jmp xchain0_n74_α
.Lx70_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n70_α:
 mov rax, qword ptr [rsp + 2528]
 mov rdx, qword ptr [rsp + 2536]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 2512], rax
 mov qword ptr [rsp + 2520], rdx
 jmp main_γ
 xchain0_n70_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n71_α:
 mov qword ptr [rsp + 2656], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [rsp + 2664], rax
 jmp xchain0_n72_α
 xchain0_n71_β:
 jmp xchain0_n74_α
.Lx72_0:
 .quad 4
 xchain0_n72_α:
# BOX CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2624] -> [zr+2576]
 mov rax, qword ptr [rsp + 2624]
 mov qword ptr [rsp + 2576], rax
 mov rax, qword ptr [rsp + 2632]
 mov qword ptr [rsp + 2584], rax
# marshal arg1 = producer-box slot [zr+2656] -> [zr+2592]
 mov rax, qword ptr [rsp + 2656]
 mov qword ptr [rsp + 2592], rax
 mov rax, qword ptr [rsp + 2664]
 mov qword ptr [rsp + 2600], rax
  .section .rodata
  .Lbynamefn73: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn73]
 lea rsi, [rsp + 2576]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2560], rax
 mov qword ptr [rsp + 2568], rdx
 cmp eax, 99
 je xchain0_n74_α
 jmp xchain0_n73_α
 xchain0_n72_β:
 jmp xchain0_n74_α
# IR_LIT_INTEGER
 xchain0_n73_α:
 mov qword ptr [rsp + 2800], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [rsp + 2808], rax
 jmp xchain0_n75_α
 xchain0_n73_β:
 jmp xchain0_n79_α
.Lx74_0:
 .quad 7
# IR_LIT_STRING
 xchain0_n74_α:
 mov qword ptr [rsp + 2704], 1
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [rsp + 2712], rax
 jmp xchain0_n76_α
 xchain0_n74_β:
 jmp main_γ
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "FAIL 912/015: ge(4,4) should succeed"
# IR_LIT_INTEGER
 xchain0_n75_α:
 mov qword ptr [rsp + 2832], 6
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [rsp + 2840], rax
 jmp xchain0_n77_α
 xchain0_n75_β:
 jmp xchain0_n79_α
.Lx76_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n76_α:
 mov rax, qword ptr [rsp + 2704]
 mov rdx, qword ptr [rsp + 2712]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 2688], rax
 mov qword ptr [rsp + 2696], rdx
 jmp main_γ
 xchain0_n76_β:
 jmp main_γ
 xchain0_n77_α:
# BOX CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2800] -> [zr+2752]
 mov rax, qword ptr [rsp + 2800]
 mov qword ptr [rsp + 2752], rax
 mov rax, qword ptr [rsp + 2808]
 mov qword ptr [rsp + 2760], rax
# marshal arg1 = producer-box slot [zr+2832] -> [zr+2768]
 mov rax, qword ptr [rsp + 2832]
 mov qword ptr [rsp + 2768], rax
 mov rax, qword ptr [rsp + 2840]
 mov qword ptr [rsp + 2776], rax
  .section .rodata
  .Lbynamefn78: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn78]
 lea rsi, [rsp + 2752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2736], rax
 mov qword ptr [rsp + 2744], rdx
 cmp eax, 99
 je xchain0_n79_α
 jmp xchain0_n78_α
 xchain0_n77_β:
 jmp xchain0_n79_α
# IR_LIT_INTEGER
 xchain0_n78_α:
 mov qword ptr [rsp + 2976], 6
 mov rax, qword ptr [rip + .Lx79_0]
 mov qword ptr [rsp + 2984], rax
 jmp xchain0_n80_α
 xchain0_n78_β:
 jmp xchain0_n85_α
.Lx79_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n79_α:
 mov qword ptr [rsp + 2880], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [rsp + 2888], rax
 jmp xchain0_n81_α
 xchain0_n79_β:
 jmp main_γ
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "FAIL 912/016: ge(7,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n80_α:
 mov qword ptr [rsp + 3040], 6
 mov rax, qword ptr [rip + .Lx81_0]
 mov qword ptr [rsp + 3048], rax
 jmp xchain0_n82_α
 xchain0_n80_β:
 jmp xchain0_n85_α
.Lx81_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n81_α:
 mov rax, qword ptr [rsp + 2880]
 mov rdx, qword ptr [rsp + 2888]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 2864], rax
 mov qword ptr [rsp + 2872], rdx
 jmp main_γ
 xchain0_n81_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n82_α:
 mov qword ptr [rsp + 3072], 6
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [rsp + 3080], rax
 jmp xchain0_n83_α
 xchain0_n82_β:
 jmp xchain0_n85_α
.Lx83_0:
 .quad 1
 xchain0_n83_α:
# IR_BINOP_ARITH
.Lx84_1:
 mov rax, 5
 mov rcx, 1
 sub rax, rcx
 mov qword ptr [rsp + 3008], 6
 mov qword ptr [rsp + 3016], rax
 jmp xchain0_n84_α
.Lx84_0:
 mov rdi, qword ptr [rsp + 3040]
 mov rsi, qword ptr [rsp + 3048]
 mov rdx, qword ptr [rsp + 3072]
 mov rcx, qword ptr [rsp + 3080]
 mov r8d, 1
 lea r9, [rsp + 3008]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx84_3
.Lx84_2:
 mov rdi, qword ptr [rsp + 3040]
 mov rsi, qword ptr [rsp + 3048]
 mov rdx, qword ptr [rsp + 3072]
 mov rcx, qword ptr [rsp + 3080]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n85_α
 mov qword ptr [rsp + 3008], rax
 mov qword ptr [rsp + 3016], rdx
.Lx84_3:
 jmp xchain0_n84_α
 xchain0_n83_β:
 jmp xchain0_n85_α
 xchain0_n84_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2976] -> [zr+2928]
 mov rax, qword ptr [rsp + 2976]
 mov qword ptr [rsp + 2928], rax
 mov rax, qword ptr [rsp + 2984]
 mov qword ptr [rsp + 2936], rax
# marshal arg1 = producer-box slot [zr+3008] -> [zr+2944]
 mov rax, qword ptr [rsp + 3008]
 mov qword ptr [rsp + 2944], rax
 mov rax, qword ptr [rsp + 3016]
 mov qword ptr [rsp + 2952], rax
  .section .rodata
  .Lbynamefn85: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn85]
 lea rsi, [rsp + 2928]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2912], rax
 mov qword ptr [rsp + 2920], rdx
 cmp eax, 99
 je xchain0_n85_α
 jmp xchain0_n86_α
 xchain0_n84_β:
 jmp xchain0_n85_α
# IR_LIT_STRING
 xchain0_n85_α:
 mov qword ptr [rsp + 3216], 1
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [rsp + 3224], rax
 jmp xchain0_n87_α
 xchain0_n85_β:
 jmp xchain0_n91_α
.Lx86_0:
 .quad .Lx86_0_s
.Lx86_0_s:
 .string "12"
# IR_LIT_STRING
 xchain0_n86_α:
 mov qword ptr [rsp + 3120], 1
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [rsp + 3128], rax
 jmp xchain0_n88_α
 xchain0_n86_β:
 jmp main_γ
.Lx87_0:
 .quad .Lx87_0_s
.Lx87_0_s:
 .string "FAIL 912/017: ne(4,5-1) should fail (both=4)"
# IR_LIT_INTEGER
 xchain0_n87_α:
 mov qword ptr [rsp + 3248], 6
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [rsp + 3256], rax
 jmp xchain0_n89_α
 xchain0_n87_β:
 jmp xchain0_n91_α
.Lx88_0:
 .quad 12
# IR_ASSIGN gva
 xchain0_n88_α:
 mov rax, qword ptr [rsp + 3120]
 mov rdx, qword ptr [rsp + 3128]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 3104], rax
 mov qword ptr [rsp + 3112], rdx
 jmp main_γ
 xchain0_n88_β:
 jmp main_γ
 xchain0_n89_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3216] -> [zr+3168]
 mov rax, qword ptr [rsp + 3216]
 mov qword ptr [rsp + 3168], rax
 mov rax, qword ptr [rsp + 3224]
 mov qword ptr [rsp + 3176], rax
# marshal arg1 = producer-box slot [zr+3248] -> [zr+3184]
 mov rax, qword ptr [rsp + 3248]
 mov qword ptr [rsp + 3184], rax
 mov rax, qword ptr [rsp + 3256]
 mov qword ptr [rsp + 3192], rax
  .section .rodata
  .Lbynamefn90: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn90]
 lea rsi, [rsp + 3168]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 3152], rax
 mov qword ptr [rsp + 3160], rdx
 cmp eax, 99
 je xchain0_n91_α
 jmp xchain0_n90_α
 xchain0_n89_β:
 jmp xchain0_n91_α
# IR_LIT_STRING
 xchain0_n90_α:
 mov qword ptr [rsp + 3296], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [rsp + 3304], rax
 jmp xchain0_n92_α
 xchain0_n90_β:
 jmp main_γ
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string "FAIL 912/018: ne(string,int) type mismatch"
# IR_LIT_STRING
 xchain0_n91_α:
 mov qword ptr [rsp + 3344], 1
 mov rax, qword ptr [rip + .Lx92_0]
 mov qword ptr [rsp + 3352], rax
 jmp xchain0_n93_α
 xchain0_n91_β:
 jmp main_γ
.Lx92_0:
 .quad .Lx92_0_s
.Lx92_0_s:
 .string "PASS 912_num_pred (18/18)"
# IR_ASSIGN gva
 xchain0_n92_α:
 mov rax, qword ptr [rsp + 3296]
 mov rdx, qword ptr [rsp + 3304]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 3280], rax
 mov qword ptr [rsp + 3288], rdx
 jmp main_γ
 xchain0_n92_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n93_α:
 mov rax, qword ptr [rsp + 3344]
 mov rdx, qword ptr [rsp + 3352]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 3328], rax
 mov qword ptr [rsp + 3336], rdx
 jmp main_γ
 xchain0_n93_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 3384]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 3384]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

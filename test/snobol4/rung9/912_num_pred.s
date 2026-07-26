  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
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
  mov qword ptr [rsp + 3320], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         lt(5, 4)                   :f(e001)
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rbp + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n2_α
.Lx2_0:
 .quad 4
 xchain0_n2_α:
# BOX CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn3: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
#         output = 'FAIL 912/001: lt(5,4) should fail'   :(end)
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rbp + 240], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 248], rax
 jmp xchain0_n5_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "FAIL 912/001: lt(5,4) should fail"
# e001    lt(4, 4)                   :f(e002)
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rbp + 336], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain0_n6_α
.Lx5_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [rbp + 240]
 mov rdx, qword ptr [rbp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rbp + 368], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rbp + 376], rax
 jmp xchain0_n7_α
.Lx7_0:
 .quad 4
 xchain0_n7_α:
# BOX CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+288]
 mov rax, qword ptr [rbp + 336]
 mov qword ptr [rbp + 288], rax
 mov rax, qword ptr [rbp + 344]
 mov qword ptr [rbp + 296], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+304]
 mov rax, qword ptr [rbp + 368]
 mov qword ptr [rbp + 304], rax
 mov rax, qword ptr [rbp + 376]
 mov qword ptr [rbp + 312], rax
  .section .rodata
  .Lbynamefn8: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rbp + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
#         output = 'FAIL 912/002: lt(4,4) should fail'   :(end)
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rbp + 416], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 424], rax
 jmp xchain0_n10_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 912/002: lt(4,4) should fail"
# e002    lt(4, 5)                   :s(e003)
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [rbp + 512], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 520], rax
 jmp xchain0_n11_α
.Lx10_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [rbp + 416]
 mov rdx, qword ptr [rbp + 424]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [rbp + 544], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 552], rax
 jmp xchain0_n12_α
.Lx12_0:
 .quad 5
 xchain0_n12_α:
# BOX CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
 mov rax, qword ptr [rbp + 512]
 mov qword ptr [rbp + 464], rax
 mov rax, qword ptr [rbp + 520]
 mov qword ptr [rbp + 472], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+480]
 mov rax, qword ptr [rbp + 544]
 mov qword ptr [rbp + 480], rax
 mov rax, qword ptr [rbp + 552]
 mov qword ptr [rbp + 488], rax
  .section .rodata
  .Lbynamefn13: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [rbp + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
#         le(5, 2)                   :f(e004)
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rbp + 688], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 696], rax
 jmp xchain0_n15_α
.Lx14_0:
 .quad 5
#         output = 'FAIL 912/003: lt(4,5) should succeed' :(end)
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rbp + 592], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 600], rax
 jmp xchain0_n16_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 912/003: lt(4,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [rbp + 720], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rbp + 728], rax
 jmp xchain0_n17_α
.Lx16_0:
 .quad 2
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rbp + 592]
 mov rdx, qword ptr [rbp + 600]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp main_γ
 xchain0_n17_α:
# BOX CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+640]
 mov rax, qword ptr [rbp + 688]
 mov qword ptr [rbp + 640], rax
 mov rax, qword ptr [rbp + 696]
 mov qword ptr [rbp + 648], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+656]
 mov rax, qword ptr [rbp + 720]
 mov qword ptr [rbp + 656], rax
 mov rax, qword ptr [rbp + 728]
 mov qword ptr [rbp + 664], rax
  .section .rodata
  .Lbynamefn18: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [rbp + 640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
#         output = 'FAIL 912/004: le(5,2) should fail'   :(end)
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rbp + 768], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rbp + 776], rax
 jmp xchain0_n20_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 912/004: le(5,2) should fail"
# e004    le(4, 4)                   :s(e005)
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rbp + 864], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rbp + 872], rax
 jmp xchain0_n21_α
.Lx20_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [rbp + 768]
 mov rdx, qword ptr [rbp + 776]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 752], rax
 mov qword ptr [rbp + 760], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [rbp + 896], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 904], rax
 jmp xchain0_n22_α
.Lx22_0:
 .quad 4
 xchain0_n22_α:
# BOX CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+864] -> [zr+816]
 mov rax, qword ptr [rbp + 864]
 mov qword ptr [rbp + 816], rax
 mov rax, qword ptr [rbp + 872]
 mov qword ptr [rbp + 824], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+832]
 mov rax, qword ptr [rbp + 896]
 mov qword ptr [rbp + 832], rax
 mov rax, qword ptr [rbp + 904]
 mov qword ptr [rbp + 840], rax
  .section .rodata
  .Lbynamefn23: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [rbp + 816]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 800], rax
 mov qword ptr [rbp + 808], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n24_α
# e005    le(4, 10)                  :s(e006)
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [rbp + 1040], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [rbp + 1048], rax
 jmp xchain0_n25_α
.Lx24_0:
 .quad 4
#         output = 'FAIL 912/005: le(4,4) should succeed' :(end)
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [rbp + 944], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rbp + 952], rax
 jmp xchain0_n26_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 912/005: le(4,4) should succeed"
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [rbp + 1072], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rbp + 1080], rax
 jmp xchain0_n27_α
.Lx26_0:
 .quad 10
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [rbp + 944]
 mov rdx, qword ptr [rbp + 952]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 928], rax
 mov qword ptr [rbp + 936], rdx
 jmp main_γ
 xchain0_n27_α:
# BOX CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1040] -> [zr+992]
 mov rax, qword ptr [rbp + 1040]
 mov qword ptr [rbp + 992], rax
 mov rax, qword ptr [rbp + 1048]
 mov qword ptr [rbp + 1000], rax
# marshal arg1 = producer-box slot [zr+1072] -> [zr+1008]
 mov rax, qword ptr [rbp + 1072]
 mov qword ptr [rbp + 1008], rax
 mov rax, qword ptr [rbp + 1080]
 mov qword ptr [rbp + 1016], rax
  .section .rodata
  .Lbynamefn28: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [rbp + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 976], rax
 mov qword ptr [rbp + 984], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n29_α
#         eq(4, 5)                   :f(e007)
# IR_LIT_INTEGER
 xchain0_n28_α:
 mov qword ptr [rbp + 1216], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rbp + 1224], rax
 jmp xchain0_n30_α
.Lx29_0:
 .quad 4
#         output = 'FAIL 912/006: le(4,10) should succeed' :(end)
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rbp + 1120], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rbp + 1128], rax
 jmp xchain0_n31_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 912/006: le(4,10) should succeed"
# IR_LIT_INTEGER
 xchain0_n30_α:
 mov qword ptr [rbp + 1248], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [rbp + 1256], rax
 jmp xchain0_n32_α
.Lx31_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [rbp + 1120]
 mov rdx, qword ptr [rbp + 1128]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1104], rax
 mov qword ptr [rbp + 1112], rdx
 jmp main_γ
 xchain0_n32_α:
# BOX CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1168]
 mov rax, qword ptr [rbp + 1216]
 mov qword ptr [rbp + 1168], rax
 mov rax, qword ptr [rbp + 1224]
 mov qword ptr [rbp + 1176], rax
# marshal arg1 = producer-box slot [zr+1248] -> [zr+1184]
 mov rax, qword ptr [rbp + 1248]
 mov qword ptr [rbp + 1184], rax
 mov rax, qword ptr [rbp + 1256]
 mov qword ptr [rbp + 1192], rax
  .section .rodata
  .Lbynamefn33: .string "eq"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn33]
 lea rsi, [rbp + 1168]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1152], rax
 mov qword ptr [rbp + 1160], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n34_α
#         output = 'FAIL 912/007: eq(4,5) should fail'   :(end)
# IR_LIT_STRING
 xchain0_n33_α:
 mov qword ptr [rbp + 1296], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [rbp + 1304], rax
 jmp xchain0_n35_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "FAIL 912/007: eq(4,5) should fail"
# e007    eq(5, 5)                   :s(e008)
# IR_LIT_INTEGER
 xchain0_n34_α:
 mov qword ptr [rbp + 1392], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [rbp + 1400], rax
 jmp xchain0_n36_α
.Lx35_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n35_α:
 mov rax, qword ptr [rbp + 1296]
 mov rdx, qword ptr [rbp + 1304]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1280], rax
 mov qword ptr [rbp + 1288], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n36_α:
 mov qword ptr [rbp + 1424], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [rbp + 1432], rax
 jmp xchain0_n37_α
.Lx37_0:
 .quad 5
 xchain0_n37_α:
# BOX CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1392] -> [zr+1344]
 mov rax, qword ptr [rbp + 1392]
 mov qword ptr [rbp + 1344], rax
 mov rax, qword ptr [rbp + 1400]
 mov qword ptr [rbp + 1352], rax
# marshal arg1 = producer-box slot [zr+1424] -> [zr+1360]
 mov rax, qword ptr [rbp + 1424]
 mov qword ptr [rbp + 1360], rax
 mov rax, qword ptr [rbp + 1432]
 mov qword ptr [rbp + 1368], rax
  .section .rodata
  .Lbynamefn38: .string "eq"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn38]
 lea rsi, [rbp + 1344]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1328], rax
 mov qword ptr [rbp + 1336], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n39_α
#         ne(4, 4)                   :f(e009)
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [rbp + 1568], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rbp + 1576], rax
 jmp xchain0_n40_α
.Lx39_0:
 .quad 4
#         output = 'FAIL 912/008: eq(5,5) should succeed' :(end)
# IR_LIT_STRING
 xchain0_n39_α:
 mov qword ptr [rbp + 1472], 1
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [rbp + 1480], rax
 jmp xchain0_n41_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "FAIL 912/008: eq(5,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n40_α:
 mov qword ptr [rbp + 1600], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [rbp + 1608], rax
 jmp xchain0_n42_α
.Lx41_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n41_α:
 mov rax, qword ptr [rbp + 1472]
 mov rdx, qword ptr [rbp + 1480]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1456], rax
 mov qword ptr [rbp + 1464], rdx
 jmp main_γ
 xchain0_n42_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1568] -> [zr+1520]
 mov rax, qword ptr [rbp + 1568]
 mov qword ptr [rbp + 1520], rax
 mov rax, qword ptr [rbp + 1576]
 mov qword ptr [rbp + 1528], rax
# marshal arg1 = producer-box slot [zr+1600] -> [zr+1536]
 mov rax, qword ptr [rbp + 1600]
 mov qword ptr [rbp + 1536], rax
 mov rax, qword ptr [rbp + 1608]
 mov qword ptr [rbp + 1544], rax
  .section .rodata
  .Lbynamefn43: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn43]
 lea rsi, [rbp + 1520]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1504], rax
 mov qword ptr [rbp + 1512], rdx
 cmp eax, 99
 je xchain0_n44_α
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n44_α
#         output = 'FAIL 912/009: ne(4,4) should fail'   :(end)
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [rbp + 1648], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [rbp + 1656], rax
 jmp xchain0_n45_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "FAIL 912/009: ne(4,4) should fail"
# e009    ne(4, 6)                   :s(e010)
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [rbp + 1744], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [rbp + 1752], rax
 jmp xchain0_n46_α
.Lx45_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n45_α:
 mov rax, qword ptr [rbp + 1648]
 mov rdx, qword ptr [rbp + 1656]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1632], rax
 mov qword ptr [rbp + 1640], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n46_α:
 mov qword ptr [rbp + 1776], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [rbp + 1784], rax
 jmp xchain0_n47_α
.Lx47_0:
 .quad 6
 xchain0_n47_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1744] -> [zr+1696]
 mov rax, qword ptr [rbp + 1744]
 mov qword ptr [rbp + 1696], rax
 mov rax, qword ptr [rbp + 1752]
 mov qword ptr [rbp + 1704], rax
# marshal arg1 = producer-box slot [zr+1776] -> [zr+1712]
 mov rax, qword ptr [rbp + 1776]
 mov qword ptr [rbp + 1712], rax
 mov rax, qword ptr [rbp + 1784]
 mov qword ptr [rbp + 1720], rax
  .section .rodata
  .Lbynamefn48: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn48]
 lea rsi, [rbp + 1696]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1680], rax
 mov qword ptr [rbp + 1688], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n49_α
#         gt(4, 6)                   :f(e011)
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [rbp + 1920], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [rbp + 1928], rax
 jmp xchain0_n50_α
.Lx49_0:
 .quad 4
#         output = 'FAIL 912/010: ne(4,6) should succeed' :(end)
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [rbp + 1824], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [rbp + 1832], rax
 jmp xchain0_n51_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 912/010: ne(4,6) should succeed"
# IR_LIT_INTEGER
 xchain0_n50_α:
 mov qword ptr [rbp + 1952], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [rbp + 1960], rax
 jmp xchain0_n52_α
.Lx51_0:
 .quad 6
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [rbp + 1824]
 mov rdx, qword ptr [rbp + 1832]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1808], rax
 mov qword ptr [rbp + 1816], rdx
 jmp main_γ
 xchain0_n52_α:
# BOX CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1920] -> [zr+1872]
 mov rax, qword ptr [rbp + 1920]
 mov qword ptr [rbp + 1872], rax
 mov rax, qword ptr [rbp + 1928]
 mov qword ptr [rbp + 1880], rax
# marshal arg1 = producer-box slot [zr+1952] -> [zr+1888]
 mov rax, qword ptr [rbp + 1952]
 mov qword ptr [rbp + 1888], rax
 mov rax, qword ptr [rbp + 1960]
 mov qword ptr [rbp + 1896], rax
  .section .rodata
  .Lbynamefn53: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [rbp + 1872]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1856], rax
 mov qword ptr [rbp + 1864], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n53_α
 xchain0_n52_β:
 jmp xchain0_n54_α
#         output = 'FAIL 912/011: gt(4,6) should fail'   :(end)
# IR_LIT_STRING
 xchain0_n53_α:
 mov qword ptr [rbp + 2000], 1
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [rbp + 2008], rax
 jmp xchain0_n55_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "FAIL 912/011: gt(4,6) should fail"
# e011    gt(4, 4)                   :f(e012)
# IR_LIT_INTEGER
 xchain0_n54_α:
 mov qword ptr [rbp + 2096], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [rbp + 2104], rax
 jmp xchain0_n56_α
.Lx55_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n55_α:
 mov rax, qword ptr [rbp + 2000]
 mov rdx, qword ptr [rbp + 2008]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1984], rax
 mov qword ptr [rbp + 1992], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n56_α:
 mov qword ptr [rbp + 2128], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [rbp + 2136], rax
 jmp xchain0_n57_α
.Lx57_0:
 .quad 4
 xchain0_n57_α:
# BOX CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2096] -> [zr+2048]
 mov rax, qword ptr [rbp + 2096]
 mov qword ptr [rbp + 2048], rax
 mov rax, qword ptr [rbp + 2104]
 mov qword ptr [rbp + 2056], rax
# marshal arg1 = producer-box slot [zr+2128] -> [zr+2064]
 mov rax, qword ptr [rbp + 2128]
 mov qword ptr [rbp + 2064], rax
 mov rax, qword ptr [rbp + 2136]
 mov qword ptr [rbp + 2072], rax
  .section .rodata
  .Lbynamefn58: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn58]
 lea rsi, [rbp + 2048]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2032], rax
 mov qword ptr [rbp + 2040], rdx
 cmp eax, 99
 je xchain0_n59_α
 jmp xchain0_n58_α
 xchain0_n57_β:
 jmp xchain0_n59_α
#         output = 'FAIL 912/012: gt(4,4) should fail'   :(end)
# IR_LIT_STRING
 xchain0_n58_α:
 mov qword ptr [rbp + 2176], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [rbp + 2184], rax
 jmp xchain0_n60_α
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "FAIL 912/012: gt(4,4) should fail"
# e012    gt(5, 2)                   :s(e013)
# IR_LIT_INTEGER
 xchain0_n59_α:
 mov qword ptr [rbp + 2272], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [rbp + 2280], rax
 jmp xchain0_n61_α
.Lx60_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n60_α:
 mov rax, qword ptr [rbp + 2176]
 mov rdx, qword ptr [rbp + 2184]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 2160], rax
 mov qword ptr [rbp + 2168], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n61_α:
 mov qword ptr [rbp + 2304], 6
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [rbp + 2312], rax
 jmp xchain0_n62_α
.Lx62_0:
 .quad 2
 xchain0_n62_α:
# BOX CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2272] -> [zr+2224]
 mov rax, qword ptr [rbp + 2272]
 mov qword ptr [rbp + 2224], rax
 mov rax, qword ptr [rbp + 2280]
 mov qword ptr [rbp + 2232], rax
# marshal arg1 = producer-box slot [zr+2304] -> [zr+2240]
 mov rax, qword ptr [rbp + 2304]
 mov qword ptr [rbp + 2240], rax
 mov rax, qword ptr [rbp + 2312]
 mov qword ptr [rbp + 2248], rax
  .section .rodata
  .Lbynamefn63: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn63]
 lea rsi, [rbp + 2224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2208], rax
 mov qword ptr [rbp + 2216], rdx
 cmp eax, 99
 je xchain0_n64_α
 jmp xchain0_n63_α
 xchain0_n62_β:
 jmp xchain0_n64_α
#         ge(5, 7)                   :f(e014)
# IR_LIT_INTEGER
 xchain0_n63_α:
 mov qword ptr [rbp + 2448], 6
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [rbp + 2456], rax
 jmp xchain0_n65_α
.Lx64_0:
 .quad 5
#         output = 'FAIL 912/013: gt(5,2) should succeed' :(end)
# IR_LIT_STRING
 xchain0_n64_α:
 mov qword ptr [rbp + 2352], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [rbp + 2360], rax
 jmp xchain0_n66_α
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "FAIL 912/013: gt(5,2) should succeed"
# IR_LIT_INTEGER
 xchain0_n65_α:
 mov qword ptr [rbp + 2480], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [rbp + 2488], rax
 jmp xchain0_n67_α
.Lx66_0:
 .quad 7
# IR_ASSIGN gva
 xchain0_n66_α:
 mov rax, qword ptr [rbp + 2352]
 mov rdx, qword ptr [rbp + 2360]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 2336], rax
 mov qword ptr [rbp + 2344], rdx
 jmp main_γ
 xchain0_n67_α:
# BOX CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2448] -> [zr+2400]
 mov rax, qword ptr [rbp + 2448]
 mov qword ptr [rbp + 2400], rax
 mov rax, qword ptr [rbp + 2456]
 mov qword ptr [rbp + 2408], rax
# marshal arg1 = producer-box slot [zr+2480] -> [zr+2416]
 mov rax, qword ptr [rbp + 2480]
 mov qword ptr [rbp + 2416], rax
 mov rax, qword ptr [rbp + 2488]
 mov qword ptr [rbp + 2424], rax
  .section .rodata
  .Lbynamefn68: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn68]
 lea rsi, [rbp + 2400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2384], rax
 mov qword ptr [rbp + 2392], rdx
 cmp eax, 99
 je xchain0_n69_α
 jmp xchain0_n68_α
 xchain0_n67_β:
 jmp xchain0_n69_α
#         output = 'FAIL 912/014: ge(5,7) should fail'   :(end)
# IR_LIT_STRING
 xchain0_n68_α:
 mov qword ptr [rbp + 2528], 1
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [rbp + 2536], rax
 jmp xchain0_n70_α
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string "FAIL 912/014: ge(5,7) should fail"
# e014    ge(4, 4)                   :s(e015)
# IR_LIT_INTEGER
 xchain0_n69_α:
 mov qword ptr [rbp + 2624], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [rbp + 2632], rax
 jmp xchain0_n71_α
.Lx70_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n70_α:
 mov rax, qword ptr [rbp + 2528]
 mov rdx, qword ptr [rbp + 2536]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 2512], rax
 mov qword ptr [rbp + 2520], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n71_α:
 mov qword ptr [rbp + 2656], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [rbp + 2664], rax
 jmp xchain0_n72_α
.Lx72_0:
 .quad 4
 xchain0_n72_α:
# BOX CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2624] -> [zr+2576]
 mov rax, qword ptr [rbp + 2624]
 mov qword ptr [rbp + 2576], rax
 mov rax, qword ptr [rbp + 2632]
 mov qword ptr [rbp + 2584], rax
# marshal arg1 = producer-box slot [zr+2656] -> [zr+2592]
 mov rax, qword ptr [rbp + 2656]
 mov qword ptr [rbp + 2592], rax
 mov rax, qword ptr [rbp + 2664]
 mov qword ptr [rbp + 2600], rax
  .section .rodata
  .Lbynamefn73: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn73]
 lea rsi, [rbp + 2576]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2560], rax
 mov qword ptr [rbp + 2568], rdx
 cmp eax, 99
 je xchain0_n74_α
 jmp xchain0_n73_α
 xchain0_n72_β:
 jmp xchain0_n74_α
# e015    ge(7, 5)                   :s(e016)
# IR_LIT_INTEGER
 xchain0_n73_α:
 mov qword ptr [rbp + 2800], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [rbp + 2808], rax
 jmp xchain0_n75_α
.Lx74_0:
 .quad 7
#         output = 'FAIL 912/015: ge(4,4) should succeed' :(end)
# IR_LIT_STRING
 xchain0_n74_α:
 mov qword ptr [rbp + 2704], 1
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [rbp + 2712], rax
 jmp xchain0_n76_α
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "FAIL 912/015: ge(4,4) should succeed"
# IR_LIT_INTEGER
 xchain0_n75_α:
 mov qword ptr [rbp + 2832], 6
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [rbp + 2840], rax
 jmp xchain0_n77_α
.Lx76_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n76_α:
 mov rax, qword ptr [rbp + 2704]
 mov rdx, qword ptr [rbp + 2712]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 2688], rax
 mov qword ptr [rbp + 2696], rdx
 jmp main_γ
 xchain0_n77_α:
# BOX CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2800] -> [zr+2752]
 mov rax, qword ptr [rbp + 2800]
 mov qword ptr [rbp + 2752], rax
 mov rax, qword ptr [rbp + 2808]
 mov qword ptr [rbp + 2760], rax
# marshal arg1 = producer-box slot [zr+2832] -> [zr+2768]
 mov rax, qword ptr [rbp + 2832]
 mov qword ptr [rbp + 2768], rax
 mov rax, qword ptr [rbp + 2840]
 mov qword ptr [rbp + 2776], rax
  .section .rodata
  .Lbynamefn78: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn78]
 lea rsi, [rbp + 2752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2736], rax
 mov qword ptr [rbp + 2744], rdx
 cmp eax, 99
 je xchain0_n79_α
 jmp xchain0_n78_α
 xchain0_n77_β:
 jmp xchain0_n79_α
#         ne(4, 5 - 1)               :f(e017)
# IR_LIT_INTEGER
 xchain0_n78_α:
 mov qword ptr [rbp + 2976], 6
 mov rax, qword ptr [rip + .Lx79_0]
 mov qword ptr [rbp + 2984], rax
 jmp xchain0_n80_α
.Lx79_0:
 .quad 4
#         output = 'FAIL 912/016: ge(7,5) should succeed' :(end)
# IR_LIT_STRING
 xchain0_n79_α:
 mov qword ptr [rbp + 2880], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [rbp + 2888], rax
 jmp xchain0_n81_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "FAIL 912/016: ge(7,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n80_α:
 mov qword ptr [rbp + 3008], 6
 mov rax, qword ptr [rip + .Lx81_0]
 mov qword ptr [rbp + 3016], rax
 jmp xchain0_n82_α
.Lx81_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n81_α:
 mov rax, qword ptr [rbp + 2880]
 mov rdx, qword ptr [rbp + 2888]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 2864], rax
 mov qword ptr [rbp + 2872], rdx
 jmp main_γ
 xchain0_n82_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2976] -> [zr+2928]
 mov rax, qword ptr [rbp + 2976]
 mov qword ptr [rbp + 2928], rax
 mov rax, qword ptr [rbp + 2984]
 mov qword ptr [rbp + 2936], rax
# marshal arg1 = producer-box slot [zr+3008] -> [zr+2944]
 mov rax, qword ptr [rbp + 3008]
 mov qword ptr [rbp + 2944], rax
 mov rax, qword ptr [rbp + 3016]
 mov qword ptr [rbp + 2952], rax
  .section .rodata
  .Lbynamefn83: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn83]
 lea rsi, [rbp + 2928]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2912], rax
 mov qword ptr [rbp + 2920], rdx
 cmp eax, 99
 je xchain0_n84_α
 jmp xchain0_n83_α
 xchain0_n82_β:
 jmp xchain0_n84_α
#         output = 'FAIL 912/017: ne(4,5-1) should fail (both=4)' :(end)
# IR_LIT_STRING
 xchain0_n83_α:
 mov qword ptr [rbp + 3056], 1
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [rbp + 3064], rax
 jmp xchain0_n85_α
.Lx84_0:
 .quad .Lx84_0_s
.Lx84_0_s:
 .string "FAIL 912/017: ne(4,5-1) should fail (both=4)"
#         ne('12', 12)                   :f(e018)
# IR_LIT_STRING
 xchain0_n84_α:
 mov qword ptr [rbp + 3152], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [rbp + 3160], rax
 jmp xchain0_n86_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "12"
# IR_ASSIGN gva
 xchain0_n85_α:
 mov rax, qword ptr [rbp + 3056]
 mov rdx, qword ptr [rbp + 3064]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 3040], rax
 mov qword ptr [rbp + 3048], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n86_α:
 mov qword ptr [rbp + 3184], 6
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [rbp + 3192], rax
 jmp xchain0_n87_α
.Lx87_0:
 .quad 12
 xchain0_n87_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3152] -> [zr+3104]
 mov rax, qword ptr [rbp + 3152]
 mov qword ptr [rbp + 3104], rax
 mov rax, qword ptr [rbp + 3160]
 mov qword ptr [rbp + 3112], rax
# marshal arg1 = producer-box slot [zr+3184] -> [zr+3120]
 mov rax, qword ptr [rbp + 3184]
 mov qword ptr [rbp + 3120], rax
 mov rax, qword ptr [rbp + 3192]
 mov qword ptr [rbp + 3128], rax
  .section .rodata
  .Lbynamefn88: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn88]
 lea rsi, [rbp + 3104]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3088], rax
 mov qword ptr [rbp + 3096], rdx
 cmp eax, 99
 je xchain0_n89_α
 jmp xchain0_n88_α
 xchain0_n87_β:
 jmp xchain0_n89_α
#         output = 'FAIL 912/018: ne(string,int) type mismatch' :(end)
# IR_LIT_STRING
 xchain0_n88_α:
 mov qword ptr [rbp + 3232], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [rbp + 3240], rax
 jmp xchain0_n90_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "FAIL 912/018: ne(string,int) type mismatch"
#         output = 'PASS 912_num_pred (18/18)'
# IR_LIT_STRING
 xchain0_n89_α:
 mov qword ptr [rbp + 3280], 1
 mov rax, qword ptr [rip + .Lx90_0]
 mov qword ptr [rbp + 3288], rax
 jmp xchain0_n91_α
.Lx90_0:
 .quad .Lx90_0_s
.Lx90_0_s:
 .string "PASS 912_num_pred (18/18)"
# IR_ASSIGN gva
 xchain0_n90_α:
 mov rax, qword ptr [rbp + 3232]
 mov rdx, qword ptr [rbp + 3240]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 3216], rax
 mov qword ptr [rbp + 3224], rdx
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n91_α:
 mov rax, qword ptr [rbp + 3280]
 mov rdx, qword ptr [rbp + 3288]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 3264], rax
 mov qword ptr [rbp + 3272], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 3320]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 3320]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

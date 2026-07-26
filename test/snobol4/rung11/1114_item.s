  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "aaa"
  .Lgvan1: .string "output"
  .Lgvan2: .string "ama"
  .Lgvan3: .string "tt"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 4
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 4
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
  mov qword ptr [rsp + 4136], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         aaa = array(10)
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rbp + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad 10
 xchain0_n1_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lbynamefn2: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [rbp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain0_n3_α
#         aaa = array(10)
#         item(aaa, 1) = 5
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rbp + 192], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string ""
 xchain0_n4_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+192] -> [zr+240]
 mov rax, qword ptr [rbp + 192]
 mov qword ptr [rbp + 240], rax
 mov rax, qword ptr [rbp + 200]
 mov qword ptr [rbp + 248], rax
  .section .rodata
  .Lrkfn6: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn6]
 lea rsi, [rbp + 240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 jmp xchain0_n7_α
#         differ(item(aaa, 1), 5)                   :f(e001)
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp xchain0_n8_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rbp + 368], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 376], rax
 jmp xchain0_n10_α
.Lx9_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [rbp + 608], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 616], rax
 jmp xchain0_n11_α
.Lx10_0:
 .quad 1
#         differ(aaa<1>, 5)                   :f(e002)
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 784], rax
 mov qword ptr [rbp + 792], rdx
 jmp xchain0_n12_α
 xchain0_n10_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn11: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rbp + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n6_α
 xchain0_n11_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+576] -> [zr+528]
 mov rax, qword ptr [rbp + 576]
 mov qword ptr [rbp + 528], rax
 mov rax, qword ptr [rbp + 584]
 mov qword ptr [rbp + 536], rax
# marshal arg1 = producer-box slot [zr+608] -> [zr+544]
 mov rax, qword ptr [rbp + 608]
 mov qword ptr [rbp + 544], rax
 mov rax, qword ptr [rbp + 616]
 mov qword ptr [rbp + 552], rax
  .section .rodata
  .Lbynamefn12: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [rbp + 528]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n9_α
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [rbp + 816], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 824], rax
 jmp xchain0_n16_α
.Lx14_0:
 .quad 1
#         aaa<2> = 22
# IR_VAR
 xchain0_n13_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 976], rax
 mov qword ptr [rbp + 984], rdx
 jmp xchain0_n17_α
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [rbp + 400], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rbp + 408], rax
 jmp xchain0_n19_α
.Lx16_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [rbp + 640], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 648], rax
 jmp xchain0_n20_α
.Lx17_0:
 .quad 5
# IR_SUBSCRIPT x[i] variable
 xchain0_n16_α:
 mov rdi, qword ptr [rbp + 784]
 mov rsi, qword ptr [rbp + 792]
 mov rdx, qword ptr [rbp + 816]
 mov rcx, qword ptr [rbp + 824]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n13_α
 mov qword ptr [rbp + 848], rax
 mov qword ptr [rbp + 856], rdx
 jmp xchain0_n21_α
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [rbp + 1008], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rbp + 1016], rax
 jmp xchain0_n22_α
.Lx19_0:
 .quad 2
#         differ(item(aaa, 2), 22)                   :f(e003)
# IR_VAR
 xchain0_n18_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 1248], rax
 mov qword ptr [rbp + 1256], rdx
 jmp xchain0_n23_α
# IR_ASSIGN_VAR
 xchain0_n19_α:
 mov rdi, qword ptr [rbp + 272]
 mov rsi, qword ptr [rbp + 280]
 mov rdx, qword ptr [rbp + 400]
 mov rcx, qword ptr [rbp + 408]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n6_α
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain0_n6_α
 xchain0_n20_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
 mov rax, qword ptr [rbp + 512]
 mov qword ptr [rbp + 464], rax
 mov rax, qword ptr [rbp + 520]
 mov qword ptr [rbp + 472], rax
# marshal arg1 = producer-box slot [zr+640] -> [zr+480]
 mov rax, qword ptr [rbp + 640]
 mov qword ptr [rbp + 480], rax
 mov rax, qword ptr [rbp + 648]
 mov qword ptr [rbp + 488], rax
  .section .rodata
  .Lbynamefn21: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [rbp + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n25_α
 xchain0_n20_β:
 jmp xchain0_n9_α
# IR_DEREF variable -> value
 xchain0_n21_α:
 mov rdi, qword ptr [rbp + 848]
 mov rsi, qword ptr [rbp + 856]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n13_α
 mov qword ptr [rbp + 880], rax
 mov qword ptr [rbp + 888], rdx
 jmp xchain0_n26_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n22_α:
 mov rdi, qword ptr [rbp + 976]
 mov rsi, qword ptr [rbp + 984]
 mov rdx, qword ptr [rbp + 1008]
 mov rcx, qword ptr [rbp + 1016]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [rbp + 1040], rax
 mov qword ptr [rbp + 1048], rdx
 jmp xchain0_n27_α
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [rbp + 1280], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rbp + 1288], rax
 jmp xchain0_n28_α
.Lx25_0:
 .quad 2
#         ama = array('2,2,2,2')
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [rbp + 1456], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rbp + 1464], rax
 jmp xchain0_n29_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "2,2,2,2"
#         output = 'FAIL 1114/001: item 1D assign/read'  :(end)
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [rbp + 688], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [rbp + 696], rax
 jmp xchain0_n30_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "FAIL 1114/001: item 1D assign/read"
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [rbp + 896], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rbp + 904], rax
 jmp xchain0_n31_α
.Lx28_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [rbp + 1072], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rbp + 1080], rax
 jmp xchain0_n32_α
.Lx29_0:
 .quad 22
 xchain0_n28_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1248] -> [zr+1200]
 mov rax, qword ptr [rbp + 1248]
 mov qword ptr [rbp + 1200], rax
 mov rax, qword ptr [rbp + 1256]
 mov qword ptr [rbp + 1208], rax
# marshal arg1 = producer-box slot [zr+1280] -> [zr+1216]
 mov rax, qword ptr [rbp + 1280]
 mov qword ptr [rbp + 1216], rax
 mov rax, qword ptr [rbp + 1288]
 mov qword ptr [rbp + 1224], rax
  .section .rodata
  .Lbynamefn29: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn29]
 lea rsi, [rbp + 1200]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1184], rax
 mov qword ptr [rbp + 1192], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n33_α
 xchain0_n28_β:
 jmp xchain0_n24_α
 xchain0_n29_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1456] -> [zr+1424]
 mov rax, qword ptr [rbp + 1456]
 mov qword ptr [rbp + 1424], rax
 mov rax, qword ptr [rbp + 1464]
 mov qword ptr [rbp + 1432], rax
  .section .rodata
  .Lbynamefn30: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn30]
 lea rsi, [rbp + 1424]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1408], rax
 mov qword ptr [rbp + 1416], rdx
 cmp eax, 99
 je xchain0_n35_α
 jmp xchain0_n34_α
 xchain0_n29_β:
 jmp xchain0_n35_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [rbp + 688]
 mov rdx, qword ptr [rbp + 696]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 680], rdx
 jmp main_γ
 xchain0_n31_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+736]
 mov rax, qword ptr [rbp + 880]
 mov qword ptr [rbp + 736], rax
 mov rax, qword ptr [rbp + 888]
 mov qword ptr [rbp + 744], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+752]
 mov rax, qword ptr [rbp + 896]
 mov qword ptr [rbp + 752], rax
 mov rax, qword ptr [rbp + 904]
 mov qword ptr [rbp + 760], rax
  .section .rodata
  .Lbynamefn32: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn32]
 lea rsi, [rbp + 736]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 720], rax
 mov qword ptr [rbp + 728], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n36_α
 xchain0_n31_β:
 jmp xchain0_n13_α
# IR_ASSIGN_VAR
 xchain0_n32_α:
 mov rdi, qword ptr [rbp + 1040]
 mov rsi, qword ptr [rbp + 1048]
 mov rdx, qword ptr [rbp + 1072]
 mov rcx, qword ptr [rbp + 1080]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [rbp + 1104], rax
 mov qword ptr [rbp + 1112], rdx
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n33_α:
 mov qword ptr [rbp + 1312], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [rbp + 1320], rax
 jmp xchain0_n37_α
.Lx35_0:
 .quad 22
# IR_ASSIGN gva
 xchain0_n34_α:
 mov rax, qword ptr [rbp + 1408]
 mov rdx, qword ptr [rbp + 1416]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 1392], rax
 mov qword ptr [rbp + 1400], rdx
 jmp xchain0_n35_α
#         item(ama, 1,2,1,2) = 1212
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [rbp + 1488], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [rbp + 1496], rax
 jmp xchain0_n38_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string ""
#         output = 'FAIL 1114/002: item == bracket read' :(end)
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [rbp + 944], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [rbp + 952], rax
 jmp xchain0_n39_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1114/002: item == bracket read"
 xchain0_n37_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1184] -> [zr+1136]
 mov rax, qword ptr [rbp + 1184]
 mov qword ptr [rbp + 1136], rax
 mov rax, qword ptr [rbp + 1192]
 mov qword ptr [rbp + 1144], rax
# marshal arg1 = producer-box slot [zr+1312] -> [zr+1152]
 mov rax, qword ptr [rbp + 1312]
 mov qword ptr [rbp + 1152], rax
 mov rax, qword ptr [rbp + 1320]
 mov qword ptr [rbp + 1160], rax
  .section .rodata
  .Lbynamefn38: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn38]
 lea rsi, [rbp + 1136]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1120], rax
 mov qword ptr [rbp + 1128], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp xchain0_n24_α
 xchain0_n38_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1488] -> [zr+1536]
 mov rax, qword ptr [rbp + 1488]
 mov qword ptr [rbp + 1536], rax
 mov rax, qword ptr [rbp + 1496]
 mov qword ptr [rbp + 1544], rax
  .section .rodata
  .Lrkfn41: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn41]
 lea rsi, [rbp + 1536]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1520], rax
 mov qword ptr [rbp + 1528], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n41_α
 xchain0_n38_β:
 jmp xchain0_n42_α
# IR_ASSIGN gva
 xchain0_n39_α:
 mov rax, qword ptr [rbp + 944]
 mov rdx, qword ptr [rbp + 952]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 928], rax
 mov qword ptr [rbp + 936], rdx
 jmp main_γ
#         output = 'FAIL 1114/003: bracket assign, item read' :(end)
# IR_LIT_STRING
 xchain0_n40_α:
 mov qword ptr [rbp + 1360], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [rbp + 1368], rax
 jmp xchain0_n43_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "FAIL 1114/003: bracket assign, item read"
# IR_VAR
 xchain0_n41_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 1680], rax
 mov qword ptr [rbp + 1688], rdx
 jmp xchain0_n44_α
#         differ(item(ama, 1,2,1,2), 1212)                   :f(e004)
# IR_VAR
 xchain0_n42_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 2064], rax
 mov qword ptr [rbp + 2072], rdx
 jmp xchain0_n45_α
# IR_ASSIGN gva
 xchain0_n43_α:
 mov rax, qword ptr [rbp + 1360]
 mov rdx, qword ptr [rbp + 1368]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 1344], rax
 mov qword ptr [rbp + 1352], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [rbp + 1712], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [rbp + 1720], rax
 jmp xchain0_n47_α
.Lx47_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n45_α:
 mov qword ptr [rbp + 2096], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [rbp + 2104], rax
 jmp xchain0_n48_α
.Lx48_0:
 .quad 1
#         differ(ama<1,2,1,2>, 1212)                   :f(e005)
# IR_VAR
 xchain0_n46_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 2368], rax
 mov qword ptr [rbp + 2376], rdx
 jmp xchain0_n49_α
# IR_LIT_INTEGER
 xchain0_n47_α:
 mov qword ptr [rbp + 1744], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [rbp + 1752], rax
 jmp xchain0_n51_α
.Lx50_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [rbp + 2128], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [rbp + 2136], rax
 jmp xchain0_n52_α
.Lx51_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n49_α:
 mov qword ptr [rbp + 2400], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [rbp + 2408], rax
 jmp xchain0_n53_α
.Lx52_0:
 .quad 1
#         ama<2,1,2,1> = 2121
# IR_VAR
 xchain0_n50_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 2752], rax
 mov qword ptr [rbp + 2760], rdx
 jmp xchain0_n54_α
# IR_LIT_INTEGER
 xchain0_n51_α:
 mov qword ptr [rbp + 1776], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [rbp + 1784], rax
 jmp xchain0_n56_α
.Lx54_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [rbp + 2160], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [rbp + 2168], rax
 jmp xchain0_n57_α
.Lx55_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n53_α:
 mov rdi, qword ptr [rbp + 2368]
 mov rsi, qword ptr [rbp + 2376]
 mov rdx, qword ptr [rbp + 2400]
 mov rcx, qword ptr [rbp + 2408]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [rbp + 2432], rax
 mov qword ptr [rbp + 2440], rdx
 jmp xchain0_n58_α
# IR_LIT_INTEGER
 xchain0_n54_α:
 mov qword ptr [rbp + 2784], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [rbp + 2792], rax
 jmp xchain0_n59_α
.Lx57_0:
 .quad 2
#         differ(item(ama, 2,1,2,1), 2121)                   :f(e006)
# IR_VAR
 xchain0_n55_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 3264], rax
 mov qword ptr [rbp + 3272], rdx
 jmp xchain0_n60_α
# IR_LIT_INTEGER
 xchain0_n56_α:
 mov qword ptr [rbp + 1808], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [rbp + 1816], rax
 jmp xchain0_n62_α
.Lx59_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n57_α:
 mov qword ptr [rbp + 2192], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [rbp + 2200], rax
 jmp xchain0_n63_α
.Lx60_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n58_α:
 mov qword ptr [rbp + 2464], 6
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [rbp + 2472], rax
 jmp xchain0_n64_α
.Lx61_0:
 .quad 2
# IR_SUBSCRIPT x[i] variable
 xchain0_n59_α:
 mov rdi, qword ptr [rbp + 2752]
 mov rsi, qword ptr [rbp + 2760]
 mov rdx, qword ptr [rbp + 2784]
 mov rcx, qword ptr [rbp + 2792]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [rbp + 2816], rax
 mov qword ptr [rbp + 2824], rdx
 jmp xchain0_n65_α
# IR_LIT_INTEGER
 xchain0_n60_α:
 mov qword ptr [rbp + 3296], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [rbp + 3304], rax
 jmp xchain0_n66_α
.Lx63_0:
 .quad 2
#         tt = table()
 xchain0_n61_α:
# BOX IR_CALL table(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn65: .string "table"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn65]
 lea rsi, [rbp + 3536]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3520], rax
 mov qword ptr [rbp + 3528], rdx
 cmp eax, 99
 je xchain0_n68_α
 jmp xchain0_n67_α
 xchain0_n61_β:
 jmp xchain0_n68_α
 xchain0_n62_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1680] -> [zr+1584]
 mov rax, qword ptr [rbp + 1680]
 mov qword ptr [rbp + 1584], rax
 mov rax, qword ptr [rbp + 1688]
 mov qword ptr [rbp + 1592], rax
# marshal arg1 = producer-box slot [zr+1712] -> [zr+1600]
 mov rax, qword ptr [rbp + 1712]
 mov qword ptr [rbp + 1600], rax
 mov rax, qword ptr [rbp + 1720]
 mov qword ptr [rbp + 1608], rax
# marshal arg2 = producer-box slot [zr+1744] -> [zr+1616]
 mov rax, qword ptr [rbp + 1744]
 mov qword ptr [rbp + 1616], rax
 mov rax, qword ptr [rbp + 1752]
 mov qword ptr [rbp + 1624], rax
# marshal arg3 = producer-box slot [zr+1776] -> [zr+1632]
 mov rax, qword ptr [rbp + 1776]
 mov qword ptr [rbp + 1632], rax
 mov rax, qword ptr [rbp + 1784]
 mov qword ptr [rbp + 1640], rax
# marshal arg4 = producer-box slot [zr+1808] -> [zr+1648]
 mov rax, qword ptr [rbp + 1808]
 mov qword ptr [rbp + 1648], rax
 mov rax, qword ptr [rbp + 1816]
 mov qword ptr [rbp + 1656], rax
  .section .rodata
  .Lbynamefn63: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn63]
 lea rsi, [rbp + 1584]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1568], rax
 mov qword ptr [rbp + 1576], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n69_α
 xchain0_n62_β:
 jmp xchain0_n42_α
 xchain0_n63_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2064] -> [zr+1968]
 mov rax, qword ptr [rbp + 2064]
 mov qword ptr [rbp + 1968], rax
 mov rax, qword ptr [rbp + 2072]
 mov qword ptr [rbp + 1976], rax
# marshal arg1 = producer-box slot [zr+2096] -> [zr+1984]
 mov rax, qword ptr [rbp + 2096]
 mov qword ptr [rbp + 1984], rax
 mov rax, qword ptr [rbp + 2104]
 mov qword ptr [rbp + 1992], rax
# marshal arg2 = producer-box slot [zr+2128] -> [zr+2000]
 mov rax, qword ptr [rbp + 2128]
 mov qword ptr [rbp + 2000], rax
 mov rax, qword ptr [rbp + 2136]
 mov qword ptr [rbp + 2008], rax
# marshal arg3 = producer-box slot [zr+2160] -> [zr+2016]
 mov rax, qword ptr [rbp + 2160]
 mov qword ptr [rbp + 2016], rax
 mov rax, qword ptr [rbp + 2168]
 mov qword ptr [rbp + 2024], rax
# marshal arg4 = producer-box slot [zr+2192] -> [zr+2032]
 mov rax, qword ptr [rbp + 2192]
 mov qword ptr [rbp + 2032], rax
 mov rax, qword ptr [rbp + 2200]
 mov qword ptr [rbp + 2040], rax
  .section .rodata
  .Lbynamefn64: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn64]
 lea rsi, [rbp + 1968]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1952], rax
 mov qword ptr [rbp + 1960], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n70_α
 xchain0_n63_β:
 jmp xchain0_n46_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n64_α:
 mov rdi, qword ptr [rbp + 2432]
 mov rsi, qword ptr [rbp + 2440]
 mov rdx, qword ptr [rbp + 2464]
 mov rcx, qword ptr [rbp + 2472]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [rbp + 2496], rax
 mov qword ptr [rbp + 2504], rdx
 jmp xchain0_n71_α
# IR_LIT_INTEGER
 xchain0_n65_α:
 mov qword ptr [rbp + 2848], 6
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [rbp + 2856], rax
 jmp xchain0_n72_α
.Lx69_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n66_α:
 mov qword ptr [rbp + 3328], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [rbp + 3336], rax
 jmp xchain0_n73_α
.Lx70_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n67_α:
 mov rax, qword ptr [rbp + 3520]
 mov rdx, qword ptr [rbp + 3528]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 3504], rax
 mov qword ptr [rbp + 3512], rdx
 jmp xchain0_n68_α
#         item(tt, 'key') = 'val'
# IR_LIT_STRING
 xchain0_n68_α:
 mov qword ptr [rbp + 3552], 1
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [rbp + 3560], rax
 jmp xchain0_n74_α
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n69_α:
 mov qword ptr [rbp + 1840], 6
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [rbp + 1848], rax
 jmp xchain0_n75_α
.Lx73_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n70_α:
 mov qword ptr [rbp + 2224], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [rbp + 2232], rax
 jmp xchain0_n76_α
.Lx74_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n71_α:
 mov qword ptr [rbp + 2528], 6
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [rbp + 2536], rax
 jmp xchain0_n77_α
.Lx75_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n72_α:
 mov rdi, qword ptr [rbp + 2816]
 mov rsi, qword ptr [rbp + 2824]
 mov rdx, qword ptr [rbp + 2848]
 mov rcx, qword ptr [rbp + 2856]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [rbp + 2880], rax
 mov qword ptr [rbp + 2888], rdx
 jmp xchain0_n78_α
# IR_LIT_INTEGER
 xchain0_n73_α:
 mov qword ptr [rbp + 3360], 6
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [rbp + 3368], rax
 jmp xchain0_n79_α
.Lx77_0:
 .quad 2
 xchain0_n74_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3552] -> [zr+3600]
 mov rax, qword ptr [rbp + 3552]
 mov qword ptr [rbp + 3600], rax
 mov rax, qword ptr [rbp + 3560]
 mov qword ptr [rbp + 3608], rax
  .section .rodata
  .Lrkfn79: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn79]
 lea rsi, [rbp + 3600]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3584], rax
 mov qword ptr [rbp + 3592], rdx
 cmp eax, 99
 je xchain0_n81_α
 jmp xchain0_n80_α
 xchain0_n74_β:
 jmp xchain0_n81_α
# IR_ASSIGN_VAR
 xchain0_n75_α:
 mov rdi, qword ptr [rbp + 1568]
 mov rsi, qword ptr [rbp + 1576]
 mov rdx, qword ptr [rbp + 1840]
 mov rcx, qword ptr [rbp + 1848]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [rbp + 1872], rax
 mov qword ptr [rbp + 1880], rdx
 jmp xchain0_n42_α
 xchain0_n76_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1952] -> [zr+1904]
 mov rax, qword ptr [rbp + 1952]
 mov qword ptr [rbp + 1904], rax
 mov rax, qword ptr [rbp + 1960]
 mov qword ptr [rbp + 1912], rax
# marshal arg1 = producer-box slot [zr+2224] -> [zr+1920]
 mov rax, qword ptr [rbp + 2224]
 mov qword ptr [rbp + 1920], rax
 mov rax, qword ptr [rbp + 2232]
 mov qword ptr [rbp + 1928], rax
  .section .rodata
  .Lbynamefn77: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn77]
 lea rsi, [rbp + 1904]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1888], rax
 mov qword ptr [rbp + 1896], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n82_α
 xchain0_n76_β:
 jmp xchain0_n46_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n77_α:
 mov rdi, qword ptr [rbp + 2496]
 mov rsi, qword ptr [rbp + 2504]
 mov rdx, qword ptr [rbp + 2528]
 mov rcx, qword ptr [rbp + 2536]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [rbp + 2560], rax
 mov qword ptr [rbp + 2568], rdx
 jmp xchain0_n83_α
# IR_LIT_INTEGER
 xchain0_n78_α:
 mov qword ptr [rbp + 2912], 6
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [rbp + 2920], rax
 jmp xchain0_n84_α
.Lx83_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n79_α:
 mov qword ptr [rbp + 3392], 6
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [rbp + 3400], rax
 jmp xchain0_n85_α
.Lx84_0:
 .quad 1
# IR_VAR
 xchain0_n80_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 3696], rax
 mov qword ptr [rbp + 3704], rdx
 jmp xchain0_n86_α
#         differ(item(tt, 'key'), 'val')                   :f(e007)
# IR_VAR
 xchain0_n81_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 3936], rax
 mov qword ptr [rbp + 3944], rdx
 jmp xchain0_n87_α
#         output = 'FAIL 1114/004: item 4D assign/read'  :(end)
# IR_LIT_STRING
 xchain0_n82_α:
 mov qword ptr [rbp + 2272], 1
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [rbp + 2280], rax
 jmp xchain0_n89_α
.Lx87_0:
 .quad .Lx87_0_s
.Lx87_0_s:
 .string "FAIL 1114/004: item 4D assign/read"
# IR_LIT_INTEGER
 xchain0_n83_α:
 mov qword ptr [rbp + 2592], 6
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [rbp + 2600], rax
 jmp xchain0_n90_α
.Lx88_0:
 .quad 2
# IR_SUBSCRIPT x[i] variable
 xchain0_n84_α:
 mov rdi, qword ptr [rbp + 2880]
 mov rsi, qword ptr [rbp + 2888]
 mov rdx, qword ptr [rbp + 2912]
 mov rcx, qword ptr [rbp + 2920]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [rbp + 2944], rax
 mov qword ptr [rbp + 2952], rdx
 jmp xchain0_n91_α
 xchain0_n85_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3264] -> [zr+3168]
 mov rax, qword ptr [rbp + 3264]
 mov qword ptr [rbp + 3168], rax
 mov rax, qword ptr [rbp + 3272]
 mov qword ptr [rbp + 3176], rax
# marshal arg1 = producer-box slot [zr+3296] -> [zr+3184]
 mov rax, qword ptr [rbp + 3296]
 mov qword ptr [rbp + 3184], rax
 mov rax, qword ptr [rbp + 3304]
 mov qword ptr [rbp + 3192], rax
# marshal arg2 = producer-box slot [zr+3328] -> [zr+3200]
 mov rax, qword ptr [rbp + 3328]
 mov qword ptr [rbp + 3200], rax
 mov rax, qword ptr [rbp + 3336]
 mov qword ptr [rbp + 3208], rax
# marshal arg3 = producer-box slot [zr+3360] -> [zr+3216]
 mov rax, qword ptr [rbp + 3360]
 mov qword ptr [rbp + 3216], rax
 mov rax, qword ptr [rbp + 3368]
 mov qword ptr [rbp + 3224], rax
# marshal arg4 = producer-box slot [zr+3392] -> [zr+3232]
 mov rax, qword ptr [rbp + 3392]
 mov qword ptr [rbp + 3232], rax
 mov rax, qword ptr [rbp + 3400]
 mov qword ptr [rbp + 3240], rax
  .section .rodata
  .Lbynamefn86: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn86]
 lea rsi, [rbp + 3168]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3152], rax
 mov qword ptr [rbp + 3160], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n92_α
 xchain0_n85_β:
 jmp xchain0_n61_α
# IR_LIT_STRING
 xchain0_n86_α:
 mov qword ptr [rbp + 3728], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [rbp + 3736], rax
 jmp xchain0_n93_α
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string "key"
# IR_LIT_STRING
 xchain0_n87_α:
 mov qword ptr [rbp + 3968], 1
 mov rax, qword ptr [rip + .Lx92_0]
 mov qword ptr [rbp + 3976], rax
 jmp xchain0_n94_α
.Lx92_0:
 .quad .Lx92_0_s
.Lx92_0_s:
 .string "key"
#         output = 'PASS 1114_item (7/7)'
# IR_LIT_STRING
 xchain0_n88_α:
 mov qword ptr [rbp + 4096], 1
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [rbp + 4104], rax
 jmp xchain0_n95_α
.Lx93_0:
 .quad .Lx93_0_s
.Lx93_0_s:
 .string "PASS 1114_item (7/7)"
# IR_ASSIGN gva
 xchain0_n89_α:
 mov rax, qword ptr [rbp + 2272]
 mov rdx, qword ptr [rbp + 2280]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 2256], rax
 mov qword ptr [rbp + 2264], rdx
 jmp main_γ
# IR_SUBSCRIPT x[i] variable
 xchain0_n90_α:
 mov rdi, qword ptr [rbp + 2560]
 mov rsi, qword ptr [rbp + 2568]
 mov rdx, qword ptr [rbp + 2592]
 mov rcx, qword ptr [rbp + 2600]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [rbp + 2624], rax
 mov qword ptr [rbp + 2632], rdx
 jmp xchain0_n96_α
# IR_LIT_INTEGER
 xchain0_n91_α:
 mov qword ptr [rbp + 2976], 6
 mov rax, qword ptr [rip + .Lx96_0]
 mov qword ptr [rbp + 2984], rax
 jmp xchain0_n97_α
.Lx96_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n92_α:
 mov qword ptr [rbp + 3424], 6
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [rbp + 3432], rax
 jmp xchain0_n98_α
.Lx97_0:
 .quad 2121
 xchain0_n93_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3696] -> [zr+3648]
 mov rax, qword ptr [rbp + 3696]
 mov qword ptr [rbp + 3648], rax
 mov rax, qword ptr [rbp + 3704]
 mov qword ptr [rbp + 3656], rax
# marshal arg1 = producer-box slot [zr+3728] -> [zr+3664]
 mov rax, qword ptr [rbp + 3728]
 mov qword ptr [rbp + 3664], rax
 mov rax, qword ptr [rbp + 3736]
 mov qword ptr [rbp + 3672], rax
  .section .rodata
  .Lbynamefn94: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn94]
 lea rsi, [rbp + 3648]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3632], rax
 mov qword ptr [rbp + 3640], rdx
 cmp eax, 99
 je xchain0_n81_α
 jmp xchain0_n99_α
 xchain0_n93_β:
 jmp xchain0_n81_α
 xchain0_n94_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3936] -> [zr+3888]
 mov rax, qword ptr [rbp + 3936]
 mov qword ptr [rbp + 3888], rax
 mov rax, qword ptr [rbp + 3944]
 mov qword ptr [rbp + 3896], rax
# marshal arg1 = producer-box slot [zr+3968] -> [zr+3904]
 mov rax, qword ptr [rbp + 3968]
 mov qword ptr [rbp + 3904], rax
 mov rax, qword ptr [rbp + 3976]
 mov qword ptr [rbp + 3912], rax
  .section .rodata
  .Lbynamefn95: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn95]
 lea rsi, [rbp + 3888]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3872], rax
 mov qword ptr [rbp + 3880], rdx
 cmp eax, 99
 je xchain0_n88_α
 jmp xchain0_n100_α
 xchain0_n94_β:
 jmp xchain0_n88_α
# IR_ASSIGN gva
 xchain0_n95_α:
 mov rax, qword ptr [rbp + 4096]
 mov rdx, qword ptr [rbp + 4104]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 4080], rax
 mov qword ptr [rbp + 4088], rdx
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n96_α:
 mov rdi, qword ptr [rbp + 2624]
 mov rsi, qword ptr [rbp + 2632]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [rbp + 2656], rax
 mov qword ptr [rbp + 2664], rdx
 jmp xchain0_n101_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n97_α:
 mov rdi, qword ptr [rbp + 2944]
 mov rsi, qword ptr [rbp + 2952]
 mov rdx, qword ptr [rbp + 2976]
 mov rcx, qword ptr [rbp + 2984]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [rbp + 3008], rax
 mov qword ptr [rbp + 3016], rdx
 jmp xchain0_n102_α
 xchain0_n98_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3152] -> [zr+3104]
 mov rax, qword ptr [rbp + 3152]
 mov qword ptr [rbp + 3104], rax
 mov rax, qword ptr [rbp + 3160]
 mov qword ptr [rbp + 3112], rax
# marshal arg1 = producer-box slot [zr+3424] -> [zr+3120]
 mov rax, qword ptr [rbp + 3424]
 mov qword ptr [rbp + 3120], rax
 mov rax, qword ptr [rbp + 3432]
 mov qword ptr [rbp + 3128], rax
  .section .rodata
  .Lbynamefn99: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn99]
 lea rsi, [rbp + 3104]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3088], rax
 mov qword ptr [rbp + 3096], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n103_α
 xchain0_n98_β:
 jmp xchain0_n61_α
# IR_LIT_STRING
 xchain0_n99_α:
 mov qword ptr [rbp + 3760], 1
 mov rax, qword ptr [rip + .Lx104_0]
 mov qword ptr [rbp + 3768], rax
 jmp xchain0_n104_α
.Lx104_0:
 .quad .Lx104_0_s
.Lx104_0_s:
 .string "val"
# IR_LIT_STRING
 xchain0_n100_α:
 mov qword ptr [rbp + 4000], 1
 mov rax, qword ptr [rip + .Lx105_0]
 mov qword ptr [rbp + 4008], rax
 jmp xchain0_n105_α
.Lx105_0:
 .quad .Lx105_0_s
.Lx105_0_s:
 .string "val"
# IR_LIT_INTEGER
 xchain0_n101_α:
 mov qword ptr [rbp + 2672], 6
 mov rax, qword ptr [rip + .Lx106_0]
 mov qword ptr [rbp + 2680], rax
 jmp xchain0_n106_α
.Lx106_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n102_α:
 mov qword ptr [rbp + 3040], 6
 mov rax, qword ptr [rip + .Lx107_0]
 mov qword ptr [rbp + 3048], rax
 jmp xchain0_n107_α
.Lx107_0:
 .quad 2121
#         output = 'FAIL 1114/006: bracket 4D assign, item read' :(end)
# IR_LIT_STRING
 xchain0_n103_α:
 mov qword ptr [rbp + 3472], 1
 mov rax, qword ptr [rip + .Lx108_0]
 mov qword ptr [rbp + 3480], rax
 jmp xchain0_n108_α
.Lx108_0:
 .quad .Lx108_0_s
.Lx108_0_s:
 .string "FAIL 1114/006: bracket 4D assign, item read"
# IR_ASSIGN_VAR
 xchain0_n104_α:
 mov rdi, qword ptr [rbp + 3632]
 mov rsi, qword ptr [rbp + 3640]
 mov rdx, qword ptr [rbp + 3760]
 mov rcx, qword ptr [rbp + 3768]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n81_α
 mov qword ptr [rbp + 3792], rax
 mov qword ptr [rbp + 3800], rdx
 jmp xchain0_n81_α
 xchain0_n105_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3872] -> [zr+3824]
 mov rax, qword ptr [rbp + 3872]
 mov qword ptr [rbp + 3824], rax
 mov rax, qword ptr [rbp + 3880]
 mov qword ptr [rbp + 3832], rax
# marshal arg1 = producer-box slot [zr+4000] -> [zr+3840]
 mov rax, qword ptr [rbp + 4000]
 mov qword ptr [rbp + 3840], rax
 mov rax, qword ptr [rbp + 4008]
 mov qword ptr [rbp + 3848], rax
  .section .rodata
  .Lbynamefn106: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn106]
 lea rsi, [rbp + 3824]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3808], rax
 mov qword ptr [rbp + 3816], rdx
 cmp eax, 99
 je xchain0_n88_α
 jmp xchain0_n109_α
 xchain0_n105_β:
 jmp xchain0_n88_α
 xchain0_n106_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2656] -> [zr+2320]
 mov rax, qword ptr [rbp + 2656]
 mov qword ptr [rbp + 2320], rax
 mov rax, qword ptr [rbp + 2664]
 mov qword ptr [rbp + 2328], rax
# marshal arg1 = producer-box slot [zr+2672] -> [zr+2336]
 mov rax, qword ptr [rbp + 2672]
 mov qword ptr [rbp + 2336], rax
 mov rax, qword ptr [rbp + 2680]
 mov qword ptr [rbp + 2344], rax
  .section .rodata
  .Lbynamefn107: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn107]
 lea rsi, [rbp + 2320]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2304], rax
 mov qword ptr [rbp + 2312], rdx
 cmp eax, 99
 je xchain0_n50_α
 jmp xchain0_n110_α
 xchain0_n106_β:
 jmp xchain0_n50_α
# IR_ASSIGN_VAR
 xchain0_n107_α:
 mov rdi, qword ptr [rbp + 3008]
 mov rsi, qword ptr [rbp + 3016]
 mov rdx, qword ptr [rbp + 3040]
 mov rcx, qword ptr [rbp + 3048]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [rbp + 3072], rax
 mov qword ptr [rbp + 3080], rdx
 jmp xchain0_n55_α
# IR_ASSIGN gva
 xchain0_n108_α:
 mov rax, qword ptr [rbp + 3472]
 mov rdx, qword ptr [rbp + 3480]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 3456], rax
 mov qword ptr [rbp + 3464], rdx
 jmp main_γ
#         output = 'FAIL 1114/007: item on table'        :(end)
# IR_LIT_STRING
 xchain0_n109_α:
 mov qword ptr [rbp + 4048], 1
 mov rax, qword ptr [rip + .Lx114_0]
 mov qword ptr [rbp + 4056], rax
 jmp xchain0_n111_α
.Lx114_0:
 .quad .Lx114_0_s
.Lx114_0_s:
 .string "FAIL 1114/007: item on table"
#         output = 'FAIL 1114/005: item 4D == bracket'   :(end)
# IR_LIT_STRING
 xchain0_n110_α:
 mov qword ptr [rbp + 2720], 1
 mov rax, qword ptr [rip + .Lx115_0]
 mov qword ptr [rbp + 2728], rax
 jmp xchain0_n112_α
.Lx115_0:
 .quad .Lx115_0_s
.Lx115_0_s:
 .string "FAIL 1114/005: item 4D == bracket"
# IR_ASSIGN gva
 xchain0_n111_α:
 mov rax, qword ptr [rbp + 4048]
 mov rdx, qword ptr [rbp + 4056]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 4032], rax
 mov qword ptr [rbp + 4040], rdx
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n112_α:
 mov rax, qword ptr [rbp + 2720]
 mov rdx, qword ptr [rbp + 2728]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 2704], rax
 mov qword ptr [rbp + 2712], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 4136]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 4136]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

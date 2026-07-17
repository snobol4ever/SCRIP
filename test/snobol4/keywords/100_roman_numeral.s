  .intel_syntax noprefix
  .text
  .globl proc_roman_α
proc_roman_α:
#=======================================================================================================================
    .global proc_roman_α
    .global proc_roman_β
    .global proc_roman_γ
    .global proc_roman_ω
  sub rsp, 5696
  mov [rsp + 5672], rcx
  mov [rsp + 5680], rdx
  mov [rsp + 5688], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 5664
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 5656], rsp
proc_roman_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string ""
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rbp + 208], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 216], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad 13
 xchain0_n3_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+208] -> [zr+176]
 mov rax, qword ptr [rbp + 208]
 mov qword ptr [rbp + 176], rax
 mov rax, qword ptr [rbp + 216]
 mov qword ptr [rbp + 184], rax
  .section .rodata
  .Lrkfn5: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn5]
 lea rsi, [rbp + 176]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 160], rax
 mov qword ptr [rbp + 168], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rbp + 160]
 mov rdx, qword ptr [rbp + 168]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 240], rax
 mov qword ptr [rbp + 248], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rbp + 272], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 280], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx8_0:
 .quad 1
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n8_α:
 mov rdi, qword ptr [rbp + 240]
 mov rsi, qword ptr [rbp + 248]
 mov rdx, qword ptr [rbp + 272]
 mov rcx, qword ptr [rbp + 280]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [rbp + 304], rax
 mov qword ptr [rbp + 312], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [rbp + 416], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rbp + 424], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx11_0:
 .quad 2
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [rbp + 336], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n7_α
.Lx13_0:
 .quad 1000
# IR_SUBSCRIPT x[i] variable
 xchain0_n12_α:
 mov rdi, qword ptr [rbp + 384]
 mov rsi, qword ptr [rbp + 392]
 mov rdx, qword ptr [rbp + 416]
 mov rcx, qword ptr [rbp + 424]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rbp + 560], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad 3
# IR_VAR
 xchain0_n14_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 680], rdx
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n19_α
# IR_ASSIGN_VAR
 xchain0_n15_α:
 mov rdi, qword ptr [rbp + 304]
 mov rsi, qword ptr [rbp + 312]
 mov rdx, qword ptr [rbp + 336]
 mov rcx, qword ptr [rbp + 344]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain0_n7_α
 xchain0_n15_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [rbp + 480], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rbp + 488], rax
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n10_α
.Lx18_0:
 .quad 900
# IR_SUBSCRIPT x[i] variable
 xchain0_n17_α:
 mov rdi, qword ptr [rbp + 528]
 mov rsi, qword ptr [rbp + 536]
 mov rdx, qword ptr [rbp + 560]
 mov rcx, qword ptr [rbp + 568]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain0_n21_α
 xchain0_n17_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n18_α:
 mov qword ptr [rbp + 704], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rbp + 712], rax
 jmp xchain0_n22_α
 xchain0_n18_β:
 jmp xchain0_n19_α
.Lx20_0:
 .quad 4
# IR_VAR
 xchain0_n19_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 816], rax
 mov qword ptr [rbp + 824], rdx
 jmp xchain0_n23_α
 xchain0_n19_β:
 jmp xchain0_n24_α
# IR_ASSIGN_VAR
 xchain0_n20_α:
 mov rdi, qword ptr [rbp + 448]
 mov rsi, qword ptr [rbp + 456]
 mov rdx, qword ptr [rbp + 480]
 mov rcx, qword ptr [rbp + 488]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
 jmp xchain0_n10_α
 xchain0_n20_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [rbp + 624], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rbp + 632], rax
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp xchain0_n14_α
.Lx23_0:
 .quad 500
# IR_SUBSCRIPT x[i] variable
 xchain0_n22_α:
 mov rdi, qword ptr [rbp + 672]
 mov rsi, qword ptr [rbp + 680]
 mov rdx, qword ptr [rbp + 704]
 mov rcx, qword ptr [rbp + 712]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n19_α
 mov qword ptr [rbp + 736], rax
 mov qword ptr [rbp + 744], rdx
 jmp xchain0_n26_α
 xchain0_n22_β:
 jmp xchain0_n19_α
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [rbp + 848], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rbp + 856], rax
 jmp xchain0_n27_α
 xchain0_n23_β:
 jmp xchain0_n24_α
.Lx25_0:
 .quad 5
# IR_VAR
 xchain0_n24_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 960], rax
 mov qword ptr [rbp + 968], rdx
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp xchain0_n29_α
# IR_ASSIGN_VAR
 xchain0_n25_α:
 mov rdi, qword ptr [rbp + 592]
 mov rsi, qword ptr [rbp + 600]
 mov rdx, qword ptr [rbp + 624]
 mov rcx, qword ptr [rbp + 632]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 jmp xchain0_n14_α
 xchain0_n25_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [rbp + 768], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rbp + 776], rax
 jmp xchain0_n30_α
 xchain0_n26_β:
 jmp xchain0_n19_α
.Lx28_0:
 .quad 400
# IR_SUBSCRIPT x[i] variable
 xchain0_n27_α:
 mov rdi, qword ptr [rbp + 816]
 mov rsi, qword ptr [rbp + 824]
 mov rdx, qword ptr [rbp + 848]
 mov rcx, qword ptr [rbp + 856]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n24_α
 mov qword ptr [rbp + 880], rax
 mov qword ptr [rbp + 888], rdx
 jmp xchain0_n31_α
 xchain0_n27_β:
 jmp xchain0_n24_α
# IR_LIT_INTEGER
 xchain0_n28_α:
 mov qword ptr [rbp + 992], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rbp + 1000], rax
 jmp xchain0_n32_α
 xchain0_n28_β:
 jmp xchain0_n29_α
.Lx30_0:
 .quad 6
# IR_VAR
 xchain0_n29_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 1104], rax
 mov qword ptr [rbp + 1112], rdx
 jmp xchain0_n33_α
 xchain0_n29_β:
 jmp xchain0_n34_α
# IR_ASSIGN_VAR
 xchain0_n30_α:
 mov rdi, qword ptr [rbp + 736]
 mov rsi, qword ptr [rbp + 744]
 mov rdx, qword ptr [rbp + 768]
 mov rcx, qword ptr [rbp + 776]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n19_α
 mov qword ptr [rbp + 800], rax
 mov qword ptr [rbp + 808], rdx
 jmp xchain0_n19_α
 xchain0_n30_β:
 jmp xchain0_n19_α
# IR_LIT_INTEGER
 xchain0_n31_α:
 mov qword ptr [rbp + 912], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [rbp + 920], rax
 jmp xchain0_n35_α
 xchain0_n31_β:
 jmp xchain0_n24_α
.Lx33_0:
 .quad 100
# IR_SUBSCRIPT x[i] variable
 xchain0_n32_α:
 mov rdi, qword ptr [rbp + 960]
 mov rsi, qword ptr [rbp + 968]
 mov rdx, qword ptr [rbp + 992]
 mov rcx, qword ptr [rbp + 1000]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [rbp + 1024], rax
 mov qword ptr [rbp + 1032], rdx
 jmp xchain0_n36_α
 xchain0_n32_β:
 jmp xchain0_n29_α
# IR_LIT_INTEGER
 xchain0_n33_α:
 mov qword ptr [rbp + 1136], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [rbp + 1144], rax
 jmp xchain0_n37_α
 xchain0_n33_β:
 jmp xchain0_n34_α
.Lx35_0:
 .quad 7
# IR_VAR
 xchain0_n34_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 1248], rax
 mov qword ptr [rbp + 1256], rdx
 jmp xchain0_n38_α
 xchain0_n34_β:
 jmp xchain0_n39_α
# IR_ASSIGN_VAR
 xchain0_n35_α:
 mov rdi, qword ptr [rbp + 880]
 mov rsi, qword ptr [rbp + 888]
 mov rdx, qword ptr [rbp + 912]
 mov rcx, qword ptr [rbp + 920]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n24_α
 mov qword ptr [rbp + 944], rax
 mov qword ptr [rbp + 952], rdx
 jmp xchain0_n24_α
 xchain0_n35_β:
 jmp xchain0_n24_α
# IR_LIT_INTEGER
 xchain0_n36_α:
 mov qword ptr [rbp + 1056], 6
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [rbp + 1064], rax
 jmp xchain0_n40_α
 xchain0_n36_β:
 jmp xchain0_n29_α
.Lx38_0:
 .quad 90
# IR_SUBSCRIPT x[i] variable
 xchain0_n37_α:
 mov rdi, qword ptr [rbp + 1104]
 mov rsi, qword ptr [rbp + 1112]
 mov rdx, qword ptr [rbp + 1136]
 mov rcx, qword ptr [rbp + 1144]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n34_α
 mov qword ptr [rbp + 1168], rax
 mov qword ptr [rbp + 1176], rdx
 jmp xchain0_n41_α
 xchain0_n37_β:
 jmp xchain0_n34_α
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [rbp + 1280], 6
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [rbp + 1288], rax
 jmp xchain0_n42_α
 xchain0_n38_β:
 jmp xchain0_n39_α
.Lx40_0:
 .quad 8
# IR_VAR
 xchain0_n39_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 1392], rax
 mov qword ptr [rbp + 1400], rdx
 jmp xchain0_n43_α
 xchain0_n39_β:
 jmp xchain0_n44_α
# IR_ASSIGN_VAR
 xchain0_n40_α:
 mov rdi, qword ptr [rbp + 1024]
 mov rsi, qword ptr [rbp + 1032]
 mov rdx, qword ptr [rbp + 1056]
 mov rcx, qword ptr [rbp + 1064]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [rbp + 1088], rax
 mov qword ptr [rbp + 1096], rdx
 jmp xchain0_n29_α
 xchain0_n40_β:
 jmp xchain0_n29_α
# IR_LIT_INTEGER
 xchain0_n41_α:
 mov qword ptr [rbp + 1200], 6
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [rbp + 1208], rax
 jmp xchain0_n45_α
 xchain0_n41_β:
 jmp xchain0_n34_α
.Lx43_0:
 .quad 50
# IR_SUBSCRIPT x[i] variable
 xchain0_n42_α:
 mov rdi, qword ptr [rbp + 1248]
 mov rsi, qword ptr [rbp + 1256]
 mov rdx, qword ptr [rbp + 1280]
 mov rcx, qword ptr [rbp + 1288]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n39_α
 mov qword ptr [rbp + 1312], rax
 mov qword ptr [rbp + 1320], rdx
 jmp xchain0_n46_α
 xchain0_n42_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n43_α:
 mov qword ptr [rbp + 1424], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [rbp + 1432], rax
 jmp xchain0_n47_α
 xchain0_n43_β:
 jmp xchain0_n44_α
.Lx45_0:
 .quad 9
# IR_VAR
 xchain0_n44_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 1536], rax
 mov qword ptr [rbp + 1544], rdx
 jmp xchain0_n48_α
 xchain0_n44_β:
 jmp xchain0_n49_α
# IR_ASSIGN_VAR
 xchain0_n45_α:
 mov rdi, qword ptr [rbp + 1168]
 mov rsi, qword ptr [rbp + 1176]
 mov rdx, qword ptr [rbp + 1200]
 mov rcx, qword ptr [rbp + 1208]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n34_α
 mov qword ptr [rbp + 1232], rax
 mov qword ptr [rbp + 1240], rdx
 jmp xchain0_n34_α
 xchain0_n45_β:
 jmp xchain0_n34_α
# IR_LIT_INTEGER
 xchain0_n46_α:
 mov qword ptr [rbp + 1344], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [rbp + 1352], rax
 jmp xchain0_n50_α
 xchain0_n46_β:
 jmp xchain0_n39_α
.Lx48_0:
 .quad 40
# IR_SUBSCRIPT x[i] variable
 xchain0_n47_α:
 mov rdi, qword ptr [rbp + 1392]
 mov rsi, qword ptr [rbp + 1400]
 mov rdx, qword ptr [rbp + 1424]
 mov rcx, qword ptr [rbp + 1432]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n44_α
 mov qword ptr [rbp + 1456], rax
 mov qword ptr [rbp + 1464], rdx
 jmp xchain0_n51_α
 xchain0_n47_β:
 jmp xchain0_n44_α
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [rbp + 1568], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [rbp + 1576], rax
 jmp xchain0_n52_α
 xchain0_n48_β:
 jmp xchain0_n49_α
.Lx50_0:
 .quad 10
# IR_VAR
 xchain0_n49_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 1680], rax
 mov qword ptr [rbp + 1688], rdx
 jmp xchain0_n53_α
 xchain0_n49_β:
 jmp xchain0_n54_α
# IR_ASSIGN_VAR
 xchain0_n50_α:
 mov rdi, qword ptr [rbp + 1312]
 mov rsi, qword ptr [rbp + 1320]
 mov rdx, qword ptr [rbp + 1344]
 mov rcx, qword ptr [rbp + 1352]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n39_α
 mov qword ptr [rbp + 1376], rax
 mov qword ptr [rbp + 1384], rdx
 jmp xchain0_n39_α
 xchain0_n50_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n51_α:
 mov qword ptr [rbp + 1488], 6
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [rbp + 1496], rax
 jmp xchain0_n55_α
 xchain0_n51_β:
 jmp xchain0_n44_α
.Lx53_0:
 .quad 10
# IR_SUBSCRIPT x[i] variable
 xchain0_n52_α:
 mov rdi, qword ptr [rbp + 1536]
 mov rsi, qword ptr [rbp + 1544]
 mov rdx, qword ptr [rbp + 1568]
 mov rcx, qword ptr [rbp + 1576]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [rbp + 1600], rax
 mov qword ptr [rbp + 1608], rdx
 jmp xchain0_n56_α
 xchain0_n52_β:
 jmp xchain0_n49_α
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [rbp + 1712], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [rbp + 1720], rax
 jmp xchain0_n57_α
 xchain0_n53_β:
 jmp xchain0_n54_α
.Lx55_0:
 .quad 11
# IR_VAR
 xchain0_n54_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 1824], rax
 mov qword ptr [rbp + 1832], rdx
 jmp xchain0_n58_α
 xchain0_n54_β:
 jmp xchain0_n59_α
# IR_ASSIGN_VAR
 xchain0_n55_α:
 mov rdi, qword ptr [rbp + 1456]
 mov rsi, qword ptr [rbp + 1464]
 mov rdx, qword ptr [rbp + 1488]
 mov rcx, qword ptr [rbp + 1496]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n44_α
 mov qword ptr [rbp + 1520], rax
 mov qword ptr [rbp + 1528], rdx
 jmp xchain0_n44_α
 xchain0_n55_β:
 jmp xchain0_n44_α
# IR_LIT_INTEGER
 xchain0_n56_α:
 mov qword ptr [rbp + 1632], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [rbp + 1640], rax
 jmp xchain0_n60_α
 xchain0_n56_β:
 jmp xchain0_n49_α
.Lx58_0:
 .quad 9
# IR_SUBSCRIPT x[i] variable
 xchain0_n57_α:
 mov rdi, qword ptr [rbp + 1680]
 mov rsi, qword ptr [rbp + 1688]
 mov rdx, qword ptr [rbp + 1712]
 mov rcx, qword ptr [rbp + 1720]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [rbp + 1744], rax
 mov qword ptr [rbp + 1752], rdx
 jmp xchain0_n61_α
 xchain0_n57_β:
 jmp xchain0_n54_α
# IR_LIT_INTEGER
 xchain0_n58_α:
 mov qword ptr [rbp + 1856], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [rbp + 1864], rax
 jmp xchain0_n62_α
 xchain0_n58_β:
 jmp xchain0_n59_α
.Lx60_0:
 .quad 12
# IR_VAR
 xchain0_n59_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 1968], rax
 mov qword ptr [rbp + 1976], rdx
 jmp xchain0_n63_α
 xchain0_n59_β:
 jmp xchain0_n64_α
# IR_ASSIGN_VAR
 xchain0_n60_α:
 mov rdi, qword ptr [rbp + 1600]
 mov rsi, qword ptr [rbp + 1608]
 mov rdx, qword ptr [rbp + 1632]
 mov rcx, qword ptr [rbp + 1640]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [rbp + 1664], rax
 mov qword ptr [rbp + 1672], rdx
 jmp xchain0_n49_α
 xchain0_n60_β:
 jmp xchain0_n49_α
# IR_LIT_INTEGER
 xchain0_n61_α:
 mov qword ptr [rbp + 1776], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [rbp + 1784], rax
 jmp xchain0_n65_α
 xchain0_n61_β:
 jmp xchain0_n54_α
.Lx63_0:
 .quad 5
# IR_SUBSCRIPT x[i] variable
 xchain0_n62_α:
 mov rdi, qword ptr [rbp + 1824]
 mov rsi, qword ptr [rbp + 1832]
 mov rdx, qword ptr [rbp + 1856]
 mov rcx, qword ptr [rbp + 1864]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n59_α
 mov qword ptr [rbp + 1888], rax
 mov qword ptr [rbp + 1896], rdx
 jmp xchain0_n66_α
 xchain0_n62_β:
 jmp xchain0_n59_α
# IR_LIT_INTEGER
 xchain0_n63_α:
 mov qword ptr [rbp + 2000], 6
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [rbp + 2008], rax
 jmp xchain0_n67_α
 xchain0_n63_β:
 jmp xchain0_n64_α
.Lx65_0:
 .quad 13
# IR_LIT_INTEGER
 xchain0_n64_α:
 mov qword ptr [rbp + 2176], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [rbp + 2184], rax
 jmp xchain0_n68_α
 xchain0_n64_β:
 jmp xchain0_n72_α
.Lx66_0:
 .quad 13
# IR_ASSIGN_VAR
 xchain0_n65_α:
 mov rdi, qword ptr [rbp + 1744]
 mov rsi, qword ptr [rbp + 1752]
 mov rdx, qword ptr [rbp + 1776]
 mov rcx, qword ptr [rbp + 1784]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [rbp + 1808], rax
 mov qword ptr [rbp + 1816], rdx
 jmp xchain0_n54_α
 xchain0_n65_β:
 jmp xchain0_n54_α
# IR_LIT_INTEGER
 xchain0_n66_α:
 mov qword ptr [rbp + 1920], 6
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [rbp + 1928], rax
 jmp xchain0_n69_α
 xchain0_n66_β:
 jmp xchain0_n59_α
.Lx68_0:
 .quad 4
# IR_SUBSCRIPT x[i] variable
 xchain0_n67_α:
 mov rdi, qword ptr [rbp + 1968]
 mov rsi, qword ptr [rbp + 1976]
 mov rdx, qword ptr [rbp + 2000]
 mov rcx, qword ptr [rbp + 2008]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n64_α
 mov qword ptr [rbp + 2032], rax
 mov qword ptr [rbp + 2040], rdx
 jmp xchain0_n70_α
 xchain0_n67_β:
 jmp xchain0_n64_α
 xchain0_n68_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2176] -> [zr+2144]
 mov rax, qword ptr [rbp + 2176]
 mov qword ptr [rbp + 2144], rax
 mov rax, qword ptr [rbp + 2184]
 mov qword ptr [rbp + 2152], rax
  .section .rodata
  .Lrkfn71: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn71]
 lea rsi, [rbp + 2144]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2128], rax
 mov qword ptr [rbp + 2136], rdx
 cmp eax, 99
 je xchain0_n72_α
 jmp xchain0_n71_α
 xchain0_n68_β:
 jmp xchain0_n72_α
# IR_ASSIGN_VAR
 xchain0_n69_α:
 mov rdi, qword ptr [rbp + 1888]
 mov rsi, qword ptr [rbp + 1896]
 mov rdx, qword ptr [rbp + 1920]
 mov rcx, qword ptr [rbp + 1928]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n59_α
 mov qword ptr [rbp + 1952], rax
 mov qword ptr [rbp + 1960], rdx
 jmp xchain0_n59_α
 xchain0_n69_β:
 jmp xchain0_n59_α
# IR_LIT_INTEGER
 xchain0_n70_α:
 mov qword ptr [rbp + 2064], 6
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [rbp + 2072], rax
 jmp xchain0_n73_α
 xchain0_n70_β:
 jmp xchain0_n64_α
.Lx73_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n71_α:
 mov rax, qword ptr [rbp + 2128]
 mov rdx, qword ptr [rbp + 2136]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rbp + 2112], rax
 mov qword ptr [rbp + 2120], rdx
 jmp xchain0_n72_α
 xchain0_n71_β:
 jmp xchain0_n72_α
# IR_VAR
 xchain0_n72_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 2208], rax
 mov qword ptr [rbp + 2216], rdx
 jmp xchain0_n74_α
 xchain0_n72_β:
 jmp xchain0_n75_α
# IR_ASSIGN_VAR
 xchain0_n73_α:
 mov rdi, qword ptr [rbp + 2032]
 mov rsi, qword ptr [rbp + 2040]
 mov rdx, qword ptr [rbp + 2064]
 mov rcx, qword ptr [rbp + 2072]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n64_α
 mov qword ptr [rbp + 2096], rax
 mov qword ptr [rbp + 2104], rdx
 jmp xchain0_n64_α
 xchain0_n73_β:
 jmp xchain0_n64_α
# IR_LIT_INTEGER
 xchain0_n74_α:
 mov qword ptr [rbp + 2240], 6
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [rbp + 2248], rax
 jmp xchain0_n76_α
 xchain0_n74_β:
 jmp xchain0_n75_α
.Lx77_0:
 .quad 1
# IR_VAR
 xchain0_n75_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 2352], rax
 mov qword ptr [rbp + 2360], rdx
 jmp xchain0_n77_α
 xchain0_n75_β:
 jmp xchain0_n78_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n76_α:
 mov rdi, qword ptr [rbp + 2208]
 mov rsi, qword ptr [rbp + 2216]
 mov rdx, qword ptr [rbp + 2240]
 mov rcx, qword ptr [rbp + 2248]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n75_α
 mov qword ptr [rbp + 2272], rax
 mov qword ptr [rbp + 2280], rdx
 jmp xchain0_n79_α
 xchain0_n76_β:
 jmp xchain0_n75_α
# IR_LIT_INTEGER
 xchain0_n77_α:
 mov qword ptr [rbp + 2384], 6
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [rbp + 2392], rax
 jmp xchain0_n80_α
 xchain0_n77_β:
 jmp xchain0_n78_α
.Lx80_0:
 .quad 2
# IR_VAR
 xchain0_n78_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 2496], rax
 mov qword ptr [rbp + 2504], rdx
 jmp xchain0_n81_α
 xchain0_n78_β:
 jmp xchain0_n82_α
# IR_LIT_STRING
 xchain0_n79_α:
 mov qword ptr [rbp + 2304], 1
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [rbp + 2312], rax
 jmp xchain0_n83_α
 xchain0_n79_β:
 jmp xchain0_n75_α
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string "M"
# IR_SUBSCRIPT x[i] variable
 xchain0_n80_α:
 mov rdi, qword ptr [rbp + 2352]
 mov rsi, qword ptr [rbp + 2360]
 mov rdx, qword ptr [rbp + 2384]
 mov rcx, qword ptr [rbp + 2392]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n78_α
 mov qword ptr [rbp + 2416], rax
 mov qword ptr [rbp + 2424], rdx
 jmp xchain0_n84_α
 xchain0_n80_β:
 jmp xchain0_n78_α
# IR_LIT_INTEGER
 xchain0_n81_α:
 mov qword ptr [rbp + 2528], 6
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [rbp + 2536], rax
 jmp xchain0_n85_α
 xchain0_n81_β:
 jmp xchain0_n82_α
.Lx84_0:
 .quad 3
# IR_VAR
 xchain0_n82_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 2640], rax
 mov qword ptr [rbp + 2648], rdx
 jmp xchain0_n86_α
 xchain0_n82_β:
 jmp xchain0_n87_α
# IR_ASSIGN_VAR
 xchain0_n83_α:
 mov rdi, qword ptr [rbp + 2272]
 mov rsi, qword ptr [rbp + 2280]
 mov rdx, qword ptr [rbp + 2304]
 mov rcx, qword ptr [rbp + 2312]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n75_α
 mov qword ptr [rbp + 2336], rax
 mov qword ptr [rbp + 2344], rdx
 jmp xchain0_n75_α
 xchain0_n83_β:
 jmp xchain0_n75_α
# IR_LIT_STRING
 xchain0_n84_α:
 mov qword ptr [rbp + 2448], 1
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [rbp + 2456], rax
 jmp xchain0_n88_α
 xchain0_n84_β:
 jmp xchain0_n78_α
.Lx87_0:
 .quad .Lx87_0_s
.Lx87_0_s:
 .string "CM"
# IR_SUBSCRIPT x[i] variable
 xchain0_n85_α:
 mov rdi, qword ptr [rbp + 2496]
 mov rsi, qword ptr [rbp + 2504]
 mov rdx, qword ptr [rbp + 2528]
 mov rcx, qword ptr [rbp + 2536]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n82_α
 mov qword ptr [rbp + 2560], rax
 mov qword ptr [rbp + 2568], rdx
 jmp xchain0_n89_α
 xchain0_n85_β:
 jmp xchain0_n82_α
# IR_LIT_INTEGER
 xchain0_n86_α:
 mov qword ptr [rbp + 2672], 6
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [rbp + 2680], rax
 jmp xchain0_n90_α
 xchain0_n86_β:
 jmp xchain0_n87_α
.Lx89_0:
 .quad 4
# IR_VAR
 xchain0_n87_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 2784], rax
 mov qword ptr [rbp + 2792], rdx
 jmp xchain0_n91_α
 xchain0_n87_β:
 jmp xchain0_n92_α
# IR_ASSIGN_VAR
 xchain0_n88_α:
 mov rdi, qword ptr [rbp + 2416]
 mov rsi, qword ptr [rbp + 2424]
 mov rdx, qword ptr [rbp + 2448]
 mov rcx, qword ptr [rbp + 2456]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n78_α
 mov qword ptr [rbp + 2480], rax
 mov qword ptr [rbp + 2488], rdx
 jmp xchain0_n78_α
 xchain0_n88_β:
 jmp xchain0_n78_α
# IR_LIT_STRING
 xchain0_n89_α:
 mov qword ptr [rbp + 2592], 1
 mov rax, qword ptr [rip + .Lx92_0]
 mov qword ptr [rbp + 2600], rax
 jmp xchain0_n93_α
 xchain0_n89_β:
 jmp xchain0_n82_α
.Lx92_0:
 .quad .Lx92_0_s
.Lx92_0_s:
 .string "D"
# IR_SUBSCRIPT x[i] variable
 xchain0_n90_α:
 mov rdi, qword ptr [rbp + 2640]
 mov rsi, qword ptr [rbp + 2648]
 mov rdx, qword ptr [rbp + 2672]
 mov rcx, qword ptr [rbp + 2680]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n87_α
 mov qword ptr [rbp + 2704], rax
 mov qword ptr [rbp + 2712], rdx
 jmp xchain0_n94_α
 xchain0_n90_β:
 jmp xchain0_n87_α
# IR_LIT_INTEGER
 xchain0_n91_α:
 mov qword ptr [rbp + 2816], 6
 mov rax, qword ptr [rip + .Lx94_0]
 mov qword ptr [rbp + 2824], rax
 jmp xchain0_n95_α
 xchain0_n91_β:
 jmp xchain0_n92_α
.Lx94_0:
 .quad 5
# IR_VAR
 xchain0_n92_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 2928], rax
 mov qword ptr [rbp + 2936], rdx
 jmp xchain0_n96_α
 xchain0_n92_β:
 jmp xchain0_n97_α
# IR_ASSIGN_VAR
 xchain0_n93_α:
 mov rdi, qword ptr [rbp + 2560]
 mov rsi, qword ptr [rbp + 2568]
 mov rdx, qword ptr [rbp + 2592]
 mov rcx, qword ptr [rbp + 2600]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n82_α
 mov qword ptr [rbp + 2624], rax
 mov qword ptr [rbp + 2632], rdx
 jmp xchain0_n82_α
 xchain0_n93_β:
 jmp xchain0_n82_α
# IR_LIT_STRING
 xchain0_n94_α:
 mov qword ptr [rbp + 2736], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [rbp + 2744], rax
 jmp xchain0_n98_α
 xchain0_n94_β:
 jmp xchain0_n87_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string "CD"
# IR_SUBSCRIPT x[i] variable
 xchain0_n95_α:
 mov rdi, qword ptr [rbp + 2784]
 mov rsi, qword ptr [rbp + 2792]
 mov rdx, qword ptr [rbp + 2816]
 mov rcx, qword ptr [rbp + 2824]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n92_α
 mov qword ptr [rbp + 2848], rax
 mov qword ptr [rbp + 2856], rdx
 jmp xchain0_n99_α
 xchain0_n95_β:
 jmp xchain0_n92_α
# IR_LIT_INTEGER
 xchain0_n96_α:
 mov qword ptr [rbp + 2960], 6
 mov rax, qword ptr [rip + .Lx99_0]
 mov qword ptr [rbp + 2968], rax
 jmp xchain0_n100_α
 xchain0_n96_β:
 jmp xchain0_n97_α
.Lx99_0:
 .quad 6
# IR_VAR
 xchain0_n97_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 3072], rax
 mov qword ptr [rbp + 3080], rdx
 jmp xchain0_n101_α
 xchain0_n97_β:
 jmp xchain0_n102_α
# IR_ASSIGN_VAR
 xchain0_n98_α:
 mov rdi, qword ptr [rbp + 2704]
 mov rsi, qword ptr [rbp + 2712]
 mov rdx, qword ptr [rbp + 2736]
 mov rcx, qword ptr [rbp + 2744]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n87_α
 mov qword ptr [rbp + 2768], rax
 mov qword ptr [rbp + 2776], rdx
 jmp xchain0_n87_α
 xchain0_n98_β:
 jmp xchain0_n87_α
# IR_LIT_STRING
 xchain0_n99_α:
 mov qword ptr [rbp + 2880], 1
 mov rax, qword ptr [rip + .Lx102_0]
 mov qword ptr [rbp + 2888], rax
 jmp xchain0_n103_α
 xchain0_n99_β:
 jmp xchain0_n92_α
.Lx102_0:
 .quad .Lx102_0_s
.Lx102_0_s:
 .string "C"
# IR_SUBSCRIPT x[i] variable
 xchain0_n100_α:
 mov rdi, qword ptr [rbp + 2928]
 mov rsi, qword ptr [rbp + 2936]
 mov rdx, qword ptr [rbp + 2960]
 mov rcx, qword ptr [rbp + 2968]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n97_α
 mov qword ptr [rbp + 2992], rax
 mov qword ptr [rbp + 3000], rdx
 jmp xchain0_n104_α
 xchain0_n100_β:
 jmp xchain0_n97_α
# IR_LIT_INTEGER
 xchain0_n101_α:
 mov qword ptr [rbp + 3104], 6
 mov rax, qword ptr [rip + .Lx104_0]
 mov qword ptr [rbp + 3112], rax
 jmp xchain0_n105_α
 xchain0_n101_β:
 jmp xchain0_n102_α
.Lx104_0:
 .quad 7
# IR_VAR
 xchain0_n102_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 3216], rax
 mov qword ptr [rbp + 3224], rdx
 jmp xchain0_n106_α
 xchain0_n102_β:
 jmp xchain0_n107_α
# IR_ASSIGN_VAR
 xchain0_n103_α:
 mov rdi, qword ptr [rbp + 2848]
 mov rsi, qword ptr [rbp + 2856]
 mov rdx, qword ptr [rbp + 2880]
 mov rcx, qword ptr [rbp + 2888]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n92_α
 mov qword ptr [rbp + 2912], rax
 mov qword ptr [rbp + 2920], rdx
 jmp xchain0_n92_α
 xchain0_n103_β:
 jmp xchain0_n92_α
# IR_LIT_STRING
 xchain0_n104_α:
 mov qword ptr [rbp + 3024], 1
 mov rax, qword ptr [rip + .Lx107_0]
 mov qword ptr [rbp + 3032], rax
 jmp xchain0_n108_α
 xchain0_n104_β:
 jmp xchain0_n97_α
.Lx107_0:
 .quad .Lx107_0_s
.Lx107_0_s:
 .string "XC"
# IR_SUBSCRIPT x[i] variable
 xchain0_n105_α:
 mov rdi, qword ptr [rbp + 3072]
 mov rsi, qword ptr [rbp + 3080]
 mov rdx, qword ptr [rbp + 3104]
 mov rcx, qword ptr [rbp + 3112]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n102_α
 mov qword ptr [rbp + 3136], rax
 mov qword ptr [rbp + 3144], rdx
 jmp xchain0_n109_α
 xchain0_n105_β:
 jmp xchain0_n102_α
# IR_LIT_INTEGER
 xchain0_n106_α:
 mov qword ptr [rbp + 3248], 6
 mov rax, qword ptr [rip + .Lx109_0]
 mov qword ptr [rbp + 3256], rax
 jmp xchain0_n110_α
 xchain0_n106_β:
 jmp xchain0_n107_α
.Lx109_0:
 .quad 8
# IR_VAR
 xchain0_n107_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 3360], rax
 mov qword ptr [rbp + 3368], rdx
 jmp xchain0_n111_α
 xchain0_n107_β:
 jmp xchain0_n112_α
# IR_ASSIGN_VAR
 xchain0_n108_α:
 mov rdi, qword ptr [rbp + 2992]
 mov rsi, qword ptr [rbp + 3000]
 mov rdx, qword ptr [rbp + 3024]
 mov rcx, qword ptr [rbp + 3032]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n97_α
 mov qword ptr [rbp + 3056], rax
 mov qword ptr [rbp + 3064], rdx
 jmp xchain0_n97_α
 xchain0_n108_β:
 jmp xchain0_n97_α
# IR_LIT_STRING
 xchain0_n109_α:
 mov qword ptr [rbp + 3168], 1
 mov rax, qword ptr [rip + .Lx112_0]
 mov qword ptr [rbp + 3176], rax
 jmp xchain0_n113_α
 xchain0_n109_β:
 jmp xchain0_n102_α
.Lx112_0:
 .quad .Lx112_0_s
.Lx112_0_s:
 .string "L"
# IR_SUBSCRIPT x[i] variable
 xchain0_n110_α:
 mov rdi, qword ptr [rbp + 3216]
 mov rsi, qword ptr [rbp + 3224]
 mov rdx, qword ptr [rbp + 3248]
 mov rcx, qword ptr [rbp + 3256]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n107_α
 mov qword ptr [rbp + 3280], rax
 mov qword ptr [rbp + 3288], rdx
 jmp xchain0_n114_α
 xchain0_n110_β:
 jmp xchain0_n107_α
# IR_LIT_INTEGER
 xchain0_n111_α:
 mov qword ptr [rbp + 3392], 6
 mov rax, qword ptr [rip + .Lx114_0]
 mov qword ptr [rbp + 3400], rax
 jmp xchain0_n115_α
 xchain0_n111_β:
 jmp xchain0_n112_α
.Lx114_0:
 .quad 9
# IR_VAR
 xchain0_n112_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 3504], rax
 mov qword ptr [rbp + 3512], rdx
 jmp xchain0_n116_α
 xchain0_n112_β:
 jmp xchain0_n117_α
# IR_ASSIGN_VAR
 xchain0_n113_α:
 mov rdi, qword ptr [rbp + 3136]
 mov rsi, qword ptr [rbp + 3144]
 mov rdx, qword ptr [rbp + 3168]
 mov rcx, qword ptr [rbp + 3176]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n102_α
 mov qword ptr [rbp + 3200], rax
 mov qword ptr [rbp + 3208], rdx
 jmp xchain0_n102_α
 xchain0_n113_β:
 jmp xchain0_n102_α
# IR_LIT_STRING
 xchain0_n114_α:
 mov qword ptr [rbp + 3312], 1
 mov rax, qword ptr [rip + .Lx117_0]
 mov qword ptr [rbp + 3320], rax
 jmp xchain0_n118_α
 xchain0_n114_β:
 jmp xchain0_n107_α
.Lx117_0:
 .quad .Lx117_0_s
.Lx117_0_s:
 .string "XL"
# IR_SUBSCRIPT x[i] variable
 xchain0_n115_α:
 mov rdi, qword ptr [rbp + 3360]
 mov rsi, qword ptr [rbp + 3368]
 mov rdx, qword ptr [rbp + 3392]
 mov rcx, qword ptr [rbp + 3400]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n112_α
 mov qword ptr [rbp + 3424], rax
 mov qword ptr [rbp + 3432], rdx
 jmp xchain0_n119_α
 xchain0_n115_β:
 jmp xchain0_n112_α
# IR_LIT_INTEGER
 xchain0_n116_α:
 mov qword ptr [rbp + 3536], 6
 mov rax, qword ptr [rip + .Lx119_0]
 mov qword ptr [rbp + 3544], rax
 jmp xchain0_n120_α
 xchain0_n116_β:
 jmp xchain0_n117_α
.Lx119_0:
 .quad 10
# IR_VAR
 xchain0_n117_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 3648], rax
 mov qword ptr [rbp + 3656], rdx
 jmp xchain0_n121_α
 xchain0_n117_β:
 jmp xchain0_n122_α
# IR_ASSIGN_VAR
 xchain0_n118_α:
 mov rdi, qword ptr [rbp + 3280]
 mov rsi, qword ptr [rbp + 3288]
 mov rdx, qword ptr [rbp + 3312]
 mov rcx, qword ptr [rbp + 3320]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n107_α
 mov qword ptr [rbp + 3344], rax
 mov qword ptr [rbp + 3352], rdx
 jmp xchain0_n107_α
 xchain0_n118_β:
 jmp xchain0_n107_α
# IR_LIT_STRING
 xchain0_n119_α:
 mov qword ptr [rbp + 3456], 1
 mov rax, qword ptr [rip + .Lx122_0]
 mov qword ptr [rbp + 3464], rax
 jmp xchain0_n123_α
 xchain0_n119_β:
 jmp xchain0_n112_α
.Lx122_0:
 .quad .Lx122_0_s
.Lx122_0_s:
 .string "X"
# IR_SUBSCRIPT x[i] variable
 xchain0_n120_α:
 mov rdi, qword ptr [rbp + 3504]
 mov rsi, qword ptr [rbp + 3512]
 mov rdx, qword ptr [rbp + 3536]
 mov rcx, qword ptr [rbp + 3544]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n117_α
 mov qword ptr [rbp + 3568], rax
 mov qword ptr [rbp + 3576], rdx
 jmp xchain0_n124_α
 xchain0_n120_β:
 jmp xchain0_n117_α
# IR_LIT_INTEGER
 xchain0_n121_α:
 mov qword ptr [rbp + 3680], 6
 mov rax, qword ptr [rip + .Lx124_0]
 mov qword ptr [rbp + 3688], rax
 jmp xchain0_n125_α
 xchain0_n121_β:
 jmp xchain0_n122_α
.Lx124_0:
 .quad 11
# IR_VAR
 xchain0_n122_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 3792], rax
 mov qword ptr [rbp + 3800], rdx
 jmp xchain0_n126_α
 xchain0_n122_β:
 jmp xchain0_n127_α
# IR_ASSIGN_VAR
 xchain0_n123_α:
 mov rdi, qword ptr [rbp + 3424]
 mov rsi, qword ptr [rbp + 3432]
 mov rdx, qword ptr [rbp + 3456]
 mov rcx, qword ptr [rbp + 3464]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n112_α
 mov qword ptr [rbp + 3488], rax
 mov qword ptr [rbp + 3496], rdx
 jmp xchain0_n112_α
 xchain0_n123_β:
 jmp xchain0_n112_α
# IR_LIT_STRING
 xchain0_n124_α:
 mov qword ptr [rbp + 3600], 1
 mov rax, qword ptr [rip + .Lx127_0]
 mov qword ptr [rbp + 3608], rax
 jmp xchain0_n128_α
 xchain0_n124_β:
 jmp xchain0_n117_α
.Lx127_0:
 .quad .Lx127_0_s
.Lx127_0_s:
 .string "IX"
# IR_SUBSCRIPT x[i] variable
 xchain0_n125_α:
 mov rdi, qword ptr [rbp + 3648]
 mov rsi, qword ptr [rbp + 3656]
 mov rdx, qword ptr [rbp + 3680]
 mov rcx, qword ptr [rbp + 3688]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n122_α
 mov qword ptr [rbp + 3712], rax
 mov qword ptr [rbp + 3720], rdx
 jmp xchain0_n129_α
 xchain0_n125_β:
 jmp xchain0_n122_α
# IR_LIT_INTEGER
 xchain0_n126_α:
 mov qword ptr [rbp + 3824], 6
 mov rax, qword ptr [rip + .Lx129_0]
 mov qword ptr [rbp + 3832], rax
 jmp xchain0_n130_α
 xchain0_n126_β:
 jmp xchain0_n127_α
.Lx129_0:
 .quad 12
# IR_VAR
 xchain0_n127_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 3936], rax
 mov qword ptr [rbp + 3944], rdx
 jmp xchain0_n131_α
 xchain0_n127_β:
 jmp xchain0_n132_α
# IR_ASSIGN_VAR
 xchain0_n128_α:
 mov rdi, qword ptr [rbp + 3568]
 mov rsi, qword ptr [rbp + 3576]
 mov rdx, qword ptr [rbp + 3600]
 mov rcx, qword ptr [rbp + 3608]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n117_α
 mov qword ptr [rbp + 3632], rax
 mov qword ptr [rbp + 3640], rdx
 jmp xchain0_n117_α
 xchain0_n128_β:
 jmp xchain0_n117_α
# IR_LIT_STRING
 xchain0_n129_α:
 mov qword ptr [rbp + 3744], 1
 mov rax, qword ptr [rip + .Lx132_0]
 mov qword ptr [rbp + 3752], rax
 jmp xchain0_n133_α
 xchain0_n129_β:
 jmp xchain0_n122_α
.Lx132_0:
 .quad .Lx132_0_s
.Lx132_0_s:
 .string "V"
# IR_SUBSCRIPT x[i] variable
 xchain0_n130_α:
 mov rdi, qword ptr [rbp + 3792]
 mov rsi, qword ptr [rbp + 3800]
 mov rdx, qword ptr [rbp + 3824]
 mov rcx, qword ptr [rbp + 3832]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n127_α
 mov qword ptr [rbp + 3856], rax
 mov qword ptr [rbp + 3864], rdx
 jmp xchain0_n134_α
 xchain0_n130_β:
 jmp xchain0_n127_α
# IR_LIT_INTEGER
 xchain0_n131_α:
 mov qword ptr [rbp + 3968], 6
 mov rax, qword ptr [rip + .Lx134_0]
 mov qword ptr [rbp + 3976], rax
 jmp xchain0_n135_α
 xchain0_n131_β:
 jmp xchain0_n132_α
.Lx134_0:
 .quad 13
# IR_LIT_INTEGER
 xchain0_n132_α:
 mov qword ptr [rbp + 4096], 6
 mov rax, qword ptr [rip + .Lx135_0]
 mov qword ptr [rbp + 4104], rax
 jmp xchain0_n136_α
 xchain0_n132_β:
 jmp xchain0_n139_α
.Lx135_0:
 .quad 1
# IR_ASSIGN_VAR
 xchain0_n133_α:
 mov rdi, qword ptr [rbp + 3712]
 mov rsi, qword ptr [rbp + 3720]
 mov rdx, qword ptr [rbp + 3744]
 mov rcx, qword ptr [rbp + 3752]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n122_α
 mov qword ptr [rbp + 3776], rax
 mov qword ptr [rbp + 3784], rdx
 jmp xchain0_n122_α
 xchain0_n133_β:
 jmp xchain0_n122_α
# IR_LIT_STRING
 xchain0_n134_α:
 mov qword ptr [rbp + 3888], 1
 mov rax, qword ptr [rip + .Lx137_0]
 mov qword ptr [rbp + 3896], rax
 jmp xchain0_n137_α
 xchain0_n134_β:
 jmp xchain0_n127_α
.Lx137_0:
 .quad .Lx137_0_s
.Lx137_0_s:
 .string "IV"
# IR_SUBSCRIPT x[i] variable
 xchain0_n135_α:
 mov rdi, qword ptr [rbp + 3936]
 mov rsi, qword ptr [rbp + 3944]
 mov rdx, qword ptr [rbp + 3968]
 mov rcx, qword ptr [rbp + 3976]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n132_α
 mov qword ptr [rbp + 4000], rax
 mov qword ptr [rbp + 4008], rdx
 jmp xchain0_n138_α
 xchain0_n135_β:
 jmp xchain0_n132_α
# IR_ASSIGN gva
 xchain0_n136_α:
 mov rax, qword ptr [rbp + 4096]
 mov rdx, qword ptr [rbp + 4104]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 4080], rax
 mov qword ptr [rbp + 4088], rdx
 jmp xchain0_n139_α
 xchain0_n136_β:
 jmp xchain0_n139_α
# IR_ASSIGN_VAR
 xchain0_n137_α:
 mov rdi, qword ptr [rbp + 3856]
 mov rsi, qword ptr [rbp + 3864]
 mov rdx, qword ptr [rbp + 3888]
 mov rcx, qword ptr [rbp + 3896]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n127_α
 mov qword ptr [rbp + 3920], rax
 mov qword ptr [rbp + 3928], rdx
 jmp xchain0_n127_α
 xchain0_n137_β:
 jmp xchain0_n127_α
# IR_LIT_STRING
 xchain0_n138_α:
 mov qword ptr [rbp + 4032], 1
 mov rax, qword ptr [rip + .Lx141_0]
 mov qword ptr [rbp + 4040], rax
 jmp xchain0_n140_α
 xchain0_n138_β:
 jmp xchain0_n132_α
.Lx141_0:
 .quad .Lx141_0_s
.Lx141_0_s:
 .string "I"
# IR_VAR
 xchain0_n139_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 4256], rax
 mov qword ptr [rbp + 4264], rdx
 jmp xchain0_n141_α
 xchain0_n139_β:
 jmp xchain0_n142_α
# IR_ASSIGN_VAR
 xchain0_n140_α:
 mov rdi, qword ptr [rbp + 4000]
 mov rsi, qword ptr [rbp + 4008]
 mov rdx, qword ptr [rbp + 4032]
 mov rcx, qword ptr [rbp + 4040]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n132_α
 mov qword ptr [rbp + 4064], rax
 mov qword ptr [rbp + 4072], rdx
 jmp xchain0_n132_α
 xchain0_n140_β:
 jmp xchain0_n132_α
# IR_LIT_INTEGER
 xchain0_n141_α:
 mov qword ptr [rbp + 4224], 6
 mov rax, qword ptr [rip + .Lx144_0]
 mov qword ptr [rbp + 4232], rax
 jmp xchain0_n143_α
 xchain0_n141_β:
 jmp xchain0_n142_α
.Lx144_0:
 .quad 0
# IR_VAR
 xchain0_n142_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 5040], rax
 mov qword ptr [rbp + 5048], rdx
 jmp xchain0_n144_α
 xchain0_n142_β:
 jmp proc_roman_γ
# IR_COERCE_NUMERIC
 xchain0_n143_α:
 mov eax, dword ptr [rbp + 4256]
 cmp eax, 7
 je .Lx147_1
 cmp eax, 6
 jne .Lx147_0
 mov eax, dword ptr [rbp + 4224]
 cmp eax, 6
 jne .Lx147_0
.Lx147_1:
 mov rax, qword ptr [rbp + 4256]
 mov qword ptr [rbp + 4192], rax
 mov rax, qword ptr [rbp + 4264]
 mov qword ptr [rbp + 4200], rax
 jmp .Lx147_2
.Lx147_0:
 lea rdi, [rbp + 4256]
 lea rsi, [rbp + 4224]
 lea rdx, [rbp + 4192]
 mov rcx, 111
 call rt_coerce_num2_d@PLT
.Lx147_2:
 jmp xchain0_n145_α
 xchain0_n143_β:
 jmp xchain0_n142_α
# IR_ASSIGN gva
 xchain0_n144_α:
 mov rax, qword ptr [rbp + 5040]
 mov rdx, qword ptr [rbp + 5048]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 5024], rax
 mov qword ptr [rbp + 5032], rdx
 jmp proc_roman_γ
 xchain0_n144_β:
 jmp proc_roman_γ
# IR_COERCE_NUMERIC
 xchain0_n145_α:
 mov eax, dword ptr [rbp + 4224]
 cmp eax, 7
 je .Lx150_1
 cmp eax, 6
 jne .Lx150_0
 mov eax, dword ptr [rbp + 4256]
 cmp eax, 6
 jne .Lx150_0
.Lx150_1:
 mov rax, qword ptr [rbp + 4224]
 mov qword ptr [rbp + 4160], rax
 mov rax, qword ptr [rbp + 4232]
 mov qword ptr [rbp + 4168], rax
 jmp .Lx150_2
.Lx150_0:
 lea rdi, [rbp + 4224]
 lea rsi, [rbp + 4256]
 lea rdx, [rbp + 4160]
 mov rcx, 112
 call rt_coerce_num2_d@PLT
.Lx150_2:
 jmp xchain0_n146_α
 xchain0_n145_β:
 jmp xchain0_n142_α
# IR_CMP_TEST
 xchain0_n146_α:
 lea rdi, [rbp + 4192]
 lea rsi, [rbp + 4160]
 call rt_cmp_d@PLT
 test eax, eax
 jle xchain0_n142_α
 mov qword ptr [rbp + 4128], 0
 mov qword ptr [rbp + 4136], 0
 jmp xchain0_n147_α
 xchain0_n146_β:
 jmp xchain0_n142_α
# IR_VAR
 xchain0_n147_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 4496], rax
 mov qword ptr [rbp + 4504], rdx
 jmp xchain0_n148_α
 xchain0_n147_β:
 jmp xchain0_n149_α
# IR_VAR
 xchain0_n148_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 4384], rax
 mov qword ptr [rbp + 4392], rdx
 jmp xchain0_n150_α
 xchain0_n148_β:
 jmp xchain0_n149_α
# IR_VAR
 xchain0_n149_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 4960], rax
 mov qword ptr [rbp + 4968], rdx
 jmp xchain0_n151_α
 xchain0_n149_β:
 jmp xchain0_n139_α
# IR_VAR
 xchain0_n150_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 4416], rax
 mov qword ptr [rbp + 4424], rdx
 jmp xchain0_n152_α
 xchain0_n150_β:
 jmp xchain0_n149_α
# IR_LIT_INTEGER
 xchain0_n151_α:
 mov qword ptr [rbp + 4992], 6
 mov rax, qword ptr [rip + .Lx157_0]
 mov qword ptr [rbp + 5000], rax
 jmp xchain0_n153_α
 xchain0_n151_β:
 jmp xchain0_n139_α
.Lx157_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n152_α:
 mov rdi, qword ptr [rbp + 4384]
 mov rsi, qword ptr [rbp + 4392]
 mov rdx, qword ptr [rbp + 4416]
 mov rcx, qword ptr [rbp + 4424]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n149_α
 mov qword ptr [rbp + 4448], rax
 mov qword ptr [rbp + 4456], rdx
 jmp xchain0_n154_α
 xchain0_n152_β:
 jmp xchain0_n149_α
 xchain0_n153_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 4960]
 cmp eax, 100
 je .Lx159_0
 mov eax, dword ptr [rbp + 4960]
 cmp eax, 6
 jne .Lx159_2
.Lx159_1:
 mov rax, qword ptr [rbp + 4968]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rbp + 4928], 6
 mov qword ptr [rbp + 4936], rax
 jmp xchain0_n155_α
.Lx159_0:
 mov rdi, qword ptr [rbp + 4960]
 mov rsi, qword ptr [rbp + 4968]
 mov rdx, qword ptr [rbp + 4992]
 mov rcx, qword ptr [rbp + 5000]
 mov r8d, 0
 lea r9, [rbp + 4928]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx159_3
.Lx159_2:
 mov rdi, qword ptr [rbp + 4960]
 mov rsi, qword ptr [rbp + 4968]
 mov rdx, qword ptr [rbp + 4992]
 mov rcx, qword ptr [rbp + 5000]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n139_α
 mov qword ptr [rbp + 4928], rax
 mov qword ptr [rbp + 4936], rdx
.Lx159_3:
 jmp xchain0_n155_α
 xchain0_n153_β:
 jmp xchain0_n139_α
# IR_DEREF variable -> value
 xchain0_n154_α:
 mov rdi, qword ptr [rbp + 4448]
 mov rsi, qword ptr [rbp + 4456]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n149_α
 mov qword ptr [rbp + 4480], rax
 mov qword ptr [rbp + 4488], rdx
 jmp xchain0_n156_α
 xchain0_n154_β:
 jmp xchain0_n149_α
# IR_ASSIGN gva
 xchain0_n155_α:
 mov rax, qword ptr [rbp + 4928]
 mov rdx, qword ptr [rbp + 4936]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 4912], rax
 mov qword ptr [rbp + 4920], rdx
 jmp xchain0_n139_α
 xchain0_n155_β:
 jmp xchain0_n139_α
# IR_COERCE_NUMERIC
 xchain0_n156_α:
 mov eax, dword ptr [rbp + 4496]
 cmp eax, 7
 je .Lx163_1
 cmp eax, 6
 jne .Lx163_0
 mov eax, dword ptr [rbp + 4480]
 cmp eax, 6
 jne .Lx163_0
.Lx163_1:
 mov rax, qword ptr [rbp + 4496]
 mov qword ptr [rbp + 4352], rax
 mov rax, qword ptr [rbp + 4504]
 mov qword ptr [rbp + 4360], rax
 jmp .Lx163_2
.Lx163_0:
 lea rdi, [rbp + 4496]
 lea rsi, [rbp + 4480]
 lea rdx, [rbp + 4352]
 mov rcx, 109
 call rt_coerce_num2_d@PLT
.Lx163_2:
 jmp xchain0_n157_α
 xchain0_n156_β:
 jmp xchain0_n149_α
# IR_COERCE_NUMERIC
 xchain0_n157_α:
 mov eax, dword ptr [rbp + 4480]
 cmp eax, 7
 je .Lx165_1
 cmp eax, 6
 jne .Lx165_0
 mov eax, dword ptr [rbp + 4496]
 cmp eax, 6
 jne .Lx165_0
.Lx165_1:
 mov rax, qword ptr [rbp + 4480]
 mov qword ptr [rbp + 4320], rax
 mov rax, qword ptr [rbp + 4488]
 mov qword ptr [rbp + 4328], rax
 jmp .Lx165_2
.Lx165_0:
 lea rdi, [rbp + 4480]
 lea rsi, [rbp + 4496]
 lea rdx, [rbp + 4320]
 mov rcx, 110
 call rt_coerce_num2_d@PLT
.Lx165_2:
 jmp xchain0_n158_α
 xchain0_n157_β:
 jmp xchain0_n149_α
# IR_CMP_TEST
 xchain0_n158_α:
 lea rdi, [rbp + 4352]
 lea rsi, [rbp + 4320]
 call rt_cmp_d@PLT
 test eax, eax
 js xchain0_n149_α
 mov qword ptr [rbp + 4288], 0
 mov qword ptr [rbp + 4296], 0
 jmp xchain0_n159_α
 xchain0_n158_β:
 jmp xchain0_n149_α
# IR_VAR
 xchain0_n159_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 4576], rax
 mov qword ptr [rbp + 4584], rdx
 jmp xchain0_n160_α
 xchain0_n159_β:
 jmp xchain0_n161_α
# IR_VAR
 xchain0_n160_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 4608], rax
 mov qword ptr [rbp + 4616], rdx
 jmp xchain0_n162_α
 xchain0_n160_β:
 jmp xchain0_n161_α
# IR_VAR
 xchain0_n161_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 4768], rax
 mov qword ptr [rbp + 4776], rdx
 jmp xchain0_n163_α
 xchain0_n161_β:
 jmp xchain0_n139_α
# IR_VAR
 xchain0_n162_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 4640], rax
 mov qword ptr [rbp + 4648], rdx
 jmp xchain0_n164_α
 xchain0_n162_β:
 jmp xchain0_n161_α
# IR_VAR
 xchain0_n163_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 4800], rax
 mov qword ptr [rbp + 4808], rdx
 jmp xchain0_n165_α
 xchain0_n163_β:
 jmp xchain0_n139_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n164_α:
 mov rdi, qword ptr [rbp + 4608]
 mov rsi, qword ptr [rbp + 4616]
 mov rdx, qword ptr [rbp + 4640]
 mov rcx, qword ptr [rbp + 4648]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n161_α
 mov qword ptr [rbp + 4672], rax
 mov qword ptr [rbp + 4680], rdx
 jmp xchain0_n166_α
 xchain0_n164_β:
 jmp xchain0_n161_α
# IR_VAR
 xchain0_n165_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 4832], rax
 mov qword ptr [rbp + 4840], rdx
 jmp xchain0_n167_α
 xchain0_n165_β:
 jmp xchain0_n139_α
# IR_DEREF variable -> value
 xchain0_n166_α:
 mov rdi, qword ptr [rbp + 4672]
 mov rsi, qword ptr [rbp + 4680]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n161_α
 mov qword ptr [rbp + 4704], rax
 mov qword ptr [rbp + 4712], rdx
 jmp xchain0_n168_α
 xchain0_n166_β:
 jmp xchain0_n161_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n167_α:
 mov rdi, qword ptr [rbp + 4800]
 mov rsi, qword ptr [rbp + 4808]
 mov rdx, qword ptr [rbp + 4832]
 mov rcx, qword ptr [rbp + 4840]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n139_α
 mov qword ptr [rbp + 4864], rax
 mov qword ptr [rbp + 4872], rdx
 jmp xchain0_n169_α
 xchain0_n167_β:
 jmp xchain0_n139_α
 xchain0_n168_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 4576]
 mov rsi, qword ptr [rbp + 4584]
 mov rdx, qword ptr [rbp + 4704]
 mov rcx, qword ptr [rbp + 4712]
 call str_concat_d@PLT
 mov qword ptr [rbp + 4544], rax
 mov qword ptr [rbp + 4552], rdx
 jmp xchain0_n170_α
 xchain0_n168_β:
 jmp xchain0_n161_α
# IR_DEREF variable -> value
 xchain0_n169_α:
 mov rdi, qword ptr [rbp + 4864]
 mov rsi, qword ptr [rbp + 4872]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n139_α
 mov qword ptr [rbp + 4896], rax
 mov qword ptr [rbp + 4904], rdx
 jmp xchain0_n171_α
 xchain0_n169_β:
 jmp xchain0_n139_α
# IR_ASSIGN gva
 xchain0_n170_α:
 mov rax, qword ptr [rbp + 4544]
 mov rdx, qword ptr [rbp + 4552]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 4528], rax
 mov qword ptr [rbp + 4536], rdx
 jmp xchain0_n161_α
 xchain0_n170_β:
 jmp xchain0_n161_α
 xchain0_n171_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 4768]
 cmp eax, 100
 je .Lx180_0
 mov eax, dword ptr [rbp + 4896]
 cmp eax, 100
 je .Lx180_0
 mov eax, dword ptr [rbp + 4768]
 cmp eax, 6
 jne .Lx180_2
 mov eax, dword ptr [rbp + 4896]
 cmp eax, 6
 jne .Lx180_2
.Lx180_1:
 mov rax, qword ptr [rbp + 4776]
 mov rcx, qword ptr [rbp + 4904]
 sub rax, rcx
 mov qword ptr [rbp + 4736], 6
 mov qword ptr [rbp + 4744], rax
 jmp xchain0_n172_α
.Lx180_0:
 mov rdi, qword ptr [rbp + 4768]
 mov rsi, qword ptr [rbp + 4776]
 mov rdx, qword ptr [rbp + 4896]
 mov rcx, qword ptr [rbp + 4904]
 mov r8d, 1
 lea r9, [rbp + 4736]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx180_3
.Lx180_2:
 mov rdi, qword ptr [rbp + 4768]
 mov rsi, qword ptr [rbp + 4776]
 mov rdx, qword ptr [rbp + 4896]
 mov rcx, qword ptr [rbp + 4904]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n139_α
 mov qword ptr [rbp + 4736], rax
 mov qword ptr [rbp + 4744], rdx
.Lx180_3:
 jmp xchain0_n172_α
 xchain0_n171_β:
 jmp xchain0_n139_α
# IR_ASSIGN gva
 xchain0_n172_α:
 mov rax, qword ptr [rbp + 4736]
 mov rdx, qword ptr [rbp + 4744]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 4720], rax
 mov qword ptr [rbp + 4728], rdx
 jmp xchain0_n139_α
 xchain0_n172_β:
 jmp xchain0_n139_α
proc_roman_res:
add rsp, 8
pop rbp
proc_roman_β:
jmp proc_roman_ω
proc_roman_γ:
mov rdi, [rsp]
mov rsi, [rsp + 8]
mov rax, [rsp + 5672]
mov rbp, [rbp + 5688]
lea rsp, [rsp + 5696]
jmp rax
proc_roman_ω:
mov rax, [rsp + 5680]
mov rbp, [rbp + 5688]
lea rsp, [rsp + 5696]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "roman"
  .Lstartup_pp0_0: .string "n"
  .Lstartup_pp0_1: .string "s"
  .Lstartup_pp0_2: .string "v"
  .Lstartup_pp0_3: .string "r"
  .Lstartup_pp0_4: .string "i"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad .Lstartup_pp0_1
  .quad .Lstartup_pp0_2
  .quad .Lstartup_pp0_3
  .quad .Lstartup_pp0_4
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 5
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_roman_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 5
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 5664
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "roman"
  .Lgvan1: .string "n"
  .Lgvan2: .string "s"
  .Lgvan3: .string "v"
  .Lgvan4: .string "r"
  .Lgvan5: .string "i"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 6
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 6
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
  mov qword ptr [rsp + 5656], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain182_n0_α:
 mov qword ptr [rbp + 5136], 6
 mov rax, qword ptr [rip + .Lx183_0]
 mov qword ptr [rbp + 5144], rax
 jmp xchain182_n1_α
 xchain182_n0_β:
 jmp xchain182_n3_α
.Lx183_0:
 .quad 1
 xchain182_n1_α:
 mov edi, 0
 mov rsi, qword ptr [rbp + 5136]
 mov rdx, qword ptr [rbp + 5144]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx185_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx185_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx185_3]
 lea rdx, [rip + .Lx185_4]
 jmp rax
.Lx185_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx185_2
.Lx185_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx185_2
.Lx185_1:
 call rt_faildescr@PLT
.Lx185_2:
 mov qword ptr [rbp + 5088], rax
 mov qword ptr [rbp + 5096], rdx
 cmp eax, 99
 je xchain182_n3_α
 jmp xchain182_n2_α
 xchain182_n1_β:
 jmp xchain182_n3_α
.Lx185_0:
 .quad .Lx185_0_s
.Lx185_0_s:
 .string "roman"
# IR_ASSIGN global
 xchain182_n2_α:
 mov rsi, qword ptr [rbp + 5088]
 mov rdx, qword ptr [rbp + 5096]
 mov rdi, qword ptr [rip + .Lx186_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 5072], rax
 mov qword ptr [rbp + 5080], rdx
 jmp xchain182_n3_α
 xchain182_n2_β:
 jmp xchain182_n3_α
.Lx186_0:
 .quad .Lx186_0_s
.Lx186_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain182_n3_α:
 mov qword ptr [rbp + 5232], 6
 mov rax, qword ptr [rip + .Lx187_0]
 mov qword ptr [rbp + 5240], rax
 jmp xchain182_n4_α
 xchain182_n3_β:
 jmp xchain182_n6_α
.Lx187_0:
 .quad 4
 xchain182_n4_α:
 mov edi, 0
 mov rsi, qword ptr [rbp + 5232]
 mov rdx, qword ptr [rbp + 5240]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx189_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx189_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx189_3]
 lea rdx, [rip + .Lx189_4]
 jmp rax
.Lx189_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx189_2
.Lx189_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx189_2
.Lx189_1:
 call rt_faildescr@PLT
.Lx189_2:
 mov qword ptr [rbp + 5184], rax
 mov qword ptr [rbp + 5192], rdx
 cmp eax, 99
 je xchain182_n6_α
 jmp xchain182_n5_α
 xchain182_n4_β:
 jmp xchain182_n6_α
.Lx189_0:
 .quad .Lx189_0_s
.Lx189_0_s:
 .string "roman"
# IR_ASSIGN global
 xchain182_n5_α:
 mov rsi, qword ptr [rbp + 5184]
 mov rdx, qword ptr [rbp + 5192]
 mov rdi, qword ptr [rip + .Lx190_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 5168], rax
 mov qword ptr [rbp + 5176], rdx
 jmp xchain182_n6_α
 xchain182_n5_β:
 jmp xchain182_n6_α
.Lx190_0:
 .quad .Lx190_0_s
.Lx190_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain182_n6_α:
 mov qword ptr [rbp + 5328], 6
 mov rax, qword ptr [rip + .Lx191_0]
 mov qword ptr [rbp + 5336], rax
 jmp xchain182_n7_α
 xchain182_n6_β:
 jmp xchain182_n9_α
.Lx191_0:
 .quad 9
 xchain182_n7_α:
 mov edi, 0
 mov rsi, qword ptr [rbp + 5328]
 mov rdx, qword ptr [rbp + 5336]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx193_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx193_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx193_3]
 lea rdx, [rip + .Lx193_4]
 jmp rax
.Lx193_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx193_2
.Lx193_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx193_2
.Lx193_1:
 call rt_faildescr@PLT
.Lx193_2:
 mov qword ptr [rbp + 5280], rax
 mov qword ptr [rbp + 5288], rdx
 cmp eax, 99
 je xchain182_n9_α
 jmp xchain182_n8_α
 xchain182_n7_β:
 jmp xchain182_n9_α
.Lx193_0:
 .quad .Lx193_0_s
.Lx193_0_s:
 .string "roman"
# IR_ASSIGN global
 xchain182_n8_α:
 mov rsi, qword ptr [rbp + 5280]
 mov rdx, qword ptr [rbp + 5288]
 mov rdi, qword ptr [rip + .Lx194_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 5264], rax
 mov qword ptr [rbp + 5272], rdx
 jmp xchain182_n9_α
 xchain182_n8_β:
 jmp xchain182_n9_α
.Lx194_0:
 .quad .Lx194_0_s
.Lx194_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain182_n9_α:
 mov qword ptr [rbp + 5424], 6
 mov rax, qword ptr [rip + .Lx195_0]
 mov qword ptr [rbp + 5432], rax
 jmp xchain182_n10_α
 xchain182_n9_β:
 jmp xchain182_n12_α
.Lx195_0:
 .quad 42
 xchain182_n10_α:
 mov edi, 0
 mov rsi, qword ptr [rbp + 5424]
 mov rdx, qword ptr [rbp + 5432]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx197_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx197_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx197_3]
 lea rdx, [rip + .Lx197_4]
 jmp rax
.Lx197_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx197_2
.Lx197_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx197_2
.Lx197_1:
 call rt_faildescr@PLT
.Lx197_2:
 mov qword ptr [rbp + 5376], rax
 mov qword ptr [rbp + 5384], rdx
 cmp eax, 99
 je xchain182_n12_α
 jmp xchain182_n11_α
 xchain182_n10_β:
 jmp xchain182_n12_α
.Lx197_0:
 .quad .Lx197_0_s
.Lx197_0_s:
 .string "roman"
# IR_ASSIGN global
 xchain182_n11_α:
 mov rsi, qword ptr [rbp + 5376]
 mov rdx, qword ptr [rbp + 5384]
 mov rdi, qword ptr [rip + .Lx198_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 5360], rax
 mov qword ptr [rbp + 5368], rdx
 jmp xchain182_n12_α
 xchain182_n11_β:
 jmp xchain182_n12_α
.Lx198_0:
 .quad .Lx198_0_s
.Lx198_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain182_n12_α:
 mov qword ptr [rbp + 5520], 6
 mov rax, qword ptr [rip + .Lx199_0]
 mov qword ptr [rbp + 5528], rax
 jmp xchain182_n13_α
 xchain182_n12_β:
 jmp xchain182_n15_α
.Lx199_0:
 .quad 1999
 xchain182_n13_α:
 mov edi, 0
 mov rsi, qword ptr [rbp + 5520]
 mov rdx, qword ptr [rbp + 5528]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx201_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx201_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx201_3]
 lea rdx, [rip + .Lx201_4]
 jmp rax
.Lx201_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx201_2
.Lx201_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx201_2
.Lx201_1:
 call rt_faildescr@PLT
.Lx201_2:
 mov qword ptr [rbp + 5472], rax
 mov qword ptr [rbp + 5480], rdx
 cmp eax, 99
 je xchain182_n15_α
 jmp xchain182_n14_α
 xchain182_n13_β:
 jmp xchain182_n15_α
.Lx201_0:
 .quad .Lx201_0_s
.Lx201_0_s:
 .string "roman"
# IR_ASSIGN global
 xchain182_n14_α:
 mov rsi, qword ptr [rbp + 5472]
 mov rdx, qword ptr [rbp + 5480]
 mov rdi, qword ptr [rip + .Lx202_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 5456], rax
 mov qword ptr [rbp + 5464], rdx
 jmp xchain182_n15_α
 xchain182_n14_β:
 jmp xchain182_n15_α
.Lx202_0:
 .quad .Lx202_0_s
.Lx202_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain182_n15_α:
 mov qword ptr [rbp + 5616], 6
 mov rax, qword ptr [rip + .Lx203_0]
 mov qword ptr [rbp + 5624], rax
 jmp xchain182_n16_α
 xchain182_n15_β:
 jmp main_γ
.Lx203_0:
 .quad 2024
 xchain182_n16_α:
 mov edi, 0
 mov rsi, qword ptr [rbp + 5616]
 mov rdx, qword ptr [rbp + 5624]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx205_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx205_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx205_3]
 lea rdx, [rip + .Lx205_4]
 jmp rax
.Lx205_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx205_2
.Lx205_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx205_2
.Lx205_1:
 call rt_faildescr@PLT
.Lx205_2:
 mov qword ptr [rbp + 5568], rax
 mov qword ptr [rbp + 5576], rdx
 cmp eax, 99
 je main_γ
 jmp xchain182_n17_α
 xchain182_n16_β:
 jmp main_γ
.Lx205_0:
 .quad .Lx205_0_s
.Lx205_0_s:
 .string "roman"
# IR_ASSIGN global
 xchain182_n17_α:
 mov rsi, qword ptr [rbp + 5568]
 mov rdx, qword ptr [rbp + 5576]
 mov rdi, qword ptr [rip + .Lx206_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 5552], rax
 mov qword ptr [rbp + 5560], rdx
 jmp main_γ
 xchain182_n17_β:
 jmp main_γ
.Lx206_0:
 .quad .Lx206_0_s
.Lx206_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 5656]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 5656]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_roman_α
proc_roman_α:
                        .global          proc_roman_α
                        .global          proc_roman_β
                        .global          proc_roman_γ
                        .global          proc_roman_ω
                        sub              rsp, 5104
                        mov              [rsp + 5080], rcx
                        mov              [rsp + 5088], rdx
                        mov              [rsp + 5096], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_roman_α_body:
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rbp + 112], 1
                        mov              rax, qword ptr [rip + .Lx173_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n1_assign_α
.Lx173_0:
                        .quad            .Lx173_0_s
.Lx173_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n1_assign_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   n2_lit_integer_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rbp + 208], 6
                        mov              rax, qword ptr [rip + .Lx175_0]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n3_call_α
.Lx175_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n3_call_α:
                        mov              rax, qword ptr [rbp + 208]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 216]
                        mov              qword ptr [rbp + 184], rax
                        .section         .rodata
.Lrkfn177:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn177]
                        lea              rsi, [rbp + 176]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                        cmp              eax, 99
                                                                                        je    n5_var_α
                                                                                        jmp   n4_assign_α
n3_call_β:
                                                                                        jmp   n5_var_α
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rbp + 160]
                        mov              rdx, qword ptr [rbp + 168]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   n5_var_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n6_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx180_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n8_subscript_α
.Lx180_0:
                        .quad            1
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n8_subscript_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n7_var_α
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rbp + 416], 6
                        mov              rax, qword ptr [rip + .Lx183_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n12_subscript_α
.Lx183_0:
                        .quad            2
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 528], rax
                        mov              qword ptr [rbp + 536], rdx
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n15_assign_var_α
.Lx185_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n12_subscript_α:
                        mov              rdi, qword ptr [rbp + 384]
                        mov              rsi, qword ptr [rbp + 392]
                        mov              rdx, qword ptr [rbp + 416]
                        mov              rcx, qword ptr [rbp + 424]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n10_var_α
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n16_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rbp + 560], 6
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n17_subscript_α
.Lx187_0:
                        .quad            3
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n18_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_var_α:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n7_var_α
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        mov              qword ptr [rbp + 480], 6
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   n20_assign_var_α
.Lx190_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n17_subscript_α:
                        mov              rdi, qword ptr [rbp + 528]
                        mov              rsi, qword ptr [rbp + 536]
                        mov              rdx, qword ptr [rbp + 560]
                        mov              rcx, qword ptr [rbp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n14_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n21_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_integer_α:
                        mov              qword ptr [rbp + 704], 6
                        mov              rax, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rbp + 712], rax
                                                                                        jmp   n22_subscript_α
.Lx192_0:
                        .quad            4
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n19_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 816], rax
                        mov              qword ptr [rbp + 824], rdx
                                                                                        jmp   n23_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_var_α:
                        mov              rdi, qword ptr [rbp + 448]
                        mov              rsi, qword ptr [rbp + 456]
                        mov              rdx, qword ptr [rbp + 480]
                        mov              rcx, qword ptr [rbp + 488]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n10_var_α
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n10_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        mov              qword ptr [rbp + 624], 6
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n25_assign_var_α
.Lx195_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n22_subscript_α:
                        mov              rdi, qword ptr [rbp + 672]
                        mov              rsi, qword ptr [rbp + 680]
                        mov              rdx, qword ptr [rbp + 704]
                        mov              rcx, qword ptr [rbp + 712]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n19_var_α
                        mov              qword ptr [rbp + 736], rax
                        mov              qword ptr [rbp + 744], rdx
                                                                                        jmp   n26_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        mov              qword ptr [rbp + 848], 6
                        mov              rax, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rbp + 856], rax
                                                                                        jmp   n27_subscript_α
.Lx197_0:
                        .quad            5
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                                                                                        jmp   n28_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_var_α:
                        mov              rdi, qword ptr [rbp + 592]
                        mov              rsi, qword ptr [rbp + 600]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n14_var_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n14_var_α
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:
                        mov              qword ptr [rbp + 768], 6
                        mov              rax, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rbp + 776], rax
                                                                                        jmp   n30_assign_var_α
.Lx200_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n27_subscript_α:
                        mov              rdi, qword ptr [rbp + 816]
                        mov              rsi, qword ptr [rbp + 824]
                        mov              rdx, qword ptr [rbp + 848]
                        mov              rcx, qword ptr [rbp + 856]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n24_var_α
                        mov              qword ptr [rbp + 880], rax
                        mov              qword ptr [rbp + 888], rdx
                                                                                        jmp   n31_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_integer_α:
                        mov              qword ptr [rbp + 992], 6
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rbp + 1000], rax
                                                                                        jmp   n32_subscript_α
.Lx202_0:
                        .quad            6
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                                                                                        jmp   n33_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_var_α:
                        mov              rdi, qword ptr [rbp + 736]
                        mov              rsi, qword ptr [rbp + 744]
                        mov              rdx, qword ptr [rbp + 768]
                        mov              rcx, qword ptr [rbp + 776]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n19_var_α
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                                                                                        jmp   n19_var_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:
                        mov              qword ptr [rbp + 912], 6
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rbp + 920], rax
                                                                                        jmp   n35_assign_var_α
.Lx205_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n32_subscript_α:
                        mov              rdi, qword ptr [rbp + 960]
                        mov              rsi, qword ptr [rbp + 968]
                        mov              rdx, qword ptr [rbp + 992]
                        mov              rcx, qword ptr [rbp + 1000]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n29_var_α
                        mov              qword ptr [rbp + 1024], rax
                        mov              qword ptr [rbp + 1032], rdx
                                                                                        jmp   n36_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:
                        mov              qword ptr [rbp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx207_0]
                        mov              qword ptr [rbp + 1144], rax
                                                                                        jmp   n37_subscript_α
.Lx207_0:
                        .quad            7
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n34_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1248], rax
                        mov              qword ptr [rbp + 1256], rdx
                                                                                        jmp   n38_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_var_α:
                        mov              rdi, qword ptr [rbp + 880]
                        mov              rsi, qword ptr [rbp + 888]
                        mov              rdx, qword ptr [rbp + 912]
                        mov              rcx, qword ptr [rbp + 920]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n24_var_α
                        mov              qword ptr [rbp + 944], rax
                        mov              qword ptr [rbp + 952], rdx
                                                                                        jmp   n24_var_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:
                        mov              qword ptr [rbp + 1056], 6
                        mov              rax, qword ptr [rip + .Lx210_0]
                        mov              qword ptr [rbp + 1064], rax
                                                                                        jmp   n40_assign_var_α
.Lx210_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n37_subscript_α:
                        mov              rdi, qword ptr [rbp + 1104]
                        mov              rsi, qword ptr [rbp + 1112]
                        mov              rdx, qword ptr [rbp + 1136]
                        mov              rcx, qword ptr [rbp + 1144]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n34_var_α
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                                                                                        jmp   n41_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_integer_α:
                        mov              qword ptr [rbp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx212_0]
                        mov              qword ptr [rbp + 1288], rax
                                                                                        jmp   n42_subscript_α
.Lx212_0:
                        .quad            8
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n39_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n43_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1024]
                        mov              rsi, qword ptr [rbp + 1032]
                        mov              rdx, qword ptr [rbp + 1056]
                        mov              rcx, qword ptr [rbp + 1064]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n29_var_α
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                                                                                        jmp   n29_var_α
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_integer_α:
                        mov              qword ptr [rbp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx215_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n45_assign_var_α
.Lx215_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n42_subscript_α:
                        mov              rdi, qword ptr [rbp + 1248]
                        mov              rsi, qword ptr [rbp + 1256]
                        mov              rdx, qword ptr [rbp + 1280]
                        mov              rcx, qword ptr [rbp + 1288]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n39_var_α
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                                                                                        jmp   n46_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        mov              qword ptr [rbp + 1424], 6
                        mov              rax, qword ptr [rip + .Lx217_0]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n47_subscript_α
.Lx217_0:
                        .quad            9
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1536], rax
                        mov              qword ptr [rbp + 1544], rdx
                                                                                        jmp   n48_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1168]
                        mov              rsi, qword ptr [rbp + 1176]
                        mov              rdx, qword ptr [rbp + 1200]
                        mov              rcx, qword ptr [rbp + 1208]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n34_var_α
                        mov              qword ptr [rbp + 1232], rax
                        mov              qword ptr [rbp + 1240], rdx
                                                                                        jmp   n34_var_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_integer_α:
                        mov              qword ptr [rbp + 1344], 6
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rbp + 1352], rax
                                                                                        jmp   n50_assign_var_α
.Lx220_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n47_subscript_α:
                        mov              rdi, qword ptr [rbp + 1392]
                        mov              rsi, qword ptr [rbp + 1400]
                        mov              rdx, qword ptr [rbp + 1424]
                        mov              rcx, qword ptr [rbp + 1432]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n44_var_α
                        mov              qword ptr [rbp + 1456], rax
                        mov              qword ptr [rbp + 1464], rdx
                                                                                        jmp   n51_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_integer_α:
                        mov              qword ptr [rbp + 1568], 6
                        mov              rax, qword ptr [rip + .Lx222_0]
                        mov              qword ptr [rbp + 1576], rax
                                                                                        jmp   n52_subscript_α
.Lx222_0:
                        .quad            10
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1680], rax
                        mov              qword ptr [rbp + 1688], rdx
                                                                                        jmp   n53_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n50_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1312]
                        mov              rsi, qword ptr [rbp + 1320]
                        mov              rdx, qword ptr [rbp + 1344]
                        mov              rcx, qword ptr [rbp + 1352]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n39_var_α
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:
                        mov              qword ptr [rbp + 1488], 6
                        mov              rax, qword ptr [rip + .Lx225_0]
                        mov              qword ptr [rbp + 1496], rax
                                                                                        jmp   n55_assign_var_α
.Lx225_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n52_subscript_α:
                        mov              rdi, qword ptr [rbp + 1536]
                        mov              rsi, qword ptr [rbp + 1544]
                        mov              rdx, qword ptr [rbp + 1568]
                        mov              rcx, qword ptr [rbp + 1576]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n49_var_α
                        mov              qword ptr [rbp + 1600], rax
                        mov              qword ptr [rbp + 1608], rdx
                                                                                        jmp   n56_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:
                        mov              qword ptr [rbp + 1712], 6
                        mov              rax, qword ptr [rip + .Lx227_0]
                        mov              qword ptr [rbp + 1720], rax
                                                                                        jmp   n57_subscript_α
.Lx227_0:
                        .quad            11
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n54_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1824], rax
                        mov              qword ptr [rbp + 1832], rdx
                                                                                        jmp   n58_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1456]
                        mov              rsi, qword ptr [rbp + 1464]
                        mov              rdx, qword ptr [rbp + 1488]
                        mov              rcx, qword ptr [rbp + 1496]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n44_var_α
                        mov              qword ptr [rbp + 1520], rax
                        mov              qword ptr [rbp + 1528], rdx
                                                                                        jmp   n44_var_α
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:
                        mov              qword ptr [rbp + 1632], 6
                        mov              rax, qword ptr [rip + .Lx230_0]
                        mov              qword ptr [rbp + 1640], rax
                                                                                        jmp   n60_assign_var_α
.Lx230_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n57_subscript_α:
                        mov              rdi, qword ptr [rbp + 1680]
                        mov              rsi, qword ptr [rbp + 1688]
                        mov              rdx, qword ptr [rbp + 1712]
                        mov              rcx, qword ptr [rbp + 1720]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n54_var_α
                        mov              qword ptr [rbp + 1744], rax
                        mov              qword ptr [rbp + 1752], rdx
                                                                                        jmp   n61_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_integer_α:
                        mov              qword ptr [rbp + 1856], 6
                        mov              rax, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rbp + 1864], rax
                                                                                        jmp   n62_subscript_α
.Lx232_0:
                        .quad            12
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n59_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1968], rax
                        mov              qword ptr [rbp + 1976], rdx
                                                                                        jmp   n63_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1600]
                        mov              rsi, qword ptr [rbp + 1608]
                        mov              rdx, qword ptr [rbp + 1632]
                        mov              rcx, qword ptr [rbp + 1640]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n49_var_α
                        mov              qword ptr [rbp + 1664], rax
                        mov              qword ptr [rbp + 1672], rdx
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:
                        mov              qword ptr [rbp + 1776], 6
                        mov              rax, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rbp + 1784], rax
                                                                                        jmp   n65_assign_var_α
.Lx235_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n62_subscript_α:
                        mov              rdi, qword ptr [rbp + 1824]
                        mov              rsi, qword ptr [rbp + 1832]
                        mov              rdx, qword ptr [rbp + 1856]
                        mov              rcx, qword ptr [rbp + 1864]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n59_var_α
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                                                                                        jmp   n66_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_integer_α:
                        mov              qword ptr [rbp + 2000], 6
                        mov              rax, qword ptr [rip + .Lx237_0]
                        mov              qword ptr [rbp + 2008], rax
                                                                                        jmp   n67_subscript_α
.Lx237_0:
                        .quad            13
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:
                        mov              qword ptr [rbp + 2176], 6
                        mov              rax, qword ptr [rip + .Lx238_0]
                        mov              qword ptr [rbp + 2184], rax
                                                                                        jmp   n68_call_α
.Lx238_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n65_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1744]
                        mov              rsi, qword ptr [rbp + 1752]
                        mov              rdx, qword ptr [rbp + 1776]
                        mov              rcx, qword ptr [rbp + 1784]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n54_var_α
                        mov              qword ptr [rbp + 1808], rax
                        mov              qword ptr [rbp + 1816], rdx
                                                                                        jmp   n54_var_α
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:
                        mov              qword ptr [rbp + 1920], 6
                        mov              rax, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rbp + 1928], rax
                                                                                        jmp   n69_assign_var_α
.Lx240_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n67_subscript_α:
                        mov              rdi, qword ptr [rbp + 1968]
                        mov              rsi, qword ptr [rbp + 1976]
                        mov              rdx, qword ptr [rbp + 2000]
                        mov              rcx, qword ptr [rbp + 2008]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n64_lit_integer_α
                        mov              qword ptr [rbp + 2032], rax
                        mov              qword ptr [rbp + 2040], rdx
                                                                                        jmp   n70_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:
                        mov              rax, qword ptr [rbp + 2176]
                        mov              qword ptr [rbp + 2144], rax
                        mov              rax, qword ptr [rbp + 2184]
                        mov              qword ptr [rbp + 2152], rax
                        .section         .rodata
.Lrkfn243:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn243]
                        lea              rsi, [rbp + 2144]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2128], rax
                        mov              qword ptr [rbp + 2136], rdx
                        cmp              eax, 99
                                                                                        je    n72_var_α
                                                                                        jmp   n71_assign_α
n68_call_β:
                                                                                        jmp   n72_var_α
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1888]
                        mov              rsi, qword ptr [rbp + 1896]
                        mov              rdx, qword ptr [rbp + 1920]
                        mov              rcx, qword ptr [rbp + 1928]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n59_var_α
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                                                                                        jmp   n59_var_α
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:
                        mov              qword ptr [rbp + 2064], 6
                        mov              rax, qword ptr [rip + .Lx245_0]
                        mov              qword ptr [rbp + 2072], rax
                                                                                        jmp   n73_assign_var_α
.Lx245_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n71_assign_α:
                        mov              rax, qword ptr [rbp + 2128]
                        mov              rdx, qword ptr [rbp + 2136]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                        mov              qword ptr [rbp + 2112], rax
                        mov              qword ptr [rbp + 2120], rdx
                                                                                        jmp   n72_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n72_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2208], rax
                        mov              qword ptr [rbp + 2216], rdx
                                                                                        jmp   n74_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n73_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2032]
                        mov              rsi, qword ptr [rbp + 2040]
                        mov              rdx, qword ptr [rbp + 2064]
                        mov              rcx, qword ptr [rbp + 2072]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n64_lit_integer_α
                        mov              qword ptr [rbp + 2096], rax
                        mov              qword ptr [rbp + 2104], rdx
                                                                                        jmp   n64_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:
                        mov              qword ptr [rbp + 2240], 6
                        mov              rax, qword ptr [rip + .Lx249_0]
                        mov              qword ptr [rbp + 2248], rax
                                                                                        jmp   n76_subscript_α
.Lx249_0:
                        .quad            1
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2352], rax
                        mov              qword ptr [rbp + 2360], rdx
                                                                                        jmp   n77_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n76_subscript_α:
                        mov              rdi, qword ptr [rbp + 2208]
                        mov              rsi, qword ptr [rbp + 2216]
                        mov              rdx, qword ptr [rbp + 2240]
                        mov              rcx, qword ptr [rbp + 2248]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n75_var_α
                        mov              qword ptr [rbp + 2272], rax
                        mov              qword ptr [rbp + 2280], rdx
                                                                                        jmp   n79_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:
                        mov              qword ptr [rbp + 2384], 6
                        mov              rax, qword ptr [rip + .Lx252_0]
                        mov              qword ptr [rbp + 2392], rax
                                                                                        jmp   n80_subscript_α
.Lx252_0:
                        .quad            2
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n78_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2496], rax
                        mov              qword ptr [rbp + 2504], rdx
                                                                                        jmp   n81_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:
                        mov              qword ptr [rbp + 2304], 1
                        mov              rax, qword ptr [rip + .Lx254_0]
                        mov              qword ptr [rbp + 2312], rax
                                                                                        jmp   n83_assign_var_α
.Lx254_0:
                        .quad            .Lx254_0_s
.Lx254_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n80_subscript_α:
                        mov              rdi, qword ptr [rbp + 2352]
                        mov              rsi, qword ptr [rbp + 2360]
                        mov              rdx, qword ptr [rbp + 2384]
                        mov              rcx, qword ptr [rbp + 2392]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n78_var_α
                        mov              qword ptr [rbp + 2416], rax
                        mov              qword ptr [rbp + 2424], rdx
                                                                                        jmp   n84_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_integer_α:
                        mov              qword ptr [rbp + 2528], 6
                        mov              rax, qword ptr [rip + .Lx256_0]
                        mov              qword ptr [rbp + 2536], rax
                                                                                        jmp   n85_subscript_α
.Lx256_0:
                        .quad            3
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n82_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2640], rax
                        mov              qword ptr [rbp + 2648], rdx
                                                                                        jmp   n86_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2272]
                        mov              rsi, qword ptr [rbp + 2280]
                        mov              rdx, qword ptr [rbp + 2304]
                        mov              rcx, qword ptr [rbp + 2312]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n75_var_α
                        mov              qword ptr [rbp + 2336], rax
                        mov              qword ptr [rbp + 2344], rdx
                                                                                        jmp   n75_var_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_string_α:
                        mov              qword ptr [rbp + 2448], 1
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rbp + 2456], rax
                                                                                        jmp   n88_assign_var_α
.Lx259_0:
                        .quad            .Lx259_0_s
.Lx259_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n85_subscript_α:
                        mov              rdi, qword ptr [rbp + 2496]
                        mov              rsi, qword ptr [rbp + 2504]
                        mov              rdx, qword ptr [rbp + 2528]
                        mov              rcx, qword ptr [rbp + 2536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n82_var_α
                        mov              qword ptr [rbp + 2560], rax
                        mov              qword ptr [rbp + 2568], rdx
                                                                                        jmp   n89_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_integer_α:
                        mov              qword ptr [rbp + 2672], 6
                        mov              rax, qword ptr [rip + .Lx261_0]
                        mov              qword ptr [rbp + 2680], rax
                                                                                        jmp   n90_subscript_α
.Lx261_0:
                        .quad            4
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n87_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2784], rax
                        mov              qword ptr [rbp + 2792], rdx
                                                                                        jmp   n91_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n88_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2416]
                        mov              rsi, qword ptr [rbp + 2424]
                        mov              rdx, qword ptr [rbp + 2448]
                        mov              rcx, qword ptr [rbp + 2456]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n78_var_α
                        mov              qword ptr [rbp + 2480], rax
                        mov              qword ptr [rbp + 2488], rdx
                                                                                        jmp   n78_var_α
#-----------------------------------------------------------------------------------------------------------------------
n89_lit_string_α:
                        mov              qword ptr [rbp + 2592], 1
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rbp + 2600], rax
                                                                                        jmp   n93_assign_var_α
.Lx264_0:
                        .quad            .Lx264_0_s
.Lx264_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n90_subscript_α:
                        mov              rdi, qword ptr [rbp + 2640]
                        mov              rsi, qword ptr [rbp + 2648]
                        mov              rdx, qword ptr [rbp + 2672]
                        mov              rcx, qword ptr [rbp + 2680]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n87_var_α
                        mov              qword ptr [rbp + 2704], rax
                        mov              qword ptr [rbp + 2712], rdx
                                                                                        jmp   n94_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_integer_α:
                        mov              qword ptr [rbp + 2816], 6
                        mov              rax, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rbp + 2824], rax
                                                                                        jmp   n95_subscript_α
.Lx266_0:
                        .quad            5
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2928], rax
                        mov              qword ptr [rbp + 2936], rdx
                                                                                        jmp   n96_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2560]
                        mov              rsi, qword ptr [rbp + 2568]
                        mov              rdx, qword ptr [rbp + 2592]
                        mov              rcx, qword ptr [rbp + 2600]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n82_var_α
                        mov              qword ptr [rbp + 2624], rax
                        mov              qword ptr [rbp + 2632], rdx
                                                                                        jmp   n82_var_α
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_string_α:
                        mov              qword ptr [rbp + 2736], 1
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rbp + 2744], rax
                                                                                        jmp   n98_assign_var_α
.Lx269_0:
                        .quad            .Lx269_0_s
.Lx269_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n95_subscript_α:
                        mov              rdi, qword ptr [rbp + 2784]
                        mov              rsi, qword ptr [rbp + 2792]
                        mov              rdx, qword ptr [rbp + 2816]
                        mov              rcx, qword ptr [rbp + 2824]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n92_var_α
                        mov              qword ptr [rbp + 2848], rax
                        mov              qword ptr [rbp + 2856], rdx
                                                                                        jmp   n99_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_integer_α:
                        mov              qword ptr [rbp + 2960], 6
                        mov              rax, qword ptr [rip + .Lx271_0]
                        mov              qword ptr [rbp + 2968], rax
                                                                                        jmp   n100_subscript_α
.Lx271_0:
                        .quad            6
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n97_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 3072], rax
                        mov              qword ptr [rbp + 3080], rdx
                                                                                        jmp   n101_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n98_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2704]
                        mov              rsi, qword ptr [rbp + 2712]
                        mov              rdx, qword ptr [rbp + 2736]
                        mov              rcx, qword ptr [rbp + 2744]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n87_var_α
                        mov              qword ptr [rbp + 2768], rax
                        mov              qword ptr [rbp + 2776], rdx
                                                                                        jmp   n87_var_α
#-----------------------------------------------------------------------------------------------------------------------
n99_lit_string_α:
                        mov              qword ptr [rbp + 2880], 1
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rbp + 2888], rax
                                                                                        jmp   n103_assign_var_α
.Lx274_0:
                        .quad            .Lx274_0_s
.Lx274_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n100_subscript_α:
                        mov              rdi, qword ptr [rbp + 2928]
                        mov              rsi, qword ptr [rbp + 2936]
                        mov              rdx, qword ptr [rbp + 2960]
                        mov              rcx, qword ptr [rbp + 2968]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n97_var_α
                        mov              qword ptr [rbp + 2992], rax
                        mov              qword ptr [rbp + 3000], rdx
                                                                                        jmp   n104_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_integer_α:
                        mov              qword ptr [rbp + 3104], 6
                        mov              rax, qword ptr [rip + .Lx276_0]
                        mov              qword ptr [rbp + 3112], rax
                                                                                        jmp   n105_subscript_α
.Lx276_0:
                        .quad            7
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n102_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 3216], rax
                        mov              qword ptr [rbp + 3224], rdx
                                                                                        jmp   n106_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n103_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2848]
                        mov              rsi, qword ptr [rbp + 2856]
                        mov              rdx, qword ptr [rbp + 2880]
                        mov              rcx, qword ptr [rbp + 2888]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n92_var_α
                        mov              qword ptr [rbp + 2912], rax
                        mov              qword ptr [rbp + 2920], rdx
                                                                                        jmp   n92_var_α
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_string_α:
                        mov              qword ptr [rbp + 3024], 1
                        mov              rax, qword ptr [rip + .Lx279_0]
                        mov              qword ptr [rbp + 3032], rax
                                                                                        jmp   n108_assign_var_α
.Lx279_0:
                        .quad            .Lx279_0_s
.Lx279_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n105_subscript_α:
                        mov              rdi, qword ptr [rbp + 3072]
                        mov              rsi, qword ptr [rbp + 3080]
                        mov              rdx, qword ptr [rbp + 3104]
                        mov              rcx, qword ptr [rbp + 3112]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n102_var_α
                        mov              qword ptr [rbp + 3136], rax
                        mov              qword ptr [rbp + 3144], rdx
                                                                                        jmp   n109_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_integer_α:
                        mov              qword ptr [rbp + 3248], 6
                        mov              rax, qword ptr [rip + .Lx281_0]
                        mov              qword ptr [rbp + 3256], rax
                                                                                        jmp   n110_subscript_α
.Lx281_0:
                        .quad            8
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n107_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 3360], rax
                        mov              qword ptr [rbp + 3368], rdx
                                                                                        jmp   n111_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n108_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2992]
                        mov              rsi, qword ptr [rbp + 3000]
                        mov              rdx, qword ptr [rbp + 3024]
                        mov              rcx, qword ptr [rbp + 3032]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n97_var_α
                        mov              qword ptr [rbp + 3056], rax
                        mov              qword ptr [rbp + 3064], rdx
                                                                                        jmp   n97_var_α
#-----------------------------------------------------------------------------------------------------------------------
n109_lit_string_α:
                        mov              qword ptr [rbp + 3168], 1
                        mov              rax, qword ptr [rip + .Lx284_0]
                        mov              qword ptr [rbp + 3176], rax
                                                                                        jmp   n113_assign_var_α
.Lx284_0:
                        .quad            .Lx284_0_s
.Lx284_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n110_subscript_α:
                        mov              rdi, qword ptr [rbp + 3216]
                        mov              rsi, qword ptr [rbp + 3224]
                        mov              rdx, qword ptr [rbp + 3248]
                        mov              rcx, qword ptr [rbp + 3256]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n107_var_α
                        mov              qword ptr [rbp + 3280], rax
                        mov              qword ptr [rbp + 3288], rdx
                                                                                        jmp   n114_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:
                        mov              qword ptr [rbp + 3392], 6
                        mov              rax, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rbp + 3400], rax
                                                                                        jmp   n115_subscript_α
.Lx286_0:
                        .quad            9
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n112_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 3504], rax
                        mov              qword ptr [rbp + 3512], rdx
                                                                                        jmp   n116_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_var_α:
                        mov              rdi, qword ptr [rbp + 3136]
                        mov              rsi, qword ptr [rbp + 3144]
                        mov              rdx, qword ptr [rbp + 3168]
                        mov              rcx, qword ptr [rbp + 3176]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n102_var_α
                        mov              qword ptr [rbp + 3200], rax
                        mov              qword ptr [rbp + 3208], rdx
                                                                                        jmp   n102_var_α
#-----------------------------------------------------------------------------------------------------------------------
n114_lit_string_α:
                        mov              qword ptr [rbp + 3312], 1
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rbp + 3320], rax
                                                                                        jmp   n118_assign_var_α
.Lx289_0:
                        .quad            .Lx289_0_s
.Lx289_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n115_subscript_α:
                        mov              rdi, qword ptr [rbp + 3360]
                        mov              rsi, qword ptr [rbp + 3368]
                        mov              rdx, qword ptr [rbp + 3392]
                        mov              rcx, qword ptr [rbp + 3400]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n112_var_α
                        mov              qword ptr [rbp + 3424], rax
                        mov              qword ptr [rbp + 3432], rdx
                                                                                        jmp   n119_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n116_lit_integer_α:
                        mov              qword ptr [rbp + 3536], 6
                        mov              rax, qword ptr [rip + .Lx291_0]
                        mov              qword ptr [rbp + 3544], rax
                                                                                        jmp   n120_subscript_α
.Lx291_0:
                        .quad            10
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n117_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 3648], rax
                        mov              qword ptr [rbp + 3656], rdx
                                                                                        jmp   n121_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n118_assign_var_α:
                        mov              rdi, qword ptr [rbp + 3280]
                        mov              rsi, qword ptr [rbp + 3288]
                        mov              rdx, qword ptr [rbp + 3312]
                        mov              rcx, qword ptr [rbp + 3320]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n107_var_α
                        mov              qword ptr [rbp + 3344], rax
                        mov              qword ptr [rbp + 3352], rdx
                                                                                        jmp   n107_var_α
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_string_α:
                        mov              qword ptr [rbp + 3456], 1
                        mov              rax, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rbp + 3464], rax
                                                                                        jmp   n123_assign_var_α
.Lx294_0:
                        .quad            .Lx294_0_s
.Lx294_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n120_subscript_α:
                        mov              rdi, qword ptr [rbp + 3504]
                        mov              rsi, qword ptr [rbp + 3512]
                        mov              rdx, qword ptr [rbp + 3536]
                        mov              rcx, qword ptr [rbp + 3544]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n117_var_α
                        mov              qword ptr [rbp + 3568], rax
                        mov              qword ptr [rbp + 3576], rdx
                                                                                        jmp   n124_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_integer_α:
                        mov              qword ptr [rbp + 3680], 6
                        mov              rax, qword ptr [rip + .Lx296_0]
                        mov              qword ptr [rbp + 3688], rax
                                                                                        jmp   n125_subscript_α
.Lx296_0:
                        .quad            11
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n122_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 3792], rax
                        mov              qword ptr [rbp + 3800], rdx
                                                                                        jmp   n126_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_var_α:
                        mov              rdi, qword ptr [rbp + 3424]
                        mov              rsi, qword ptr [rbp + 3432]
                        mov              rdx, qword ptr [rbp + 3456]
                        mov              rcx, qword ptr [rbp + 3464]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n112_var_α
                        mov              qword ptr [rbp + 3488], rax
                        mov              qword ptr [rbp + 3496], rdx
                                                                                        jmp   n112_var_α
#-----------------------------------------------------------------------------------------------------------------------
n124_lit_string_α:
                        mov              qword ptr [rbp + 3600], 1
                        mov              rax, qword ptr [rip + .Lx299_0]
                        mov              qword ptr [rbp + 3608], rax
                                                                                        jmp   n128_assign_var_α
.Lx299_0:
                        .quad            .Lx299_0_s
.Lx299_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n125_subscript_α:
                        mov              rdi, qword ptr [rbp + 3648]
                        mov              rsi, qword ptr [rbp + 3656]
                        mov              rdx, qword ptr [rbp + 3680]
                        mov              rcx, qword ptr [rbp + 3688]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n122_var_α
                        mov              qword ptr [rbp + 3712], rax
                        mov              qword ptr [rbp + 3720], rdx
                                                                                        jmp   n129_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n126_lit_integer_α:
                        mov              qword ptr [rbp + 3824], 6
                        mov              rax, qword ptr [rip + .Lx301_0]
                        mov              qword ptr [rbp + 3832], rax
                                                                                        jmp   n130_subscript_α
.Lx301_0:
                        .quad            12
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n127_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 3936], rax
                        mov              qword ptr [rbp + 3944], rdx
                                                                                        jmp   n131_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n128_assign_var_α:
                        mov              rdi, qword ptr [rbp + 3568]
                        mov              rsi, qword ptr [rbp + 3576]
                        mov              rdx, qword ptr [rbp + 3600]
                        mov              rcx, qword ptr [rbp + 3608]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n117_var_α
                        mov              qword ptr [rbp + 3632], rax
                        mov              qword ptr [rbp + 3640], rdx
                                                                                        jmp   n117_var_α
#-----------------------------------------------------------------------------------------------------------------------
n129_lit_string_α:
                        mov              qword ptr [rbp + 3744], 1
                        mov              rax, qword ptr [rip + .Lx304_0]
                        mov              qword ptr [rbp + 3752], rax
                                                                                        jmp   n133_assign_var_α
.Lx304_0:
                        .quad            .Lx304_0_s
.Lx304_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n130_subscript_α:
                        mov              rdi, qword ptr [rbp + 3792]
                        mov              rsi, qword ptr [rbp + 3800]
                        mov              rdx, qword ptr [rbp + 3824]
                        mov              rcx, qword ptr [rbp + 3832]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n127_var_α
                        mov              qword ptr [rbp + 3856], rax
                        mov              qword ptr [rbp + 3864], rdx
                                                                                        jmp   n134_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n131_lit_integer_α:
                        mov              qword ptr [rbp + 3968], 6
                        mov              rax, qword ptr [rip + .Lx306_0]
                        mov              qword ptr [rbp + 3976], rax
                                                                                        jmp   n135_subscript_α
.Lx306_0:
                        .quad            13
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_integer_α:
                        mov              qword ptr [rbp + 4096], 6
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rbp + 4104], rax
                                                                                        jmp   n136_assign_α
.Lx307_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n133_assign_var_α:
                        mov              rdi, qword ptr [rbp + 3712]
                        mov              rsi, qword ptr [rbp + 3720]
                        mov              rdx, qword ptr [rbp + 3744]
                        mov              rcx, qword ptr [rbp + 3752]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n122_var_α
                        mov              qword ptr [rbp + 3776], rax
                        mov              qword ptr [rbp + 3784], rdx
                                                                                        jmp   n122_var_α
#-----------------------------------------------------------------------------------------------------------------------
n134_lit_string_α:
                        mov              qword ptr [rbp + 3888], 1
                        mov              rax, qword ptr [rip + .Lx309_0]
                        mov              qword ptr [rbp + 3896], rax
                                                                                        jmp   n137_assign_var_α
.Lx309_0:
                        .quad            .Lx309_0_s
.Lx309_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n135_subscript_α:
                        mov              rdi, qword ptr [rbp + 3936]
                        mov              rsi, qword ptr [rbp + 3944]
                        mov              rdx, qword ptr [rbp + 3968]
                        mov              rcx, qword ptr [rbp + 3976]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n132_lit_integer_α
                        mov              qword ptr [rbp + 4000], rax
                        mov              qword ptr [rbp + 4008], rdx
                                                                                        jmp   n138_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n136_assign_α:
                        mov              rax, qword ptr [rbp + 4096]
                        mov              rdx, qword ptr [rbp + 4104]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        mov              qword ptr [rbp + 4080], rax
                        mov              qword ptr [rbp + 4088], rdx
                                                                                        jmp   n139_var_α
#-----------------------------------------------------------------------------------------------------------------------
n137_assign_var_α:
                        mov              rdi, qword ptr [rbp + 3856]
                        mov              rsi, qword ptr [rbp + 3864]
                        mov              rdx, qword ptr [rbp + 3888]
                        mov              rcx, qword ptr [rbp + 3896]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n127_var_α
                        mov              qword ptr [rbp + 3920], rax
                        mov              qword ptr [rbp + 3928], rdx
                                                                                        jmp   n127_var_α
#-----------------------------------------------------------------------------------------------------------------------
n138_lit_string_α:
                        mov              qword ptr [rbp + 4032], 1
                        mov              rax, qword ptr [rip + .Lx313_0]
                        mov              qword ptr [rbp + 4040], rax
                                                                                        jmp   n140_assign_var_α
.Lx313_0:
                        .quad            .Lx313_0_s
.Lx313_0_s:
                        .string          "I"
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n139_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 4256], rax
                        mov              qword ptr [rbp + 4264], rdx
                                                                                        jmp   n141_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n140_assign_var_α:
                        mov              rdi, qword ptr [rbp + 4000]
                        mov              rsi, qword ptr [rbp + 4008]
                        mov              rdx, qword ptr [rbp + 4032]
                        mov              rcx, qword ptr [rbp + 4040]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n132_lit_integer_α
                        mov              qword ptr [rbp + 4064], rax
                        mov              qword ptr [rbp + 4072], rdx
                                                                                        jmp   n132_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n141_lit_integer_α:
                        mov              qword ptr [rbp + 4224], 6
                        mov              rax, qword ptr [rip + .Lx316_0]
                        mov              qword ptr [rbp + 4232], rax
                                                                                        jmp   n143_op74_α
.Lx316_0:
                        .quad            0
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n142_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 5040], rax
                        mov              qword ptr [rbp + 5048], rdx
                                                                                        jmp   n144_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n143_op74_α:
                        mov              eax, dword ptr [rbp + 4256]
                        cmp              eax, 7
                                                                                        je    .Lx319_1
                        cmp              eax, 6
                                                                                        jne   .Lx319_0
                        mov              eax, dword ptr [rbp + 4224]
                        cmp              eax, 6
                                                                                        jne   .Lx319_0
.Lx319_1:
                        mov              rax, qword ptr [rbp + 4256]
                        mov              qword ptr [rbp + 4192], rax
                        mov              rax, qword ptr [rbp + 4264]
                        mov              qword ptr [rbp + 4200], rax
                                                                                        jmp   n145_op74_α
.Lx319_0:
                        lea              rdi, [rbp + 4256]
                        lea              rsi, [rbp + 4224]
                        lea              rdx, [rbp + 4192]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n145_op74_α
#-----------------------------------------------------------------------------------------------------------------------
n144_assign_α:
                        mov              rax, qword ptr [rbp + 5040]
                        mov              rdx, qword ptr [rbp + 5048]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 5024], rax
                        mov              qword ptr [rbp + 5032], rdx
                                                                                        jmp   proc_roman_γ
#-----------------------------------------------------------------------------------------------------------------------
n145_op74_α:
                        mov              eax, dword ptr [rbp + 4224]
                        cmp              eax, 7
                                                                                        je    .Lx322_1
                        cmp              eax, 6
                                                                                        jne   .Lx322_0
                        mov              eax, dword ptr [rbp + 4256]
                        cmp              eax, 6
                                                                                        jne   .Lx322_0
.Lx322_1:
                        mov              rax, qword ptr [rbp + 4224]
                        mov              qword ptr [rbp + 4160], rax
                        mov              rax, qword ptr [rbp + 4232]
                        mov              qword ptr [rbp + 4168], rax
                                                                                        jmp   n146_op76_α
.Lx322_0:
                        lea              rdi, [rbp + 4224]
                        lea              rsi, [rbp + 4256]
                        lea              rdx, [rbp + 4160]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n146_op76_α
#-----------------------------------------------------------------------------------------------------------------------
n146_op76_α:
                        lea              rdi, [rbp + 4192]
                        lea              rsi, [rbp + 4160]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n142_var_α
                        mov              qword ptr [rbp + 4128], 0
                        mov              qword ptr [rbp + 4136], 0
                                                                                        jmp   n147_var_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n147_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 4496], rax
                        mov              qword ptr [rbp + 4504], rdx
                                                                                        jmp   n148_var_α
#-----------------------------------------------------------------------------------------------------------------------
n148_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 4384], rax
                        mov              qword ptr [rbp + 4392], rdx
                                                                                        jmp   n150_var_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n149_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 4960], rax
                        mov              qword ptr [rbp + 4968], rdx
                                                                                        jmp   n151_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n150_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 4416], rax
                        mov              qword ptr [rbp + 4424], rdx
                                                                                        jmp   n152_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n151_lit_integer_α:
                        mov              qword ptr [rbp + 4992], 6
                        mov              rax, qword ptr [rip + .Lx329_0]
                        mov              qword ptr [rbp + 5000], rax
                                                                                        jmp   n153_binop_α
.Lx329_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n152_subscript_α:
                        mov              rdi, qword ptr [rbp + 4384]
                        mov              rsi, qword ptr [rbp + 4392]
                        mov              rdx, qword ptr [rbp + 4416]
                        mov              rcx, qword ptr [rbp + 4424]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n149_var_α
                        mov              qword ptr [rbp + 4448], rax
                        mov              qword ptr [rbp + 4456], rdx
                                                                                        jmp   n154_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n153_binop_α:
                        mov              eax, dword ptr [rbp + 4960]
                        cmp              eax, 100
                                                                                        je    .Lx331_0
                        mov              eax, dword ptr [rbp + 4960]
                        cmp              eax, 6
                                                                                        jne   .Lx331_2
.Lx331_1:
                        mov              rax, qword ptr [rbp + 4968]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 4928], 6
                        mov              qword ptr [rbp + 4936], rax
                                                                                        jmp   n155_assign_α
.Lx331_0:
                        mov              rdi, qword ptr [rbp + 4960]
                        mov              rsi, qword ptr [rbp + 4968]
                        mov              rdx, qword ptr [rbp + 4992]
                        mov              rcx, qword ptr [rbp + 5000]
                        mov              r8d, 0
                        lea              r9, [rbp + 4928]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n155_assign_α
.Lx331_2:
                        mov              rdi, qword ptr [rbp + 4960]
                        mov              rsi, qword ptr [rbp + 4968]
                        mov              rdx, qword ptr [rbp + 4992]
                        mov              rcx, qword ptr [rbp + 5000]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n139_var_α
                        mov              qword ptr [rbp + 4928], rax
                        mov              qword ptr [rbp + 4936], rdx
                                                                                        jmp   n155_assign_α
n153_binop_β:
                                                                                        jmp   n139_var_α
#-----------------------------------------------------------------------------------------------------------------------
n154_deref_α:
                        mov              rdi, qword ptr [rbp + 4448]
                        mov              rsi, qword ptr [rbp + 4456]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n149_var_α
                        mov              qword ptr [rbp + 4480], rax
                        mov              qword ptr [rbp + 4488], rdx
                                                                                        jmp   n156_op74_α
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_α:
                        mov              rax, qword ptr [rbp + 4928]
                        mov              rdx, qword ptr [rbp + 4936]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        mov              qword ptr [rbp + 4912], rax
                        mov              qword ptr [rbp + 4920], rdx
                                                                                        jmp   n139_var_α
#-----------------------------------------------------------------------------------------------------------------------
n156_op74_α:
                        mov              eax, dword ptr [rbp + 4496]
                        cmp              eax, 7
                                                                                        je    .Lx335_1
                        cmp              eax, 6
                                                                                        jne   .Lx335_0
                        mov              eax, dword ptr [rbp + 4480]
                        cmp              eax, 6
                                                                                        jne   .Lx335_0
.Lx335_1:
                        mov              rax, qword ptr [rbp + 4496]
                        mov              qword ptr [rbp + 4352], rax
                        mov              rax, qword ptr [rbp + 4504]
                        mov              qword ptr [rbp + 4360], rax
                                                                                        jmp   n157_op74_α
.Lx335_0:
                        lea              rdi, [rbp + 4496]
                        lea              rsi, [rbp + 4480]
                        lea              rdx, [rbp + 4352]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n157_op74_α
#-----------------------------------------------------------------------------------------------------------------------
n157_op74_α:
                        mov              eax, dword ptr [rbp + 4480]
                        cmp              eax, 7
                                                                                        je    .Lx337_1
                        cmp              eax, 6
                                                                                        jne   .Lx337_0
                        mov              eax, dword ptr [rbp + 4496]
                        cmp              eax, 6
                                                                                        jne   .Lx337_0
.Lx337_1:
                        mov              rax, qword ptr [rbp + 4480]
                        mov              qword ptr [rbp + 4320], rax
                        mov              rax, qword ptr [rbp + 4488]
                        mov              qword ptr [rbp + 4328], rax
                                                                                        jmp   n158_op76_α
.Lx337_0:
                        lea              rdi, [rbp + 4480]
                        lea              rsi, [rbp + 4496]
                        lea              rdx, [rbp + 4320]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n158_op76_α
#-----------------------------------------------------------------------------------------------------------------------
n158_op76_α:
                        lea              rdi, [rbp + 4352]
                        lea              rsi, [rbp + 4320]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        js    n149_var_α
                        mov              qword ptr [rbp + 4288], 0
                        mov              qword ptr [rbp + 4296], 0
                                                                                        jmp   n159_var_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n159_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 4576], rax
                        mov              qword ptr [rbp + 4584], rdx
                                                                                        jmp   n160_var_α
#-----------------------------------------------------------------------------------------------------------------------
n160_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 4608], rax
                        mov              qword ptr [rbp + 4616], rdx
                                                                                        jmp   n162_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n161_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 4768], rax
                        mov              qword ptr [rbp + 4776], rdx
                                                                                        jmp   n163_var_α
#-----------------------------------------------------------------------------------------------------------------------
n162_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 4640], rax
                        mov              qword ptr [rbp + 4648], rdx
                                                                                        jmp   n164_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n163_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 4800], rax
                        mov              qword ptr [rbp + 4808], rdx
                                                                                        jmp   n165_var_α
#-----------------------------------------------------------------------------------------------------------------------
n164_subscript_α:
                        mov              rdi, qword ptr [rbp + 4608]
                        mov              rsi, qword ptr [rbp + 4616]
                        mov              rdx, qword ptr [rbp + 4640]
                        mov              rcx, qword ptr [rbp + 4648]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n161_var_α
                        mov              qword ptr [rbp + 4672], rax
                        mov              qword ptr [rbp + 4680], rdx
                                                                                        jmp   n166_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n165_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 4832], rax
                        mov              qword ptr [rbp + 4840], rdx
                                                                                        jmp   n167_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n166_deref_α:
                        mov              rdi, qword ptr [rbp + 4672]
                        mov              rsi, qword ptr [rbp + 4680]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n161_var_α
                        mov              qword ptr [rbp + 4704], rax
                        mov              qword ptr [rbp + 4712], rdx
                                                                                        jmp   n168_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n167_subscript_α:
                        mov              rdi, qword ptr [rbp + 4800]
                        mov              rsi, qword ptr [rbp + 4808]
                        mov              rdx, qword ptr [rbp + 4832]
                        mov              rcx, qword ptr [rbp + 4840]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n139_var_α
                        mov              qword ptr [rbp + 4864], rax
                        mov              qword ptr [rbp + 4872], rdx
                                                                                        jmp   n169_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n168_binop_α:
                        mov              rdi, qword ptr [rbp + 4576]
                        mov              rsi, qword ptr [rbp + 4584]
                        mov              rdx, qword ptr [rbp + 4704]
                        mov              rcx, qword ptr [rbp + 4712]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 4544], rax
                        mov              qword ptr [rbp + 4552], rdx
                                                                                        jmp   n170_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n169_deref_α:
                        mov              rdi, qword ptr [rbp + 4864]
                        mov              rsi, qword ptr [rbp + 4872]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n139_var_α
                        mov              qword ptr [rbp + 4896], rax
                        mov              qword ptr [rbp + 4904], rdx
                                                                                        jmp   n171_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n170_assign_α:
                        mov              rax, qword ptr [rbp + 4544]
                        mov              rdx, qword ptr [rbp + 4552]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        mov              qword ptr [rbp + 4528], rax
                        mov              qword ptr [rbp + 4536], rdx
                                                                                        jmp   n161_var_α
#-----------------------------------------------------------------------------------------------------------------------
n171_binop_α:
                        mov              eax, dword ptr [rbp + 4768]
                        cmp              eax, 100
                                                                                        je    .Lx352_0
                        mov              eax, dword ptr [rbp + 4896]
                        cmp              eax, 100
                                                                                        je    .Lx352_0
                        mov              eax, dword ptr [rbp + 4768]
                        cmp              eax, 6
                                                                                        jne   .Lx352_2
                        mov              eax, dword ptr [rbp + 4896]
                        cmp              eax, 6
                                                                                        jne   .Lx352_2
.Lx352_1:
                        mov              rax, qword ptr [rbp + 4776]
                        mov              rcx, qword ptr [rbp + 4904]
                        sub              rax, rcx
                        mov              qword ptr [rbp + 4736], 6
                        mov              qword ptr [rbp + 4744], rax
                                                                                        jmp   n172_assign_α
.Lx352_0:
                        mov              rdi, qword ptr [rbp + 4768]
                        mov              rsi, qword ptr [rbp + 4776]
                        mov              rdx, qword ptr [rbp + 4896]
                        mov              rcx, qword ptr [rbp + 4904]
                        mov              r8d, 1
                        lea              r9, [rbp + 4736]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n172_assign_α
.Lx352_2:
                        mov              rdi, qword ptr [rbp + 4768]
                        mov              rsi, qword ptr [rbp + 4776]
                        mov              rdx, qword ptr [rbp + 4896]
                        mov              rcx, qword ptr [rbp + 4904]
                        mov              r8d, 1
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n139_var_α
                        mov              qword ptr [rbp + 4736], rax
                        mov              qword ptr [rbp + 4744], rdx
                                                                                        jmp   n172_assign_α
n171_binop_β:
                                                                                        jmp   n139_var_α
#-----------------------------------------------------------------------------------------------------------------------
n172_assign_α:
                        mov              rax, qword ptr [rbp + 4736]
                        mov              rdx, qword ptr [rbp + 4744]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 4720], rax
                        mov              qword ptr [rbp + 4728], rdx
                                                                                        jmp   n139_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_β:
                                                                                        jmp   proc_roman_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 5080]
                        lea              rsp, [rbp + 5104]
                        mov              rbp, [rbp + 5096]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_ω:
                        mov              rax, [rbp + 5088]
                        lea              rsp, [rbp + 5104]
                        mov              rbp, [rbp + 5096]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "roman"
.Lstartup_pp0_0:        .string          "n"
.Lstartup_pp0_1:        .string          "s"
.Lstartup_pp0_2:        .string          "v"
.Lstartup_pp0_3:        .string          "r"
.Lstartup_pp0_4:        .string          "i"
                        .align           8
.Lstartup_pnames0:
                        .quad            .Lstartup_pp0_0
                        .quad            .Lstartup_pp0_1
                        .quad            .Lstartup_pp0_2
                        .quad            .Lstartup_pp0_3
                        .quad            .Lstartup_pp0_4
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + .Lstartup_pnames0]
                        mov              edx, 5
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_roman_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 5
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 5072
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "roman"
.Lgvan1:                .string          "n"
.Lgvan2:                .string          "s"
.Lgvan3:                .string          "v"
.Lgvan4:                .string          "r"
.Lgvan5:                .string          "i"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 6
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 6
                        call             gva_register@PLT
                        mov              r12, qword ptr [1879048192]
                        xor              esi, esi
                        call             main_α
                        xor              eax, eax
                        add              rsp, 24
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_α:
                        .global          main_α
                        .global          main_β
                        .global          main_γ
                        .global          main_ω
                        sub              rsp, 5656
                        mov              rdi, rsp
                        mov              ecx, 5656
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 5648], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         DEFINE('roman(n)s,v,r,i')                                     :(roman_end)
#         OUTPUT = roman(1)
#-----------------------------------------------------------------------------------------------------------------------
n354_lit_integer_α:
                        mov              qword ptr [rbp + 5136], 6
                        mov              rax, qword ptr [rip + .Lx372_0]
                        mov              qword ptr [rbp + 5144], rax
                                                                                        jmp   n355_call_α
.Lx372_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n355_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx374_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx374_5
                        mov              rax, qword ptr [rbp + 5136]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 5144]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx374_6]
                        lea              rdx, [rip + .Lx374_7]
                                                                                        jmp   rax
.Lx374_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx374_2
.Lx374_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx374_2
.Lx374_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx374_20
                        mov              rax, qword ptr [rbp + 5136]
                        mov              rdx, qword ptr [rbp + 5144]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx374_21
.Lx374_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 5136]
                        mov              rdx, qword ptr [rbp + 5144]
                        call             rt_arg_stage@PLT
.Lx374_21:
                        mov              rdi, qword ptr [rip + .Lx374_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx374_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx374_3]
                        lea              rdx, [rip + .Lx374_4]
                                                                                        jmp   rax
.Lx374_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx374_2
.Lx374_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx374_2
.Lx374_1:
                        call             rt_faildescr@PLT
.Lx374_2:
                        mov              qword ptr [rbp + 5088], rax
                        mov              qword ptr [rbp + 5096], rdx
                        cmp              eax, 99
                                                                                        je    n357_lit_integer_α
                                                                                        jmp   n356_assign_α
n355_call_β:
                                                                                        jmp   n357_lit_integer_α
.Lx374_0:
                        .quad            .Lx374_0_s
.Lx374_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n356_assign_α:
                        mov              rsi, qword ptr [rbp + 5088]
                        mov              rdx, qword ptr [rbp + 5096]
                        mov              rdi, qword ptr [rip + .Lx375_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 5072], rax
                        mov              qword ptr [rbp + 5080], rdx
                                                                                        jmp   n357_lit_integer_α
.Lx375_0:
                        .quad            .Lx375_0_s
.Lx375_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n357_lit_integer_α:
                        mov              qword ptr [rbp + 5232], 6
                        mov              rax, qword ptr [rip + .Lx376_0]
                        mov              qword ptr [rbp + 5240], rax
                                                                                        jmp   n358_call_α
.Lx376_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n358_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx378_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx378_5
                        mov              rax, qword ptr [rbp + 5232]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 5240]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx378_6]
                        lea              rdx, [rip + .Lx378_7]
                                                                                        jmp   rax
.Lx378_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx378_2
.Lx378_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx378_2
.Lx378_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx378_20
                        mov              rax, qword ptr [rbp + 5232]
                        mov              rdx, qword ptr [rbp + 5240]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx378_21
.Lx378_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 5232]
                        mov              rdx, qword ptr [rbp + 5240]
                        call             rt_arg_stage@PLT
.Lx378_21:
                        mov              rdi, qword ptr [rip + .Lx378_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx378_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx378_3]
                        lea              rdx, [rip + .Lx378_4]
                                                                                        jmp   rax
.Lx378_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx378_2
.Lx378_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx378_2
.Lx378_1:
                        call             rt_faildescr@PLT
.Lx378_2:
                        mov              qword ptr [rbp + 5184], rax
                        mov              qword ptr [rbp + 5192], rdx
                        cmp              eax, 99
                                                                                        je    n360_lit_integer_α
                                                                                        jmp   n359_assign_α
n358_call_β:
                                                                                        jmp   n360_lit_integer_α
.Lx378_0:
                        .quad            .Lx378_0_s
.Lx378_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n359_assign_α:
                        mov              rsi, qword ptr [rbp + 5184]
                        mov              rdx, qword ptr [rbp + 5192]
                        mov              rdi, qword ptr [rip + .Lx379_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 5168], rax
                        mov              qword ptr [rbp + 5176], rdx
                                                                                        jmp   n360_lit_integer_α
.Lx379_0:
                        .quad            .Lx379_0_s
.Lx379_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n360_lit_integer_α:
                        mov              qword ptr [rbp + 5328], 6
                        mov              rax, qword ptr [rip + .Lx380_0]
                        mov              qword ptr [rbp + 5336], rax
                                                                                        jmp   n361_call_α
.Lx380_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n361_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx382_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx382_5
                        mov              rax, qword ptr [rbp + 5328]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 5336]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx382_6]
                        lea              rdx, [rip + .Lx382_7]
                                                                                        jmp   rax
.Lx382_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx382_2
.Lx382_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx382_2
.Lx382_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx382_20
                        mov              rax, qword ptr [rbp + 5328]
                        mov              rdx, qword ptr [rbp + 5336]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx382_21
.Lx382_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 5328]
                        mov              rdx, qword ptr [rbp + 5336]
                        call             rt_arg_stage@PLT
.Lx382_21:
                        mov              rdi, qword ptr [rip + .Lx382_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx382_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx382_3]
                        lea              rdx, [rip + .Lx382_4]
                                                                                        jmp   rax
.Lx382_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx382_2
.Lx382_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx382_2
.Lx382_1:
                        call             rt_faildescr@PLT
.Lx382_2:
                        mov              qword ptr [rbp + 5280], rax
                        mov              qword ptr [rbp + 5288], rdx
                        cmp              eax, 99
                                                                                        je    n363_lit_integer_α
                                                                                        jmp   n362_assign_α
n361_call_β:
                                                                                        jmp   n363_lit_integer_α
.Lx382_0:
                        .quad            .Lx382_0_s
.Lx382_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n362_assign_α:
                        mov              rsi, qword ptr [rbp + 5280]
                        mov              rdx, qword ptr [rbp + 5288]
                        mov              rdi, qword ptr [rip + .Lx383_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 5264], rax
                        mov              qword ptr [rbp + 5272], rdx
                                                                                        jmp   n363_lit_integer_α
.Lx383_0:
                        .quad            .Lx383_0_s
.Lx383_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n363_lit_integer_α:
                        mov              qword ptr [rbp + 5424], 6
                        mov              rax, qword ptr [rip + .Lx384_0]
                        mov              qword ptr [rbp + 5432], rax
                                                                                        jmp   n364_call_α
.Lx384_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n364_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx386_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx386_5
                        mov              rax, qword ptr [rbp + 5424]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 5432]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx386_6]
                        lea              rdx, [rip + .Lx386_7]
                                                                                        jmp   rax
.Lx386_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx386_2
.Lx386_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx386_2
.Lx386_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx386_20
                        mov              rax, qword ptr [rbp + 5424]
                        mov              rdx, qword ptr [rbp + 5432]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx386_21
.Lx386_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 5424]
                        mov              rdx, qword ptr [rbp + 5432]
                        call             rt_arg_stage@PLT
.Lx386_21:
                        mov              rdi, qword ptr [rip + .Lx386_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx386_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx386_3]
                        lea              rdx, [rip + .Lx386_4]
                                                                                        jmp   rax
.Lx386_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx386_2
.Lx386_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx386_2
.Lx386_1:
                        call             rt_faildescr@PLT
.Lx386_2:
                        mov              qword ptr [rbp + 5376], rax
                        mov              qword ptr [rbp + 5384], rdx
                        cmp              eax, 99
                                                                                        je    n366_lit_integer_α
                                                                                        jmp   n365_assign_α
n364_call_β:
                                                                                        jmp   n366_lit_integer_α
.Lx386_0:
                        .quad            .Lx386_0_s
.Lx386_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n365_assign_α:
                        mov              rsi, qword ptr [rbp + 5376]
                        mov              rdx, qword ptr [rbp + 5384]
                        mov              rdi, qword ptr [rip + .Lx387_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 5360], rax
                        mov              qword ptr [rbp + 5368], rdx
                                                                                        jmp   n366_lit_integer_α
.Lx387_0:
                        .quad            .Lx387_0_s
.Lx387_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n366_lit_integer_α:
                        mov              qword ptr [rbp + 5520], 6
                        mov              rax, qword ptr [rip + .Lx388_0]
                        mov              qword ptr [rbp + 5528], rax
                                                                                        jmp   n367_call_α
.Lx388_0:
                        .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n367_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx390_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx390_5
                        mov              rax, qword ptr [rbp + 5520]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 5528]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx390_6]
                        lea              rdx, [rip + .Lx390_7]
                                                                                        jmp   rax
.Lx390_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx390_2
.Lx390_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx390_2
.Lx390_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx390_20
                        mov              rax, qword ptr [rbp + 5520]
                        mov              rdx, qword ptr [rbp + 5528]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx390_21
.Lx390_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 5520]
                        mov              rdx, qword ptr [rbp + 5528]
                        call             rt_arg_stage@PLT
.Lx390_21:
                        mov              rdi, qword ptr [rip + .Lx390_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx390_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx390_3]
                        lea              rdx, [rip + .Lx390_4]
                                                                                        jmp   rax
.Lx390_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx390_2
.Lx390_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx390_2
.Lx390_1:
                        call             rt_faildescr@PLT
.Lx390_2:
                        mov              qword ptr [rbp + 5472], rax
                        mov              qword ptr [rbp + 5480], rdx
                        cmp              eax, 99
                                                                                        je    n369_lit_integer_α
                                                                                        jmp   n368_assign_α
n367_call_β:
                                                                                        jmp   n369_lit_integer_α
.Lx390_0:
                        .quad            .Lx390_0_s
.Lx390_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n368_assign_α:
                        mov              rsi, qword ptr [rbp + 5472]
                        mov              rdx, qword ptr [rbp + 5480]
                        mov              rdi, qword ptr [rip + .Lx391_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 5456], rax
                        mov              qword ptr [rbp + 5464], rdx
                                                                                        jmp   n369_lit_integer_α
.Lx391_0:
                        .quad            .Lx391_0_s
.Lx391_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n369_lit_integer_α:
                        mov              qword ptr [rbp + 5616], 6
                        mov              rax, qword ptr [rip + .Lx392_0]
                        mov              qword ptr [rbp + 5624], rax
                                                                                        jmp   n370_call_α
.Lx392_0:
                        .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n370_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx394_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx394_5
                        mov              rax, qword ptr [rbp + 5616]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 5624]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx394_6]
                        lea              rdx, [rip + .Lx394_7]
                                                                                        jmp   rax
.Lx394_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx394_2
.Lx394_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx394_2
.Lx394_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx394_20
                        mov              rax, qword ptr [rbp + 5616]
                        mov              rdx, qword ptr [rbp + 5624]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx394_21
.Lx394_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 5616]
                        mov              rdx, qword ptr [rbp + 5624]
                        call             rt_arg_stage@PLT
.Lx394_21:
                        mov              rdi, qword ptr [rip + .Lx394_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx394_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx394_3]
                        lea              rdx, [rip + .Lx394_4]
                                                                                        jmp   rax
.Lx394_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx394_2
.Lx394_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx394_2
.Lx394_1:
                        call             rt_faildescr@PLT
.Lx394_2:
                        mov              qword ptr [rbp + 5568], rax
                        mov              qword ptr [rbp + 5576], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n371_assign_α
n370_call_β:
                                                                                        jmp   main_γ
.Lx394_0:
                        .quad            .Lx394_0_s
.Lx394_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n371_assign_α:
                        mov              rsi, qword ptr [rbp + 5568]
                        mov              rdx, qword ptr [rbp + 5576]
                        mov              rdi, qword ptr [rip + .Lx395_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 5552], rax
                        mov              qword ptr [rbp + 5560], rdx
                                                                                        jmp   main_γ
.Lx395_0:
                        .quad            .Lx395_0_s
.Lx395_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 5648]
                        add              rsp, 5656
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 5648]
                        add              rsp, 5656
                        ret
                        .section         .note.GNU-stack,"",@progbits

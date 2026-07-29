                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_roman_α
proc_roman_α:
                        .global          proc_roman_α
                        .global          proc_roman_β
                        .global          proc_roman_γ
                        .global          proc_roman_ω
                        sub              rsp, 1520
                        mov              [rsp + 1496], rcx
                        mov              [rsp + 1504], rdx
                        mov              [rsp + 1512], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 1424
                        mov              edx, 1488
                        call             rt_jmp_frame_lexprep2@PLT
proc_roman_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rbp + 1216], 6
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rbp + 1224], rax
                                                                                        jmp   n1_lit_integer_α
.Lx64_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        mov              qword ptr [rbp + 1232], 6
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rbp + 1240], rax
                                                                                        jmp   n2_lit_integer_α
.Lx65_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rbp + 1248], 6
                        mov              rax, qword ptr [rip + .Lx66_0]
                        mov              qword ptr [rbp + 1256], rax
                                                                                        jmp   n3_lit_integer_α
.Lx66_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        mov              qword ptr [rbp + 1264], 6
                        mov              rax, qword ptr [rip + .Lx67_0]
                        mov              qword ptr [rbp + 1272], rax
                                                                                        jmp   n4_lit_integer_α
.Lx67_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        mov              qword ptr [rbp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx68_0]
                        mov              qword ptr [rbp + 1288], rax
                                                                                        jmp   n5_lit_integer_α
.Lx68_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:
                        mov              qword ptr [rbp + 1296], 6
                        mov              rax, qword ptr [rip + .Lx69_0]
                        mov              qword ptr [rbp + 1304], rax
                                                                                        jmp   n6_lit_integer_α
.Lx69_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        mov              qword ptr [rbp + 1312], 6
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rbp + 1320], rax
                                                                                        jmp   n7_lit_integer_α
.Lx70_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        mov              qword ptr [rbp + 1328], 6
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rbp + 1336], rax
                                                                                        jmp   n8_lit_integer_α
.Lx71_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        mov              qword ptr [rbp + 1344], 6
                        mov              rax, qword ptr [rip + .Lx72_0]
                        mov              qword ptr [rbp + 1352], rax
                                                                                        jmp   n9_lit_integer_α
.Lx72_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rbp + 1360], 6
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rbp + 1368], rax
                                                                                        jmp   n10_lit_integer_α
.Lx73_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rbp + 1376], 6
                        mov              rax, qword ptr [rip + .Lx74_0]
                        mov              qword ptr [rbp + 1384], rax
                                                                                        jmp   n11_lit_integer_α
.Lx74_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rbp + 1392], 6
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rbp + 1400], rax
                                                                                        jmp   n12_lit_integer_α
.Lx75_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:
                        mov              qword ptr [rbp + 1408], 6
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rbp + 1416], rax
                                                                                        jmp   n13_make_list_α
.Lx76_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n13_make_list_α:
                        mov              rax, qword ptr [rbp + 1216]
                        mov              qword ptr [rbp + 1008], rax
                        mov              rax, qword ptr [rbp + 1224]
                        mov              qword ptr [rbp + 1016], rax
                        mov              rax, qword ptr [rbp + 1232]
                        mov              qword ptr [rbp + 1024], rax
                        mov              rax, qword ptr [rbp + 1240]
                        mov              qword ptr [rbp + 1032], rax
                        mov              rax, qword ptr [rbp + 1248]
                        mov              qword ptr [rbp + 1040], rax
                        mov              rax, qword ptr [rbp + 1256]
                        mov              qword ptr [rbp + 1048], rax
                        mov              rax, qword ptr [rbp + 1264]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1272]
                        mov              qword ptr [rbp + 1064], rax
                        mov              rax, qword ptr [rbp + 1280]
                        mov              qword ptr [rbp + 1072], rax
                        mov              rax, qword ptr [rbp + 1288]
                        mov              qword ptr [rbp + 1080], rax
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 1088], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 1096], rax
                        mov              rax, qword ptr [rbp + 1312]
                        mov              qword ptr [rbp + 1104], rax
                        mov              rax, qword ptr [rbp + 1320]
                        mov              qword ptr [rbp + 1112], rax
                        mov              rax, qword ptr [rbp + 1328]
                        mov              qword ptr [rbp + 1120], rax
                        mov              rax, qword ptr [rbp + 1336]
                        mov              qword ptr [rbp + 1128], rax
                        mov              rax, qword ptr [rbp + 1344]
                        mov              qword ptr [rbp + 1136], rax
                        mov              rax, qword ptr [rbp + 1352]
                        mov              qword ptr [rbp + 1144], rax
                        mov              rax, qword ptr [rbp + 1360]
                        mov              qword ptr [rbp + 1152], rax
                        mov              rax, qword ptr [rbp + 1368]
                        mov              qword ptr [rbp + 1160], rax
                        mov              rax, qword ptr [rbp + 1376]
                        mov              qword ptr [rbp + 1168], rax
                        mov              rax, qword ptr [rbp + 1384]
                        mov              qword ptr [rbp + 1176], rax
                        mov              rax, qword ptr [rbp + 1392]
                        mov              qword ptr [rbp + 1184], rax
                        mov              rax, qword ptr [rbp + 1400]
                        mov              qword ptr [rbp + 1192], rax
                        mov              rax, qword ptr [rbp + 1408]
                        mov              qword ptr [rbp + 1200], rax
                        mov              rax, qword ptr [rbp + 1416]
                        mov              qword ptr [rbp + 1208], rax
                        lea              rdi, [rbp + 1008]
                        mov              esi, 13
                        call             rt_make_list@PLT
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n14_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:
                        mov              rax, qword ptr [rbp + 992]
                        mov              rdx, qword ptr [rbp + 1000]
                        mov              qword ptr [rbp + 1456], rax
                        mov              qword ptr [rbp + 1464], rdx
                                                                                        jmp   n15_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        mov              qword ptr [rbp + 784], 1
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rbp + 792], rax
                                                                                        jmp   n16_lit_string_α
.Lx80_0:
                        .quad            .Lx80_0_s
.Lx80_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        mov              qword ptr [rbp + 800], 1
                        mov              rax, qword ptr [rip + .Lx81_0]
                        mov              qword ptr [rbp + 808], rax
                                                                                        jmp   n17_lit_string_α
.Lx81_0:
                        .quad            .Lx81_0_s
.Lx81_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:
                        mov              qword ptr [rbp + 816], 1
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   n18_lit_string_α
.Lx82_0:
                        .quad            .Lx82_0_s
.Lx82_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        mov              qword ptr [rbp + 832], 1
                        mov              rax, qword ptr [rip + .Lx83_0]
                        mov              qword ptr [rbp + 840], rax
                                                                                        jmp   n19_lit_string_α
.Lx83_0:
                        .quad            .Lx83_0_s
.Lx83_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        mov              qword ptr [rbp + 848], 1
                        mov              rax, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rbp + 856], rax
                                                                                        jmp   n20_lit_string_α
.Lx84_0:
                        .quad            .Lx84_0_s
.Lx84_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:
                        mov              qword ptr [rbp + 864], 1
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rbp + 872], rax
                                                                                        jmp   n21_lit_string_α
.Lx85_0:
                        .quad            .Lx85_0_s
.Lx85_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:
                        mov              qword ptr [rbp + 880], 1
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rbp + 888], rax
                                                                                        jmp   n22_lit_string_α
.Lx86_0:
                        .quad            .Lx86_0_s
.Lx86_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        mov              qword ptr [rbp + 896], 1
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   n23_lit_string_α
.Lx87_0:
                        .quad            .Lx87_0_s
.Lx87_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        mov              qword ptr [rbp + 912], 1
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rbp + 920], rax
                                                                                        jmp   n24_lit_string_α
.Lx88_0:
                        .quad            .Lx88_0_s
.Lx88_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        mov              qword ptr [rbp + 928], 1
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rbp + 936], rax
                                                                                        jmp   n25_lit_string_α
.Lx89_0:
                        .quad            .Lx89_0_s
.Lx89_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:
                        mov              qword ptr [rbp + 944], 1
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rbp + 952], rax
                                                                                        jmp   n26_lit_string_α
.Lx90_0:
                        .quad            .Lx90_0_s
.Lx90_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:
                        mov              qword ptr [rbp + 960], 1
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rbp + 968], rax
                                                                                        jmp   n27_lit_string_α
.Lx91_0:
                        .quad            .Lx91_0_s
.Lx91_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        mov              qword ptr [rbp + 976], 1
                        mov              rax, qword ptr [rip + .Lx92_0]
                        mov              qword ptr [rbp + 984], rax
                                                                                        jmp   n28_make_list_α
.Lx92_0:
                        .quad            .Lx92_0_s
.Lx92_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n28_make_list_α:
                        mov              rax, qword ptr [rbp + 784]
                        mov              qword ptr [rbp + 576], rax
                        mov              rax, qword ptr [rbp + 792]
                        mov              qword ptr [rbp + 584], rax
                        mov              rax, qword ptr [rbp + 800]
                        mov              qword ptr [rbp + 592], rax
                        mov              rax, qword ptr [rbp + 808]
                        mov              qword ptr [rbp + 600], rax
                        mov              rax, qword ptr [rbp + 816]
                        mov              qword ptr [rbp + 608], rax
                        mov              rax, qword ptr [rbp + 824]
                        mov              qword ptr [rbp + 616], rax
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 624], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 632], rax
                        mov              rax, qword ptr [rbp + 848]
                        mov              qword ptr [rbp + 640], rax
                        mov              rax, qword ptr [rbp + 856]
                        mov              qword ptr [rbp + 648], rax
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 656], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 664], rax
                        mov              rax, qword ptr [rbp + 880]
                        mov              qword ptr [rbp + 672], rax
                        mov              rax, qword ptr [rbp + 888]
                        mov              qword ptr [rbp + 680], rax
                        mov              rax, qword ptr [rbp + 896]
                        mov              qword ptr [rbp + 688], rax
                        mov              rax, qword ptr [rbp + 904]
                        mov              qword ptr [rbp + 696], rax
                        mov              rax, qword ptr [rbp + 912]
                        mov              qword ptr [rbp + 704], rax
                        mov              rax, qword ptr [rbp + 920]
                        mov              qword ptr [rbp + 712], rax
                        mov              rax, qword ptr [rbp + 928]
                        mov              qword ptr [rbp + 720], rax
                        mov              rax, qword ptr [rbp + 936]
                        mov              qword ptr [rbp + 728], rax
                        mov              rax, qword ptr [rbp + 944]
                        mov              qword ptr [rbp + 736], rax
                        mov              rax, qword ptr [rbp + 952]
                        mov              qword ptr [rbp + 744], rax
                        mov              rax, qword ptr [rbp + 960]
                        mov              qword ptr [rbp + 752], rax
                        mov              rax, qword ptr [rbp + 968]
                        mov              qword ptr [rbp + 760], rax
                        mov              rax, qword ptr [rbp + 976]
                        mov              qword ptr [rbp + 768], rax
                        mov              rax, qword ptr [rbp + 984]
                        mov              qword ptr [rbp + 776], rax
                        lea              rdi, [rbp + 576]
                        mov              esi, 13
                        call             rt_make_list@PLT
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n29_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:
                        mov              rax, qword ptr [rbp + 560]
                        mov              rdx, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                                                                                        jmp   n30_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_string_α:
                        mov              qword ptr [rbp + 544], 1
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n31_assign_α
.Lx96_0:
                        .quad            .Lx96_0_s
.Lx96_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:
                        mov              rax, qword ptr [rbp + 544]
                        mov              rdx, qword ptr [rbp + 552]
                        mov              qword ptr [rbp + 1424], rax
                        mov              qword ptr [rbp + 1432], rdx
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rbp + 96], 6
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n33_var_α
.Lx98_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n33_var_α:
                        mov              rax, qword ptr [rbp + 1456]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 1464]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n34_unop_α
#-----------------------------------------------------------------------------------------------------------------------
n34_unop_α:
                        mov              rdi, qword ptr [rbp + 1456]
                        mov              rsi, qword ptr [rbp + 1464]
                        call             rt_size_d@PLT
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                                                                                        jmp   n36_to_α
#-----------------------------------------------------------------------------------------------------------------------
n35_var_α:
                        mov              rax, qword ptr [rbp + 1424]
                        mov              qword ptr [rbp + 32], rax
                        mov              rax, qword ptr [rbp + 1432]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n37_return_α
#-----------------------------------------------------------------------------------------------------------------------
n36_to_α:
                        mov              rdi, qword ptr [rbp + 96]
                        mov              rsi, qword ptr [rbp + 104]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 96], 6
                        mov              qword ptr [rbp + 104], rax
                        mov              rdi, qword ptr [rbp + 112]
                        mov              rsi, qword ptr [rbp + 120]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 112], 6
                        mov              qword ptr [rbp + 120], rax
                        mov              rax, qword ptr [rbp + 104]
                        mov              qword ptr [rbp + 80], rax
.Lx105_0:
                        mov              rax, qword ptr [rbp + 80]
                        mov              rcx, qword ptr [rbp + 120]
                        cmp              rax, rcx
                                                                                        jg    n35_var_α
                        mov              qword ptr [rbp + 64], 6
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n38_assign_α
n36_to_β:
                        inc              qword ptr [rbp + 80]
                                                                                        jmp   .Lx105_0
#-----------------------------------------------------------------------------------------------------------------------
n37_return_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              rdx, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                                                                                        jmp   proc_roman_γ
#-----------------------------------------------------------------------------------------------------------------------
n38_assign_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              rdx, qword ptr [rbp + 72]
                        mov              qword ptr [rbp + 1440], rax
                        mov              qword ptr [rbp + 1448], rdx
                                                                                        jmp   n39_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n39_bound_α:
                        mov              qword ptr [rbp + 144], rsp
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n41_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n41_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 1456]
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                                                                                        jmp   n43_var_α
#-----------------------------------------------------------------------------------------------------------------------
n42_unmark_α:
                        mov              rsp, qword ptr [rbp + 144]
                                                                                        jmp   n36_to_β
#-----------------------------------------------------------------------------------------------------------------------
n43_var_α:
                        mov              rax, qword ptr [rbp + 1440]
                        mov              qword ptr [rbp + 240], rax
                        mov              rax, qword ptr [rbp + 1448]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n44_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n44_subscript_α:
                        mov              rdi, qword ptr [rbp + 224]
                        mov              rsi, qword ptr [rbp + 232]
                        mov              rdx, qword ptr [rbp + 240]
                        mov              rcx, qword ptr [rbp + 248]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n42_unmark_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n45_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n45_deref_α:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n42_unmark_α
                        mov              qword ptr [rbp + 272], rax
                        mov              qword ptr [rbp + 280], rdx
                                                                                        jmp   n46_binop_test_α
#-----------------------------------------------------------------------------------------------------------------------
n46_binop_test_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 100
                                                                                        je    .Lx120_0
                        mov              eax, dword ptr [rbp + 272]
                        cmp              eax, 100
                                                                                        je    .Lx120_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx120_2
                        mov              eax, dword ptr [rbp + 272]
                        cmp              eax, 6
                                                                                        jne   .Lx120_2
.Lx120_1:
                        mov              rax, qword ptr [rbp + 24]
                        mov              rcx, qword ptr [rbp + 280]
                        cmp              rax, rcx
                                                                                        jl    n42_unmark_α
                        mov              rcx, qword ptr [rbp + 272]
                        mov              qword ptr [rbp + 192], rcx
                        mov              rcx, qword ptr [rbp + 280]
                        mov              qword ptr [rbp + 200], rcx
                                                                                        jmp   n47_var_α
.Lx120_0:
                        mov              rdi, qword ptr [rbp + 16]
                        mov              rsi, qword ptr [rbp + 24]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 8
                        lea              r9, [rbp + 192]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx120_1
                        cmp              eax, 1
                                                                                        je    n42_unmark_α
                                                                                        jmp   n47_var_α
.Lx120_2:
                        mov              rdi, qword ptr [rbp + 16]
                        mov              rsi, qword ptr [rbp + 24]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 8
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n42_unmark_α
                        mov              rax, qword ptr [rbp + 272]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 280]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n47_var_α
#-----------------------------------------------------------------------------------------------------------------------
n47_var_α:
                        mov              rax, qword ptr [rbp + 1424]
                        mov              qword ptr [rbp + 464], rax
                        mov              rax, qword ptr [rbp + 1432]
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   n48_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n48_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 1472]
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   n51_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:
                        mov              rax, qword ptr [rbp + 1440]
                        mov              qword ptr [rbp + 496], rax
                        mov              rax, qword ptr [rbp + 1448]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n52_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n51_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 1456]
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n53_var_α
#-----------------------------------------------------------------------------------------------------------------------
n52_subscript_α:
                        mov              rdi, qword ptr [rbp + 480]
                        mov              rsi, qword ptr [rbp + 488]
                        mov              rdx, qword ptr [rbp + 496]
                        mov              rcx, qword ptr [rbp + 504]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n49_var_α
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n54_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n53_var_α:
                        mov              rax, qword ptr [rbp + 1440]
                        mov              qword ptr [rbp + 400], rax
                        mov              rax, qword ptr [rbp + 1448]
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n55_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n54_deref_α:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n49_var_α
                        mov              qword ptr [rbp + 528], rax
                        mov              qword ptr [rbp + 536], rdx
                                                                                        jmp   n56_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n55_subscript_α:
                        mov              rdi, qword ptr [rbp + 384]
                        mov              rsi, qword ptr [rbp + 392]
                        mov              rdx, qword ptr [rbp + 400]
                        mov              rcx, qword ptr [rbp + 408]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n40_var_α
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                                                                                        jmp   n57_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n56_binop_α:
                        mov              rdi, qword ptr [rbp + 1424]
                        mov              rsi, qword ptr [rbp + 1432]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n58_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n57_deref_α:
                        mov              rdi, qword ptr [rbp + 416]
                        mov              rsi, qword ptr [rbp + 424]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n40_var_α
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                                                                                        jmp   n59_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rax, qword ptr [rbp + 448]
                        mov              rdx, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 1424], rax
                        mov              qword ptr [rbp + 1432], rdx
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n59_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx140_1
                        cmp              eax, 6
                                                                                        jne   .Lx140_0
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 6
                                                                                        jne   .Lx140_0
.Lx140_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n60_op75_α
.Lx140_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 432]
                        lea              rdx, [rbp + 352]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n60_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n60_op75_α:
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 7
                                                                                        je    .Lx142_1
                        cmp              eax, 6
                                                                                        jne   .Lx142_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx142_0
.Lx142_1:
                        mov              rax, qword ptr [rbp + 432]
                        mov              qword ptr [rbp + 336], rax
                        mov              rax, qword ptr [rbp + 440]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n61_binop_α
.Lx142_0:
                        lea              rdi, [rbp + 432]
                        lea              rsi, [rbp + 16]
                        lea              rdx, [rbp + 336]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n61_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n61_binop_α:
                        mov              eax, dword ptr [rbp + 352]
                        cmp              eax, 6
                                                                                        jne   .Lx143_0
                        mov              eax, dword ptr [rbp + 336]
                        cmp              eax, 6
                                                                                        jne   .Lx143_0
                        mov              rax, qword ptr [rbp + 360]
                        mov              rcx, qword ptr [rbp + 344]
                        sub              rax, rcx
                        mov              qword ptr [rbp + 320], 6
                        mov              qword ptr [rbp + 328], rax
                                                                                        jmp   n62_assign_α
.Lx143_0:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n40_var_α
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n62_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:
                        mov              rax, qword ptr [rbp + 320]
                        mov              rdx, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 16], rax
                        mov              qword ptr [rbp + 24], rdx
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n63_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n63_conjunction_α:
                        mov              rax, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   n40_var_α
n63_conjunction_β:
                                                                                        jmp   n40_var_α
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
                        mov              rax, [rbp + 1496]
                        lea              rsp, [rbp + 1520]
                        mov              rbp, [rbp + 1512]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_ω:
                        mov              rax, [rbp + 1504]
                        lea              rsp, [rbp + 1520]
                        mov              rbp, [rbp + 1512]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_dcα:
                        pop              r11
                        sub              rsp, 1536
                        mov              qword ptr [rsp + 1512], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rbp + 1488], r11
                        lea              rax, [rip + .Lx146_2]
                        mov              qword ptr [rbp + 1496], rax
                        lea              rax, [rip + .Lx146_3]
                        mov              qword ptr [rbp + 1504], rax
                        mov              qword ptr [rbp + 16], rsi
                        mov              rdi, rbp
                        mov              esi, 1424
                        mov              edx, 1488
                        mov              ecx, 1
                        mov              r8d, 1
                        mov              r9d, 0
                        call             rt_pl_dc_prep@PLT
                                                                                        jmp   proc_roman_α_body
.Lx146_2:
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, rsp
                        add              rcx, -1520
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_γ@PLT
.Lx146_3:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, rsp
                        add              rsi, -1520
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_ω@PLT
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "roman"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_roman_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1488
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_roman_dcα]
                        call             rt_proc_set_dcfn@PLT
                        add              rsp, 8
                        ret
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
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
                        sub              rsp, 344
                        mov              rdi, rsp
                        mov              ecx, 344
                        xor              eax, eax
                        rep stosb
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n147_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n148_call_proc_staged_α
.Lx156_0:
                        .quad            1776
#-----------------------------------------------------------------------------------------------------------------------
n148_call_proc_staged_α:
                        lea              rsi, [rsp + 320]
                        call             proc_roman_dcα
                                                                                        jmp   .Lx158_2
.Lx158_2:
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                        cmp              eax, 99
                                                                                        je    n150_lit_integer_α
                                                                                        jmp   n149_call_builtin_icon_α
n148_call_proc_staged_β:
                                                                                        jmp   n150_lit_integer_α
.Lx158_0:
                        .quad            .Lx158_0_s
.Lx158_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n149_call_builtin_icon_α:
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 240], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 248], rax
                        .section         .rodata
.Lrkfn160:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn160]
                        lea              rsi, [rsp + 240]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                        cmp              eax, 99
                                                                                        je    n150_lit_integer_α
                                                                                        jmp   n150_lit_integer_α
n149_call_builtin_icon_β:
                                                                                        jmp   n150_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_integer_α:
                        mov              qword ptr [rsp + 208], 6
                        mov              rax, qword ptr [rip + .Lx161_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n151_call_proc_staged_α
.Lx161_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n151_call_proc_staged_α:
                        lea              rsi, [rsp + 208]
                        call             proc_roman_dcα
                                                                                        jmp   .Lx163_2
.Lx163_2:
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                        cmp              eax, 99
                                                                                        je    n153_lit_integer_α
                                                                                        jmp   n152_call_builtin_icon_α
n151_call_proc_staged_β:
                                                                                        jmp   n153_lit_integer_α
.Lx163_0:
                        .quad            .Lx163_0_s
.Lx163_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n152_call_builtin_icon_α:
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 128], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 136], rax
                        .section         .rodata
.Lrkfn165:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn165]
                        lea              rsi, [rsp + 128]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                        cmp              eax, 99
                                                                                        je    n153_lit_integer_α
                                                                                        jmp   n153_lit_integer_α
n152_call_builtin_icon_β:
                                                                                        jmp   n153_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n153_lit_integer_α:
                        mov              qword ptr [rsp + 96], 6
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n154_call_proc_staged_α
.Lx166_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n154_call_proc_staged_α:
                        lea              rsi, [rsp + 96]
                        call             proc_roman_dcα
                                                                                        jmp   .Lx168_2
.Lx168_2:
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   n155_call_builtin_icon_α
n154_call_proc_staged_β:
                                                                                        jmp   main_ω
.Lx168_0:
                        .quad            .Lx168_0_s
.Lx168_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n155_call_builtin_icon_α:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], rax
                        .section         .rodata
.Lrkfn170:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn170]
                        lea              rsi, [rsp + 16]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   main_ω
n155_call_builtin_icon_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 344
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 344
                        ret
                        .section         .note.GNU-stack,"",@progbits

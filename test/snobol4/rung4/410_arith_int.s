                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "output"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 1
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 1
                        call             gva_register@PLT
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
                        sub              rsp, 1336
                        mov              rdi, rsp
                        mov              ecx, 1336
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         differ(3 + 2, 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rsp + 80], 6
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n11_lit_integer_α
.Lx75_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n12_lit_integer_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n13_lit_integer_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n14_lit_integer_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n15_lit_integer_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   n16_lit_string_α
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_goto_α:
                                                                                        jmp   n17_lit_integer_α
n6_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n7_goto_α:
                                                                                        jmp   n18_lit_string_α
n7_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n8_goto_α:
                                                                                        jmp   n19_lit_integer_α
n8_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n9_goto_α:
                                                                                        jmp   n20_lit_string_α
n9_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n10_goto_α:
                                                                                        jmp   main_γ
n10_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rsp + 96], 6
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n21_binop_α
.Lx86_0:
                        .quad            2
#=======================================================================================================================
#         differ(3 - 2, 1)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:
                        mov              qword ptr [rsp + 240], 6
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rsp + 248], rax
                                                                                        jmp   n22_lit_integer_α
.Lx87_0:
                        .quad            3
#=======================================================================================================================
#         differ(3 * 2, 6)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rsp + 384], 6
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rsp + 392], rax
                                                                                        jmp   n23_lit_integer_α
.Lx88_0:
                        .quad            3
#=======================================================================================================================
#         differ(5 / 2, 2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n24_lit_integer_α
.Lx89_0:
                        .quad            5
#=======================================================================================================================
#         differ(2 ** 3, 8)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        mov              qword ptr [rsp + 672], 6
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 680], rax
                                                                                        jmp   n25_lit_integer_α
.Lx90_0:
                        .quad            2
#=======================================================================================================================
#         differ('3' + 2, 5)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        mov              qword ptr [rsp + 816], 1
                        mov              dword ptr [rsp + 820], 1
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 824], rax
                                                                                        jmp   n26_lit_integer_α
.Lx91_0:
                        .quad            .Lx91_0_s
.Lx91_0_s:
                        .string          "3"
#=======================================================================================================================
#         differ(3 + '-2', 1)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        mov              qword ptr [rsp + 960], 6
                        mov              rax, qword ptr [rip + .Lx92_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n27_lit_string_α
.Lx92_0:
                        .quad            3
#=======================================================================================================================
#         differ('1' + '0', 1)                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        mov              qword ptr [rsp + 1104], 1
                        mov              dword ptr [rsp + 1108], 1
                        mov              rax, qword ptr [rip + .Lx93_0]
                        mov              qword ptr [rsp + 1112], rax
                                                                                        jmp   n28_lit_string_α
.Lx93_0:
                        .quad            .Lx93_0_s
.Lx93_0_s:
                        .string          "1"
#=======================================================================================================================
#         differ(5 + '', 5)                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        mov              qword ptr [rsp + 1248], 6
                        mov              rax, qword ptr [rip + .Lx94_0]
                        mov              qword ptr [rsp + 1256], rax
                                                                                        jmp   n29_lit_string_α
.Lx94_0:
                        .quad            5
#=======================================================================================================================
#         output = 'PASS 410_arith_int (9/9)'
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 24
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n30_assign_α
.Lx95_0:
                        .quad            .Lx95_0_s
.Lx95_0_s:
                        .string          "PASS 410_arith_int (9/9)"
#-----------------------------------------------------------------------------------------------------------------------
n21_binop_α:
                        mov              rdi, qword ptr [rsp + 80]
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 96]
                        mov              rcx, qword ptr [rsp + 104]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n12_lit_integer_α
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                                                                                        jmp   n31_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        mov              qword ptr [rsp + 256], 6
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 264], rax
                                                                                        jmp   n32_binop_α
.Lx97_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        mov              qword ptr [rsp + 400], 6
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rsp + 408], rax
                                                                                        jmp   n33_binop_α
.Lx98_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n34_binop_α
.Lx99_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx100_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n35_binop_α
.Lx100_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:
                        mov              qword ptr [rsp + 832], 6
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n36_binop_α
.Lx101_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        mov              qword ptr [rsp + 976], 1
                        mov              dword ptr [rsp + 980], 2
                        mov              rax, qword ptr [rip + .Lx102_0]
                        mov              qword ptr [rsp + 984], rax
                                                                                        jmp   n37_binop_α
.Lx102_0:
                        .quad            .Lx102_0_s
.Lx102_0_s:
                        .string          "-2"
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_string_α:
                        mov              qword ptr [rsp + 1120], 1
                        mov              dword ptr [rsp + 1124], 1
                        mov              rax, qword ptr [rip + .Lx103_0]
                        mov              qword ptr [rsp + 1128], rax
                                                                                        jmp   n38_binop_α
.Lx103_0:
                        .quad            .Lx103_0_s
.Lx103_0_s:
                        .string          "0"
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:
                        mov              qword ptr [rsp + 1264], 1
                        mov              dword ptr [rsp + 1268], 0
                        mov              rax, qword ptr [rip + .Lx104_0]
                        mov              qword ptr [rsp + 1272], rax
                                                                                        jmp   n39_binop_α
.Lx104_0:
                        .quad            .Lx104_0_s
.Lx104_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:
                        mov              qword ptr [rsp + 112], 6
                        mov              rax, qword ptr [rip + .Lx106_0]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n40_call_α
.Lx106_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n32_binop_α:
                        mov              rdi, qword ptr [rsp + 240]
                        mov              rsi, qword ptr [rsp + 248]
                        mov              rdx, qword ptr [rsp + 256]
                        mov              rcx, qword ptr [rsp + 264]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n13_lit_integer_α
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                                                                                        jmp   n41_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n33_binop_α:
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        mov              rdx, qword ptr [rsp + 400]
                        mov              rcx, qword ptr [rsp + 408]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n14_lit_integer_α
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n34_binop_α:
                        mov              rdi, qword ptr [rsp + 528]
                        mov              rsi, qword ptr [rsp + 536]
                        mov              rdx, qword ptr [rsp + 544]
                        mov              rcx, qword ptr [rsp + 552]
                        call             rt_div@PLT
                        cmp              eax, 99
                                                                                        je    n15_lit_integer_α
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n43_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n35_binop_α:
                        mov              rdi, qword ptr [rsp + 672]
                        mov              rsi, qword ptr [rsp + 680]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        call             rt_pow@PLT
                        cmp              eax, 99
                                                                                        je    n16_lit_string_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n44_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n36_binop_α:
                        mov              eax, dword ptr [rsp + 816]
                        cmp              eax, 6
                                                                                        jne   .Lx111_0
                        mov              rax, qword ptr [rsp + 824]
                        mov              rcx, 2
                        add              rax, rcx
                        mov              qword ptr [rsp + 800], 6
                        mov              qword ptr [rsp + 808], rax
                                                                                        jmp   n45_lit_integer_α
.Lx111_0:
                        mov              rdi, qword ptr [rsp + 816]
                        mov              rsi, qword ptr [rsp + 824]
                        mov              rdx, qword ptr [rsp + 832]
                        mov              rcx, qword ptr [rsp + 840]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n17_lit_integer_α
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                                                                                        jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n37_binop_α:
                        mov              eax, dword ptr [rsp + 976]
                        cmp              eax, 6
                                                                                        jne   .Lx112_0
                        mov              rax, 3
                        mov              rcx, qword ptr [rsp + 984]
                        add              rax, rcx
                        mov              qword ptr [rsp + 944], 6
                        mov              qword ptr [rsp + 952], rax
                                                                                        jmp   n46_lit_integer_α
.Lx112_0:
                        mov              rdi, qword ptr [rsp + 960]
                        mov              rsi, qword ptr [rsp + 968]
                        mov              rdx, qword ptr [rsp + 976]
                        mov              rcx, qword ptr [rsp + 984]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n18_lit_string_α
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                                                                                        jmp   n46_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n38_binop_α:
                        mov              eax, dword ptr [rsp + 1104]
                        cmp              eax, 6
                                                                                        jne   .Lx113_0
                        mov              eax, dword ptr [rsp + 1120]
                        cmp              eax, 6
                                                                                        jne   .Lx113_0
                        mov              rax, qword ptr [rsp + 1112]
                        mov              rcx, qword ptr [rsp + 1128]
                        add              rax, rcx
                        mov              qword ptr [rsp + 1088], 6
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n47_lit_integer_α
.Lx113_0:
                        mov              rdi, qword ptr [rsp + 1104]
                        mov              rsi, qword ptr [rsp + 1112]
                        mov              rdx, qword ptr [rsp + 1120]
                        mov              rcx, qword ptr [rsp + 1128]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n19_lit_integer_α
                        mov              qword ptr [rsp + 1088], rax
                        mov              qword ptr [rsp + 1096], rdx
                                                                                        jmp   n47_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n39_binop_α:
                        mov              eax, dword ptr [rsp + 1264]
                        cmp              eax, 6
                                                                                        jne   .Lx114_0
                        mov              rax, 5
                        mov              rcx, qword ptr [rsp + 1272]
                        add              rax, rcx
                        mov              qword ptr [rsp + 1232], 6
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n48_lit_integer_α
.Lx114_0:
                        mov              rdi, qword ptr [rsp + 1248]
                        mov              rsi, qword ptr [rsp + 1256]
                        mov              rdx, qword ptr [rsp + 1264]
                        mov              rcx, qword ptr [rsp + 1272]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n20_lit_string_α
                        mov              qword ptr [rsp + 1232], rax
                        mov              qword ptr [rsp + 1240], rdx
                                                                                        jmp   n48_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n40_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lbynamefn41:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn41]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n12_lit_integer_α
                                                                                        jmp   n49_lit_string_α
n40_call_β:
                                                                                        jmp   n12_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_integer_α:
                        mov              qword ptr [rsp + 272], 6
                        mov              rax, qword ptr [rip + .Lx116_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n50_call_α
.Lx116_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rsp + 416], 6
                        mov              rax, qword ptr [rip + .Lx117_0]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n51_call_α
.Lx117_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        mov              qword ptr [rsp + 560], 6
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n52_call_α
.Lx118_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_integer_α:
                        mov              qword ptr [rsp + 704], 6
                        mov              rax, qword ptr [rip + .Lx119_0]
                        mov              qword ptr [rsp + 712], rax
                                                                                        jmp   n53_call_α
.Lx119_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:
                        mov              qword ptr [rsp + 848], 6
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 856], rax
                                                                                        jmp   n54_call_α
.Lx120_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_integer_α:
                        mov              qword ptr [rsp + 992], 6
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rsp + 1000], rax
                                                                                        jmp   n55_call_α
.Lx121_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:
                        mov              qword ptr [rsp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx122_0]
                        mov              qword ptr [rsp + 1144], rax
                                                                                        jmp   n56_call_α
.Lx122_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_integer_α:
                        mov              qword ptr [rsp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rsp + 1288], rax
                                                                                        jmp   n57_call_α
.Lx123_0:
                        .quad            5
#=======================================================================================================================
#         output = 'FAIL 410/001: 3+2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n58_assign_α
.Lx124_0:
                        .quad            .Lx124_0_s
.Lx124_0_s:
                        .string          "FAIL 410/001: 3+2"
#-----------------------------------------------------------------------------------------------------------------------
n50_call_α:
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 176], rax
                        mov              rax, qword ptr [rsp + 232]
                        mov              qword ptr [rsp + 184], rax
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 200], rax
                        .section         .rodata
.Lbynamefn51:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn51]
                        lea              rsi, [rsp + 176]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                        cmp              eax, 99
                                                                                        je    n13_lit_integer_α
                                                                                        jmp   n59_lit_string_α
n50_call_β:
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n51_call_α:
                        mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 320], rax
                        mov              rax, qword ptr [rsp + 376]
                        mov              qword ptr [rsp + 328], rax
                        mov              rax, qword ptr [rsp + 416]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 424]
                        mov              qword ptr [rsp + 344], rax
                        .section         .rodata
.Lbynamefn52:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn52]
                        lea              rsi, [rsp + 320]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                        cmp              eax, 99
                                                                                        je    n14_lit_integer_α
                                                                                        jmp   n60_lit_string_α
n51_call_β:
                                                                                        jmp   n14_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 520]
                        mov              qword ptr [rsp + 472], rax
                        mov              rax, qword ptr [rsp + 560]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 568]
                        mov              qword ptr [rsp + 488], rax
                        .section         .rodata
.Lbynamefn53:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn53]
                        lea              rsi, [rsp + 464]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                        cmp              eax, 99
                                                                                        je    n15_lit_integer_α
                                                                                        jmp   n61_lit_string_α
n52_call_β:
                                                                                        jmp   n15_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 616], rax
                        mov              rax, qword ptr [rsp + 704]
                        mov              qword ptr [rsp + 624], rax
                        mov              rax, qword ptr [rsp + 712]
                        mov              qword ptr [rsp + 632], rax
                        .section         .rodata
.Lbynamefn54:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn54]
                        lea              rsi, [rsp + 608]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n16_lit_string_α
                                                                                        jmp   n62_lit_string_α
n53_call_β:
                                                                                        jmp   n16_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n54_call_α:
                        mov              rax, qword ptr [rsp + 800]
                        mov              qword ptr [rsp + 752], rax
                        mov              rax, qword ptr [rsp + 808]
                        mov              qword ptr [rsp + 760], rax
                        mov              rax, qword ptr [rsp + 848]
                        mov              qword ptr [rsp + 768], rax
                        mov              rax, qword ptr [rsp + 856]
                        mov              qword ptr [rsp + 776], rax
                        .section         .rodata
.Lbynamefn55:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn55]
                        lea              rsi, [rsp + 752]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                        cmp              eax, 99
                                                                                        je    n17_lit_integer_α
                                                                                        jmp   n63_lit_string_α
n54_call_β:
                                                                                        jmp   n17_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n55_call_α:
                        mov              rax, qword ptr [rsp + 944]
                        mov              qword ptr [rsp + 896], rax
                        mov              rax, qword ptr [rsp + 952]
                        mov              qword ptr [rsp + 904], rax
                        mov              rax, qword ptr [rsp + 992]
                        mov              qword ptr [rsp + 912], rax
                        mov              rax, qword ptr [rsp + 1000]
                        mov              qword ptr [rsp + 920], rax
                        .section         .rodata
.Lbynamefn56:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn56]
                        lea              rsi, [rsp + 896]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 880], rax
                        mov              qword ptr [rsp + 888], rdx
                        cmp              eax, 99
                                                                                        je    n18_lit_string_α
                                                                                        jmp   n64_lit_string_α
n55_call_β:
                                                                                        jmp   n18_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n56_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1040], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1048], rax
                        mov              rax, qword ptr [rsp + 1136]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1144]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lbynamefn57:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn57]
                        lea              rsi, [rsp + 1040]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        cmp              eax, 99
                                                                                        je    n19_lit_integer_α
                                                                                        jmp   n65_lit_string_α
n56_call_β:
                                                                                        jmp   n19_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_call_α:
                        mov              rax, qword ptr [rsp + 1232]
                        mov              qword ptr [rsp + 1184], rax
                        mov              rax, qword ptr [rsp + 1240]
                        mov              qword ptr [rsp + 1192], rax
                        mov              rax, qword ptr [rsp + 1280]
                        mov              qword ptr [rsp + 1200], rax
                        mov              rax, qword ptr [rsp + 1288]
                        mov              qword ptr [rsp + 1208], rax
                        .section         .rodata
.Lbynamefn58:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn58]
                        lea              rsi, [rsp + 1184]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n20_lit_string_α
                                                                                        jmp   n66_lit_string_α
n57_call_β:
                                                                                        jmp   n20_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 410/002: 3-2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n67_assign_α
.Lx134_0:
                        .quad            .Lx134_0_s
.Lx134_0_s:
                        .string          "FAIL 410/002: 3-2"
#=======================================================================================================================
#         output = 'FAIL 410/003: 3*2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n68_assign_α
.Lx135_0:
                        .quad            .Lx135_0_s
.Lx135_0_s:
                        .string          "FAIL 410/003: 3*2"
#=======================================================================================================================
#         output = 'FAIL 410/004: 5/2 integer division'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n69_assign_α
.Lx136_0:
                        .quad            .Lx136_0_s
.Lx136_0_s:
                        .string          "FAIL 410/004: 5/2 integer division"
#=======================================================================================================================
#         output = 'FAIL 410/005: 2**3'                  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 18
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_assign_α
.Lx137_0:
                        .quad            .Lx137_0_s
.Lx137_0_s:
                        .string          "FAIL 410/005: 2**3"
#=======================================================================================================================
#         output = 'FAIL 410/006: string+int coerce'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n71_assign_α
.Lx138_0:
                        .quad            .Lx138_0_s
.Lx138_0_s:
                        .string          "FAIL 410/006: string+int coerce"
#=======================================================================================================================
#         output = 'FAIL 410/007: int+neg-string'        :(end)
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n72_assign_α
.Lx139_0:
                        .quad            .Lx139_0_s
.Lx139_0_s:
                        .string          "FAIL 410/007: int+neg-string"
#=======================================================================================================================
#         output = 'FAIL 410/008: string+string coerce'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n73_assign_α
.Lx140_0:
                        .quad            .Lx140_0_s
.Lx140_0_s:
                        .string          "FAIL 410/008: string+string coerce"
#=======================================================================================================================
#         output = 'FAIL 410/009: null addend is zero'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n74_assign_α
.Lx141_0:
                        .quad            .Lx141_0_s
.Lx141_0_s:
                        .string          "FAIL 410/009: null addend is zero"
#-----------------------------------------------------------------------------------------------------------------------
n67_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n68_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n71_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n73_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n74_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 1336
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 1336
                        ret
                        .section         .note.GNU-stack,"",@progbits

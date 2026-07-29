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
                        sub              rsp, 920
                        mov              rdi, rsp
                        mov              ecx, 920
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 912], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         differ(2.0 + 3.0, 5.0)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_real_α:
                        mov              qword ptr [rbp + 80], 7
                        mov              rax, qword ptr [rip + .Lx52_0]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n8_lit_real_α
.Lx52_0:
                        .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n9_lit_real_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n10_lit_real_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n11_lit_real_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n12_lit_real_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   n13_lit_real_α
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_goto_α:
                                                                                        jmp   n14_lit_string_α
n6_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n7_goto_α:
                                                                                        jmp   main_γ
n7_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_real_α:
                        mov              qword ptr [rbp + 96], 7
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n15_binop_α
.Lx60_0:
                        .quad            4613937818241073152
#=======================================================================================================================
#         differ(3.0 - 1.0, 2.0)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_real_α:
                        mov              qword ptr [rbp + 240], 7
                        mov              rax, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n16_lit_real_α
.Lx61_0:
                        .quad            4613937818241073152
#=======================================================================================================================
#         differ(3.0 * 2.0, 6.0)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_real_α:
                        mov              qword ptr [rbp + 384], 7
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n17_lit_real_α
.Lx62_0:
                        .quad            4613937818241073152
#=======================================================================================================================
#         differ(3.0 / 2.0, 1.5)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_real_α:
                        mov              qword ptr [rbp + 528], 7
                        mov              rax, qword ptr [rip + .Lx63_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n18_lit_real_α
.Lx63_0:
                        .quad            4613937818241073152
#=======================================================================================================================
#         differ(3.0 ** 3, 27.0)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_real_α:
                        mov              qword ptr [rbp + 672], 7
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rbp + 680], rax
                                                                                        jmp   n19_lit_integer_α
.Lx64_0:
                        .quad            4613937818241073152
#=======================================================================================================================
#         differ(-1.0, 0.0 - 1.0)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_real_α:
                        mov              qword ptr [rbp + 816], 7
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   n20_unop_α
.Lx65_0:
                        .quad            4607182418800017408
#=======================================================================================================================
#         output = 'PASS 412_arith_real (6/6)'
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx66_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n21_assign_α
.Lx66_0:
                        .quad            .Lx66_0_s
.Lx66_0_s:
                        .string          "PASS 412_arith_real (6/6)"
#-----------------------------------------------------------------------------------------------------------------------
n15_binop_α:
                        mov              rdi, qword ptr [rbp + 80]
                        mov              rsi, qword ptr [rbp + 88]
                        mov              rdx, qword ptr [rbp + 96]
                        mov              rcx, qword ptr [rbp + 104]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n9_lit_real_α
                        mov              qword ptr [rbp + 64], rax
                        mov              qword ptr [rbp + 72], rdx
                                                                                        jmp   n22_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_real_α:
                        mov              qword ptr [rbp + 256], 7
                        mov              rax, qword ptr [rip + .Lx68_0]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n23_binop_α
.Lx68_0:
                        .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_real_α:
                        mov              qword ptr [rbp + 400], 7
                        mov              rax, qword ptr [rip + .Lx69_0]
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n24_binop_α
.Lx69_0:
                        .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_real_α:
                        mov              qword ptr [rbp + 544], 7
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n25_binop_α
.Lx70_0:
                        .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n26_binop_α
.Lx71_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n20_unop_α:
                        mov              rdi, qword ptr [rbp + 816]
                        mov              rsi, qword ptr [rbp + 824]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                                                                                        jmp   n27_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_real_α:
                        mov              qword ptr [rbp + 112], 7
                        mov              rax, qword ptr [rip + .Lx74_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n28_call_α
.Lx74_0:
                        .quad            4617315517961601024
#-----------------------------------------------------------------------------------------------------------------------
n23_binop_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        mov              rdx, qword ptr [rbp + 256]
                        mov              rcx, qword ptr [rbp + 264]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n10_lit_real_α
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                                                                                        jmp   n29_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n24_binop_α:
                        mov              rdi, qword ptr [rbp + 384]
                        mov              rsi, qword ptr [rbp + 392]
                        mov              rdx, qword ptr [rbp + 400]
                        mov              rcx, qword ptr [rbp + 408]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n11_lit_real_α
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n30_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n25_binop_α:
                        mov              rdi, qword ptr [rbp + 528]
                        mov              rsi, qword ptr [rbp + 536]
                        mov              rdx, qword ptr [rbp + 544]
                        mov              rcx, qword ptr [rbp + 552]
                        call             rt_div@PLT
                        cmp              eax, 99
                                                                                        je    n12_lit_real_α
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n31_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n26_binop_α:
                        mov              rdi, qword ptr [rbp + 672]
                        mov              rsi, qword ptr [rbp + 680]
                        mov              rdx, qword ptr [rbp + 688]
                        mov              rcx, qword ptr [rbp + 696]
                        call             rt_pow@PLT
                        cmp              eax, 99
                                                                                        je    n13_lit_real_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n32_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_real_α:
                        mov              qword ptr [rbp + 848], 7
                        mov              rax, qword ptr [rip + .Lx79_0]
                        mov              qword ptr [rbp + 856], rax
                                                                                        jmp   n33_lit_real_α
.Lx79_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n28_call_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              qword ptr [rbp + 16], rax
                        mov              rax, qword ptr [rbp + 72]
                        mov              qword ptr [rbp + 24], rax
                        mov              rax, qword ptr [rbp + 112]
                        mov              qword ptr [rbp + 32], rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              qword ptr [rbp + 40], rax
                        .section         .rodata
.Lbynamefn29:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn29]
                        lea              rsi, [rbp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n9_lit_real_α
                                                                                        jmp   n34_lit_string_α
n28_call_β:
                                                                                        jmp   n9_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_real_α:
                        mov              qword ptr [rbp + 272], 7
                        mov              rax, qword ptr [rip + .Lx81_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n35_call_α
.Lx81_0:
                        .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_real_α:
                        mov              qword ptr [rbp + 416], 7
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n36_call_α
.Lx82_0:
                        .quad            4618441417868443648
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_real_α:
                        mov              qword ptr [rbp + 560], 7
                        mov              rax, qword ptr [rip + .Lx83_0]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n37_call_α
.Lx83_0:
                        .quad            4609434218613702656
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_real_α:
                        mov              qword ptr [rbp + 704], 7
                        mov              rax, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rbp + 712], rax
                                                                                        jmp   n38_call_α
.Lx84_0:
                        .quad            4628293042053316608
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_real_α:
                        mov              qword ptr [rbp + 864], 7
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rbp + 872], rax
                                                                                        jmp   n39_binop_α
.Lx85_0:
                        .quad            4607182418800017408
#=======================================================================================================================
#         output = 'FAIL 412/001: 2.0+3.0'              :(end)
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n40_assign_α
.Lx86_0:
                        .quad            .Lx86_0_s
.Lx86_0_s:
                        .string          "FAIL 412/001: 2.0+3.0"
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 184], rax
                        mov              rax, qword ptr [rbp + 272]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 280]
                        mov              qword ptr [rbp + 200], rax
                        .section         .rodata
.Lbynamefn36:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn36]
                        lea              rsi, [rbp + 176]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                        cmp              eax, 99
                                                                                        je    n10_lit_real_α
                                                                                        jmp   n41_lit_string_α
n35_call_β:
                                                                                        jmp   n10_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n36_call_α:
                        mov              rax, qword ptr [rbp + 368]
                        mov              qword ptr [rbp + 320], rax
                        mov              rax, qword ptr [rbp + 376]
                        mov              qword ptr [rbp + 328], rax
                        mov              rax, qword ptr [rbp + 416]
                        mov              qword ptr [rbp + 336], rax
                        mov              rax, qword ptr [rbp + 424]
                        mov              qword ptr [rbp + 344], rax
                        .section         .rodata
.Lbynamefn37:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn37]
                        lea              rsi, [rbp + 320]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                        cmp              eax, 99
                                                                                        je    n11_lit_real_α
                                                                                        jmp   n42_lit_string_α
n36_call_β:
                                                                                        jmp   n11_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n37_call_α:
                        mov              rax, qword ptr [rbp + 512]
                        mov              qword ptr [rbp + 464], rax
                        mov              rax, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 472], rax
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 488], rax
                        .section         .rodata
.Lbynamefn38:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn38]
                        lea              rsi, [rbp + 464]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                        cmp              eax, 99
                                                                                        je    n12_lit_real_α
                                                                                        jmp   n43_lit_string_α
n37_call_β:
                                                                                        jmp   n12_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n38_call_α:
                        mov              rax, qword ptr [rbp + 656]
                        mov              qword ptr [rbp + 608], rax
                        mov              rax, qword ptr [rbp + 664]
                        mov              qword ptr [rbp + 616], rax
                        mov              rax, qword ptr [rbp + 704]
                        mov              qword ptr [rbp + 624], rax
                        mov              rax, qword ptr [rbp + 712]
                        mov              qword ptr [rbp + 632], rax
                        .section         .rodata
.Lbynamefn39:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn39]
                        lea              rsi, [rbp + 608]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n13_lit_real_α
                                                                                        jmp   n44_lit_string_α
n38_call_β:
                                                                                        jmp   n13_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n39_binop_α:
                        mov              rdi, qword ptr [rbp + 848]
                        mov              rsi, qword ptr [rbp + 856]
                        mov              rdx, qword ptr [rbp + 864]
                        mov              rcx, qword ptr [rbp + 872]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n14_lit_string_α
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                                                                                        jmp   n45_call_α
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 412/002: 3.0-1.0'              :(end)
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx93_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n46_assign_α
.Lx93_0:
                        .quad            .Lx93_0_s
.Lx93_0_s:
                        .string          "FAIL 412/002: 3.0-1.0"
#=======================================================================================================================
#         output = 'FAIL 412/003: 3.0*2.0'              :(end)
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx94_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n47_assign_α
.Lx94_0:
                        .quad            .Lx94_0_s
.Lx94_0_s:
                        .string          "FAIL 412/003: 3.0*2.0"
#=======================================================================================================================
#         output = 'FAIL 412/004: 3.0/2.0'              :(end)
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n48_assign_α
.Lx95_0:
                        .quad            .Lx95_0_s
.Lx95_0_s:
                        .string          "FAIL 412/004: 3.0/2.0"
#=======================================================================================================================
#         output = 'FAIL 412/005: 3.0**3'               :(end)
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n49_assign_α
.Lx96_0:
                        .quad            .Lx96_0_s
.Lx96_0_s:
                        .string          "FAIL 412/005: 3.0**3"
#-----------------------------------------------------------------------------------------------------------------------
n45_call_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              qword ptr [rbp + 752], rax
                        mov              rax, qword ptr [rbp + 808]
                        mov              qword ptr [rbp + 760], rax
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 768], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 776], rax
                        .section         .rodata
.Lbynamefn46:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn46]
                        lea              rsi, [rbp + 752]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 736], rax
                        mov              qword ptr [rbp + 744], rdx
                        cmp              eax, 99
                                                                                        je    n14_lit_string_α
                                                                                        jmp   n50_lit_string_α
n45_call_β:
                                                                                        jmp   n14_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n46_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n48_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 412/006: unary minus on real'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx102_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n51_assign_α
.Lx102_0:
                        .quad            .Lx102_0_s
.Lx102_0_s:
                        .string          "FAIL 412/006: unary minus on real"
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 912]
                        add              rsp, 920
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 912]
                        add              rsp, 920
                        ret
                        .section         .note.GNU-stack,"",@progbits

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
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_statement_end_α
n0_statement_begin_β:
                                                                                        jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:
                                                                                        jmp   n2_statement_begin_α
#=======================================================================================================================
#         differ(convert('12', 'integer'), 12)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_lit_string_α
n2_statement_begin_β:
                                                                                        jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_lit_string_α
.Lx71_0:
                        .quad            .Lx71_0_s
.Lx71_0_s:
                        .string          "12"
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx72_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_call_α
n4_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n2_statement_begin_β
.Lx72_0:
                        .quad            .Lx72_0_s
.Lx72_0_s:
                        .string          "integer"
#-----------------------------------------------------------------------------------------------------------------------
n5_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lbynamefnzd6:          .string          "convert"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd6]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx73_240
                        add              rsp, 16
                                                                                        jmp   n4_lit_string_β
.Lx73_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_lit_integer_α
n5_call_β:
                        add              rsp, 16
                                                                                        jmp   n4_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx74_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n7_call_α
n6_lit_integer_β:
                        add              rsp, 32
                                                                                        jmp   n4_lit_string_β
.Lx74_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lbynamefnzd8:          .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd8]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx75_240
                        add              rsp, 16
                                                                                        jmp   n6_lit_integer_β
.Lx75_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n8_statement_end_α
n7_call_β:
                        add              rsp, 16
                                                                                        jmp   n6_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n9_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 910/001: string->integer'        :(end)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:
                                                                                        jmp   n10_lit_string_α
n9_statement_begin_β:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 29
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n11_assign_α
.Lx80_0:
                        .quad            .Lx80_0_s
.Lx80_0_s:
                        .string          "FAIL 910/001: string->integer"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # output
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n12_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:
                                                                                        jmp   n14_statement_end_α
n13_statement_begin_β:
                                                                                        jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:
                                                                                        jmp   n15_statement_begin_α
#=======================================================================================================================
#         differ(convert(2.5, 'integer'), 2)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:
                                                                                        jmp   n16_lit_real_α
n15_statement_begin_β:
                                                                                        jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n17_lit_string_α
.Lx90_0:
                        .quad            4612811918334230528
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_call_α
n17_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n15_statement_begin_β
.Lx91_0:
                        .quad            .Lx91_0_s
.Lx91_0_s:
                        .string          "integer"
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lbynamefnzd19:         .string          "convert"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd19]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx92_240
                        add              rsp, 16
                                                                                        jmp   n17_lit_string_β
.Lx92_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n19_lit_integer_α
n18_call_β:
                        add              rsp, 16
                                                                                        jmp   n17_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx93_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n20_call_α
n19_lit_integer_β:
                        add              rsp, 32
                                                                                        jmp   n17_lit_string_β
.Lx93_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lbynamefnzd21:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd21]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx94_240
                        add              rsp, 16
                                                                                        jmp   n19_lit_integer_β
.Lx94_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_statement_end_α
n20_call_β:
                        add              rsp, 16
                                                                                        jmp   n19_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n22_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 910/002: real->integer truncation' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:
                                                                                        jmp   n23_lit_string_α
n22_statement_begin_β:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n24_assign_α
.Lx99_0:
                        .quad            .Lx99_0_s
.Lx99_0_s:
                        .string          "FAIL 910/002: real->integer truncation"
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # output
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n25_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:
                                                                                        jmp   n27_statement_end_α
n26_statement_begin_β:
                                                                                        jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:
                                                                                        jmp   n28_statement_begin_α
#=======================================================================================================================
#         differ(convert(2, 'real'), 2.0)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:
                                                                                        jmp   n29_lit_integer_α
n28_statement_begin_β:
                                                                                        jmp   n39_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n30_lit_string_α
.Lx109_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n31_call_α
n30_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n28_statement_begin_β
.Lx110_0:
                        .quad            .Lx110_0_s
.Lx110_0_s:
                        .string          "real"
#-----------------------------------------------------------------------------------------------------------------------
n31_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lbynamefnzd32:         .string          "convert"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd32]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx111_240
                        add              rsp, 16
                                                                                        jmp   n30_lit_string_β
.Lx111_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n32_lit_real_α
n31_call_β:
                        add              rsp, 16
                                                                                        jmp   n30_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx112_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n33_call_α
.Lx112_0:
                        .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n33_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lbynamefnzd34:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd34]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx113_240
                        add              rsp, 48
                                                                                        jmp   n30_lit_string_β
.Lx113_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n34_statement_end_α
n33_call_β:
                        add              rsp, 48
                                                                                        jmp   n30_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n35_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 910/003: integer->real'          :(end)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:
                                                                                        jmp   n36_lit_string_α
n35_statement_begin_β:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n37_assign_α
.Lx118_0:
                        .quad            .Lx118_0_s
.Lx118_0_s:
                        .string          "FAIL 910/003: integer->real"
#-----------------------------------------------------------------------------------------------------------------------
n37_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # output
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n38_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:
                                                                                        jmp   n40_statement_end_α
n39_statement_begin_β:
                                                                                        jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:
                                                                                        jmp   n41_statement_begin_α
#=======================================================================================================================
#         differ(convert('.2', 'real'), 0.2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:
                                                                                        jmp   n42_lit_string_α
n41_statement_begin_β:
                                                                                        jmp   n52_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx128_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n43_lit_string_α
.Lx128_0:
                        .quad            .Lx128_0_s
.Lx128_0_s:
                        .string          ".2"
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n44_call_α
n43_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n41_statement_begin_β
.Lx129_0:
                        .quad            .Lx129_0_s
.Lx129_0_s:
                        .string          "real"
#-----------------------------------------------------------------------------------------------------------------------
n44_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lbynamefnzd45:         .string          "convert"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd45]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx130_240
                        add              rsp, 16
                                                                                        jmp   n43_lit_string_β
.Lx130_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n45_lit_real_α
n44_call_β:
                        add              rsp, 16
                                                                                        jmp   n43_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n46_call_α
.Lx131_0:
                        .quad            4596373779694328218
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lbynamefnzd47:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd47]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx132_240
                        add              rsp, 48
                                                                                        jmp   n43_lit_string_β
.Lx132_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n47_statement_end_α
n46_call_β:
                        add              rsp, 48
                                                                                        jmp   n43_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n48_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 910/004: string->real'           :(end)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:
                                                                                        jmp   n49_lit_string_α
n48_statement_begin_β:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n50_assign_α
.Lx137_0:
                        .quad            .Lx137_0_s
.Lx137_0_s:
                        .string          "FAIL 910/004: string->real"
#-----------------------------------------------------------------------------------------------------------------------
n50_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # output
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n51_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:
                                                                                        jmp   n53_statement_end_α
n52_statement_begin_β:
                                                                                        jmp   n54_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:
                                                                                        jmp   n54_statement_begin_α
#=======================================================================================================================
#         output = 'PASS 910_convert (4/4)'
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:
                                                                                        jmp   n55_lit_string_α
n54_statement_begin_β:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 22
                        mov              rax, qword ptr [rip + .Lx147_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n56_assign_α
.Lx147_0:
                        .quad            .Lx147_0_s
.Lx147_0_s:
                        .string          "PASS 910_convert (4/4)"
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # output
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n57_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_begin_α:
                                                                                        jmp   n59_statement_end_α
n58_statement_begin_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n60_goto_α:
                                                                                        jmp   n13_statement_begin_α
n60_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n61_goto_α:
                                                                                        jmp   n26_statement_begin_α
n61_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n62_goto_α:
                                                                                        jmp   n39_statement_begin_α
n62_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n63_goto_α:
                                                                                        jmp   n52_statement_begin_α
n63_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n64_goto_α:
                                                                                        jmp   n58_statement_begin_α
n64_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              edi, 1
                        call             exit@PLT
                        .section         .note.GNU-stack,"",@progbits

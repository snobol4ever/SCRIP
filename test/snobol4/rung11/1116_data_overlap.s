                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "a"
.Lgvan1:                .string          "b"
.Lgvan2:                .string          "output"
.Lgvan3:                .string          "c"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 4
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 4
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
#         data('node(val,lson,rson)')
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_lit_string_α
n2_statement_begin_β:
                                                                                        jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 19
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_call_α
.Lx90_0:
                        .quad            .Lx90_0_s
.Lx90_0_s:
                        .string          "node(val,lson,rson)"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd5:          .string          "data"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd5]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
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
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx91_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n2_statement_begin_β
.Lx91_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n5_statement_end_α
n4_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n6_statement_begin_α
#=======================================================================================================================
#         data('clunk(value,lson)')
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:
                                                                                        jmp   n7_lit_string_α
n6_statement_begin_β:
                                                                                        jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n8_call_α
.Lx96_0:
                        .quad            .Lx96_0_s
.Lx96_0_s:
                        .string          "clunk(value,lson)"
#-----------------------------------------------------------------------------------------------------------------------
n8_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd9:          .string          "data"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd9]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
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
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx97_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n6_statement_begin_β
.Lx97_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n9_statement_end_α
n8_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n6_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n10_statement_begin_α
#=======================================================================================================================
#         data('clunk(value,lson)')
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:
                                                                                        jmp   n11_statement_end_α
n10_statement_begin_β:
                                                                                        jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:
                                                                                        jmp   n12_statement_begin_α
#=======================================================================================================================
#         a = node('x', 'y', 'z')
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:
                                                                                        jmp   n13_lit_string_α
n12_statement_begin_β:
                                                                                        jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx106_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_lit_string_α
.Lx106_0:
                        .quad            .Lx106_0_s
.Lx106_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n15_lit_string_α
n14_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n12_statement_begin_β
.Lx107_0:
                        .quad            .Lx107_0_s
.Lx107_0_s:
                        .string          "y"
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx108_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n16_call_α
n15_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n14_lit_string_β
.Lx108_0:
                        .quad            .Lx108_0_s
.Lx108_0_s:
                        .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n16_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lbynamefnzd17:         .string          "node"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd17]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
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
                        add              rsp, 48
                        cmp              eax, 104
                                                                                        jne   .Lx109_240
                        add              rsp, 16
                                                                                        jmp   n15_lit_string_β
.Lx109_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n17_assign_α
n16_call_β:
                        add              rsp, 16
                                                                                        jmp   n15_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # a
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n18_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n19_statement_begin_α
#=======================================================================================================================
#         b = node()
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_begin_α:
                                                                                        jmp   n20_call_α
n19_statement_begin_β:
                                                                                        jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
                        sub              rsp, 16
                        .section         .rodata
.Lbynamefnzd21:         .string          "node"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd21]
                        xor              esi, esi
                        mov              edx, 0
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
                        cmp              eax, 104
                                                                                        jne   .Lx115_240
                        add              rsp, 16
                                                                                        jmp   n19_statement_begin_β
.Lx115_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_assign_α
n20_call_β:
                        add              rsp, 16
                                                                                        jmp   n19_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # b
                        mov              qword ptr [r9 + 24], rdx
                                                                                        jmp   n22_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n23_statement_begin_α
#=======================================================================================================================
#         lson(b) = a
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:
                                                                                        jmp   n24_lit_string_α
n23_statement_begin_β:
                                                                                        jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n25_call_α
.Lx121_0:
                        .quad            .Lx121_0_s
.Lx121_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd123:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd123]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
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
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx122_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n23_statement_begin_β
.Lx122_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n26_var_α
n25_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n23_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n27_call_α
n26_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n23_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n27_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd28:         .string          "lson"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd28]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
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
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx125_240
                        add              rsp, 16
                                                                                        jmp   n26_var_β
.Lx125_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n28_var_α
n27_call_β:
                        add              rsp, 16
                                                                                        jmp   n26_var_β
#-----------------------------------------------------------------------------------------------------------------------
n28_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n29_assign_var_α
n28_var_β:
                        add              rsp, 32
                                                                                        jmp   n26_var_β
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104
                                                                                        jne   .Lx127_240
                        add              rsp, 16
                                                                                        jmp   n28_var_β
.Lx127_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n30_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n31_statement_begin_α
#=======================================================================================================================
#         differ(rson(lson(b)), 'z')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:
                                                                                        jmp   n32_var_α
n31_statement_begin_β:
                                                                                        jmp   n42_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n32_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n33_call_α
#-----------------------------------------------------------------------------------------------------------------------
n33_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd34:         .string          "lson"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd34]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
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
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx133_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n31_statement_begin_β
.Lx133_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n34_call_α
n33_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n31_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n34_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd35:         .string          "rson"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd35]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
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
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx134_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n31_statement_begin_β
.Lx134_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n35_lit_string_α
n34_call_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n31_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n36_call_α
n35_lit_string_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n31_statement_begin_β
.Lx135_0:
                        .quad            .Lx135_0_s
.Lx135_0_s:
                        .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n36_call_α:
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
.Lbynamefnzd37:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd37]
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
                                                                                        jne   .Lx136_240
                        add              rsp, 16
                                                                                        jmp   n35_lit_string_β
.Lx136_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n37_statement_end_α
n36_call_β:
                        add              rsp, 16
                                                                                        jmp   n35_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n38_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1116/001: node.rson after clunk data def' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_begin_α:
                                                                                        jmp   n39_lit_string_α
n38_statement_begin_β:
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n40_assign_α
.Lx141_0:
                        .quad            .Lx141_0_s
.Lx141_0_s:
                        .string          "FAIL 1116/001: node.rson after clunk data def"
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # output
                        mov              qword ptr [r9 + 40], rdx
                                                                                        jmp   n41_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_end_α:
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:
                                                                                        jmp   n43_statement_end_α
n42_statement_begin_β:
                                                                                        jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:
                                                                                        jmp   n44_statement_begin_α
#=======================================================================================================================
#         differ(value('b'), b)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:
                                                                                        jmp   n45_lit_string_α
n44_statement_begin_β:
                                                                                        jmp   n54_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx151_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n46_call_α
.Lx151_0:
                        .quad            .Lx151_0_s
.Lx151_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd47:         .string          "value"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd47]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
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
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx152_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n44_statement_begin_β
.Lx152_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n47_var_α
n46_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n44_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n47_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n48_call_α
n47_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n44_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n48_call_α:
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
.Lbynamefnzd49:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd49]
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
                                                                                        jne   .Lx154_240
                        add              rsp, 16
                                                                                        jmp   n47_var_β
.Lx154_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n49_statement_end_α
n48_call_β:
                        add              rsp, 16
                                                                                        jmp   n47_var_β
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n50_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1116/002: value() still works after clunk' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_begin_α:
                                                                                        jmp   n51_lit_string_α
n50_statement_begin_β:
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 46
                        mov              rax, qword ptr [rip + .Lx159_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n52_assign_α
.Lx159_0:
                        .quad            .Lx159_0_s
.Lx159_0_s:
                        .string          "FAIL 1116/002: value() still works after clunk"
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # output
                        mov              qword ptr [r9 + 40], rdx
                                                                                        jmp   n53_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:
                                                                                        jmp   n55_statement_end_α
n54_statement_begin_β:
                                                                                        jmp   n56_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_end_α:
                                                                                        jmp   n56_statement_begin_α
#=======================================================================================================================
#         c = clunk('alpha', 'beta')
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_begin_α:
                                                                                        jmp   n57_lit_string_α
n56_statement_begin_β:
                                                                                        jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n58_lit_string_α
.Lx169_0:
                        .quad            .Lx169_0_s
.Lx169_0_s:
                        .string          "alpha"
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx170_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n59_call_α
n58_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n56_statement_begin_β
.Lx170_0:
                        .quad            .Lx170_0_s
.Lx170_0_s:
                        .string          "beta"
#-----------------------------------------------------------------------------------------------------------------------
n59_call_α:
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
.Lbynamefnzd60:         .string          "clunk"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd60]
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
                                                                                        jne   .Lx171_240
                        add              rsp, 16
                                                                                        jmp   n58_lit_string_β
.Lx171_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n60_assign_α
n59_call_β:
                        add              rsp, 16
                                                                                        jmp   n58_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n61_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n62_statement_begin_α
#=======================================================================================================================
#         differ(lson(c), 'beta')                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_begin_α:
                                                                                        jmp   n63_var_α
n62_statement_begin_β:
                                                                                        jmp   n72_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n63_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n64_call_α
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd65:         .string          "lson"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd65]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
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
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx178_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n62_statement_begin_β
.Lx178_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n65_lit_string_α
n64_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n62_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n66_call_α
n65_lit_string_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n62_statement_begin_β
.Lx179_0:
                        .quad            .Lx179_0_s
.Lx179_0_s:
                        .string          "beta"
#-----------------------------------------------------------------------------------------------------------------------
n66_call_α:
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
.Lbynamefnzd67:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd67]
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
                                                                                        jne   .Lx180_240
                        add              rsp, 16
                                                                                        jmp   n65_lit_string_β
.Lx180_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_statement_end_α
n66_call_β:
                        add              rsp, 16
                                                                                        jmp   n65_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n68_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1116/003: clunk.lson accessor'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:
                                                                                        jmp   n69_lit_string_α
n68_statement_begin_β:
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_assign_α
.Lx185_0:
                        .quad            .Lx185_0_s
.Lx185_0_s:
                        .string          "FAIL 1116/003: clunk.lson accessor"
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # output
                        mov              qword ptr [r9 + 40], rdx
                                                                                        jmp   n71_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:
                                                                                        jmp   n73_statement_end_α
n72_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:
                                                                                        jmp   n74_statement_begin_α
#=======================================================================================================================
#         output = 'PASS 1116_data_overlap (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:
                                                                                        jmp   n75_lit_string_α
n74_statement_begin_β:
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n76_assign_α
.Lx195_0:
                        .quad            .Lx195_0_s
.Lx195_0_s:
                        .string          "PASS 1116_data_overlap (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n76_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # output
                        mov              qword ptr [r9 + 40], rdx
                                                                                        jmp   n77_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:
                                                                                        jmp   n79_statement_end_α
n78_statement_begin_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n80_goto_α:
                                                                                        jmp   n42_statement_begin_α
n80_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n81_goto_α:
                                                                                        jmp   n54_statement_begin_α
n81_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n82_goto_α:
                                                                                        jmp   n72_statement_begin_α
n82_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n83_goto_α:
                                                                                        jmp   n78_statement_begin_α
n83_goto_β:
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

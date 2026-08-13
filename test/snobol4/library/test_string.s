                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_α:
proc_LBL__pad_left_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_var_α
n0_statement_begin_β:
                                                                                        jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_call_α
#-----------------------------------------------------------------------------------------------------------------------
n2_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd34:             .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd34]
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
                                                                                        jne   .Lx33_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n0_statement_begin_β
.Lx33_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n3_lit_string_α
n2_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_binop_α
n3_lit_string_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n0_statement_begin_β
.Lx35_0:
                        .quad            .Lx35_0_s
.Lx35_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n4_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n5_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:
                                                                                        jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:
                                                                                        jmp   n8_var_α
n7_statement_begin_β:
                        add              rsp, 64
                                                                                        jmp   n18_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n9_call_α
n8_var_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n7_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd44:             .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd44]
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
                                                                                        jne   .Lx43_240
                        add              rsp, 16
                                                                                        jmp   n8_var_β
.Lx43_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n10_var_α
n9_call_β:
                        add              rsp, 16
                                                                                        jmp   n8_var_β
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n11_coerce_numeric_α
n10_var_β:
                        add              rsp, 32
                                                                                        jmp   n8_var_β
#-----------------------------------------------------------------------------------------------------------------------
n11_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # call
                        cmp              eax, 5
                                                                                        je    .Lx47_1
                        cmp              eax, 3
                                                                                        jne   .Lx47_0
                        mov              eax, dword ptr [rsp + 16]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx47_0
.Lx47_1:
                        mov              rax, qword ptr [rsp + 32]                      # call
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # call
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n12_coerce_numeric_α
.Lx47_0:
                        lea              rdi, [rsp + 32]                                # call
                        lea              rsi, [rsp + 16]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 109
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n12_coerce_numeric_α
n11_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n10_var_β
#-----------------------------------------------------------------------------------------------------------------------
n12_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx49_1
                        cmp              eax, 3
                                                                                        jne   .Lx49_0
                        mov              eax, dword ptr [rsp + 48]                      # call
                        cmp              eax, 3
                                                                                        jne   .Lx49_0
.Lx49_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n13_cmp_test_α
.Lx49_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 48]                                # call
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 110
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n13_cmp_test_α
n12_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n11_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n13_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_cmp_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax
                                                                                        jns   .Lx51_240
                        add              rsp, 16
                                                                                        jmp   n12_coerce_numeric_β
.Lx51_240:
                        mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0
                                                                                        jmp   n14_var_α
n13_cmp_test_β:
                        add              rsp, 16
                                                                                        jmp   n12_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n15_binop_α
n14_var_β:
                        add              rsp, 16
                                                                                        jmp   n13_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n15_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # cmp_test
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n16_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n17_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:
                                                                                        jmp   n29_RETURN_α
#=======================================================================================================================
#         startswith('foobar', 'bar')     :S(bad_sw2)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:
                                                                                        jmp   n19_var_α
n18_statement_begin_β:
                                                                                        jmp   n29_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n19_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n20_var_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_var_α
n20_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n18_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n22_call_α
n21_var_β:
                        add              rsp, 16
                                                                                        jmp   n20_var_β
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd63:             .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd63]
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
                                                                                        jne   .Lx62_240
                        add              rsp, 16
                                                                                        jmp   n21_var_β
.Lx62_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n23_binop_α
n22_call_β:
                        add              rsp, 16
                                                                                        jmp   n21_var_β
#-----------------------------------------------------------------------------------------------------------------------
n23_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx64_240
                        add              rsp, 32
                                                                                        jmp   n21_var_β
.Lx64_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n24_call_α
n23_binop_β:
                        add              rsp, 32
                                                                                        jmp   n21_var_β
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd66:             .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd66]
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
                                                                                        jne   .Lx65_240
                        add              rsp, 16
                                                                                        jmp   n23_binop_β
.Lx65_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n25_var_α
n24_call_β:
                        add              rsp, 16
                                                                                        jmp   n23_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n26_binop_α
n25_var_β:
                        add              rsp, 32
                                                                                        jmp   n23_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n26_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n27_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:
                        add              rsp, 128
                                                                                        jmp   n29_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n29_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_β:
                                                                                        jmp   proc_LBL__pad_left_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_α:
proc_LBL__pad_right_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:
                                                                                        jmp   n75_var_α
n74_statement_begin_β:
                                                                                        jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n76_call_α
#-----------------------------------------------------------------------------------------------------------------------
n76_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd108:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd108]
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
                                                                                        jne   .Lx107_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_β
.Lx107_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n77_lit_string_α
n76_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n78_binop_α
n77_lit_string_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n74_statement_begin_β
.Lx109_0:
                        .quad            .Lx109_0_s
.Lx109_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n78_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n79_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n80_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n81_statement_begin_α
#=======================================================================================================================
#         OUTPUT = index('foobar', 'xyz') ;* 0
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:
                                                                                        jmp   n82_var_α
n81_statement_begin_β:
                                                                                        jmp   n92_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n82_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n83_call_α
#-----------------------------------------------------------------------------------------------------------------------
n83_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd118:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd118]
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
                                                                                        jne   .Lx117_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n81_statement_begin_β
.Lx117_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n84_var_α
n83_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n81_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n84_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n85_coerce_numeric_α
n84_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n81_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n85_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # call
                        cmp              eax, 5
                                                                                        je    .Lx121_1
                        cmp              eax, 3
                                                                                        jne   .Lx121_0
                        mov              eax, dword ptr [rsp + 16]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx121_0
.Lx121_1:
                        mov              rax, qword ptr [rsp + 32]                      # call
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # call
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n86_coerce_numeric_α
.Lx121_0:
                        lea              rdi, [rsp + 32]                                # call
                        lea              rsi, [rsp + 16]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 109
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n86_coerce_numeric_α
n85_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n84_var_β
#-----------------------------------------------------------------------------------------------------------------------
n86_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx123_1
                        cmp              eax, 3
                                                                                        jne   .Lx123_0
                        mov              eax, dword ptr [rsp + 48]                      # call
                        cmp              eax, 3
                                                                                        jne   .Lx123_0
.Lx123_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n87_cmp_test_α
.Lx123_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 48]                                # call
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 110
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n87_cmp_test_α
n86_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n85_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n87_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_cmp_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax
                                                                                        jns   .Lx125_240
                        add              rsp, 16
                                                                                        jmp   n86_coerce_numeric_β
.Lx125_240:
                        mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0
                                                                                        jmp   n88_var_α
n87_cmp_test_β:
                        add              rsp, 16
                                                                                        jmp   n86_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n88_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n89_binop_α
n88_var_β:
                        add              rsp, 16
                                                                                        jmp   n87_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n89_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # cmp_test
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n90_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n90_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              qword ptr [r9 + 72], rdx
                                                                                        jmp   n91_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:
                                                                                        jmp   n103_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:
                                                                                        jmp   n93_var_α
n92_statement_begin_β:
                                                                                        jmp   n103_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 816], rax                     # result
                        mov              qword ptr [rsp + 824], rdx
                                                                                        jmp   n94_var_α
#-----------------------------------------------------------------------------------------------------------------------
n94_var_α:
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 896], rax                     # result
                        mov              qword ptr [rsp + 904], rdx
                                                                                        jmp   n95_var_α
#-----------------------------------------------------------------------------------------------------------------------
n95_var_α:
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 928], rax                     # result
                        mov              qword ptr [rsp + 936], rdx
                                                                                        jmp   n96_var_α
#-----------------------------------------------------------------------------------------------------------------------
n96_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 992], rax                     # result
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n97_call_α
#-----------------------------------------------------------------------------------------------------------------------
n97_call_α:
                        mov              rax, qword ptr [rsp + 992]
                        mov              qword ptr [rsp + 960], rax
                        mov              rax, qword ptr [rsp + 1000]
                        mov              qword ptr [rsp + 968], rax
                        .section         .rodata
.Lrkfn138:              .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn138]
                        lea              rsi, [rsp + 960]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                        cmp              eax, 104
                                                                                        je    n92_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n98_binop_α
n97_call_β:
                                                                                        jmp   n92_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n98_binop_α:
                        mov              eax, dword ptr [rsp + 928]
                        cmp              eax, 3
                                                                                        jne   .Lx139_0
                        mov              eax, dword ptr [rsp + 944]
                        cmp              eax, 3
                                                                                        jne   .Lx139_0
                        mov              rax, qword ptr [rsp + 936]
                        mov              rcx, qword ptr [rsp + 952]
                        sub              rax, rcx
                        mov              qword ptr [rsp + 912], 3
                        mov              qword ptr [rsp + 920], rax
                                                                                        jmp   n99_call_α
.Lx139_0:
                        mov              rdi, qword ptr [rsp + 928]
                        mov              rsi, qword ptr [rsp + 936]
                        mov              rdx, qword ptr [rsp + 944]
                        mov              rcx, qword ptr [rsp + 952]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        je    n92_statement_begin_β
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n99_call_α
#-----------------------------------------------------------------------------------------------------------------------
n99_call_α:
                        mov              rax, qword ptr [rsp + 912]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 920]
                        mov              qword ptr [rsp + 872], rax
                        mov              rax, qword ptr [rsp + 896]
                        mov              qword ptr [rsp + 848], rax
                        mov              rax, qword ptr [rsp + 904]
                        mov              qword ptr [rsp + 856], rax
                        .section         .rodata
.Lrkfn141:              .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn141]
                        lea              rsi, [rsp + 848]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                        cmp              eax, 104
                                                                                        je    n92_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n100_binop_α
n99_call_β:
                                                                                        jmp   n92_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n100_binop_α:
                        mov              rdi, qword ptr [rsp + 816]
                        mov              rsi, qword ptr [rsp + 824]
                        mov              rdx, qword ptr [rsp + 832]
                        mov              rcx, qword ptr [rsp + 840]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n101_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:
                        mov              rax, qword ptr [rsp + 800]
                        mov              rdx, qword ptr [rsp + 808]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              qword ptr [r9 + 72], rdx
                                                                                        jmp   n102_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:
                                                                                        jmp   n103_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n103_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_β:
                                                                                        jmp   proc_LBL__pad_right_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_α:
proc_LBL__ltrim_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_begin_α:
                                                                                        jmp   n149_lit_string_α
n148_statement_begin_β:
                                                                                        jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n149_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx182_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n150_lit_integer_α
n149_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n148_statement_begin_β
.Lx182_0:
                        .quad            .Lx182_0_s
.Lx182_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx183_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n151_call_α
n150_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n148_statement_begin_β
.Lx183_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n151_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd63:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd63]
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
                                                                                        jne   .Lx184_240
                        add              rsp, 16
                                                                                        jmp   n150_lit_integer_β
.Lx184_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n152_binop_α
n151_call_β:
                        add              rsp, 16
                                                                                        jmp   n150_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n152_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # lit_string
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n153_lit_integer_α
n152_binop_β:
                        add              rsp, 32
                                                                                        jmp   n150_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n153_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx186_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n154_call_α
n153_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n152_binop_β
.Lx186_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n154_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd66:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd66]
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
                                                                                        jne   .Lx187_240
                        add              rsp, 16
                                                                                        jmp   n153_lit_integer_β
.Lx187_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n155_binop_α
n154_call_β:
                        add              rsp, 16
                                                                                        jmp   n153_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n155_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n156_lit_integer_α
n155_binop_β:
                        add              rsp, 32
                                                                                        jmp   n153_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n156_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx189_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n157_call_α
n156_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n155_binop_β
.Lx189_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n157_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd69:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd69]
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
                                                                                        jne   .Lx190_240
                        add              rsp, 16
                                                                                        jmp   n156_lit_integer_β
.Lx190_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n158_binop_α
n157_call_β:
                        add              rsp, 16
                                                                                        jmp   n156_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n158_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n159_assign_α
n158_binop_β:
                        add              rsp, 32
                                                                                        jmp   n156_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n159_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              qword ptr [r9 + 104], rdx
                                                                                        jmp   n160_statement_end_α
n159_assign_β:
                                                                                        jmp   n148_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_end_α:
                                                                                        jmp   n161_statement_begin_α
n160_statement_end_β:
                                                                                        jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_begin_α:
                                                                                        jmp   n162_var_α
n161_statement_begin_β:
                                                                                        jmp   n175_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n162_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n163_match_begin_α
n162_var_β:
                        add              rsp, 16
                                                                                        jmp   n175_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n163_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 1504], rdi
                        mov              qword ptr [rsp + 1512], rsi
                        mov              qword ptr [rsp + 1336], rsp                    # old____
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx199_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n164_match_pos_α
n163_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx199_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx199_1
                                                                                        jmp   .Lx199_0
.Lx199_1:
n163_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n175_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n164_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n163_match_begin_β
                                                                                        jmp   n165_match_alternate_α
n164_match_pos_β:
                                                                                        jmp   n163_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n165_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx202_21]
                        mov              qword ptr [rsp + 16], rax
                                                                                        jmp   n174_match_span_α
.Lx202_21:
                        lea              rax, [rip + .Lx202_19]
                        mov              qword ptr [rsp + 16], rax
                                                                                        jmp   n173_match_lit_α
n165_match_alternate_s0:
                        lea              rax, [rip + .Lx202_40]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n165_match_alternate_as
n165_match_alternate_s1:
                        lea              rax, [rip + .Lx202_41]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n165_match_alternate_as
.Lx202_40:
                                                                                        jmp   n174_match_span_β
.Lx202_41:
                                                                                        jmp   n173_match_lit_β
n165_match_alternate_as:
                                                                                        jmp   n166_match_assign_save_α
n165_match_alternate_β:
                        mov              rax, qword ptr [rsp + 8]
                                                                                        jmp   rax
n165_match_alternate_af:
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16]
                                                                                        jmp   rax
.Lx202_19:
                        add              rsp, 32
                                                                                        jmp   n164_match_pos_β
#-----------------------------------------------------------------------------------------------------------------------
n166_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n167_match_rem_α
n166_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n165_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n167_match_rem_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d
                                                                                        jmp   n168_match_assign_cond_α
n167_match_rem_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n165_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n168_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n169_match_end_α
n168_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n167_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n169_match_end_α:
                        mov              r8, r12
.Lx209_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx209_9
                        mov              eax, dword ptr [rsp + 32]
                        mov              dword ptr [rsp + 1360], eax
                        mov              qword ptr [rsp + 1384], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx209_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx209_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx209_1:
                        test             rax, rax
                                                                                        je    .Lx209_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx209_3]
                        lea              rdx, [rip + .Lx209_4]
                                                                                        jmp   rax
.Lx209_3:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx209_1
.Lx209_4:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx209_1
.Lx209_2:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx209_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx209_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n170_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n170_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx210_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n171_match_replace_α
n170_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n175_statement_begin_α
.Lx210_0:
                        .quad            .Lx210_0_s
.Lx210_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n171_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx212_0]
                        mov              rsi, qword ptr [rsp + 1520]
                        mov              rdx, qword ptr [rsp + 1528]
                        mov              ecx, dword ptr [rsp + 1312]
                        mov              r8, qword ptr [rsp + 1336]
                        lea              r9, [rsp + 0]                                  # lit_string
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx212_1
.Lx212_0:
                        .quad            .Lx212_0_s
.Lx212_0_s:
                        .string          "s"
.Lx212_1:
                                                                                        jmp   n172_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:
                                                                                        jmp   n175_statement_begin_α
n172_statement_end_β:
                                                                                        jmp   n175_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n173_match_lit_α:
                                                                                        jmp   n165_match_alternate_s1
n173_match_lit_β:
                                                                                        jmp   n165_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n174_match_span_α:
                        lea              rdi, [rip + .S2]
                        lea              rsi, [rsp + 1440]
                        lea              rdx, [rsp + 1448]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_pat_prim_str@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        js    n165_match_alternate_af
                        mov              r8, qword ptr [rsp + 1440]
                        mov              r9d, dword ptr [rsp + 1448]
                        mov              dword ptr [rsp + 1440], 0
.Lx218_0:
                        mov              eax, r14d
                        add              eax, dword ptr [rsp + 1440]
                        cmp              eax, r15d
                                                                                        jge   .Lx218_1
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edx, 0
.Lx218_2:
                        cmp              edx, r9d
                                                                                        jge   .Lx218_1
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx218_3
                        add              edx, 1
                                                                                        jmp   .Lx218_2
.Lx218_3:
                        add              dword ptr [rsp + 1440], 1
                                                                                        jmp   .Lx218_0
.Lx218_1:
                        mov              eax, dword ptr [rsp + 1440]
                        test             eax, eax
                                                                                        jle   n165_match_alternate_af
                        mov              edx, r14d
                        mov              dword ptr [rsp + 1444], edx
                        add              edx, eax
                        mov              r14d, edx
                                                                                        jmp   n165_match_alternate_s0
n174_match_span_β:
                        mov              r14d, dword ptr [rsp + 1444]
                                                                                        jmp   n165_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_begin_α:
                                                                                        jmp   n176_var_α
n175_statement_begin_β:
                                                                                        jmp   n179_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n176_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n177_assign_α
n176_var_β:
                        add              rsp, 16
                        add              rsp, 224
                                                                                        jmp   n175_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n177_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax                       # ltrim
                        mov              qword ptr [r9 + 88], rdx
                                                                                        jmp   n178_statement_end_α
n177_assign_β:
                                                                                        jmp   n175_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n178_statement_end_α:
                                                                                        jmp   n179_RETURN_α
n178_statement_end_β:
                                                                                        jmp   n179_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n179_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_β:
                                                                                        jmp   proc_LBL__ltrim_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_α:
proc_LBL__rtrim_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_begin_α:
                                                                                        jmp   n228_lit_string_α
n227_statement_begin_β:
                                                                                        jmp   n240_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n228_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx283_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n229_lit_integer_α
.Lx283_0:
                        .quad            .Lx283_0_s
.Lx283_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n229_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx284_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n230_call_α
n229_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n227_statement_begin_β
.Lx284_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n230_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd94:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd94]
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
                                                                                        jne   .Lx285_240
                        add              rsp, 16
                                                                                        jmp   n229_lit_integer_β
.Lx285_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n231_binop_α
n230_call_β:
                        add              rsp, 16
                                                                                        jmp   n229_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n231_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # lit_string
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n232_lit_integer_α
n231_binop_β:
                        add              rsp, 32
                                                                                        jmp   n229_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n232_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx287_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n233_call_α
n232_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n231_binop_β
.Lx287_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n233_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd97:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd97]
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
                                                                                        jne   .Lx288_240
                        add              rsp, 16
                                                                                        jmp   n232_lit_integer_β
.Lx288_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n234_binop_α
n233_call_β:
                        add              rsp, 16
                                                                                        jmp   n232_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n234_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n235_lit_integer_α
n234_binop_β:
                        add              rsp, 32
                                                                                        jmp   n232_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n235_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n236_call_α
n235_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n234_binop_β
.Lx290_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n236_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd100:        .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd100]
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
                                                                                        jne   .Lx291_240
                        add              rsp, 16
                                                                                        jmp   n235_lit_integer_β
.Lx291_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n237_binop_α
n236_call_β:
                        add              rsp, 16
                                                                                        jmp   n235_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n237_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n238_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n238_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              qword ptr [r9 + 104], rdx
                                                                                        jmp   n239_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_end_α:
                                                                                        jmp   n240_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_begin_α:
                                                                                        jmp   n241_var_α
n240_statement_begin_β:
                                                                                        jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n241_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n242_call_α
n241_var_β:
                        add              rsp, 16
                        add              rsp, 160
                                                                                        jmp   n240_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n242_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd300:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd300]
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
                                                                                        jne   .Lx299_240
                        add              rsp, 16
                                                                                        jmp   n241_var_β
.Lx299_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n243_assign_α
n242_call_β:
                        add              rsp, 16
                                                                                        jmp   n241_var_β
#-----------------------------------------------------------------------------------------------------------------------
n243_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n244_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_end_α:
                                                                                        jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n245_statement_begin_α:
                                                                                        jmp   n246_var_α
n245_statement_begin_β:
                        add              rsp, 192
                                                                                        jmp   n252_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n246_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n247_lit_integer_α
n246_var_β:
                        add              rsp, 16
                        add              rsp, 192
                                                                                        jmp   n245_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n247_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n248_coerce_numeric_α
n247_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n246_var_β
.Lx307_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n248_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx309_1
                        cmp              eax, 3
                                                                                        jne   .Lx309_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx309_0
.Lx309_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n249_coerce_numeric_α
.Lx309_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 118
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n249_coerce_numeric_α
n248_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n247_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n249_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx311_1
                        cmp              eax, 3
                                                                                        jne   .Lx311_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx311_0
.Lx311_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n250_cmp_test_α
.Lx311_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 119
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n250_cmp_test_α
n249_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n248_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n250_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_cmp_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax
                                                                                        jle   .Lx313_240
                        add              rsp, 16
                                                                                        jmp   n249_coerce_numeric_β
.Lx313_240:
                                                                                        jmp   n251_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n251_statement_end_α:
                                                                                        jmp   n273_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n252_statement_begin_α:
                                                                                        jmp   n253_var_α
n252_statement_begin_β:
                                                                                        jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n253_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2016], rax                    # result
                        mov              qword ptr [rsp + 2024], rdx
                                                                                        jmp   n254_var_α
#-----------------------------------------------------------------------------------------------------------------------
n254_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2032], rax                    # result
                        mov              qword ptr [rsp + 2040], rdx
                                                                                        jmp   n255_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n255_lit_integer_α:
                        mov              qword ptr [rsp + 2048], 3                      # result
                        mov              rax, qword ptr [rip + .Lx320_0]
                        mov              qword ptr [rsp + 2056], rax
                                                                                        jmp   n256_call_α
.Lx320_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n256_call_α:
                        mov              rax, qword ptr [rsp + 2048]
                        mov              qword ptr [rsp + 1984], rax
                        mov              rax, qword ptr [rsp + 2056]
                        mov              qword ptr [rsp + 1992], rax
                        mov              rax, qword ptr [rsp + 2032]
                        mov              qword ptr [rsp + 1968], rax
                        mov              rax, qword ptr [rsp + 2040]
                        mov              qword ptr [rsp + 1976], rax
                        mov              rax, qword ptr [rsp + 2016]
                        mov              qword ptr [rsp + 1952], rax
                        mov              rax, qword ptr [rsp + 2024]
                        mov              qword ptr [rsp + 1960], rax
                        .section         .rodata
.Lrkfn322:              .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn322]
                        lea              rsi, [rsp + 1952]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                        cmp              eax, 104
                                                                                        je    n252_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n257_assign_α
n256_call_β:
                                                                                        jmp   n252_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n257_assign_α:
                        mov              rax, qword ptr [rsp + 1936]
                        mov              rdx, qword ptr [rsp + 1944]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              qword ptr [r9 + 168], rdx
                                                                                        jmp   n258_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_end_α:
                                                                                        jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_begin_α:
                                                                                        jmp   n260_var_α
n259_statement_begin_β:
                                                                                        jmp   n273_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n260_var_α:
                        mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 2192], rax                    # result
                        mov              qword ptr [rsp + 2200], rdx
                                                                                        jmp   n261_var_α
#-----------------------------------------------------------------------------------------------------------------------
n261_var_α:
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 2176], rax                    # result
                        mov              qword ptr [rsp + 2184], rdx
                                                                                        jmp   n262_coerce_string_α
#-----------------------------------------------------------------------------------------------------------------------
n262_coerce_string_α:
                        lea              rdi, [rsp + 2176]
                        lea              rsi, [rsp + 2160]
                        mov              rdx, 3866683
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n263_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n263_match_begin_α:
                        mov              qword ptr [rsp + 2120], rsp                    # old____
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rdi, qword ptr [rsp + 2192]
                        mov              rsi, qword ptr [rsp + 2200]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx333_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n264_match_any_α
n263_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx333_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx333_1
                                                                                        jmp   .Lx333_0
.Lx333_1:
n263_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n273_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n264_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n263_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rsp + 2168]
                        mov              r9d, dword ptr [rsp + 2164]
                        mov              edx, 0
.Lx335_5:
                        cmp              edx, r9d
                                                                                        jge   n263_match_begin_β
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx335_6
                        add              edx, 1
                                                                                        jmp   .Lx335_5
.Lx335_6:
                        add              r14d, 1
                                                                                        jmp   n265_match_end_α
n264_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n263_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n265_match_end_α:
                        mov              r8, r12
.Lx337_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx337_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx337_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx337_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx337_1:
                        test             rax, rax
                                                                                        je    .Lx337_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx337_3]
                        lea              rdx, [rip + .Lx337_4]
                                                                                        jmp   rax
.Lx337_3:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx337_1
.Lx337_4:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx337_1
.Lx337_2:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx337_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx337_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n266_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_end_α:
                                                                                        jmp   n267_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_statement_begin_α:
                                                                                        jmp   n268_var_α
n267_statement_begin_β:
                                                                                        jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n268_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2224], rax                    # result
                        mov              qword ptr [rsp + 2232], rdx
                                                                                        jmp   n269_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n269_lit_integer_α:
                        mov              qword ptr [rsp + 2240], 3                      # result
                        mov              rax, qword ptr [rip + .Lx343_0]
                        mov              qword ptr [rsp + 2248], rax
                                                                                        jmp   n270_binop_α
.Lx343_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n270_binop_α:
                        mov              eax, dword ptr [rsp + 2224]
                        cmp              eax, 3
                                                                                        jne   .Lx344_0
                        mov              rax, qword ptr [rsp + 2232]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2208], 3
                        mov              qword ptr [rsp + 2216], rax
                                                                                        jmp   n271_assign_α
.Lx344_0:
                        mov              rdi, qword ptr [rsp + 2224]
                        mov              rsi, qword ptr [rsp + 2232]
                        mov              rdx, qword ptr [rsp + 2240]
                        mov              rcx, qword ptr [rsp + 2248]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx344_240
                        add              rsp, 32
                                                                                        jmp   n267_statement_begin_β
.Lx344_240:
                        mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n271_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n271_assign_α:
                        mov              rax, qword ptr [rsp + 2208]
                        mov              rdx, qword ptr [rsp + 2216]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n272_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n272_statement_end_α:
                                                                                        jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_begin_α:
                                                                                        jmp   n274_var_α
n273_statement_begin_β:
                                                                                        jmp   n280_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n274_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n275_lit_integer_α
n274_var_β:
                        add              rsp, 16
                        add              rsp, 272
                                                                                        jmp   n273_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n275_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx351_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n276_var_α
n275_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n274_var_β
.Lx351_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n276_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n277_call_α
n276_var_β:
                        add              rsp, 16
                                                                                        jmp   n275_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n277_call_α:
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
.Lrkfnzd354:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd354]
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
                                                                                        jne   .Lx353_240
                        add              rsp, 16
                                                                                        jmp   n276_var_β
.Lx353_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n278_assign_α
n277_call_β:
                        add              rsp, 16
                                                                                        jmp   n276_var_β
#-----------------------------------------------------------------------------------------------------------------------
n278_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n279_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n279_statement_end_α:
                                                                                        jmp   n280_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n280_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_β:
                                                                                        jmp   proc_LBL__rtrim_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_α:
proc_LBL__trimws_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n360_statement_begin_α:
                                                                                        jmp   n361_var_α
n360_statement_begin_β:
                                                                                        jmp   n366_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n361_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n362_call_α
#-----------------------------------------------------------------------------------------------------------------------
n362_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # var
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx371_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + rtrim_alpha]
                                                                                        jmp   rax
.Lx371_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx371_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n360_statement_begin_β
.Lx371_240:
                                                                                        jmp   n363_call_α
n362_call_β:
                                                                                        jmp   n360_statement_begin_β
.Lx371_0:
                        .quad            .Lx371_0_s
.Lx371_0_s:
                        .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n363_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # call
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx373_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ltrim_alpha]
                                                                                        jmp   rax
.Lx373_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx373_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n360_statement_begin_β
.Lx373_240:
                                                                                        jmp   n364_assign_α
n363_call_β:
                                                                                        jmp   n360_statement_begin_β
.Lx373_0:
                        .quad            .Lx373_0_s
.Lx373_0_s:
                        .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n364_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 176], rax                      # trimws
                        mov              qword ptr [r9 + 184], rdx
                                                                                        jmp   n365_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n365_statement_end_α:
                                                                                        jmp   n366_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n366_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_β:
                                                                                        jmp   proc_LBL__trimws_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_α:
proc_LBL__repeat_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n379_statement_begin_α:
                                                                                        jmp   n380_var_α
n379_statement_begin_β:
                                                                                        jmp   n385_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n380_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n381_var_α
#-----------------------------------------------------------------------------------------------------------------------
n381_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n382_call_α
n381_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n379_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n382_call_α:
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
.Lrkfnzd391:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd391]
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
                                                                                        jne   .Lx390_240
                        add              rsp, 16
                                                                                        jmp   n381_var_β
.Lx390_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n383_assign_α
n382_call_β:
                        add              rsp, 16
                                                                                        jmp   n381_var_β
#-----------------------------------------------------------------------------------------------------------------------
n383_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax                      # repeat
                        mov              qword ptr [r9 + 200], rdx
                                                                                        jmp   n384_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n384_statement_end_α:
                                                                                        jmp   n385_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n385_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_β:
                                                                                        jmp   proc_LBL__repeat_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_α:
proc_LBL__contains_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n397_statement_begin_α:
                                                                                        jmp   n398_var_α
n397_statement_begin_β:
                                                                                        jmp   n409_FRETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n398_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n399_var_α
#-----------------------------------------------------------------------------------------------------------------------
n399_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n400_assign_α
n399_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n409_FRETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n400_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx414_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n401_var_α
n400_assign_β:
                                                                                        jmp   n399_var_β
.Lx414_0:
                        .quad            .Lx414_0_s
.Lx414_0_s:
                        .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n401_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n402_coerce_string_α
n401_var_β:
                        add              rsp, 16
                                                                                        jmp   n400_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n402_coerce_string_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 16]                                # var
                        lea              rsi, [rsp + 0]                                 # result
                        mov              rdx, 4522053
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n403_match_begin_α
n402_coerce_string_β:
                        add              rsp, 16
                                                                                        jmp   n401_var_β
#-----------------------------------------------------------------------------------------------------------------------
n403_match_begin_α:
                        mov              rdi, qword ptr [rsp + 48]                      # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 2800], rdi
                        mov              qword ptr [rsp + 2808], rsi
                        mov              qword ptr [rsp + 2696], rsp                    # old____
                        mov              qword ptr [rsp + 2704], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2712], r14                    # outer_δ
                        mov              qword ptr [rsp + 2720], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2728], rax                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rsp + 2672], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2656], 0                      # start_δ
.Lx419_0:
                        mov              r14d, dword ptr [rsp + 2656]
                                                                                        jmp   n404_match_break_α
n403_match_begin_β:
                        add              dword ptr [rsp + 2656], 1
                        mov              eax, dword ptr [rsp + 2656]
                        cmp              eax, r15d
                                                                                        jg    .Lx419_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx419_1
                                                                                        jmp   .Lx419_0
.Lx419_1:
n403_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2672]
                        mov              r13, qword ptr [rsp + 2704]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2712]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2720]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2728]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n402_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n404_match_break_α:
                        sub              rsp, 16
                        mov              edi, r14d
                        mov              rsi, qword ptr [rsp + 24]                      # coerce_string
                        mov              edx, dword ptr [rsp + 20]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sg_scan_nonmember@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, r15d
                                                                                        jl    .Lx421_240
                        add              rsp, 16
                                                                                        jmp   n403_match_begin_β
.Lx421_240:
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax
                                                                                        jmp   n405_match_defer_α
n404_match_break_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n403_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n405_match_defer_α:
                        lea              rdi, [rip + .S3]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        jz    .Lx422_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx422_4]
                        lea              r11, [rip + .Lx422_5]
                                                                                        jmp   rax
.Lx422_4:
                                                                                        jmp   n406_match_end_α
.Lx422_5:
                                                                                        jmp   n404_match_break_β
.Lx422_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx422_2:
                        test             rax, rax
                                                                                        je    .Lx422_3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx422_7]
                        lea              rdx, [rip + .Lx422_8]
                                                                                        jmp   rax
.Lx422_7:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx422_2
.Lx422_8:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx422_2
.Lx422_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax
                                                                                        js    n404_match_break_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx422_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n406_match_end_α
.Lx422_6:
                        add              rsp, 16
                                                                                        jmp   n404_match_break_β
n405_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n406_match_end_α:
                        mov              r8, r12
.Lx424_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx424_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx424_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx424_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx424_1:
                        test             rax, rax
                                                                                        je    .Lx424_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx424_3]
                        lea              rdx, [rip + .Lx424_4]
                                                                                        jmp   rax
.Lx424_3:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx424_1
.Lx424_4:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx424_1
.Lx424_2:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx424_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx424_10
                        mov              r13, qword ptr [rsp + 2704]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2712]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2720]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n407_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n407_statement_end_α:
                                                                                        jmp   n408_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n408_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
n409_FRETURN_α:
                        lea              rdi, [rip + .S4]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_β:
                                                                                        jmp   proc_LBL__contains_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_α:
proc_LBL__startswith_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n431_statement_begin_α:
                                                                                        jmp   n432_var_α
n431_statement_begin_β:
                                                                                        jmp   n441_FRETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n432_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n433_var_α
#-----------------------------------------------------------------------------------------------------------------------
n433_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n434_assign_α
n433_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n441_FRETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n434_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx446_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n435_match_begin_α
n434_assign_β:
                                                                                        jmp   n433_var_β
.Lx446_0:
                        .quad            .Lx446_0_s
.Lx446_0_s:
                        .string          "PATV$1"
#-----------------------------------------------------------------------------------------------------------------------
n435_match_begin_α:
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rsp + 2888], rsp                    # old____
                        mov              qword ptr [rsp + 2896], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2904], r14                    # outer_δ
                        mov              qword ptr [rsp + 2912], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2920], rax                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rsp + 2864], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2848], 0                      # start_δ
.Lx448_0:
                        mov              r14d, dword ptr [rsp + 2848]
                                                                                        jmp   n436_match_pos_α
n435_match_begin_β:
                        add              dword ptr [rsp + 2848], 1
                        mov              eax, dword ptr [rsp + 2848]
                        cmp              eax, r15d
                                                                                        jg    .Lx448_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx448_1
                                                                                        jmp   .Lx448_0
.Lx448_1:
n435_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2864]
                        mov              r13, qword ptr [rsp + 2896]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2904]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2912]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2920]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n434_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n436_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n435_match_begin_β
                                                                                        jmp   n437_match_defer_α
n436_match_pos_β:
                                                                                        jmp   n435_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n437_match_defer_α:
                        lea              rdi, [rip + .S5]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        jz    .Lx450_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx450_4]
                        lea              r11, [rip + .Lx450_5]
                                                                                        jmp   rax
.Lx450_4:
                                                                                        jmp   n438_match_end_α
.Lx450_5:
                                                                                        jmp   n435_match_begin_β
.Lx450_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx450_2:
                        test             rax, rax
                                                                                        je    .Lx450_3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx450_7]
                        lea              rdx, [rip + .Lx450_8]
                                                                                        jmp   rax
.Lx450_7:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx450_2
.Lx450_8:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx450_2
.Lx450_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax
                                                                                        js    n435_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx450_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n438_match_end_α
.Lx450_6:
                        add              rsp, 16
                                                                                        jmp   n435_match_begin_β
n437_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n438_match_end_α:
                        mov              r8, r12
.Lx452_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx452_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx452_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx452_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx452_1:
                        test             rax, rax
                                                                                        je    .Lx452_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx452_3]
                        lea              rdx, [rip + .Lx452_4]
                                                                                        jmp   rax
.Lx452_3:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx452_1
.Lx452_4:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx452_1
.Lx452_2:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx452_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx452_10
                        mov              r13, qword ptr [rsp + 2896]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2904]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2912]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n439_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n439_statement_end_α:
                                                                                        jmp   n440_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n440_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
n441_FRETURN_α:
                        lea              rdi, [rip + .S4]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_β:
                                                                                        jmp   proc_LBL__startswith_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_α:
proc_LBL__endswith_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n459_statement_begin_α:
                                                                                        jmp   n460_var_α
n459_statement_begin_β:
                                                                                        jmp   n469_FRETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n460_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n461_var_α
#-----------------------------------------------------------------------------------------------------------------------
n461_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n462_assign_α
n461_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n469_FRETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n462_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx474_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n463_match_begin_α
n462_assign_β:
                                                                                        jmp   n461_var_β
.Lx474_0:
                        .quad            .Lx474_0_s
.Lx474_0_s:
                        .string          "PATV$2"
#-----------------------------------------------------------------------------------------------------------------------
n463_match_begin_α:
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rsp + 3048], rsp                    # old____
                        mov              qword ptr [rsp + 3056], r13                    # outer_Σ
                        mov              qword ptr [rsp + 3064], r14                    # outer_δ
                        mov              qword ptr [rsp + 3072], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 3080], rax                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rsp + 3024], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 3008], 0                      # start_δ
.Lx476_0:
                        mov              r14d, dword ptr [rsp + 3008]
                                                                                        jmp   n464_match_defer_α
n463_match_begin_β:
                        add              dword ptr [rsp + 3008], 1
                        mov              eax, dword ptr [rsp + 3008]
                        cmp              eax, r15d
                                                                                        jg    .Lx476_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx476_1
                                                                                        jmp   .Lx476_0
.Lx476_1:
n463_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 3024]
                        mov              r13, qword ptr [rsp + 3056]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 3064]                    # outer_δ
                        mov              r15, qword ptr [rsp + 3072]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 3080]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n462_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n464_match_defer_α:
                        lea              rdi, [rip + .S6]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        jz    .Lx477_0
                        mov              r8d, 1
                        lea              r10, [rip + .Lx477_4]
                        lea              r11, [rip + .Lx477_5]
                                                                                        jmp   rax
.Lx477_4:
                        lea              rcx, [rip + g_scan_hit_start]
                        mov              rax, qword ptr [rcx]
                        mov              dword ptr [rsp + 3008], eax
                                                                                        jmp   n465_match_rpos_α
.Lx477_5:
                                                                                        jmp   n463_match_begin_β
.Lx477_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx477_2:
                        test             rax, rax
                                                                                        je    .Lx477_3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx477_7]
                        lea              rdx, [rip + .Lx477_8]
                                                                                        jmp   rax
.Lx477_7:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx477_2
.Lx477_8:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx477_2
.Lx477_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax
                                                                                        js    n463_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx477_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n465_match_rpos_α
.Lx477_6:
                        add              rsp, 16
                                                                                        jmp   n463_match_begin_β
n464_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n465_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n463_match_begin_β
                                                                                        jmp   n466_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n466_match_end_α:
                        mov              r8, r12
.Lx480_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx480_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx480_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx480_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx480_1:
                        test             rax, rax
                                                                                        je    .Lx480_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx480_3]
                        lea              rdx, [rip + .Lx480_4]
                                                                                        jmp   rax
.Lx480_3:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx480_1
.Lx480_4:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx480_1
.Lx480_2:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx480_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx480_10
                        mov              r13, qword ptr [rsp + 3056]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 3064]                    # outer_δ
                        mov              r15, qword ptr [rsp + 3072]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n467_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n467_statement_end_α:
                                                                                        jmp   n468_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n468_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
n469_FRETURN_α:
                        lea              rdi, [rip + .S4]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_β:
                                                                                        jmp   proc_LBL__endswith_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_α:
proc_LBL__index_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n487_statement_begin_α:
                                                                                        jmp   n488_lit_integer_α
n487_statement_begin_β:
                                                                                        jmp   n491_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n488_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx515_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n489_assign_α
.Lx515_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n489_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              qword ptr [r9 + 280], rdx
                                                                                        jmp   n490_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n490_statement_end_α:
                                                                                        jmp   n491_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n491_statement_begin_α:
                                                                                        jmp   n492_var_α
n491_statement_begin_β:
                                                                                        jmp   n495_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n492_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n493_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n493_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              qword ptr [r9 + 296], rdx
                                                                                        jmp   n494_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n494_statement_end_α:
                                                                                        jmp   n495_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n495_statement_begin_α:
                                                                                        jmp   n496_var_α
n495_statement_begin_β:
                                                                                        jmp   n512_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n496_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n497_var_α
#-----------------------------------------------------------------------------------------------------------------------
n497_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n498_coerce_string_α
#-----------------------------------------------------------------------------------------------------------------------
n498_coerce_string_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 16]                                # var
                        lea              rsi, [rsp + 0]                                 # result
                        mov              rdx, 4522053
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n499_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n499_match_begin_α:
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 3360], rdi
                        mov              qword ptr [rsp + 3368], rsi
                        mov              qword ptr [rsp + 3240], rsp                    # old____
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx532_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n500_match_assign_save_α
n499_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx532_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx532_1
                                                                                        jmp   .Lx532_0
.Lx532_1:
n499_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n512_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n500_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n501_match_break_α
n500_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n499_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n501_match_break_α:
                        sub              rsp, 16
                        mov              edi, r14d
                        mov              rsi, qword ptr [rsp + 72]                      # coerce_string
                        mov              edx, dword ptr [rsp + 68]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sg_scan_nonmember@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, r15d
                                                                                        jl    .Lx536_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n499_match_begin_β
.Lx536_240:
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax
                                                                                        jmp   n502_match_assign_cond_α
n501_match_break_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n499_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n502_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S7]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n503_match_end_α
n502_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n501_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n503_match_end_α:
                        mov              r8, r12
.Lx540_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx540_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx540_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx540_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx540_1:
                        test             rax, rax
                                                                                        je    .Lx540_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx540_3]
                        lea              rdx, [rip + .Lx540_4]
                                                                                        jmp   rax
.Lx540_3:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx540_1
.Lx540_4:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx540_1
.Lx540_2:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx540_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx540_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n504_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n504_statement_end_α:
                                                                                        jmp   n505_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n505_statement_begin_α:
                                                                                        jmp   n506_var_α
n505_statement_begin_β:
                                                                                        jmp   n512_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n506_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n507_call_α
n506_var_β:
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n505_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n507_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd547:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd547]
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
                                                                                        jne   .Lx546_240
                        add              rsp, 16
                                                                                        jmp   n506_var_β
.Lx546_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n508_lit_integer_α
n507_call_β:
                        add              rsp, 16
                                                                                        jmp   n506_var_β
#-----------------------------------------------------------------------------------------------------------------------
n508_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx548_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n509_binop_α
n508_lit_integer_β:
                        add              rsp, 32
                                                                                        jmp   n506_var_β
.Lx548_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n509_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx549_240
                        add              rsp, 16
                                                                                        jmp   n508_lit_integer_β
.Lx549_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n510_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n510_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              qword ptr [r9 + 280], rdx
                                                                                        jmp   n511_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n511_statement_end_α:
                                                                                        jmp   n512_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n512_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_β:
                                                                                        jmp   proc_LBL__index_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_pad_left_α
proc_pad_left_α:
proc_pad_left_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n555_save_restore_α:
                                                                                        jmp   n556_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n556_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__pad_left_α]
                                                                                        jmp   rax
                                                                                        jmp   n557_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n557_save_restore_α:
pad_left_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 48
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3
                                                                                        jbe   .Lx563_2
                        mov              eax, 48
.Lx563_2:
                        sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # pad_left
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx563_10
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx563_41
.Lx563_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx563_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx563_11
                        mov              rax, qword ptr [r8 + 96]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 96], rdx
                        mov              rax, qword ptr [r8 + 104]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 104], rdx
                                                                                        jmp   .Lx563_42
.Lx563_11:
                        mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx563_42:
                        cmp              rcx, 2
                                                                                        jbe   .Lx563_12
                        mov              rax, qword ptr [r8 + 112]
                        mov              rdx, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 112], rdx
                        mov              rax, qword ptr [r8 + 120]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 120], rdx
                                                                                        jmp   .Lx563_43
.Lx563_12:
                        mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx563_43:
                        lea              r10, [rip + pad_left_gamma]
                        lea              r11, [rip + pad_left_omega]
                        lea              rax, [rip + proc_LBL__pad_left_α]
                                                                                        jmp   rax
pad_left_gamma:
                        mov              rdi, qword ptr [r9 + 0]                        # pad_left
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3
                                                                                        jbe   .Lx563_75
                        mov              eax, 48
.Lx563_75:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx563_80
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx563_110
.Lx563_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx563_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx563_81
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   .Lx563_111
.Lx563_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx563_111:
                        cmp              rcx, 2
                                                                                        jbe   .Lx563_82
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax
                                                                                        jmp   .Lx563_112
.Lx563_82:
                        mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx563_112:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3
                                                                                        jbe   .Lx563_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx563_3:
                        mov              rcx, qword ptr [r8 + 64]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
pad_left_omega:
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3
                                                                                        jbe   .Lx563_145
                        mov              eax, 48
.Lx563_145:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx563_150
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx563_180
.Lx563_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx563_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx563_151
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   .Lx563_181
.Lx563_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx563_181:
                        cmp              rcx, 2
                                                                                        jbe   .Lx563_152
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax
                                                                                        jmp   .Lx563_182
.Lx563_152:
                        mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx563_182:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3
                                                                                        jbe   .Lx563_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx563_4:
                        mov              rcx, qword ptr [r8 + 72]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_pad_left_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_left_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_left_β:
                                                                                        jmp   proc_pad_left_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_left_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_left_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_pad_right_α
proc_pad_right_α:
proc_pad_right_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n564_save_restore_α:
                                                                                        jmp   n565_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n565_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__pad_right_α]
                                                                                        jmp   rax
                                                                                        jmp   n566_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n566_save_restore_α:
pad_right_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 48
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3
                                                                                        jbe   .Lx572_2
                        mov              eax, 48
.Lx572_2:
                        sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 64]                       # pad_right
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx572_10
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx572_41
.Lx572_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx572_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx572_11
                        mov              rax, qword ptr [r8 + 96]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 96], rdx
                        mov              rax, qword ptr [r8 + 104]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 104], rdx
                                                                                        jmp   .Lx572_42
.Lx572_11:
                        mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx572_42:
                        cmp              rcx, 2
                                                                                        jbe   .Lx572_12
                        mov              rax, qword ptr [r8 + 112]
                        mov              rdx, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 112], rdx
                        mov              rax, qword ptr [r8 + 120]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 120], rdx
                                                                                        jmp   .Lx572_43
.Lx572_12:
                        mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx572_43:
                        lea              r10, [rip + pad_right_gamma]
                        lea              r11, [rip + pad_right_omega]
                        lea              rax, [rip + proc_LBL__pad_right_α]
                                                                                        jmp   rax
pad_right_gamma:
                        mov              rdi, qword ptr [r9 + 64]                       # pad_right
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3
                                                                                        jbe   .Lx572_75
                        mov              eax, 48
.Lx572_75:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx572_80
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx572_110
.Lx572_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx572_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx572_81
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   .Lx572_111
.Lx572_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx572_111:
                        cmp              rcx, 2
                                                                                        jbe   .Lx572_82
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax
                                                                                        jmp   .Lx572_112
.Lx572_82:
                        mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx572_112:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3
                                                                                        jbe   .Lx572_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx572_3:
                        mov              rcx, qword ptr [r8 + 64]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
pad_right_omega:
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3
                                                                                        jbe   .Lx572_145
                        mov              eax, 48
.Lx572_145:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx572_150
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx572_180
.Lx572_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx572_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx572_151
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   .Lx572_181
.Lx572_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx572_181:
                        cmp              rcx, 2
                                                                                        jbe   .Lx572_152
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax
                                                                                        jmp   .Lx572_182
.Lx572_152:
                        mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx572_182:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3
                                                                                        jbe   .Lx572_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx572_4:
                        mov              rcx, qword ptr [r8 + 72]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_pad_right_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_right_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_right_β:
                                                                                        jmp   proc_pad_right_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_right_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_right_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_ltrim_α
proc_ltrim_α:
proc_ltrim_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n573_save_restore_α:
                                                                                        jmp   n574_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n574_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__ltrim_α]
                                                                                        jmp   rax
                                                                                        jmp   n575_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n575_save_restore_α:
ltrim_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx581_2
                        mov              eax, 16
.Lx581_2:
                        sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              rax, qword ptr [r9 + 80]                       # ltrim
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              qword ptr [rsp + 48], r10
                        mov              qword ptr [rsp + 56], r11
                        mov              qword ptr [rsp + 64], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx581_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx
                                                                                        jmp   .Lx581_41
.Lx581_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx581_41:
                        lea              r10, [rip + ltrim_gamma]
                        lea              r11, [rip + ltrim_omega]
                        lea              rax, [rip + proc_LBL__ltrim_α]
                                                                                        jmp   rax
ltrim_gamma:
                        mov              rdi, qword ptr [r9 + 80]                       # ltrim
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 64]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx581_75
                        mov              eax, 16
.Lx581_75:
                        lea              r8, [rsp + 80]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx581_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx581_110
.Lx581_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx581_110:
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              eax, 128
                        cmp              rcx, 1
                                                                                        jbe   .Lx581_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 112
.Lx581_3:
                        mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
ltrim_omega:
                        mov              rcx, qword ptr [rsp + 64]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx581_145
                        mov              eax, 16
.Lx581_145:
                        lea              r8, [rsp + 80]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 80], rax                       # ltrim
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx581_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx581_180
.Lx581_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx581_180:
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              eax, 128
                        cmp              rcx, 1
                                                                                        jbe   .Lx581_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 112
.Lx581_4:
                        mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_ltrim_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_ltrim_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_ltrim_β:
                                                                                        jmp   proc_ltrim_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_ltrim_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_ltrim_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_rtrim_α
proc_rtrim_α:
proc_rtrim_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n582_save_restore_α:
                                                                                        jmp   n583_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n583_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__rtrim_α]
                                                                                        jmp   rax
                                                                                        jmp   n584_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n584_save_restore_α:
rtrim_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx590_2
                        mov              eax, 16
.Lx590_2:
                        sub              rsp, 112
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 144], 0
                        mov              qword ptr [r9 + 152], 0
                        mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 160], 0
                        mov              qword ptr [r9 + 168], 0
                        mov              rax, qword ptr [r9 + 128]                      # rtrim
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
                        mov              qword ptr [rsp + 64], r10
                        mov              qword ptr [rsp + 72], r11
                        mov              qword ptr [rsp + 80], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx590_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx
                                                                                        jmp   .Lx590_41
.Lx590_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx590_41:
                        lea              r10, [rip + rtrim_gamma]
                        lea              r11, [rip + rtrim_omega]
                        lea              rax, [rip + proc_LBL__rtrim_α]
                                                                                        jmp   rax
rtrim_gamma:
                        mov              rdi, qword ptr [r9 + 128]                      # rtrim
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx590_75
                        mov              eax, 16
.Lx590_75:
                        lea              r8, [rsp + 96]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx590_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx590_110
.Lx590_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx590_110:
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 144
                        cmp              rcx, 1
                                                                                        jbe   .Lx590_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx590_3:
                        mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
rtrim_omega:
                        mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx590_145
                        mov              eax, 16
.Lx590_145:
                        lea              r8, [rsp + 96]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx590_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx590_180
.Lx590_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx590_180:
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 144
                        cmp              rcx, 1
                                                                                        jbe   .Lx590_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx590_4:
                        mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_rtrim_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_rtrim_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_rtrim_β:
                                                                                        jmp   proc_rtrim_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_rtrim_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_rtrim_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_trimws_α
proc_trimws_α:
proc_trimws_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n591_save_restore_α:
                                                                                        jmp   n592_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n592_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__trimws_α]
                                                                                        jmp   rax
                                                                                        jmp   n593_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n593_save_restore_α:
trimws_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx599_2
                        mov              eax, 16
.Lx599_2:
                        sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 176]                      # trimws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 176], 0
                        mov              qword ptr [r9 + 184], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx599_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx
                                                                                        jmp   .Lx599_41
.Lx599_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx599_41:
                        lea              r10, [rip + trimws_gamma]
                        lea              r11, [rip + trimws_omega]
                        lea              rax, [rip + proc_LBL__trimws_α]
                                                                                        jmp   rax
trimws_gamma:
                        mov              rdi, qword ptr [r9 + 176]                      # trimws
                        mov              rsi, qword ptr [r9 + 184]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx599_75
                        mov              eax, 16
.Lx599_75:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx599_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx599_110
.Lx599_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx599_110:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1
                                                                                        jbe   .Lx599_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx599_3:
                        mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
trimws_omega:
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx599_145
                        mov              eax, 16
.Lx599_145:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax                      # trimws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx599_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx599_180
.Lx599_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx599_180:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1
                                                                                        jbe   .Lx599_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx599_4:
                        mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_trimws_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_trimws_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_trimws_β:
                                                                                        jmp   proc_trimws_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_trimws_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_trimws_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_repeat_α
proc_repeat_α:
proc_repeat_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n600_save_restore_α:
                                                                                        jmp   n601_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n601_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__repeat_α]
                                                                                        jmp   rax
                                                                                        jmp   n602_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n602_save_restore_α:
repeat_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx608_2
                        mov              eax, 32
.Lx608_2:
                        sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 192]                      # repeat
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 192], 0
                        mov              qword ptr [r9 + 200], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx608_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx608_41
.Lx608_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx608_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx608_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx608_42
.Lx608_11:
                        mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx608_42:
                        lea              r10, [rip + repeat_gamma]
                        lea              r11, [rip + repeat_omega]
                        lea              rax, [rip + proc_LBL__repeat_α]
                                                                                        jmp   rax
repeat_gamma:
                        mov              rdi, qword ptr [r9 + 192]                      # repeat
                        mov              rsi, qword ptr [r9 + 200]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx608_75
                        mov              eax, 32
.Lx608_75:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx608_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx608_110
.Lx608_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx608_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx608_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   .Lx608_111
.Lx608_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx608_111:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx608_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx608_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
repeat_omega:
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx608_145
                        mov              eax, 32
.Lx608_145:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax                      # repeat
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx608_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx608_180
.Lx608_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx608_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx608_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   .Lx608_181
.Lx608_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx608_181:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx608_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx608_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_repeat_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_repeat_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_repeat_β:
                                                                                        jmp   proc_repeat_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_repeat_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_repeat_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_contains_α
proc_contains_α:
proc_contains_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n609_save_restore_α:
                                                                                        jmp   n610_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n610_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__contains_α]
                                                                                        jmp   rax
                                                                                        jmp   n611_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n611_save_restore_α:
contains_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx617_2
                        mov              eax, 32
.Lx617_2:
                        sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 208]                      # contains
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 208], 0
                        mov              qword ptr [r9 + 216], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx617_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx617_41
.Lx617_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx617_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx617_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx617_42
.Lx617_11:
                        mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx617_42:
                        lea              r10, [rip + contains_gamma]
                        lea              r11, [rip + contains_omega]
                        lea              rax, [rip + proc_LBL__contains_α]
                                                                                        jmp   rax
contains_gamma:
                        mov              rdi, qword ptr [r9 + 208]                      # contains
                        mov              rsi, qword ptr [r9 + 216]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx617_75
                        mov              eax, 32
.Lx617_75:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx617_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx617_110
.Lx617_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx617_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx617_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax
                                                                                        jmp   .Lx617_111
.Lx617_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx617_111:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx617_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx617_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
contains_omega:
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx617_145
                        mov              eax, 32
.Lx617_145:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax                      # contains
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx617_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx617_180
.Lx617_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx617_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx617_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax
                                                                                        jmp   .Lx617_181
.Lx617_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx617_181:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx617_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx617_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_contains_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_contains_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_contains_β:
                                                                                        jmp   proc_contains_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_contains_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_contains_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_startswith_α
proc_startswith_α:
proc_startswith_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n618_save_restore_α:
                                                                                        jmp   n619_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n619_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__startswith_α]
                                                                                        jmp   rax
                                                                                        jmp   n620_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n620_save_restore_α:
startswith_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx626_2
                        mov              eax, 32
.Lx626_2:
                        sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 240]                      # startswith
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 240], 0
                        mov              qword ptr [r9 + 248], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx626_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx626_41
.Lx626_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx626_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx626_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx626_42
.Lx626_11:
                        mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx626_42:
                        lea              r10, [rip + startswith_gamma]
                        lea              r11, [rip + startswith_omega]
                        lea              rax, [rip + proc_LBL__startswith_α]
                                                                                        jmp   rax
startswith_gamma:
                        mov              rdi, qword ptr [r9 + 240]                      # startswith
                        mov              rsi, qword ptr [r9 + 248]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx626_75
                        mov              eax, 32
.Lx626_75:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx626_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx626_110
.Lx626_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx626_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx626_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax
                                                                                        jmp   .Lx626_111
.Lx626_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx626_111:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx626_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx626_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
startswith_omega:
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx626_145
                        mov              eax, 32
.Lx626_145:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax                      # startswith
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx626_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx626_180
.Lx626_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx626_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx626_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax
                                                                                        jmp   .Lx626_181
.Lx626_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx626_181:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx626_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx626_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_startswith_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_startswith_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_startswith_β:
                                                                                        jmp   proc_startswith_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_startswith_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_startswith_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_endswith_α
proc_endswith_α:
proc_endswith_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n627_save_restore_α:
                                                                                        jmp   n628_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n628_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__endswith_α]
                                                                                        jmp   rax
                                                                                        jmp   n629_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n629_save_restore_α:
endswith_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx635_2
                        mov              eax, 32
.Lx635_2:
                        sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 256]                      # endswith
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 264]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 256], 0
                        mov              qword ptr [r9 + 264], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx635_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx635_41
.Lx635_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx635_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx635_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx635_42
.Lx635_11:
                        mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx635_42:
                        lea              r10, [rip + endswith_gamma]
                        lea              r11, [rip + endswith_omega]
                        lea              rax, [rip + proc_LBL__endswith_α]
                                                                                        jmp   rax
endswith_gamma:
                        mov              rdi, qword ptr [r9 + 256]                      # endswith
                        mov              rsi, qword ptr [r9 + 264]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx635_75
                        mov              eax, 32
.Lx635_75:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx635_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx635_110
.Lx635_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx635_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx635_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax
                                                                                        jmp   .Lx635_111
.Lx635_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx635_111:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx635_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx635_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
endswith_omega:
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx635_145
                        mov              eax, 32
.Lx635_145:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax                      # endswith
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx635_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx635_180
.Lx635_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx635_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx635_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax
                                                                                        jmp   .Lx635_181
.Lx635_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx635_181:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx635_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx635_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_endswith_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_endswith_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_endswith_β:
                                                                                        jmp   proc_endswith_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_endswith_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_endswith_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_index_α
proc_index_α:
proc_index_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n636_save_restore_α:
                                                                                        jmp   n637_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n637_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__index_α]
                                                                                        jmp   rax
                                                                                        jmp   n638_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n638_save_restore_α:
index_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx644_2
                        mov              eax, 32
.Lx644_2:
                        sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 288], 0
                        mov              qword ptr [r9 + 296], 0
                        mov              rax, qword ptr [r9 + 272]                      # index
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 280]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 272], 0
                        mov              qword ptr [r9 + 280], 0
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        mov              qword ptr [rsp + 48], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx644_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx644_41
.Lx644_10:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx644_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx644_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx644_42
.Lx644_11:
                        mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx644_42:
                        lea              r10, [rip + index_gamma]
                        lea              r11, [rip + index_omega]
                        lea              rax, [rip + proc_LBL__index_α]
                                                                                        jmp   rax
index_gamma:
                        mov              rdi, qword ptr [r9 + 272]                      # index
                        mov              rsi, qword ptr [r9 + 280]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx644_75
                        mov              eax, 32
.Lx644_75:
                        lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 272], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 296], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx644_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx644_110
.Lx644_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx644_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx644_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax
                                                                                        jmp   .Lx644_111
.Lx644_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx644_111:
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2
                                                                                        jbe   .Lx644_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx644_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
index_omega:
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx644_145
                        mov              eax, 32
.Lx644_145:
                        lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 296], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx644_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx644_180
.Lx644_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx644_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx644_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax
                                                                                        jmp   .Lx644_181
.Lx644_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx644_181:
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2
                                                                                        jbe   .Lx644_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx644_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
                                                                                        jmp   proc_index_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_index_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_index_β:
                                                                                        jmp   proc_index_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_index_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_index_ω:
                                                                                        jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__pad_left"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__pad_left_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__pad_right"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__pad_right_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__ltrim"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__ltrim_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__rtrim"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__rtrim_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "LBL__trimws"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_LBL__trimws_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "LBL__repeat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_LBL__repeat_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "LBL__contains"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + proc_LBL__contains_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname7:       .string          "LBL__startswith"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + proc_LBL__startswith_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "LBL__endswith"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + proc_LBL__endswith_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "LBL__index"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + proc_LBL__index_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 4816
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "pad_left"
.Lgvan1:                .string          "s"
.Lgvan2:                .string          "n"
.Lgvan3:                .string          "c"
.Lgvan4:                .string          "pad_right"
.Lgvan5:                .string          "ltrim"
.Lgvan6:                .string          "ws"
.Lgvan7:                .string          "r"
.Lgvan8:                .string          "rtrim"
.Lgvan9:                .string          "i"
.Lgvan10:               .string          "ch"
.Lgvan11:               .string          "trimws"
.Lgvan12:               .string          "repeat"
.Lgvan13:               .string          "contains"
.Lgvan14:               .string          "t"
.Lgvan15:               .string          "startswith"
.Lgvan16:               .string          "endswith"
.Lgvan17:               .string          "index"
.Lgvan18:               .string          "ix"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .quad            .Lgvan7
                        .quad            .Lgvan8
                        .quad            .Lgvan9
                        .quad            .Lgvan10
                        .quad            .Lgvan11
                        .quad            .Lgvan12
                        .quad            .Lgvan13
                        .quad            .Lgvan14
                        .quad            .Lgvan15
                        .quad            .Lgvan16
                        .quad            .Lgvan17
                        .quad            .Lgvan18
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 19
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 19
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        sub              rsp, 0
#-----------------------------------------------------------------------------------------------------------------------
n645_statement_begin_α:
                                                                                        jmp   n646_statement_end_α
n645_statement_begin_β:
                                                                                        jmp   n647_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n646_statement_end_α:
                                                                                        jmp   n647_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'endswith ok'          :(ew1)
#-----------------------------------------------------------------------------------------------------------------------
n647_statement_begin_α:
                                                                                        jmp   n648_func_activate_α
n647_statement_begin_β:
                                                                                        jmp   n650_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n648_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx878_0]
                        mov              rsi, qword ptr [rip + .Lx878_1]
                        mov              edx, 3
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + proc_pad_left_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n649_statement_end_α
n648_func_activate_β:
                                                                                        jmp   n647_statement_begin_β
.Lx878_0:
                        .quad            .Lx878_0_s
.Lx878_0_s:
                        .string          "pad_left"
.Lx878_1:
                        .quad            .Lx878_1_s
.Lx878_1_s:
                        .string          "s,n,c"
#-----------------------------------------------------------------------------------------------------------------------
n649_statement_end_α:
                                                                                        jmp   n650_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n650_statement_begin_α:
                                                                                        jmp   n651_statement_end_α
n650_statement_begin_β:
                                                                                        jmp   n652_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n651_statement_end_α:
                                                                                        jmp   n652_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n652_statement_begin_α:
                                                                                        jmp   n653_func_activate_α
n652_statement_begin_β:
                                                                                        jmp   n655_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n653_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx888_0]
                        mov              rsi, qword ptr [rip + .Lx888_1]
                        mov              edx, 3
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + proc_pad_right_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n654_statement_end_α
n653_func_activate_β:
                                                                                        jmp   n652_statement_begin_β
.Lx888_0:
                        .quad            .Lx888_0_s
.Lx888_0_s:
                        .string          "pad_right"
.Lx888_1:
                        .quad            .Lx888_1_s
.Lx888_1_s:
                        .string          "s,n,c"
#-----------------------------------------------------------------------------------------------------------------------
n654_statement_end_α:
                                                                                        jmp   n655_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n655_statement_begin_α:
                                                                                        jmp   n656_statement_end_α
n655_statement_begin_β:
                                                                                        jmp   n657_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n656_statement_end_α:
                                                                                        jmp   n657_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n657_statement_begin_α:
                                                                                        jmp   n658_func_activate_α
n657_statement_begin_β:
                                                                                        jmp   n660_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n658_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx898_0]
                        mov              rsi, qword ptr [rip + .Lx898_1]
                        mov              edx, 3
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_ltrim_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n659_statement_end_α
n658_func_activate_β:
                                                                                        jmp   n657_statement_begin_β
.Lx898_0:
                        .quad            .Lx898_0_s
.Lx898_0_s:
                        .string          "ltrim"
.Lx898_1:
                        .quad            .Lx898_1_s
.Lx898_1_s:
                        .string          "s,ws,r"
#-----------------------------------------------------------------------------------------------------------------------
n659_statement_end_α:
                                                                                        jmp   n660_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n660_statement_begin_α:
                                                                                        jmp   n661_statement_end_α
n660_statement_begin_β:
                                                                                        jmp   n662_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n661_statement_end_α:
                                                                                        jmp   n662_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n662_statement_begin_α:
                                                                                        jmp   n663_func_activate_α
n662_statement_begin_β:
                                                                                        jmp   n665_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n663_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx908_0]
                        mov              rsi, qword ptr [rip + .Lx908_1]
                        mov              edx, 4
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_rtrim_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n664_statement_end_α
n663_func_activate_β:
                                                                                        jmp   n662_statement_begin_β
.Lx908_0:
                        .quad            .Lx908_0_s
.Lx908_0_s:
                        .string          "rtrim"
.Lx908_1:
                        .quad            .Lx908_1_s
.Lx908_1_s:
                        .string          "s,ws,i,ch"
#-----------------------------------------------------------------------------------------------------------------------
n664_statement_end_α:
                                                                                        jmp   n665_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n665_statement_begin_α:
                                                                                        jmp   n666_statement_end_α
n665_statement_begin_β:
                                                                                        jmp   n667_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n666_statement_end_α:
                                                                                        jmp   n667_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n667_statement_begin_α:
                                                                                        jmp   n668_func_activate_α
n667_statement_begin_β:
                                                                                        jmp   n670_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n668_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx918_0]
                        mov              rsi, qword ptr [rip + .Lx918_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_trimws_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n669_statement_end_α
n668_func_activate_β:
                                                                                        jmp   n667_statement_begin_β
.Lx918_0:
                        .quad            .Lx918_0_s
.Lx918_0_s:
                        .string          "trimws"
.Lx918_1:
                        .quad            .Lx918_1_s
.Lx918_1_s:
                        .string          "s"
#-----------------------------------------------------------------------------------------------------------------------
n669_statement_end_α:
                                                                                        jmp   n670_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n670_statement_begin_α:
                                                                                        jmp   n671_statement_end_α
n670_statement_begin_β:
                                                                                        jmp   n672_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n671_statement_end_α:
                                                                                        jmp   n672_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n672_statement_begin_α:
                                                                                        jmp   n673_func_activate_α
n672_statement_begin_β:
                                                                                        jmp   n675_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n673_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx928_0]
                        mov              rsi, qword ptr [rip + .Lx928_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_repeat_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n674_statement_end_α
n673_func_activate_β:
                                                                                        jmp   n672_statement_begin_β
.Lx928_0:
                        .quad            .Lx928_0_s
.Lx928_0_s:
                        .string          "repeat"
.Lx928_1:
                        .quad            .Lx928_1_s
.Lx928_1_s:
                        .string          "s,n"
#-----------------------------------------------------------------------------------------------------------------------
n674_statement_end_α:
                                                                                        jmp   n675_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n675_statement_begin_α:
                                                                                        jmp   n676_statement_end_α
n675_statement_begin_β:
                                                                                        jmp   n677_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n676_statement_end_α:
                                                                                        jmp   n677_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n677_statement_begin_α:
                                                                                        jmp   n678_func_activate_α
n677_statement_begin_β:
                                                                                        jmp   n680_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n678_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx938_0]
                        mov              rsi, qword ptr [rip + .Lx938_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_contains_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n679_statement_end_α
n678_func_activate_β:
                                                                                        jmp   n677_statement_begin_β
.Lx938_0:
                        .quad            .Lx938_0_s
.Lx938_0_s:
                        .string          "contains"
.Lx938_1:
                        .quad            .Lx938_1_s
.Lx938_1_s:
                        .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n679_statement_end_α:
                                                                                        jmp   n680_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n680_statement_begin_α:
                                                                                        jmp   n681_statement_end_α
n680_statement_begin_β:
                                                                                        jmp   n682_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n681_statement_end_α:
                                                                                        jmp   n682_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n682_statement_begin_α:
                                                                                        jmp   n683_func_activate_α
n682_statement_begin_β:
                                                                                        jmp   n685_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n683_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx948_0]
                        mov              rsi, qword ptr [rip + .Lx948_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_startswith_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n684_statement_end_α
n683_func_activate_β:
                                                                                        jmp   n682_statement_begin_β
.Lx948_0:
                        .quad            .Lx948_0_s
.Lx948_0_s:
                        .string          "startswith"
.Lx948_1:
                        .quad            .Lx948_1_s
.Lx948_1_s:
                        .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n684_statement_end_α:
                                                                                        jmp   n685_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n685_statement_begin_α:
                                                                                        jmp   n686_statement_end_α
n685_statement_begin_β:
                                                                                        jmp   n687_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n686_statement_end_α:
                                                                                        jmp   n687_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n687_statement_begin_α:
                                                                                        jmp   n688_func_activate_α
n687_statement_begin_β:
                                                                                        jmp   n690_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n688_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx958_0]
                        mov              rsi, qword ptr [rip + .Lx958_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_endswith_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n689_statement_end_α
n688_func_activate_β:
                                                                                        jmp   n687_statement_begin_β
.Lx958_0:
                        .quad            .Lx958_0_s
.Lx958_0_s:
                        .string          "endswith"
.Lx958_1:
                        .quad            .Lx958_1_s
.Lx958_1_s:
                        .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n689_statement_end_α:
                                                                                        jmp   n690_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n690_statement_begin_α:
                                                                                        jmp   n691_statement_end_α
n690_statement_begin_β:
                                                                                        jmp   n692_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n691_statement_end_α:
                                                                                        jmp   n692_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n692_statement_begin_α:
                                                                                        jmp   n693_func_activate_α
n692_statement_begin_β:
                                                                                        jmp   n695_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n693_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx968_0]
                        mov              rsi, qword ptr [rip + .Lx968_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_index_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n694_statement_end_α
n693_func_activate_β:
                                                                                        jmp   n692_statement_begin_β
.Lx968_0:
                        .quad            .Lx968_0_s
.Lx968_0_s:
                        .string          "index"
.Lx968_1:
                        .quad            .Lx968_1_s
.Lx968_1_s:
                        .string          "s,t,ix"
#-----------------------------------------------------------------------------------------------------------------------
n694_statement_end_α:
                                                                                        jmp   n695_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n695_statement_begin_α:
                                                                                        jmp   n696_statement_end_α
n695_statement_begin_β:
                                                                                        jmp   n697_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n696_statement_end_α:
                                                                                        jmp   n697_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n697_statement_begin_α:
                                                                                        jmp   n698_statement_end_α
n697_statement_begin_β:
                                                                                        jmp   n699_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n698_statement_end_α:
                                                                                        jmp   n699_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n699_statement_begin_α:
                                                                                        jmp   n700_lit_string_α
n699_statement_begin_β:
                                                                                        jmp   n704_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n700_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx981_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n701_lit_integer_α
.Lx981_0:
                        .quad            .Lx981_0_s
.Lx981_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n701_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx982_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n702_call_α
n701_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n699_statement_begin_β
.Lx982_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n702_call_α:
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
.Lrkfnzd984:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd984]
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
                                                                                        jne   .Lx983_240
                        add              rsp, 16
                                                                                        jmp   n701_lit_integer_β
.Lx983_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n703_statement_end_α
n702_call_β:
                        add              rsp, 16
                                                                                        jmp   n701_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n703_statement_end_α:
                                                                                        jmp   n704_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n704_statement_begin_α:
                                                                                        jmp   n705_statement_end_α
n704_statement_begin_β:
                                                                                        jmp   n706_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n705_statement_end_α:
                                                                                        jmp   n706_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n706_statement_begin_α:
                                                                                        jmp   n707_lit_string_α
n706_statement_begin_β:
                                                                                        jmp   n713_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n707_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx993_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n708_lit_integer_α
n707_lit_string_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n706_statement_begin_β
.Lx993_0:
                        .quad            .Lx993_0_s
.Lx993_0_s:
                        .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n708_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx994_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n709_lit_string_α
n708_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n707_lit_string_β
.Lx994_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n709_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx995_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n710_call_α
n709_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n708_lit_integer_β
.Lx995_0:
                        .quad            .Lx995_0_s
.Lx995_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n710_call_α:
                        sub              rsp, 16
                        sub              rsp, 80
                        mov              rax, qword ptr [rsp + 128]                     # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 112]                     # lit_integer
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                        mov              eax, 3
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx997_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + pad_left_alpha]
                                                                                        jmp   rax
.Lx997_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx997_240
                        add              rsp, 16
                                                                                        jmp   n709_lit_string_β
.Lx997_240:
                                                                                        jmp   n711_assign_α
n710_call_β:
                                                                                        jmp   n709_lit_string_β
.Lx997_0:
                        .quad            .Lx997_0_s
.Lx997_0_s:
                        .string          "pad_left"
#-----------------------------------------------------------------------------------------------------------------------
n711_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx998_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n712_statement_end_α
.Lx998_0:
                        .quad            .Lx998_0_s
.Lx998_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n712_statement_end_α:
                                                                                        jmp   n713_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n713_statement_begin_α:
                                                                                        jmp   n714_lit_string_α
n713_statement_begin_β:
                                                                                        jmp   n720_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n714_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1003_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n715_lit_integer_α
n714_lit_string_β:
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n713_statement_begin_β
.Lx1003_0:
                        .quad            .Lx1003_0_s
.Lx1003_0_s:
                        .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n715_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1004_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n716_lit_string_α
n715_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n714_lit_string_β
.Lx1004_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n716_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1005_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n717_call_α
n716_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n715_lit_integer_β
.Lx1005_0:
                        .quad            .Lx1005_0_s
.Lx1005_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n717_call_α:
                        sub              rsp, 16
                        sub              rsp, 80
                        mov              rax, qword ptr [rsp + 128]                     # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 112]                     # lit_integer
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                        mov              eax, 3
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1007_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + pad_right_alpha]
                                                                                        jmp   rax
.Lx1007_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1007_240
                        add              rsp, 16
                                                                                        jmp   n716_lit_string_β
.Lx1007_240:
                                                                                        jmp   n718_assign_α
n717_call_β:
                                                                                        jmp   n716_lit_string_β
.Lx1007_0:
                        .quad            .Lx1007_0_s
.Lx1007_0_s:
                        .string          "pad_right"
#-----------------------------------------------------------------------------------------------------------------------
n718_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1008_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n719_statement_end_α
.Lx1008_0:
                        .quad            .Lx1008_0_s
.Lx1008_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n719_statement_end_α:
                                                                                        jmp   n720_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n720_statement_begin_α:
                                                                                        jmp   n721_lit_string_α
n720_statement_begin_β:
                                                                                        jmp   n725_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n721_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx1013_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n722_call_α
n721_lit_string_β:
                        add              rsp, 16
                        add              rsp, 176
                                                                                        jmp   n720_statement_begin_β
.Lx1013_0:
                        .quad            .Lx1013_0_s
.Lx1013_0_s:
                        .string          "   hello"
#-----------------------------------------------------------------------------------------------------------------------
n722_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1015_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ltrim_alpha]
                                                                                        jmp   rax
.Lx1015_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1015_240
                        add              rsp, 16
                                                                                        jmp   n721_lit_string_β
.Lx1015_240:
                                                                                        jmp   n723_assign_α
n722_call_β:
                                                                                        jmp   n721_lit_string_β
.Lx1015_0:
                        .quad            .Lx1015_0_s
.Lx1015_0_s:
                        .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n723_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1016_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n724_statement_end_α
.Lx1016_0:
                        .quad            .Lx1016_0_s
.Lx1016_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n724_statement_end_α:
                                                                                        jmp   n725_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n725_statement_begin_α:
                                                                                        jmp   n726_lit_string_α
n725_statement_begin_β:
                                                                                        jmp   n730_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n726_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx1021_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n727_call_α
n726_lit_string_β:
                        add              rsp, 16
                        add              rsp, 208
                                                                                        jmp   n725_statement_begin_β
.Lx1021_0:
                        .quad            .Lx1021_0_s
.Lx1021_0_s:
                        .string          "hello   "
#-----------------------------------------------------------------------------------------------------------------------
n727_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1023_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + rtrim_alpha]
                                                                                        jmp   rax
.Lx1023_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1023_240
                        add              rsp, 16
                                                                                        jmp   n726_lit_string_β
.Lx1023_240:
                                                                                        jmp   n728_assign_α
n727_call_β:
                                                                                        jmp   n726_lit_string_β
.Lx1023_0:
                        .quad            .Lx1023_0_s
.Lx1023_0_s:
                        .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n728_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1024_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n729_statement_end_α
.Lx1024_0:
                        .quad            .Lx1024_0_s
.Lx1024_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n729_statement_end_α:
                                                                                        jmp   n730_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n730_statement_begin_α:
                                                                                        jmp   n731_lit_string_α
n730_statement_begin_β:
                                                                                        jmp   n735_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n731_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 9
                        mov              rax, qword ptr [rip + .Lx1029_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n732_call_α
n731_lit_string_β:
                        add              rsp, 16
                        add              rsp, 240
                                                                                        jmp   n730_statement_begin_β
.Lx1029_0:
                        .quad            .Lx1029_0_s
.Lx1029_0_s:
                        .string          "  hello  "
#-----------------------------------------------------------------------------------------------------------------------
n732_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1031_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + trimws_alpha]
                                                                                        jmp   rax
.Lx1031_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1031_240
                        add              rsp, 16
                                                                                        jmp   n731_lit_string_β
.Lx1031_240:
                                                                                        jmp   n733_assign_α
n732_call_β:
                                                                                        jmp   n731_lit_string_β
.Lx1031_0:
                        .quad            .Lx1031_0_s
.Lx1031_0_s:
                        .string          "trimws"
#-----------------------------------------------------------------------------------------------------------------------
n733_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1032_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n734_statement_end_α
.Lx1032_0:
                        .quad            .Lx1032_0_s
.Lx1032_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n734_statement_end_α:
                                                                                        jmp   n735_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n735_statement_begin_α:
                                                                                        jmp   n736_lit_string_α
n735_statement_begin_β:
                                                                                        jmp   n741_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n736_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1037_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n737_lit_integer_α
n736_lit_string_β:
                        add              rsp, 16
                        add              rsp, 272
                                                                                        jmp   n735_statement_begin_β
.Lx1037_0:
                        .quad            .Lx1037_0_s
.Lx1037_0_s:
                        .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n737_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1038_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n738_call_α
n737_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n736_lit_string_β
.Lx1038_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n738_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]                      # lit_integer
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1040_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + repeat_alpha]
                                                                                        jmp   rax
.Lx1040_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1040_240
                        add              rsp, 16
                                                                                        jmp   n737_lit_integer_β
.Lx1040_240:
                                                                                        jmp   n739_assign_α
n738_call_β:
                                                                                        jmp   n737_lit_integer_β
.Lx1040_0:
                        .quad            .Lx1040_0_s
.Lx1040_0_s:
                        .string          "repeat"
#-----------------------------------------------------------------------------------------------------------------------
n739_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1041_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n740_statement_end_α
.Lx1041_0:
                        .quad            .Lx1041_0_s
.Lx1041_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n740_statement_end_α:
                                                                                        jmp   n741_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n741_statement_begin_α:
                                                                                        jmp   n742_statement_end_α
n741_statement_begin_β:
                                                                                        jmp   n743_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n742_statement_end_α:
                                                                                        jmp   n743_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n743_statement_begin_α:
                                                                                        jmp   n744_lit_string_α
n743_statement_begin_β:
                        add              rsp, 320
                                                                                        jmp   n752_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n744_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1050_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n745_lit_string_α
n744_lit_string_β:
                        add              rsp, 16
                        add              rsp, 320
                                                                                        jmp   n743_statement_begin_β
.Lx1050_0:
                        .quad            .Lx1050_0_s
.Lx1050_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n745_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1051_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n746_call_α
n745_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n744_lit_string_β
.Lx1051_0:
                        .quad            .Lx1051_0_s
.Lx1051_0_s:
                        .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n746_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1053_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + contains_alpha]
                                                                                        jmp   rax
.Lx1053_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1053_240
                        add              rsp, 16
                                                                                        jmp   n745_lit_string_β
.Lx1053_240:
                                                                                        jmp   n747_statement_end_α
n746_call_β:
                                                                                        jmp   n745_lit_string_β
.Lx1053_0:
                        .quad            .Lx1053_0_s
.Lx1053_0_s:
                        .string          "contains"
#-----------------------------------------------------------------------------------------------------------------------
n747_statement_end_α:
                                                                                        jmp   n748_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n748_statement_begin_α:
                                                                                        jmp   n749_lit_string_α
n748_statement_begin_β:
                                                                                        jmp   n756_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n749_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx1058_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n750_assign_α
.Lx1058_0:
                        .quad            .Lx1058_0_s
.Lx1058_0_s:
                        .string          "contains ok"
#-----------------------------------------------------------------------------------------------------------------------
n750_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1059_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n751_statement_end_α
.Lx1059_0:
                        .quad            .Lx1059_0_s
.Lx1059_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n751_statement_end_α:
                                                                                        jmp   n756_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n752_statement_begin_α:
                                                                                        jmp   n753_lit_string_α
n752_statement_begin_β:
                                                                                        jmp   n756_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n753_lit_string_α:
                        mov              qword ptr [rsp + 4224], 2                      # result
                        mov              dword ptr [rsp + 4228], 14
                        mov              rax, qword ptr [rip + .Lx1064_0]
                        mov              qword ptr [rsp + 4232], rax
                                                                                        jmp   n754_assign_α
.Lx1064_0:
                        .quad            .Lx1064_0_s
.Lx1064_0_s:
                        .string          "FAIL: contains"
#-----------------------------------------------------------------------------------------------------------------------
n754_assign_α:
                        mov              rsi, qword ptr [rsp + 4224]
                        mov              rdx, qword ptr [rsp + 4232]
                        mov              rdi, qword ptr [rip + .Lx1065_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n755_statement_end_α
.Lx1065_0:
                        .quad            .Lx1065_0_s
.Lx1065_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n755_statement_end_α:
                                                                                        jmp   n756_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n756_statement_begin_α:
                                                                                        jmp   n757_statement_end_α
n756_statement_begin_β:
                                                                                        jmp   n758_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n757_statement_end_α:
                                                                                        jmp   n758_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n758_statement_begin_α:
                                                                                        jmp   n759_lit_string_α
n758_statement_begin_β:
                        add              rsp, 384
                                                                                        jmp   n767_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n759_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1074_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n760_lit_string_α
n759_lit_string_β:
                        add              rsp, 16
                        add              rsp, 384
                                                                                        jmp   n758_statement_begin_β
.Lx1074_0:
                        .quad            .Lx1074_0_s
.Lx1074_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n760_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1075_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n761_call_α
n760_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n759_lit_string_β
.Lx1075_0:
                        .quad            .Lx1075_0_s
.Lx1075_0_s:
                        .string          "foo"
#-----------------------------------------------------------------------------------------------------------------------
n761_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1077_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + startswith_alpha]
                                                                                        jmp   rax
.Lx1077_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1077_240
                        add              rsp, 16
                                                                                        jmp   n760_lit_string_β
.Lx1077_240:
                                                                                        jmp   n762_statement_end_α
n761_call_β:
                                                                                        jmp   n760_lit_string_β
.Lx1077_0:
                        .quad            .Lx1077_0_s
.Lx1077_0_s:
                        .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n762_statement_end_α:
                                                                                        jmp   n763_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n763_statement_begin_α:
                                                                                        jmp   n764_lit_string_α
n763_statement_begin_β:
                                                                                        jmp   n771_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n764_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx1082_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n765_assign_α
.Lx1082_0:
                        .quad            .Lx1082_0_s
.Lx1082_0_s:
                        .string          "startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n765_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1083_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n766_statement_end_α
.Lx1083_0:
                        .quad            .Lx1083_0_s
.Lx1083_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n766_statement_end_α:
                                                                                        jmp   n771_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n767_statement_begin_α:
                                                                                        jmp   n768_lit_string_α
n767_statement_begin_β:
                                                                                        jmp   n771_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n768_lit_string_α:
                        mov              qword ptr [rsp + 4352], 2                      # result
                        mov              dword ptr [rsp + 4356], 16
                        mov              rax, qword ptr [rip + .Lx1088_0]
                        mov              qword ptr [rsp + 4360], rax
                                                                                        jmp   n769_assign_α
.Lx1088_0:
                        .quad            .Lx1088_0_s
.Lx1088_0_s:
                        .string          "FAIL: startswith"
#-----------------------------------------------------------------------------------------------------------------------
n769_assign_α:
                        mov              rsi, qword ptr [rsp + 4352]
                        mov              rdx, qword ptr [rsp + 4360]
                        mov              rdi, qword ptr [rip + .Lx1089_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n770_statement_end_α
.Lx1089_0:
                        .quad            .Lx1089_0_s
.Lx1089_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n770_statement_end_α:
                                                                                        jmp   n771_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n771_statement_begin_α:
                                                                                        jmp   n772_statement_end_α
n771_statement_begin_β:
                                                                                        jmp   n773_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n772_statement_end_α:
                                                                                        jmp   n773_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n773_statement_begin_α:
                                                                                        jmp   n774_lit_string_α
n773_statement_begin_β:
                        add              rsp, 448
                                                                                        jmp   n782_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n774_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1098_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n775_lit_string_α
n774_lit_string_β:
                        add              rsp, 16
                        add              rsp, 448
                                                                                        jmp   n773_statement_begin_β
.Lx1098_0:
                        .quad            .Lx1098_0_s
.Lx1098_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n775_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1099_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n776_call_α
n775_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n774_lit_string_β
.Lx1099_0:
                        .quad            .Lx1099_0_s
.Lx1099_0_s:
                        .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n776_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1101_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + endswith_alpha]
                                                                                        jmp   rax
.Lx1101_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1101_240
                        add              rsp, 16
                                                                                        jmp   n775_lit_string_β
.Lx1101_240:
                                                                                        jmp   n777_statement_end_α
n776_call_β:
                                                                                        jmp   n775_lit_string_β
.Lx1101_0:
                        .quad            .Lx1101_0_s
.Lx1101_0_s:
                        .string          "endswith"
#-----------------------------------------------------------------------------------------------------------------------
n777_statement_end_α:
                                                                                        jmp   n778_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n778_statement_begin_α:
                                                                                        jmp   n779_lit_string_α
n778_statement_begin_β:
                                                                                        jmp   n786_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n779_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx1106_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n780_assign_α
.Lx1106_0:
                        .quad            .Lx1106_0_s
.Lx1106_0_s:
                        .string          "endswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n780_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1107_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n781_statement_end_α
.Lx1107_0:
                        .quad            .Lx1107_0_s
.Lx1107_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n781_statement_end_α:
                                                                                        jmp   n786_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n782_statement_begin_α:
                                                                                        jmp   n783_lit_string_α
n782_statement_begin_β:
                                                                                        jmp   n786_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n783_lit_string_α:
                        mov              qword ptr [rsp + 4480], 2                      # result
                        mov              dword ptr [rsp + 4484], 14
                        mov              rax, qword ptr [rip + .Lx1112_0]
                        mov              qword ptr [rsp + 4488], rax
                                                                                        jmp   n784_assign_α
.Lx1112_0:
                        .quad            .Lx1112_0_s
.Lx1112_0_s:
                        .string          "FAIL: endswith"
#-----------------------------------------------------------------------------------------------------------------------
n784_assign_α:
                        mov              rsi, qword ptr [rsp + 4480]
                        mov              rdx, qword ptr [rsp + 4488]
                        mov              rdi, qword ptr [rip + .Lx1113_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n785_statement_end_α
.Lx1113_0:
                        .quad            .Lx1113_0_s
.Lx1113_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n785_statement_end_α:
                                                                                        jmp   n786_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n786_statement_begin_α:
                                                                                        jmp   n787_statement_end_α
n786_statement_begin_β:
                                                                                        jmp   n788_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n787_statement_end_α:
                                                                                        jmp   n788_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n788_statement_begin_α:
                                                                                        jmp   n789_lit_string_α
n788_statement_begin_β:
                        add              rsp, 512
                                                                                        jmp   n797_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n789_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1122_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n790_lit_string_α
n789_lit_string_β:
                        add              rsp, 16
                        add              rsp, 512
                                                                                        jmp   n788_statement_begin_β
.Lx1122_0:
                        .quad            .Lx1122_0_s
.Lx1122_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n790_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1123_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n791_call_α
n790_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n789_lit_string_β
.Lx1123_0:
                        .quad            .Lx1123_0_s
.Lx1123_0_s:
                        .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n791_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1125_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + startswith_alpha]
                                                                                        jmp   rax
.Lx1125_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1125_240
                        add              rsp, 16
                                                                                        jmp   n790_lit_string_β
.Lx1125_240:
                                                                                        jmp   n792_statement_end_α
n791_call_β:
                                                                                        jmp   n790_lit_string_β
.Lx1125_0:
                        .quad            .Lx1125_0_s
.Lx1125_0_s:
                        .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n792_statement_end_α:
                                                                                        jmp   n793_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n793_statement_begin_α:
                                                                                        jmp   n794_lit_string_α
n793_statement_begin_β:
                                                                                        jmp   n801_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n794_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx1130_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n795_assign_α
.Lx1130_0:
                        .quad            .Lx1130_0_s
.Lx1130_0_s:
                        .string          "FAIL: startswith matched wrong"
#-----------------------------------------------------------------------------------------------------------------------
n795_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1131_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n796_statement_end_α
.Lx1131_0:
                        .quad            .Lx1131_0_s
.Lx1131_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n796_statement_end_α:
                                                                                        jmp   n801_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n797_statement_begin_α:
                                                                                        jmp   n798_lit_string_α
n797_statement_begin_β:
                                                                                        jmp   n801_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n798_lit_string_α:
                        mov              qword ptr [rsp + 4592], 2                      # result
                        mov              dword ptr [rsp + 4596], 16
                        mov              rax, qword ptr [rip + .Lx1136_0]
                        mov              qword ptr [rsp + 4600], rax
                                                                                        jmp   n799_assign_α
.Lx1136_0:
                        .quad            .Lx1136_0_s
.Lx1136_0_s:
                        .string          "no startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n799_assign_α:
                        mov              rsi, qword ptr [rsp + 4592]
                        mov              rdx, qword ptr [rsp + 4600]
                        mov              rdi, qword ptr [rip + .Lx1137_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n800_statement_end_α
.Lx1137_0:
                        .quad            .Lx1137_0_s
.Lx1137_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n800_statement_end_α:
                                                                                        jmp   n801_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n801_statement_begin_α:
                                                                                        jmp   n802_statement_end_α
n801_statement_begin_β:
                                                                                        jmp   n803_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n802_statement_end_α:
                                                                                        jmp   n803_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n803_statement_begin_α:
                                                                                        jmp   n804_lit_string_α
n803_statement_begin_β:
                                                                                        jmp   n809_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n804_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1146_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n805_lit_string_α
n804_lit_string_β:
                        add              rsp, 16
                        add              rsp, 576
                                                                                        jmp   n803_statement_begin_β
.Lx1146_0:
                        .quad            .Lx1146_0_s
.Lx1146_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n805_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1147_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n806_call_α
n805_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n804_lit_string_β
.Lx1147_0:
                        .quad            .Lx1147_0_s
.Lx1147_0_s:
                        .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n806_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1149_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + index_alpha]
                                                                                        jmp   rax
.Lx1149_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1149_240
                        add              rsp, 16
                                                                                        jmp   n805_lit_string_β
.Lx1149_240:
                                                                                        jmp   n807_assign_α
n806_call_β:
                                                                                        jmp   n805_lit_string_β
.Lx1149_0:
                        .quad            .Lx1149_0_s
.Lx1149_0_s:
                        .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n807_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1150_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n808_statement_end_α
.Lx1150_0:
                        .quad            .Lx1150_0_s
.Lx1150_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n808_statement_end_α:
                                                                                        jmp   n809_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n809_statement_begin_α:
                                                                                        jmp   n810_lit_string_α
n809_statement_begin_β:
                        add              rsp, 624
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n810_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1155_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n811_lit_string_α
n810_lit_string_β:
                        add              rsp, 16
                        add              rsp, 624
                                                                                        jmp   n809_statement_begin_β
.Lx1155_0:
                        .quad            .Lx1155_0_s
.Lx1155_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n811_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1156_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n812_call_α
n811_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n810_lit_string_β
.Lx1156_0:
                        .quad            .Lx1156_0_s
.Lx1156_0_s:
                        .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
n812_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1158_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + index_alpha]
                                                                                        jmp   rax
.Lx1158_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1158_240
                        add              rsp, 16
                                                                                        jmp   n811_lit_string_β
.Lx1158_240:
                                                                                        jmp   n813_assign_α
n812_call_β:
                                                                                        jmp   n811_lit_string_β
.Lx1158_0:
                        .quad            .Lx1158_0_s
.Lx1158_0_s:
                        .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n813_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1159_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n814_statement_end_α
.Lx1159_0:
                        .quad            .Lx1159_0_s
.Lx1159_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n814_statement_end_α:
                        add              rsp, 672
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n815_goto_α:
                                                                                        jmp   n650_statement_begin_α
n815_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n816_goto_α:
                                                                                        jmp   n655_statement_begin_α
n816_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n817_goto_α:
                                                                                        jmp   n660_statement_begin_α
n817_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n818_goto_α:
                                                                                        jmp   n819_statement_begin_α
n818_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n819_statement_begin_α:
                                                                                        jmp   n820_var_α
n819_statement_begin_β:
                                                                                        jmp   n826_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n820_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1920], rax                    # result
                        mov              qword ptr [rsp + 1928], rdx
                                                                                        jmp   n821_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n821_lit_integer_α:
                        mov              qword ptr [rsp + 1904], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1169_0]
                        mov              qword ptr [rsp + 1912], rax
                                                                                        jmp   n822_coerce_numeric_α
.Lx1169_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n822_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 1920]
                        cmp              eax, 5
                                                                                        je    .Lx1171_1
                        cmp              eax, 3
                                                                                        jne   .Lx1171_0
                        mov              eax, dword ptr [rsp + 1904]
                        cmp              eax, 3
                                                                                        jne   .Lx1171_0
.Lx1171_1:
                        mov              rax, qword ptr [rsp + 1920]
                        mov              qword ptr [rsp + 1888], rax
                        mov              rax, qword ptr [rsp + 1928]
                        mov              qword ptr [rsp + 1896], rax
                                                                                        jmp   n823_coerce_numeric_α
.Lx1171_0:
                        lea              rdi, [rsp + 1920]
                        lea              rsi, [rsp + 1904]
                        lea              rdx, [rsp + 1888]
                        mov              rcx, 118
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n823_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n823_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 1904]
                        cmp              eax, 5
                                                                                        je    .Lx1173_1
                        cmp              eax, 3
                                                                                        jne   .Lx1173_0
                        mov              eax, dword ptr [rsp + 1920]
                        cmp              eax, 3
                                                                                        jne   .Lx1173_0
.Lx1173_1:
                        mov              rax, qword ptr [rsp + 1904]
                        mov              qword ptr [rsp + 1872], rax
                        mov              rax, qword ptr [rsp + 1912]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n824_cmp_test_α
.Lx1173_0:
                        lea              rdi, [rsp + 1904]
                        lea              rsi, [rsp + 1920]
                        lea              rdx, [rsp + 1872]
                        mov              rcx, 119
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n824_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n824_cmp_test_α:
                        lea              rdi, [rsp + 1888]
                        lea              rsi, [rsp + 1872]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_cmp_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax
                                                                                        jg    n819_statement_begin_β
                                                                                        jmp   n825_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n825_statement_end_α:
                                                                                        jmp   n847_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n826_statement_begin_α:
                                                                                        jmp   n827_var_α
n826_statement_begin_β:
                                                                                        jmp   n833_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n827_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2016], rax                    # result
                        mov              qword ptr [rsp + 2024], rdx
                                                                                        jmp   n828_var_α
#-----------------------------------------------------------------------------------------------------------------------
n828_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2032], rax                    # result
                        mov              qword ptr [rsp + 2040], rdx
                                                                                        jmp   n829_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n829_lit_integer_α:
                        mov              qword ptr [rsp + 2048], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1182_0]
                        mov              qword ptr [rsp + 2056], rax
                                                                                        jmp   n830_call_α
.Lx1182_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n830_call_α:
                        mov              rax, qword ptr [rsp + 2048]
                        mov              qword ptr [rsp + 1984], rax
                        mov              rax, qword ptr [rsp + 2056]
                        mov              qword ptr [rsp + 1992], rax
                        mov              rax, qword ptr [rsp + 2032]
                        mov              qword ptr [rsp + 1968], rax
                        mov              rax, qword ptr [rsp + 2040]
                        mov              qword ptr [rsp + 1976], rax
                        mov              rax, qword ptr [rsp + 2016]
                        mov              qword ptr [rsp + 1952], rax
                        mov              rax, qword ptr [rsp + 2024]
                        mov              qword ptr [rsp + 1960], rax
                        .section         .rodata
.Lrkfn1184:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1184]
                        lea              rsi, [rsp + 1952]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                        cmp              eax, 104
                                                                                        je    n826_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n831_assign_α
n830_call_β:
                                                                                        jmp   n826_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n831_assign_α:
                        mov              rax, qword ptr [rsp + 1936]
                        mov              rdx, qword ptr [rsp + 1944]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              qword ptr [r9 + 168], rdx
                                                                                        jmp   n832_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n832_statement_end_α:
                                                                                        jmp   n833_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n833_statement_begin_α:
                                                                                        jmp   n834_var_α
n833_statement_begin_β:
                                                                                        jmp   n847_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n834_var_α:
                        mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 2192], rax                    # result
                        mov              qword ptr [rsp + 2200], rdx
                                                                                        jmp   n835_var_α
#-----------------------------------------------------------------------------------------------------------------------
n835_var_α:
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 2176], rax                    # result
                        mov              qword ptr [rsp + 2184], rdx
                                                                                        jmp   n836_coerce_string_α
#-----------------------------------------------------------------------------------------------------------------------
n836_coerce_string_α:
                        lea              rdi, [rsp + 2176]
                        lea              rsi, [rsp + 2160]
                        mov              rdx, 3866683
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n837_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n837_match_begin_α:
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rsp                      # old____
                        mov              rdi, qword ptr [rsp + 2192]
                        mov              rsi, qword ptr [rsp + 2200]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx1195_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n838_match_any_α
n837_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx1195_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx1195_1
                                                                                        jmp   .Lx1195_0
.Lx1195_1:
n837_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n847_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n838_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n837_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rsp + 2168]
                        mov              r9d, dword ptr [rsp + 2164]
                        mov              edx, 0
.Lx1197_5:
                        cmp              edx, r9d
                                                                                        jge   n837_match_begin_β
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx1197_6
                        add              edx, 1
                                                                                        jmp   .Lx1197_5
.Lx1197_6:
                        add              r14d, 1
                                                                                        jmp   n839_match_end_α
n838_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n837_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n839_match_end_α:
                        mov              r8, r12
.Lx1199_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx1199_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx1199_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx1199_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx1199_1:
                        test             rax, rax
                                                                                        je    .Lx1199_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx1199_3]
                        lea              rdx, [rip + .Lx1199_4]
                                                                                        jmp   rax
.Lx1199_3:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1199_1
.Lx1199_4:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1199_1
.Lx1199_2:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx1199_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx1199_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n840_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n840_statement_end_α:
                                                                                        jmp   n841_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n841_statement_begin_α:
                                                                                        jmp   n842_var_α
n841_statement_begin_β:
                                                                                        jmp   n819_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n842_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2224], rax                    # result
                        mov              qword ptr [rsp + 2232], rdx
                                                                                        jmp   n843_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n843_lit_integer_α:
                        mov              qword ptr [rsp + 2240], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1205_0]
                        mov              qword ptr [rsp + 2248], rax
                                                                                        jmp   n844_binop_α
.Lx1205_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n844_binop_α:
                        mov              eax, dword ptr [rsp + 2224]
                        cmp              eax, 3
                                                                                        jne   .Lx1206_0
                        mov              rax, qword ptr [rsp + 2232]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2208], 3
                        mov              qword ptr [rsp + 2216], rax
                                                                                        jmp   n845_assign_α
.Lx1206_0:
                        mov              rdi, qword ptr [rsp + 2224]
                        mov              rsi, qword ptr [rsp + 2232]
                        mov              rdx, qword ptr [rsp + 2240]
                        mov              rcx, qword ptr [rsp + 2248]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1206_240
                        add              rsp, 32
                                                                                        jmp   n841_statement_begin_β
.Lx1206_240:
                        mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n845_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n845_assign_α:
                        mov              rax, qword ptr [rsp + 2208]
                        mov              rdx, qword ptr [rsp + 2216]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n846_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n846_statement_end_α:
                                                                                        jmp   n819_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n847_statement_begin_α:
                                                                                        jmp   n848_var_α
n847_statement_begin_β:
                                                                                        jmp   n854_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n848_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2336], rax                    # result
                        mov              qword ptr [rsp + 2344], rdx
                                                                                        jmp   n849_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n849_lit_integer_α:
                        mov              qword ptr [rsp + 2352], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1213_0]
                        mov              qword ptr [rsp + 2360], rax
                                                                                        jmp   n850_var_α
.Lx1213_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n850_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2368], rax                    # result
                        mov              qword ptr [rsp + 2376], rdx
                                                                                        jmp   n851_call_α
#-----------------------------------------------------------------------------------------------------------------------
n851_call_α:
                        mov              rax, qword ptr [rsp + 2368]
                        mov              qword ptr [rsp + 2304], rax
                        mov              rax, qword ptr [rsp + 2376]
                        mov              qword ptr [rsp + 2312], rax
                        mov              rax, qword ptr [rsp + 2352]
                        mov              qword ptr [rsp + 2288], rax
                        mov              rax, qword ptr [rsp + 2360]
                        mov              qword ptr [rsp + 2296], rax
                        mov              rax, qword ptr [rsp + 2336]
                        mov              qword ptr [rsp + 2272], rax
                        mov              rax, qword ptr [rsp + 2344]
                        mov              qword ptr [rsp + 2280], rax
                        .section         .rodata
.Lrkfn1216:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1216]
                        lea              rsi, [rsp + 2272]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2256], rax
                        mov              qword ptr [rsp + 2264], rdx
                        cmp              eax, 104
                                                                                        je    n847_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n852_assign_α
n851_call_β:
                                                                                        jmp   n847_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n852_assign_α:
                        mov              rax, qword ptr [rsp + 2256]
                        mov              rdx, qword ptr [rsp + 2264]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n853_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n853_statement_end_α:
                                                                                        jmp   n854_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n854_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
n855_goto_α:
                                                                                        jmp   n847_statement_begin_α
n855_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n856_goto_α:
                                                                                        jmp   n665_statement_begin_α
n856_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n857_goto_α:
                                                                                        jmp   n670_statement_begin_α
n857_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n858_goto_α:
                                                                                        jmp   n675_statement_begin_α
n858_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n859_goto_α:
                                                                                        jmp   n680_statement_begin_α
n859_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n860_goto_α:
                                                                                        jmp   n685_statement_begin_α
n860_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n861_goto_α:
                                                                                        jmp   n690_statement_begin_α
n861_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n862_goto_α:
                                                                                        jmp   n695_statement_begin_α
n862_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n863_goto_α:
                                                                                        jmp   n752_statement_begin_α
n863_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n864_goto_α:
                                                                                        jmp   n756_statement_begin_α
n864_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n865_goto_α:
                                                                                        jmp   n767_statement_begin_α
n865_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n866_goto_α:
                                                                                        jmp   n771_statement_begin_α
n866_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n867_goto_α:
                                                                                        jmp   n782_statement_begin_α
n867_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n868_goto_α:
                                                                                        jmp   n786_statement_begin_α
n868_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n869_goto_α:
                                                                                        jmp   n793_statement_begin_α
n869_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n870_goto_α:
                                                                                        jmp   n801_statement_begin_α
n870_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        add              rsp, 0
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        add              rsp, 0
                        mov              edi, 1
                        call             exit@PLT
                        .section         .rodata
.S0:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
.S1:                    .string          "r"
.S2:                    .string          "ws"
.S3:                    .string          "PATV$0"
.S4:                    .string          "BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
.S5:                    .string          "PATV$1"
.S6:                    .string          "PATV$2"
.S7:                    .string          "ix"
                        .text
                        .section         .note.GNU-stack,"",@progbits

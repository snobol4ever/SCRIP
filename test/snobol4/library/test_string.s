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
                                                                                        jmp   n29_save_restore_α
#=======================================================================================================================
#         startswith('foobar', 'bar')     :S(bad_sw2)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:
                                                                                        jmp   n19_var_α
n18_statement_begin_β:
                                                                                        jmp   n29_save_restore_α
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
                                                                                        jmp   n29_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n29_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx73_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx73_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_β:
                                                                                        jmp   proc_LBL__pad_left_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                                                                                        jmp   n103_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:
                                                                                        jmp   n93_var_α
n92_statement_begin_β:
                                                                                        jmp   n103_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 784], rax                     # result
                        mov              qword ptr [rsp + 792], rdx
                                                                                        jmp   n94_var_α
#-----------------------------------------------------------------------------------------------------------------------
n94_var_α:
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 864], rax                     # result
                        mov              qword ptr [rsp + 872], rdx
                                                                                        jmp   n95_var_α
#-----------------------------------------------------------------------------------------------------------------------
n95_var_α:
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 896], rax                     # result
                        mov              qword ptr [rsp + 904], rdx
                                                                                        jmp   n96_var_α
#-----------------------------------------------------------------------------------------------------------------------
n96_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 960], rax                     # result
                        mov              qword ptr [rsp + 968], rdx
                                                                                        jmp   n97_call_α
#-----------------------------------------------------------------------------------------------------------------------
n97_call_α:
                        mov              rax, qword ptr [rsp + 960]
                        mov              qword ptr [rsp + 928], rax
                        mov              rax, qword ptr [rsp + 968]
                        mov              qword ptr [rsp + 936], rax
                        .section         .rodata
.Lrkfn138:              .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn138]
                        lea              rsi, [rsp + 928]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
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
                        mov              eax, dword ptr [rsp + 896]
                        cmp              eax, 3
                                                                                        jne   .Lx139_0
                        mov              eax, dword ptr [rsp + 912]
                        cmp              eax, 3
                                                                                        jne   .Lx139_0
                        mov              rax, qword ptr [rsp + 904]
                        mov              rcx, qword ptr [rsp + 920]
                        sub              rax, rcx
                        mov              qword ptr [rsp + 880], 3
                        mov              qword ptr [rsp + 888], rax
                                                                                        jmp   n99_call_α
.Lx139_0:
                        mov              rdi, qword ptr [rsp + 896]
                        mov              rsi, qword ptr [rsp + 904]
                        mov              rdx, qword ptr [rsp + 912]
                        mov              rcx, qword ptr [rsp + 920]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        je    n92_statement_begin_β
                        mov              qword ptr [rsp + 880], rax
                        mov              qword ptr [rsp + 888], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n99_call_α
#-----------------------------------------------------------------------------------------------------------------------
n99_call_α:
                        mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 832], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 840], rax
                        mov              rax, qword ptr [rsp + 864]
                        mov              qword ptr [rsp + 816], rax
                        mov              rax, qword ptr [rsp + 872]
                        mov              qword ptr [rsp + 824], rax
                        .section         .rodata
.Lrkfn141:              .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn141]
                        lea              rsi, [rsp + 816]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
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
                        mov              rdi, qword ptr [rsp + 784]
                        mov              rsi, qword ptr [rsp + 792]
                        mov              rdx, qword ptr [rsp + 800]
                        mov              rcx, qword ptr [rsp + 808]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n101_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              rdx, qword ptr [rsp + 776]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              qword ptr [r9 + 72], rdx
                                                                                        jmp   n102_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:
                                                                                        jmp   n103_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n103_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx147_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx147_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_β:
                                                                                        jmp   proc_LBL__pad_right_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_right_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              rax, qword ptr [rip + .Lx183_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n150_lit_integer_α
n149_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n148_statement_begin_β
.Lx183_0:
                        .quad            .Lx183_0_s
.Lx183_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n151_call_α
n150_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n148_statement_begin_β
.Lx184_0:
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
                                                                                        jne   .Lx185_240
                        add              rsp, 16
                                                                                        jmp   n150_lit_integer_β
.Lx185_240:
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
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n154_call_α
n153_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n152_binop_β
.Lx187_0:
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
                                                                                        jne   .Lx188_240
                        add              rsp, 16
                                                                                        jmp   n153_lit_integer_β
.Lx188_240:
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
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n157_call_α
n156_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n155_binop_β
.Lx190_0:
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
                                                                                        jne   .Lx191_240
                        add              rsp, 16
                                                                                        jmp   n156_lit_integer_β
.Lx191_240:
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
                                                                                        jmp   n176_statement_begin_α
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
                                                                                        jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n163_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 1472], rdi
                        mov              qword ptr [rsp + 1480], rsi
                        mov              qword ptr [rsp + 1288], rsp                    # old____
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
.Lx200_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n164_lit_integer_α
n163_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx200_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx200_1
                                                                                        jmp   .Lx200_0
.Lx200_1:
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
                                                                                        jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n164_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n165_match_pos_α
n164_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n163_match_begin_β
.Lx201_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n165_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx202_240
                        add              rsp, 16
                                                                                        jmp   n163_match_begin_β
.Lx202_240:
                                                                                        jmp   n166_match_alternate_α
n165_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n163_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n166_match_alternate_α:
                        mov              dword ptr [rsp + 1376], r14d
                        lea              rax, [rip + .Lx204_21]
                        mov              qword ptr [rsp + 1392], rax
                                                                                        jmp   n175_match_span_α
.Lx204_21:
                        lea              rax, [rip + .Lx204_19]
                        mov              qword ptr [rsp + 1392], rax
                                                                                        jmp   n174_match_lit_α
n166_match_alternate_s0:
                        lea              rax, [rip + .Lx204_40]
                        mov              qword ptr [rsp + 1384], rax
                                                                                        jmp   n166_match_alternate_as
n166_match_alternate_s1:
                        lea              rax, [rip + .Lx204_41]
                        mov              qword ptr [rsp + 1384], rax
                                                                                        jmp   n166_match_alternate_as
.Lx204_40:
                                                                                        jmp   n175_match_span_β
.Lx204_41:
                                                                                        jmp   n174_match_lit_β
n166_match_alternate_as:
                                                                                        jmp   n167_match_assign_save_α
n166_match_alternate_β:
                        mov              rax, qword ptr [rsp + 1384]
                                                                                        jmp   rax
n166_match_alternate_af:
                        mov              r14d, dword ptr [rsp + 1376]
                        mov              rax, qword ptr [rsp + 1392]
                                                                                        jmp   rax
.Lx204_19:
                                                                                        jmp   n165_match_pos_β
#-----------------------------------------------------------------------------------------------------------------------
n167_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n168_match_rem_α
n167_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n166_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n168_match_rem_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d
                                                                                        jmp   n169_match_assign_cond_α
n168_match_rem_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n166_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n169_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n170_match_end_α
n169_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n168_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n170_match_end_α:
                        mov              r8, r12
.Lx211_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx211_9
                        mov              eax, dword ptr [rsp + 48]
                        mov              dword ptr [rsp + 1328], eax
                        mov              qword ptr [rsp + 1352], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx211_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx211_5
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
.Lx211_1:
                        test             rax, rax
                                                                                        je    .Lx211_2
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
                        lea              rcx, [rip + .Lx211_3]
                        lea              rdx, [rip + .Lx211_4]
                                                                                        jmp   rax
.Lx211_3:
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
                                                                                        jmp   .Lx211_1
.Lx211_4:
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
                                                                                        jmp   .Lx211_1
.Lx211_2:
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
.Lx211_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx211_10
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
                                                                                        jmp   n171_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n171_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx212_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n172_match_replace_α
n171_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n176_statement_begin_α
.Lx212_0:
                        .quad            .Lx212_0_s
.Lx212_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n172_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx214_0]
                        mov              rsi, qword ptr [rsp + 1488]
                        mov              rdx, qword ptr [rsp + 1496]
                        mov              ecx, dword ptr [rsp + 1264]
                        mov              r8, qword ptr [rsp + 1288]
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
                                                                                        jmp   .Lx214_1
.Lx214_0:
                        .quad            .Lx214_0_s
.Lx214_0_s:
                        .string          "s"
.Lx214_1:
                                                                                        jmp   n173_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_end_α:
                                                                                        jmp   n176_statement_begin_α
n173_statement_end_β:
                                                                                        jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n174_match_lit_α:
                                                                                        jmp   n166_match_alternate_s1
n174_match_lit_β:
                                                                                        jmp   n166_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n175_match_span_α:
                        lea              rdi, [rip + .S1]
                        lea              rsi, [rsp + 1408]
                        lea              rdx, [rsp + 1416]
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
                                                                                        js    n166_match_alternate_af
                        mov              r8, qword ptr [rsp + 1408]
                        mov              r9d, dword ptr [rsp + 1416]
                        mov              dword ptr [rsp + 1408], 0
.Lx220_0:
                        mov              eax, r14d
                        add              eax, dword ptr [rsp + 1408]
                        cmp              eax, r15d
                                                                                        jge   .Lx220_1
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edx, 0
.Lx220_2:
                        cmp              edx, r9d
                                                                                        jge   .Lx220_1
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx220_3
                        add              edx, 1
                                                                                        jmp   .Lx220_2
.Lx220_3:
                        add              dword ptr [rsp + 1408], 1
                                                                                        jmp   .Lx220_0
.Lx220_1:
                        mov              eax, dword ptr [rsp + 1408]
                        test             eax, eax
                                                                                        jle   n166_match_alternate_af
                        mov              edx, r14d
                        mov              dword ptr [rsp + 1412], edx
                        add              edx, eax
                        mov              r14d, edx
                                                                                        jmp   n166_match_alternate_s0
n175_match_span_β:
                        mov              r14d, dword ptr [rsp + 1412]
                                                                                        jmp   n166_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_begin_α:
                                                                                        jmp   n177_var_α
n176_statement_begin_β:
                                                                                        jmp   n180_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n177_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n178_assign_α
n177_var_β:
                        add              rsp, 16
                        add              rsp, 240
                                                                                        jmp   n176_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n178_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax                       # ltrim
                        mov              qword ptr [r9 + 88], rdx
                                                                                        jmp   n179_statement_end_α
n178_assign_β:
                                                                                        jmp   n176_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n179_statement_end_α:
                                                                                        jmp   n180_save_restore_α
n179_statement_end_β:
                                                                                        jmp   n180_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n180_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx228_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx228_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_β:
                                                                                        jmp   proc_LBL__ltrim_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ltrim_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_α:
proc_LBL__rtrim_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n229_statement_begin_α:
                                                                                        jmp   n230_lit_string_α
n229_statement_begin_β:
                                                                                        jmp   n242_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n230_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx285_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n231_lit_integer_α
.Lx285_0:
                        .quad            .Lx285_0_s
.Lx285_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n231_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n232_call_α
n231_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n229_statement_begin_β
.Lx286_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n232_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd95:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd95]
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
                                                                                        jne   .Lx287_240
                        add              rsp, 16
                                                                                        jmp   n231_lit_integer_β
.Lx287_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n233_binop_α
n232_call_β:
                        add              rsp, 16
                                                                                        jmp   n231_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n233_binop_α:
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
                                                                                        jmp   n234_lit_integer_α
n233_binop_β:
                        add              rsp, 32
                                                                                        jmp   n231_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n234_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n235_call_α
n234_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n233_binop_β
.Lx289_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n235_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd98:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd98]
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
                                                                                        jne   .Lx290_240
                        add              rsp, 16
                                                                                        jmp   n234_lit_integer_β
.Lx290_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n236_binop_α
n235_call_β:
                        add              rsp, 16
                                                                                        jmp   n234_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n236_binop_α:
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
                                                                                        jmp   n237_lit_integer_α
n236_binop_β:
                        add              rsp, 32
                                                                                        jmp   n234_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n237_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx292_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n238_call_α
n237_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n236_binop_β
.Lx292_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n238_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd101:        .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd101]
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
                                                                                        jne   .Lx293_240
                        add              rsp, 16
                                                                                        jmp   n237_lit_integer_β
.Lx293_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n239_binop_α
n238_call_β:
                        add              rsp, 16
                                                                                        jmp   n237_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n239_binop_α:
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
                                                                                        jmp   n240_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n240_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              qword ptr [r9 + 104], rdx
                                                                                        jmp   n241_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n241_statement_end_α:
                                                                                        jmp   n242_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n242_statement_begin_α:
                                                                                        jmp   n243_var_α
n242_statement_begin_β:
                                                                                        jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n243_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n244_call_α
n243_var_β:
                        add              rsp, 16
                        add              rsp, 160
                                                                                        jmp   n242_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n244_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd302:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd302]
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
                                                                                        jne   .Lx301_240
                        add              rsp, 16
                                                                                        jmp   n243_var_β
.Lx301_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n245_assign_α
n244_call_β:
                        add              rsp, 16
                                                                                        jmp   n243_var_β
#-----------------------------------------------------------------------------------------------------------------------
n245_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n246_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n246_statement_end_α:
                                                                                        jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n247_statement_begin_α:
                                                                                        jmp   n248_var_α
n247_statement_begin_β:
                        add              rsp, 192
                                                                                        jmp   n254_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n248_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n249_lit_integer_α
n248_var_β:
                        add              rsp, 16
                        add              rsp, 192
                                                                                        jmp   n247_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n249_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx309_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n250_coerce_numeric_α
n249_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n248_var_β
.Lx309_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n250_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx311_1
                        cmp              eax, 3
                                                                                        jne   .Lx311_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx311_0
.Lx311_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n251_coerce_numeric_α
.Lx311_0:
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
                                                                                        jmp   n251_coerce_numeric_α
n250_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n249_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n251_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx313_1
                        cmp              eax, 3
                                                                                        jne   .Lx313_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx313_0
.Lx313_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n252_cmp_test_α
.Lx313_0:
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
                                                                                        jmp   n252_cmp_test_α
n251_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n250_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n252_cmp_test_α:
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
                                                                                        jle   .Lx315_240
                        add              rsp, 16
                                                                                        jmp   n251_coerce_numeric_β
.Lx315_240:
                                                                                        jmp   n253_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n253_statement_end_α:
                                                                                        jmp   n275_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_begin_α:
                                                                                        jmp   n255_var_α
n254_statement_begin_β:
                                                                                        jmp   n261_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n255_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 1968], rax                    # result
                        mov              qword ptr [rsp + 1976], rdx
                                                                                        jmp   n256_var_α
#-----------------------------------------------------------------------------------------------------------------------
n256_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1984], rax                    # result
                        mov              qword ptr [rsp + 1992], rdx
                                                                                        jmp   n257_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n257_lit_integer_α:
                        mov              qword ptr [rsp + 2000], 3                      # result
                        mov              rax, qword ptr [rip + .Lx322_0]
                        mov              qword ptr [rsp + 2008], rax
                                                                                        jmp   n258_call_α
.Lx322_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n258_call_α:
                        mov              rax, qword ptr [rsp + 2000]
                        mov              qword ptr [rsp + 1936], rax
                        mov              rax, qword ptr [rsp + 2008]
                        mov              qword ptr [rsp + 1944], rax
                        mov              rax, qword ptr [rsp + 1984]
                        mov              qword ptr [rsp + 1920], rax
                        mov              rax, qword ptr [rsp + 1992]
                        mov              qword ptr [rsp + 1928], rax
                        mov              rax, qword ptr [rsp + 1968]
                        mov              qword ptr [rsp + 1904], rax
                        mov              rax, qword ptr [rsp + 1976]
                        mov              qword ptr [rsp + 1912], rax
                        .section         .rodata
.Lrkfn324:              .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn324]
                        lea              rsi, [rsp + 1904]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                        cmp              eax, 104
                                                                                        je    n254_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n259_assign_α
n258_call_β:
                                                                                        jmp   n254_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n259_assign_α:
                        mov              rax, qword ptr [rsp + 1888]
                        mov              rdx, qword ptr [rsp + 1896]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              qword ptr [r9 + 168], rdx
                                                                                        jmp   n260_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n260_statement_end_α:
                                                                                        jmp   n261_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n261_statement_begin_α:
                                                                                        jmp   n262_var_α
n261_statement_begin_β:
                                                                                        jmp   n275_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n262_var_α:
                        mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 2144], rax                    # result
                        mov              qword ptr [rsp + 2152], rdx
                                                                                        jmp   n263_var_α
#-----------------------------------------------------------------------------------------------------------------------
n263_var_α:
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 2128], rax                    # result
                        mov              qword ptr [rsp + 2136], rdx
                                                                                        jmp   n264_coerce_string_α
#-----------------------------------------------------------------------------------------------------------------------
n264_coerce_string_α:
                        lea              rdi, [rsp + 2128]
                        lea              rsi, [rsp + 2112]
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
                                                                                        jmp   n265_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n265_match_begin_α:
                        mov              qword ptr [rsp + 2072], rsp                    # old____
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rdi, qword ptr [rsp + 2144]
                        mov              rsi, qword ptr [rsp + 2152]
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
.Lx335_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n266_match_any_α
n265_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx335_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx335_1
                                                                                        jmp   .Lx335_0
.Lx335_1:
n265_match_begin_af:
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
                                                                                        jmp   n275_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n265_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rsp + 2120]
                        mov              r9d, dword ptr [rsp + 2116]
                        mov              edx, 0
.Lx337_5:
                        cmp              edx, r9d
                                                                                        jge   n265_match_begin_β
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx337_6
                        add              edx, 1
                                                                                        jmp   .Lx337_5
.Lx337_6:
                        add              r14d, 1
                                                                                        jmp   n267_match_end_α
n266_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n265_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n267_match_end_α:
                        mov              r8, r12
.Lx339_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx339_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx339_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx339_5
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
.Lx339_1:
                        test             rax, rax
                                                                                        je    .Lx339_2
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
                        lea              rcx, [rip + .Lx339_3]
                        lea              rdx, [rip + .Lx339_4]
                                                                                        jmp   rax
.Lx339_3:
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
                                                                                        jmp   .Lx339_1
.Lx339_4:
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
                                                                                        jmp   .Lx339_1
.Lx339_2:
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
.Lx339_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx339_10
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
                                                                                        jmp   n268_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n268_statement_end_α:
                                                                                        jmp   n269_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n269_statement_begin_α:
                                                                                        jmp   n270_var_α
n269_statement_begin_β:
                                                                                        jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n270_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2176], rax                    # result
                        mov              qword ptr [rsp + 2184], rdx
                                                                                        jmp   n271_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n271_lit_integer_α:
                        mov              qword ptr [rsp + 2192], 3                      # result
                        mov              rax, qword ptr [rip + .Lx345_0]
                        mov              qword ptr [rsp + 2200], rax
                                                                                        jmp   n272_binop_α
.Lx345_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n272_binop_α:
                        mov              eax, dword ptr [rsp + 2176]
                        cmp              eax, 3
                                                                                        jne   .Lx346_0
                        mov              rax, qword ptr [rsp + 2184]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2160], 3
                        mov              qword ptr [rsp + 2168], rax
                                                                                        jmp   n273_assign_α
.Lx346_0:
                        mov              rdi, qword ptr [rsp + 2176]
                        mov              rsi, qword ptr [rsp + 2184]
                        mov              rdx, qword ptr [rsp + 2192]
                        mov              rcx, qword ptr [rsp + 2200]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx346_240
                        add              rsp, 32
                                                                                        jmp   n269_statement_begin_β
.Lx346_240:
                        mov              qword ptr [rsp + 2160], rax
                        mov              qword ptr [rsp + 2168], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n273_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n273_assign_α:
                        mov              rax, qword ptr [rsp + 2160]
                        mov              rdx, qword ptr [rsp + 2168]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n274_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_end_α:
                                                                                        jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_begin_α:
                                                                                        jmp   n276_var_α
n275_statement_begin_β:
                                                                                        jmp   n282_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n276_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n277_lit_integer_α
n276_var_β:
                        add              rsp, 16
                        add              rsp, 272
                                                                                        jmp   n275_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n277_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx353_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n278_var_α
n277_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n276_var_β
.Lx353_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n278_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n279_call_α
n278_var_β:
                        add              rsp, 16
                                                                                        jmp   n277_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n279_call_α:
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
.Lrkfnzd356:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd356]
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
                                                                                        jne   .Lx355_240
                        add              rsp, 16
                                                                                        jmp   n278_var_β
.Lx355_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n280_assign_α
n279_call_β:
                        add              rsp, 16
                                                                                        jmp   n278_var_β
#-----------------------------------------------------------------------------------------------------------------------
n280_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n281_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n281_statement_end_α:
                                                                                        jmp   n282_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n282_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx361_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx361_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_β:
                                                                                        jmp   proc_LBL__rtrim_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__rtrim_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_α:
proc_LBL__trimws_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_begin_α:
                                                                                        jmp   n363_var_α
n362_statement_begin_β:
                                                                                        jmp   n368_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n363_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n364_call_α
#-----------------------------------------------------------------------------------------------------------------------
n364_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 80
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [r9 + 128]                      # rtrim
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 72], rax
                        mov              rdi, qword ptr [rip + .Lx373_0]
                        mov              esi, 4
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx373_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
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
                        lea              rcx, [rip + .Lx373_6]
                        lea              rdx, [rip + .Lx373_7]
                                                                                        jmp   rax
.Lx373_6:
                        mov              rdi, qword ptr [r9 + 128]                      # rtrim
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 80
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx373_2
.Lx373_7:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 80
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx373_2
.Lx373_5:
                        add              rsp, 80
.Lx373_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx373_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n362_statement_begin_β
.Lx373_240:
                                                                                        jmp   n365_call_α
n364_call_β:
                                                                                        jmp   n362_statement_begin_β
.Lx373_0:
                        .quad            .Lx373_0_s
.Lx373_0_s:
                        .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n365_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # call
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [r9 + 80]                       # ltrim
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx375_0]
                        mov              esi, 3
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx375_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
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
                        lea              rcx, [rip + .Lx375_6]
                        lea              rdx, [rip + .Lx375_7]
                                                                                        jmp   rax
.Lx375_6:
                        mov              rdi, qword ptr [r9 + 80]                       # ltrim
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx375_2
.Lx375_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax                       # ltrim
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx375_2
.Lx375_5:
                        add              rsp, 64
.Lx375_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx375_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n362_statement_begin_β
.Lx375_240:
                                                                                        jmp   n366_assign_α
n365_call_β:
                                                                                        jmp   n362_statement_begin_β
.Lx375_0:
                        .quad            .Lx375_0_s
.Lx375_0_s:
                        .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n366_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 176], rax                      # trimws
                        mov              qword ptr [r9 + 184], rdx
                                                                                        jmp   n367_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n367_statement_end_α:
                                                                                        jmp   n368_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n368_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx380_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx380_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_β:
                                                                                        jmp   proc_LBL__trimws_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__trimws_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_α:
proc_LBL__repeat_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n381_statement_begin_α:
                                                                                        jmp   n382_var_α
n381_statement_begin_β:
                                                                                        jmp   n387_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n382_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n383_var_α
#-----------------------------------------------------------------------------------------------------------------------
n383_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n384_call_α
n383_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n381_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n384_call_α:
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
.Lrkfnzd393:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd393]
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
                                                                                        jne   .Lx392_240
                        add              rsp, 16
                                                                                        jmp   n383_var_β
.Lx392_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n385_assign_α
n384_call_β:
                        add              rsp, 16
                                                                                        jmp   n383_var_β
#-----------------------------------------------------------------------------------------------------------------------
n385_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax                      # repeat
                        mov              qword ptr [r9 + 200], rdx
                                                                                        jmp   n386_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n386_statement_end_α:
                                                                                        jmp   n387_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n387_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx398_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx398_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_β:
                                                                                        jmp   proc_LBL__repeat_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__repeat_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_α:
proc_LBL__contains_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n399_statement_begin_α:
                                                                                        jmp   n400_var_α
n399_statement_begin_β:
                                                                                        jmp   n411_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n400_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n401_var_α
#-----------------------------------------------------------------------------------------------------------------------
n401_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n402_assign_α
n401_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n411_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n402_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx416_0]
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
                                                                                        jmp   n403_var_α
n402_assign_β:
                                                                                        jmp   n401_var_β
.Lx416_0:
                        .quad            .Lx416_0_s
.Lx416_0_s:
                        .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n403_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n404_coerce_string_α
n403_var_β:
                        add              rsp, 16
                                                                                        jmp   n402_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n404_coerce_string_α:
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
                                                                                        jmp   n405_match_begin_α
n404_coerce_string_β:
                        add              rsp, 16
                                                                                        jmp   n403_var_β
#-----------------------------------------------------------------------------------------------------------------------
n405_match_begin_α:
                        mov              rdi, qword ptr [rsp + 48]                      # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 2704], rdi
                        mov              qword ptr [rsp + 2712], rsi
                        mov              qword ptr [rsp + 2600], rsp                    # old____
                        mov              qword ptr [rsp + 2608], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2616], r14                    # outer_δ
                        mov              qword ptr [rsp + 2624], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2632], rax                    # cap_gen
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
                        mov              qword ptr [rsp + 2576], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2560], 0                      # start_δ
.Lx421_0:
                        mov              r14d, dword ptr [rsp + 2560]
                                                                                        jmp   n406_match_break_α
n405_match_begin_β:
                        add              dword ptr [rsp + 2560], 1
                        mov              eax, dword ptr [rsp + 2560]
                        cmp              eax, r15d
                                                                                        jg    .Lx421_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx421_1
                                                                                        jmp   .Lx421_0
.Lx421_1:
n405_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2576]
                        mov              r13, qword ptr [rsp + 2608]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2616]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2624]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2632]                    # cap_gen
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
                                                                                        jmp   n404_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n406_match_break_α:
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
                                                                                        jl    .Lx423_240
                        add              rsp, 16
                                                                                        jmp   n405_match_begin_β
.Lx423_240:
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax
                                                                                        jmp   n407_match_defer_α
n406_match_break_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n405_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n407_match_defer_α:
                        lea              rdi, [rip + .S2]
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
                                                                                        jz    .Lx424_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx424_4]
                        lea              r11, [rip + .Lx424_5]
                                                                                        jmp   rax
.Lx424_4:
                                                                                        jmp   n408_match_end_α
.Lx424_5:
                                                                                        jmp   n406_match_break_β
.Lx424_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
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
.Lx424_2:
                        test             rax, rax
                                                                                        je    .Lx424_3
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
                        lea              rcx, [rip + .Lx424_7]
                        lea              rdx, [rip + .Lx424_8]
                                                                                        jmp   rax
.Lx424_7:
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
                                                                                        jmp   .Lx424_2
.Lx424_8:
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
                                                                                        jmp   .Lx424_2
.Lx424_3:
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
                                                                                        js    n406_match_break_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx424_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n408_match_end_α
.Lx424_6:
                        add              rsp, 16
                                                                                        jmp   n406_match_break_β
n407_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n408_match_end_α:
                        mov              r8, r12
.Lx426_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx426_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx426_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx426_5
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
.Lx426_1:
                        test             rax, rax
                                                                                        je    .Lx426_2
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
                        lea              rcx, [rip + .Lx426_3]
                        lea              rdx, [rip + .Lx426_4]
                                                                                        jmp   rax
.Lx426_3:
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
                                                                                        jmp   .Lx426_1
.Lx426_4:
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
                                                                                        jmp   .Lx426_1
.Lx426_2:
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
.Lx426_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx426_10
                        mov              r13, qword ptr [rsp + 2608]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2616]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2624]                    # outer_Δ
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
                                                                                        jmp   n409_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n409_statement_end_α:
                                                                                        jmp   n410_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n410_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx430_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx430_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n411_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx432_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx432_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_β:
                                                                                        jmp   proc_LBL__contains_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__contains_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_α:
proc_LBL__startswith_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n433_statement_begin_α:
                                                                                        jmp   n434_var_α
n433_statement_begin_β:
                                                                                        jmp   n444_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n434_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n435_var_α
#-----------------------------------------------------------------------------------------------------------------------
n435_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n436_assign_α
n435_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n444_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n436_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx449_0]
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
                                                                                        jmp   n437_match_begin_α
n436_assign_β:
                                                                                        jmp   n435_var_β
.Lx449_0:
                        .quad            .Lx449_0_s
.Lx449_0_s:
                        .string          "PATV$1"
#-----------------------------------------------------------------------------------------------------------------------
n437_match_begin_α:
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rsp + 2776], rsp                    # old____
                        mov              qword ptr [rsp + 2784], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2792], r14                    # outer_δ
                        mov              qword ptr [rsp + 2800], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2808], rax                    # cap_gen
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
                        mov              qword ptr [rsp + 2752], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2736], 0                      # start_δ
.Lx451_0:
                        mov              r14d, dword ptr [rsp + 2736]
                                                                                        jmp   n438_lit_integer_α
n437_match_begin_β:
                        add              dword ptr [rsp + 2736], 1
                        mov              eax, dword ptr [rsp + 2736]
                        cmp              eax, r15d
                                                                                        jg    .Lx451_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx451_1
                                                                                        jmp   .Lx451_0
.Lx451_1:
n437_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2752]
                        mov              r13, qword ptr [rsp + 2784]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2792]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2800]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2808]                    # cap_gen
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
                                                                                        jmp   n436_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n438_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx452_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n439_match_pos_α
.Lx452_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n439_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx453_240
                        add              rsp, 16
                                                                                        jmp   n437_match_begin_β
.Lx453_240:
                                                                                        jmp   n440_match_defer_α
n439_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n437_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n440_match_defer_α:
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
                                                                                        jz    .Lx454_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx454_4]
                        lea              r11, [rip + .Lx454_5]
                                                                                        jmp   rax
.Lx454_4:
                                                                                        jmp   n441_match_end_α
.Lx454_5:
                                                                                        jmp   n437_match_begin_β
.Lx454_0:
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
.Lx454_2:
                        test             rax, rax
                                                                                        je    .Lx454_3
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
                        lea              rcx, [rip + .Lx454_7]
                        lea              rdx, [rip + .Lx454_8]
                                                                                        jmp   rax
.Lx454_7:
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
                                                                                        jmp   .Lx454_2
.Lx454_8:
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
                                                                                        jmp   .Lx454_2
.Lx454_3:
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
                                                                                        js    n437_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx454_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n441_match_end_α
.Lx454_6:
                        add              rsp, 16
                                                                                        jmp   n437_match_begin_β
n440_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n441_match_end_α:
                        mov              r8, r12
.Lx456_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx456_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx456_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx456_5
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
.Lx456_1:
                        test             rax, rax
                                                                                        je    .Lx456_2
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
                        lea              rcx, [rip + .Lx456_3]
                        lea              rdx, [rip + .Lx456_4]
                                                                                        jmp   rax
.Lx456_3:
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
                                                                                        jmp   .Lx456_1
.Lx456_4:
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
                                                                                        jmp   .Lx456_1
.Lx456_2:
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
.Lx456_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx456_10
                        mov              r13, qword ptr [rsp + 2784]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2792]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2800]                    # outer_Δ
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
                                                                                        jmp   n442_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n442_statement_end_α:
                                                                                        jmp   n443_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n443_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx460_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx460_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n444_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx462_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx462_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_β:
                                                                                        jmp   proc_LBL__startswith_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__startswith_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_α:
proc_LBL__endswith_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n463_statement_begin_α:
                                                                                        jmp   n464_var_α
n463_statement_begin_β:
                                                                                        jmp   n474_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n464_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n465_var_α
#-----------------------------------------------------------------------------------------------------------------------
n465_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n466_assign_α
n465_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n474_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n466_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx479_0]
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
                                                                                        jmp   n467_match_begin_α
n466_assign_β:
                                                                                        jmp   n465_var_β
.Lx479_0:
                        .quad            .Lx479_0_s
.Lx479_0_s:
                        .string          "PATV$2"
#-----------------------------------------------------------------------------------------------------------------------
n467_match_begin_α:
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rsp + 2936], rsp                    # old____
                        mov              qword ptr [rsp + 2944], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2952], r14                    # outer_δ
                        mov              qword ptr [rsp + 2960], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2968], rax                    # cap_gen
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
                        mov              qword ptr [rsp + 2912], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2896], 0                      # start_δ
.Lx481_0:
                        mov              r14d, dword ptr [rsp + 2896]
                                                                                        jmp   n468_match_defer_α
n467_match_begin_β:
                        add              dword ptr [rsp + 2896], 1
                        mov              eax, dword ptr [rsp + 2896]
                        cmp              eax, r15d
                                                                                        jg    .Lx481_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx481_1
                                                                                        jmp   .Lx481_0
.Lx481_1:
n467_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2912]
                        mov              r13, qword ptr [rsp + 2944]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2952]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2960]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2968]                    # cap_gen
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
                                                                                        jmp   n466_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n468_match_defer_α:
                        lea              rdi, [rip + .S4]
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
                                                                                        jz    .Lx482_0
                        mov              r8d, 1
                        lea              r10, [rip + .Lx482_4]
                        lea              r11, [rip + .Lx482_5]
                                                                                        jmp   rax
.Lx482_4:
                        lea              rcx, [rip + g_scan_hit_start]
                        mov              rax, qword ptr [rcx]
                        mov              dword ptr [rsp + 2896], eax
                                                                                        jmp   n469_lit_integer_α
.Lx482_5:
                                                                                        jmp   n467_match_begin_β
.Lx482_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]
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
.Lx482_2:
                        test             rax, rax
                                                                                        je    .Lx482_3
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
                        lea              rcx, [rip + .Lx482_7]
                        lea              rdx, [rip + .Lx482_8]
                                                                                        jmp   rax
.Lx482_7:
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
                                                                                        jmp   .Lx482_2
.Lx482_8:
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
                                                                                        jmp   .Lx482_2
.Lx482_3:
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
                                                                                        js    n467_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx482_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n469_lit_integer_α
.Lx482_6:
                        add              rsp, 16
                                                                                        jmp   n467_match_begin_β
n468_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n469_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx483_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n470_match_rpos_α
.Lx483_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n470_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        je    .Lx484_240
                        add              rsp, 16
                                                                                        jmp   n467_match_begin_β
.Lx484_240:
                                                                                        jmp   n471_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n471_match_end_α:
                        mov              r8, r12
.Lx486_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx486_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx486_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx486_5
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
.Lx486_1:
                        test             rax, rax
                                                                                        je    .Lx486_2
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
                        lea              rcx, [rip + .Lx486_3]
                        lea              rdx, [rip + .Lx486_4]
                                                                                        jmp   rax
.Lx486_3:
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
                                                                                        jmp   .Lx486_1
.Lx486_4:
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
                                                                                        jmp   .Lx486_1
.Lx486_2:
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
.Lx486_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx486_10
                        mov              r13, qword ptr [rsp + 2944]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2952]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2960]                    # outer_Δ
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
                                                                                        jmp   n472_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n472_statement_end_α:
                                                                                        jmp   n473_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n473_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx490_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx490_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n474_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx492_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx492_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_β:
                                                                                        jmp   proc_LBL__endswith_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__endswith_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_α:
proc_LBL__index_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n493_statement_begin_α:
                                                                                        jmp   n494_lit_integer_α
n493_statement_begin_β:
                                                                                        jmp   n497_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n494_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx521_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n495_assign_α
.Lx521_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n495_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              qword ptr [r9 + 280], rdx
                                                                                        jmp   n496_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n496_statement_end_α:
                                                                                        jmp   n497_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n497_statement_begin_α:
                                                                                        jmp   n498_var_α
n497_statement_begin_β:
                                                                                        jmp   n501_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n498_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n499_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n499_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              qword ptr [r9 + 296], rdx
                                                                                        jmp   n500_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n500_statement_end_α:
                                                                                        jmp   n501_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n501_statement_begin_α:
                                                                                        jmp   n502_var_α
n501_statement_begin_β:
                                                                                        jmp   n518_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n502_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n503_var_α
#-----------------------------------------------------------------------------------------------------------------------
n503_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n504_coerce_string_α
#-----------------------------------------------------------------------------------------------------------------------
n504_coerce_string_α:
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
                                                                                        jmp   n505_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n505_match_begin_α:
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 3248], rdi
                        mov              qword ptr [rsp + 3256], rsi
                        mov              qword ptr [rsp + 3128], rsp                    # old____
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
.Lx538_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n506_match_assign_save_α
n505_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx538_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx538_1
                                                                                        jmp   .Lx538_0
.Lx538_1:
n505_match_begin_af:
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
                                                                                        jmp   n518_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n506_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n507_match_break_α
n506_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n505_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n507_match_break_α:
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
                                                                                        jl    .Lx542_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n505_match_begin_β
.Lx542_240:
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax
                                                                                        jmp   n508_match_assign_cond_α
n507_match_break_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n505_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n508_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S5]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n509_match_end_α
n508_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n507_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n509_match_end_α:
                        mov              r8, r12
.Lx546_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx546_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx546_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx546_5
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
.Lx546_1:
                        test             rax, rax
                                                                                        je    .Lx546_2
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
                        lea              rcx, [rip + .Lx546_3]
                        lea              rdx, [rip + .Lx546_4]
                                                                                        jmp   rax
.Lx546_3:
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
                                                                                        jmp   .Lx546_1
.Lx546_4:
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
                                                                                        jmp   .Lx546_1
.Lx546_2:
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
.Lx546_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx546_10
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
                                                                                        jmp   n510_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n510_statement_end_α:
                                                                                        jmp   n511_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n511_statement_begin_α:
                                                                                        jmp   n512_var_α
n511_statement_begin_β:
                                                                                        jmp   n518_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n512_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n513_call_α
n512_var_β:
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n511_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n513_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd553:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd553]
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
                                                                                        jne   .Lx552_240
                        add              rsp, 16
                                                                                        jmp   n512_var_β
.Lx552_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n514_lit_integer_α
n513_call_β:
                        add              rsp, 16
                                                                                        jmp   n512_var_β
#-----------------------------------------------------------------------------------------------------------------------
n514_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx554_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n515_binop_α
n514_lit_integer_β:
                        add              rsp, 32
                                                                                        jmp   n512_var_β
.Lx554_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n515_binop_α:
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
                                                                                        jne   .Lx555_240
                        add              rsp, 16
                                                                                        jmp   n514_lit_integer_β
.Lx555_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n516_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n516_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              qword ptr [r9 + 280], rdx
                                                                                        jmp   n517_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n517_statement_end_α:
                                                                                        jmp   n518_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n518_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx560_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx560_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_β:
                                                                                        jmp   proc_LBL__index_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__index_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_pad_left_α
proc_pad_left_α:
proc_pad_left_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n561_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n562_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n562_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx566_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx566_1
.Lx566_0:
                        .quad            .Lx566_0_s
.Lx566_0_s:
                        .string          "pad_left"
.Lx566_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_left_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_pad_right_α
proc_pad_right_α:
proc_pad_right_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n567_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n568_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n568_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx572_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx572_1
.Lx572_0:
                        .quad            .Lx572_0_s
.Lx572_0_s:
                        .string          "pad_right"
.Lx572_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_pad_right_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_ltrim_α
proc_ltrim_α:
proc_ltrim_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n573_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n574_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n574_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx578_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx578_1
.Lx578_0:
                        .quad            .Lx578_0_s
.Lx578_0_s:
                        .string          "ltrim"
.Lx578_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_ltrim_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_rtrim_α
proc_rtrim_α:
proc_rtrim_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n579_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n580_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n580_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx584_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx584_1
.Lx584_0:
                        .quad            .Lx584_0_s
.Lx584_0_s:
                        .string          "rtrim"
.Lx584_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_rtrim_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_trimws_α
proc_trimws_α:
proc_trimws_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n585_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n586_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n586_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx590_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx590_1
.Lx590_0:
                        .quad            .Lx590_0_s
.Lx590_0_s:
                        .string          "trimws"
.Lx590_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_trimws_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_repeat_α
proc_repeat_α:
proc_repeat_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n591_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n592_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n592_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx596_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx596_1
.Lx596_0:
                        .quad            .Lx596_0_s
.Lx596_0_s:
                        .string          "repeat"
.Lx596_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_repeat_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_contains_α
proc_contains_α:
proc_contains_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n597_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n598_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n598_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx602_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx602_1
.Lx602_0:
                        .quad            .Lx602_0_s
.Lx602_0_s:
                        .string          "contains"
.Lx602_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_contains_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_startswith_α
proc_startswith_α:
proc_startswith_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n603_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n604_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n604_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx608_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx608_1
.Lx608_0:
                        .quad            .Lx608_0_s
.Lx608_0_s:
                        .string          "startswith"
.Lx608_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_startswith_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_endswith_α
proc_endswith_α:
proc_endswith_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n609_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n610_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n610_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx614_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx614_1
.Lx614_0:
                        .quad            .Lx614_0_s
.Lx614_0_s:
                        .string          "endswith"
.Lx614_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_endswith_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_index_α
proc_index_α:
proc_index_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n615_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n616_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n616_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx620_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx620_1
.Lx620_0:
                        .quad            .Lx620_0_s
.Lx620_0_s:
                        .string          "index"
.Lx620_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_index_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              esi, 4704
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
                        mov              esi, 4704
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
                        mov              esi, 4704
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
                        mov              esi, 4704
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
                        mov              esi, 4704
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
                        mov              esi, 4704
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
                        mov              esi, 4704
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
                        mov              esi, 4704
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
                        mov              esi, 4704
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
                        mov              esi, 4704
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "pad_left"
.Lstartup_pp10_0:       .string          "s"
.Lstartup_pp10_1:       .string          "n"
.Lstartup_pp10_2:       .string          "c"
                        .align           8
.Lstartup_pnames10:
                        .quad            .Lstartup_pp10_0
                        .quad            .Lstartup_pp10_1
                        .quad            .Lstartup_pp10_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + .Lstartup_pnames10]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + proc_pad_left_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 3
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "pad_right"
.Lstartup_pp11_0:       .string          "s"
.Lstartup_pp11_1:       .string          "n"
.Lstartup_pp11_2:       .string          "c"
                        .align           8
.Lstartup_pnames11:
                        .quad            .Lstartup_pp11_0
                        .quad            .Lstartup_pp11_1
                        .quad            .Lstartup_pp11_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + .Lstartup_pnames11]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + proc_pad_right_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 3
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname12:      .string          "ltrim"
.Lstartup_pp12_0:       .string          "s"
.Lstartup_pp12_1:       .string          "ws"
.Lstartup_pp12_2:       .string          "r"
                        .align           8
.Lstartup_pnames12:
                        .quad            .Lstartup_pp12_0
                        .quad            .Lstartup_pp12_1
                        .quad            .Lstartup_pp12_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname12]
                        lea              rsi, [rip + .Lstartup_pnames12]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        lea              rsi, [rip + proc_ltrim_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname13:      .string          "rtrim"
.Lstartup_pp13_0:       .string          "s"
.Lstartup_pp13_1:       .string          "ws"
.Lstartup_pp13_2:       .string          "i"
.Lstartup_pp13_3:       .string          "ch"
                        .align           8
.Lstartup_pnames13:
                        .quad            .Lstartup_pp13_0
                        .quad            .Lstartup_pp13_1
                        .quad            .Lstartup_pp13_2
                        .quad            .Lstartup_pp13_3
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname13]
                        lea              rsi, [rip + .Lstartup_pnames13]
                        mov              edx, 4
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        lea              rsi, [rip + proc_rtrim_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 4
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname14:      .string          "trimws"
.Lstartup_pp14_0:       .string          "s"
                        .align           8
.Lstartup_pnames14:
                        .quad            .Lstartup_pp14_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname14]
                        lea              rsi, [rip + .Lstartup_pnames14]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        lea              rsi, [rip + proc_trimws_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname15:      .string          "repeat"
.Lstartup_pp15_0:       .string          "s"
.Lstartup_pp15_1:       .string          "n"
                        .align           8
.Lstartup_pnames15:
                        .quad            .Lstartup_pp15_0
                        .quad            .Lstartup_pp15_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname15]
                        lea              rsi, [rip + .Lstartup_pnames15]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        lea              rsi, [rip + proc_repeat_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 2
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname16:      .string          "contains"
.Lstartup_pp16_0:       .string          "s"
.Lstartup_pp16_1:       .string          "t"
                        .align           8
.Lstartup_pnames16:
                        .quad            .Lstartup_pp16_0
                        .quad            .Lstartup_pp16_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname16]
                        lea              rsi, [rip + .Lstartup_pnames16]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        lea              rsi, [rip + proc_contains_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 2
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname17:      .string          "startswith"
.Lstartup_pp17_0:       .string          "s"
.Lstartup_pp17_1:       .string          "t"
                        .align           8
.Lstartup_pnames17:
                        .quad            .Lstartup_pp17_0
                        .quad            .Lstartup_pp17_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname17]
                        lea              rsi, [rip + .Lstartup_pnames17]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        lea              rsi, [rip + proc_startswith_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 2
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname18:      .string          "endswith"
.Lstartup_pp18_0:       .string          "s"
.Lstartup_pp18_1:       .string          "t"
                        .align           8
.Lstartup_pnames18:
                        .quad            .Lstartup_pp18_0
                        .quad            .Lstartup_pp18_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname18]
                        lea              rsi, [rip + .Lstartup_pnames18]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname18]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname18]
                        lea              rsi, [rip + proc_endswith_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname18]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname18]
                        mov              esi, 2
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname18]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname18]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname19:      .string          "index"
.Lstartup_pp19_0:       .string          "s"
.Lstartup_pp19_1:       .string          "t"
.Lstartup_pp19_2:       .string          "ix"
                        .align           8
.Lstartup_pnames19:
                        .quad            .Lstartup_pp19_0
                        .quad            .Lstartup_pp19_1
                        .quad            .Lstartup_pp19_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname19]
                        lea              rsi, [rip + .Lstartup_pnames19]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname19]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname19]
                        lea              rsi, [rip + proc_index_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname19]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname19]
                        mov              esi, 2
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname19]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname19]
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
n621_statement_begin_α:
                                                                                        jmp   n622_statement_end_α
n621_statement_begin_β:
                                                                                        jmp   n623_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n622_statement_end_α:
                                                                                        jmp   n623_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'endswith ok'          :(ew1)
#-----------------------------------------------------------------------------------------------------------------------
n623_statement_begin_α:
                                                                                        jmp   n624_statement_end_α
n623_statement_begin_β:
                                                                                        jmp   n625_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n624_statement_end_α:
                                                                                        jmp   n625_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n625_statement_begin_α:
                                                                                        jmp   n626_statement_end_α
n625_statement_begin_β:
                                                                                        jmp   n627_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n626_statement_end_α:
                                                                                        jmp   n627_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n627_statement_begin_α:
                                                                                        jmp   n628_statement_end_α
n627_statement_begin_β:
                                                                                        jmp   n629_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n628_statement_end_α:
                                                                                        jmp   n629_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n629_statement_begin_α:
                                                                                        jmp   n630_statement_end_α
n629_statement_begin_β:
                                                                                        jmp   n631_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n630_statement_end_α:
                                                                                        jmp   n631_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n631_statement_begin_α:
                                                                                        jmp   n632_statement_end_α
n631_statement_begin_β:
                                                                                        jmp   n633_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n632_statement_end_α:
                                                                                        jmp   n633_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n633_statement_begin_α:
                                                                                        jmp   n634_statement_end_α
n633_statement_begin_β:
                                                                                        jmp   n635_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n634_statement_end_α:
                                                                                        jmp   n635_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n635_statement_begin_α:
                                                                                        jmp   n636_statement_end_α
n635_statement_begin_β:
                                                                                        jmp   n637_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n636_statement_end_α:
                                                                                        jmp   n637_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n637_statement_begin_α:
                                                                                        jmp   n638_statement_end_α
n637_statement_begin_β:
                                                                                        jmp   n639_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n638_statement_end_α:
                                                                                        jmp   n639_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n639_statement_begin_α:
                                                                                        jmp   n640_statement_end_α
n639_statement_begin_β:
                                                                                        jmp   n641_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n640_statement_end_α:
                                                                                        jmp   n641_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n641_statement_begin_α:
                                                                                        jmp   n642_statement_end_α
n641_statement_begin_β:
                                                                                        jmp   n643_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n642_statement_end_α:
                                                                                        jmp   n643_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n643_statement_begin_α:
                                                                                        jmp   n644_statement_end_α
n643_statement_begin_β:
                                                                                        jmp   n645_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n644_statement_end_α:
                                                                                        jmp   n645_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n645_statement_begin_α:
                                                                                        jmp   n646_statement_end_α
n645_statement_begin_β:
                                                                                        jmp   n647_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n646_statement_end_α:
                                                                                        jmp   n647_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n647_statement_begin_α:
                                                                                        jmp   n648_statement_end_α
n647_statement_begin_β:
                                                                                        jmp   n649_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n648_statement_end_α:
                                                                                        jmp   n649_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n649_statement_begin_α:
                                                                                        jmp   n650_statement_end_α
n649_statement_begin_β:
                                                                                        jmp   n651_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n650_statement_end_α:
                                                                                        jmp   n651_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n651_statement_begin_α:
                                                                                        jmp   n652_statement_end_α
n651_statement_begin_β:
                                                                                        jmp   n653_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n652_statement_end_α:
                                                                                        jmp   n653_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n653_statement_begin_α:
                                                                                        jmp   n654_statement_end_α
n653_statement_begin_β:
                                                                                        jmp   n655_statement_begin_α
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
                                                                                        jmp   n658_statement_end_α
n657_statement_begin_β:
                                                                                        jmp   n659_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n658_statement_end_α:
                                                                                        jmp   n659_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n659_statement_begin_α:
                                                                                        jmp   n660_statement_end_α
n659_statement_begin_β:
                                                                                        jmp   n661_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n660_statement_end_α:
                                                                                        jmp   n661_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n661_statement_begin_α:
                                                                                        jmp   n662_statement_end_α
n661_statement_begin_β:
                                                                                        jmp   n663_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n662_statement_end_α:
                                                                                        jmp   n663_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n663_statement_begin_α:
                                                                                        jmp   n664_statement_end_α
n663_statement_begin_β:
                                                                                        jmp   n665_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n664_statement_end_α:
                                                                                        jmp   n665_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n665_statement_begin_α:
                                                                                        jmp   n666_lit_string_α
n665_statement_begin_β:
                                                                                        jmp   n670_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n666_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx927_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n667_lit_integer_α
.Lx927_0:
                        .quad            .Lx927_0_s
.Lx927_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n667_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx928_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n668_call_α
n667_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n665_statement_begin_β
.Lx928_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n668_call_α:
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
.Lrkfnzd930:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd930]
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
                                                                                        jne   .Lx929_240
                        add              rsp, 16
                                                                                        jmp   n667_lit_integer_β
.Lx929_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n669_statement_end_α
n668_call_β:
                        add              rsp, 16
                                                                                        jmp   n667_lit_integer_β
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
                                                                                        jmp   n673_lit_string_α
n672_statement_begin_β:
                                                                                        jmp   n679_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n673_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx939_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n674_lit_integer_α
n673_lit_string_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n672_statement_begin_β
.Lx939_0:
                        .quad            .Lx939_0_s
.Lx939_0_s:
                        .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n674_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx940_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n675_lit_string_α
n674_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n673_lit_string_β
.Lx940_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n675_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx941_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n676_call_α
n675_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n674_lit_integer_β
.Lx941_0:
                        .quad            .Lx941_0_s
.Lx941_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n676_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 48]                      # lit_string
                        mov              rdx, qword ptr [rsp + 56]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 2
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [r9 + 0]                        # pad_left
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx943_0]
                        mov              esi, 3
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx943_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
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
                        lea              rcx, [rip + .Lx943_6]
                        lea              rdx, [rip + .Lx943_7]
                                                                                        jmp   rax
.Lx943_6:
                        mov              rdi, qword ptr [r9 + 0]                        # pad_left
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx943_2
.Lx943_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx943_2
.Lx943_5:
                        add              rsp, 64
.Lx943_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx943_240
                        add              rsp, 16
                                                                                        jmp   n675_lit_string_β
.Lx943_240:
                                                                                        jmp   n677_assign_α
n676_call_β:
                                                                                        jmp   n675_lit_string_β
.Lx943_0:
                        .quad            .Lx943_0_s
.Lx943_0_s:
                        .string          "pad_left"
#-----------------------------------------------------------------------------------------------------------------------
n677_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx944_0]
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
                                                                                        jmp   n678_statement_end_α
.Lx944_0:
                        .quad            .Lx944_0_s
.Lx944_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n678_statement_end_α:
                                                                                        jmp   n679_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n679_statement_begin_α:
                                                                                        jmp   n680_lit_string_α
n679_statement_begin_β:
                                                                                        jmp   n686_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n680_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx949_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n681_lit_integer_α
n680_lit_string_β:
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n679_statement_begin_β
.Lx949_0:
                        .quad            .Lx949_0_s
.Lx949_0_s:
                        .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n681_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx950_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n682_lit_string_α
n681_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n680_lit_string_β
.Lx950_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n682_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx951_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n683_call_α
n682_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n681_lit_integer_β
.Lx951_0:
                        .quad            .Lx951_0_s
.Lx951_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n683_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 48]                      # lit_string
                        mov              rdx, qword ptr [rsp + 56]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 2
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [r9 + 64]                       # pad_right
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx953_0]
                        mov              esi, 3
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx953_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
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
                        lea              rcx, [rip + .Lx953_6]
                        lea              rdx, [rip + .Lx953_7]
                                                                                        jmp   rax
.Lx953_6:
                        mov              rdi, qword ptr [r9 + 64]                       # pad_right
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx953_2
.Lx953_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx953_2
.Lx953_5:
                        add              rsp, 64
.Lx953_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx953_240
                        add              rsp, 16
                                                                                        jmp   n682_lit_string_β
.Lx953_240:
                                                                                        jmp   n684_assign_α
n683_call_β:
                                                                                        jmp   n682_lit_string_β
.Lx953_0:
                        .quad            .Lx953_0_s
.Lx953_0_s:
                        .string          "pad_right"
#-----------------------------------------------------------------------------------------------------------------------
n684_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx954_0]
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
                                                                                        jmp   n685_statement_end_α
.Lx954_0:
                        .quad            .Lx954_0_s
.Lx954_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n685_statement_end_α:
                                                                                        jmp   n686_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n686_statement_begin_α:
                                                                                        jmp   n687_lit_string_α
n686_statement_begin_β:
                                                                                        jmp   n691_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n687_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx959_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n688_call_α
n687_lit_string_β:
                        add              rsp, 16
                        add              rsp, 176
                                                                                        jmp   n686_statement_begin_β
.Lx959_0:
                        .quad            .Lx959_0_s
.Lx959_0_s:
                        .string          "   hello"
#-----------------------------------------------------------------------------------------------------------------------
n688_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [r9 + 80]                       # ltrim
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx961_0]
                        mov              esi, 3
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx961_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
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
                        lea              rcx, [rip + .Lx961_6]
                        lea              rdx, [rip + .Lx961_7]
                                                                                        jmp   rax
.Lx961_6:
                        mov              rdi, qword ptr [r9 + 80]                       # ltrim
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx961_2
.Lx961_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax                       # ltrim
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx961_2
.Lx961_5:
                        add              rsp, 64
.Lx961_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx961_240
                        add              rsp, 16
                                                                                        jmp   n687_lit_string_β
.Lx961_240:
                                                                                        jmp   n689_assign_α
n688_call_β:
                                                                                        jmp   n687_lit_string_β
.Lx961_0:
                        .quad            .Lx961_0_s
.Lx961_0_s:
                        .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n689_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx962_0]
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
                                                                                        jmp   n690_statement_end_α
.Lx962_0:
                        .quad            .Lx962_0_s
.Lx962_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n690_statement_end_α:
                                                                                        jmp   n691_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n691_statement_begin_α:
                                                                                        jmp   n692_lit_string_α
n691_statement_begin_β:
                                                                                        jmp   n696_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n692_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx967_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n693_call_α
n692_lit_string_β:
                        add              rsp, 16
                        add              rsp, 208
                                                                                        jmp   n691_statement_begin_β
.Lx967_0:
                        .quad            .Lx967_0_s
.Lx967_0_s:
                        .string          "hello   "
#-----------------------------------------------------------------------------------------------------------------------
n693_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 80
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [r9 + 128]                      # rtrim
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 72], rax
                        mov              rdi, qword ptr [rip + .Lx969_0]
                        mov              esi, 4
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx969_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
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
                        lea              rcx, [rip + .Lx969_6]
                        lea              rdx, [rip + .Lx969_7]
                                                                                        jmp   rax
.Lx969_6:
                        mov              rdi, qword ptr [r9 + 128]                      # rtrim
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 80
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx969_2
.Lx969_7:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 80
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx969_2
.Lx969_5:
                        add              rsp, 80
.Lx969_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx969_240
                        add              rsp, 16
                                                                                        jmp   n692_lit_string_β
.Lx969_240:
                                                                                        jmp   n694_assign_α
n693_call_β:
                                                                                        jmp   n692_lit_string_β
.Lx969_0:
                        .quad            .Lx969_0_s
.Lx969_0_s:
                        .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n694_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx970_0]
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
                                                                                        jmp   n695_statement_end_α
.Lx970_0:
                        .quad            .Lx970_0_s
.Lx970_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n695_statement_end_α:
                                                                                        jmp   n696_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n696_statement_begin_α:
                                                                                        jmp   n697_lit_string_α
n696_statement_begin_β:
                                                                                        jmp   n701_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n697_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 9
                        mov              rax, qword ptr [rip + .Lx975_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n698_call_α
n697_lit_string_β:
                        add              rsp, 16
                        add              rsp, 240
                                                                                        jmp   n696_statement_begin_β
.Lx975_0:
                        .quad            .Lx975_0_s
.Lx975_0_s:
                        .string          "  hello  "
#-----------------------------------------------------------------------------------------------------------------------
n698_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 176]                      # trimws
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx977_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx977_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
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
                        lea              rcx, [rip + .Lx977_6]
                        lea              rdx, [rip + .Lx977_7]
                                                                                        jmp   rax
.Lx977_6:
                        mov              rdi, qword ptr [r9 + 176]                      # trimws
                        mov              rsi, qword ptr [r9 + 184]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 176], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 184], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx977_2
.Lx977_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 176], rax                      # trimws
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 184], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx977_2
.Lx977_5:
                        add              rsp, 32
.Lx977_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx977_240
                        add              rsp, 16
                                                                                        jmp   n697_lit_string_β
.Lx977_240:
                                                                                        jmp   n699_assign_α
n698_call_β:
                                                                                        jmp   n697_lit_string_β
.Lx977_0:
                        .quad            .Lx977_0_s
.Lx977_0_s:
                        .string          "trimws"
#-----------------------------------------------------------------------------------------------------------------------
n699_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx978_0]
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
                                                                                        jmp   n700_statement_end_α
.Lx978_0:
                        .quad            .Lx978_0_s
.Lx978_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n700_statement_end_α:
                                                                                        jmp   n701_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n701_statement_begin_α:
                                                                                        jmp   n702_lit_string_α
n701_statement_begin_β:
                                                                                        jmp   n707_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n702_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx983_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n703_lit_integer_α
n702_lit_string_β:
                        add              rsp, 16
                        add              rsp, 272
                                                                                        jmp   n701_statement_begin_β
.Lx983_0:
                        .quad            .Lx983_0_s
.Lx983_0_s:
                        .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n703_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx984_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n704_call_α
n703_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n702_lit_string_β
.Lx984_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n704_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_string
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 48
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 192]                      # repeat
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx986_0]
                        mov              esi, 2
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx986_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
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
                        lea              rcx, [rip + .Lx986_6]
                        lea              rdx, [rip + .Lx986_7]
                                                                                        jmp   rax
.Lx986_6:
                        mov              rdi, qword ptr [r9 + 192]                      # repeat
                        mov              rsi, qword ptr [r9 + 200]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 192], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 200], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx986_2
.Lx986_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 192], rax                      # repeat
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 200], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx986_2
.Lx986_5:
                        add              rsp, 48
.Lx986_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx986_240
                        add              rsp, 16
                                                                                        jmp   n703_lit_integer_β
.Lx986_240:
                                                                                        jmp   n705_assign_α
n704_call_β:
                                                                                        jmp   n703_lit_integer_β
.Lx986_0:
                        .quad            .Lx986_0_s
.Lx986_0_s:
                        .string          "repeat"
#-----------------------------------------------------------------------------------------------------------------------
n705_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx987_0]
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
                                                                                        jmp   n706_statement_end_α
.Lx987_0:
                        .quad            .Lx987_0_s
.Lx987_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n706_statement_end_α:
                                                                                        jmp   n707_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n707_statement_begin_α:
                                                                                        jmp   n708_statement_end_α
n707_statement_begin_β:
                                                                                        jmp   n709_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n708_statement_end_α:
                                                                                        jmp   n709_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n709_statement_begin_α:
                                                                                        jmp   n710_lit_string_α
n709_statement_begin_β:
                        add              rsp, 320
                                                                                        jmp   n718_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n710_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx996_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n711_lit_string_α
n710_lit_string_β:
                        add              rsp, 16
                        add              rsp, 320
                                                                                        jmp   n709_statement_begin_β
.Lx996_0:
                        .quad            .Lx996_0_s
.Lx996_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n711_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx997_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n712_call_α
n711_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n710_lit_string_β
.Lx997_0:
                        .quad            .Lx997_0_s
.Lx997_0_s:
                        .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n712_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_string
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 48
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 208]                      # contains
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx999_0]
                        mov              esi, 2
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx999_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
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
                        lea              rcx, [rip + .Lx999_6]
                        lea              rdx, [rip + .Lx999_7]
                                                                                        jmp   rax
.Lx999_6:
                        mov              rdi, qword ptr [r9 + 208]                      # contains
                        mov              rsi, qword ptr [r9 + 216]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 208], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 216], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx999_2
.Lx999_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 208], rax                      # contains
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 216], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx999_2
.Lx999_5:
                        add              rsp, 48
.Lx999_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx999_240
                        add              rsp, 16
                                                                                        jmp   n711_lit_string_β
.Lx999_240:
                                                                                        jmp   n713_statement_end_α
n712_call_β:
                                                                                        jmp   n711_lit_string_β
.Lx999_0:
                        .quad            .Lx999_0_s
.Lx999_0_s:
                        .string          "contains"
#-----------------------------------------------------------------------------------------------------------------------
n713_statement_end_α:
                                                                                        jmp   n714_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n714_statement_begin_α:
                                                                                        jmp   n715_lit_string_α
n714_statement_begin_β:
                                                                                        jmp   n722_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n715_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx1004_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n716_assign_α
.Lx1004_0:
                        .quad            .Lx1004_0_s
.Lx1004_0_s:
                        .string          "contains ok"
#-----------------------------------------------------------------------------------------------------------------------
n716_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1005_0]
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
                                                                                        jmp   n717_statement_end_α
.Lx1005_0:
                        .quad            .Lx1005_0_s
.Lx1005_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n717_statement_end_α:
                                                                                        jmp   n722_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n718_statement_begin_α:
                                                                                        jmp   n719_lit_string_α
n718_statement_begin_β:
                                                                                        jmp   n722_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n719_lit_string_α:
                        mov              qword ptr [rsp + 4112], 2                      # result
                        mov              dword ptr [rsp + 4116], 14
                        mov              rax, qword ptr [rip + .Lx1010_0]
                        mov              qword ptr [rsp + 4120], rax
                                                                                        jmp   n720_assign_α
.Lx1010_0:
                        .quad            .Lx1010_0_s
.Lx1010_0_s:
                        .string          "FAIL: contains"
#-----------------------------------------------------------------------------------------------------------------------
n720_assign_α:
                        mov              rsi, qword ptr [rsp + 4112]
                        mov              rdx, qword ptr [rsp + 4120]
                        mov              rdi, qword ptr [rip + .Lx1011_0]
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
                                                                                        jmp   n721_statement_end_α
.Lx1011_0:
                        .quad            .Lx1011_0_s
.Lx1011_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n721_statement_end_α:
                                                                                        jmp   n722_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n722_statement_begin_α:
                                                                                        jmp   n723_statement_end_α
n722_statement_begin_β:
                                                                                        jmp   n724_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n723_statement_end_α:
                                                                                        jmp   n724_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n724_statement_begin_α:
                                                                                        jmp   n725_lit_string_α
n724_statement_begin_β:
                        add              rsp, 384
                                                                                        jmp   n733_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n725_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1020_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n726_lit_string_α
n725_lit_string_β:
                        add              rsp, 16
                        add              rsp, 384
                                                                                        jmp   n724_statement_begin_β
.Lx1020_0:
                        .quad            .Lx1020_0_s
.Lx1020_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n726_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1021_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n727_call_α
n726_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n725_lit_string_β
.Lx1021_0:
                        .quad            .Lx1021_0_s
.Lx1021_0_s:
                        .string          "foo"
#-----------------------------------------------------------------------------------------------------------------------
n727_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_string
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 48
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 240]                      # startswith
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx1023_0]
                        mov              esi, 2
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx1023_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
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
                        lea              rcx, [rip + .Lx1023_6]
                        lea              rdx, [rip + .Lx1023_7]
                                                                                        jmp   rax
.Lx1023_6:
                        mov              rdi, qword ptr [r9 + 240]                      # startswith
                        mov              rsi, qword ptr [r9 + 248]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 240], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 248], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1023_2
.Lx1023_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 240], rax                      # startswith
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 248], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1023_2
.Lx1023_5:
                        add              rsp, 48
.Lx1023_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1023_240
                        add              rsp, 16
                                                                                        jmp   n726_lit_string_β
.Lx1023_240:
                                                                                        jmp   n728_statement_end_α
n727_call_β:
                                                                                        jmp   n726_lit_string_β
.Lx1023_0:
                        .quad            .Lx1023_0_s
.Lx1023_0_s:
                        .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n728_statement_end_α:
                                                                                        jmp   n729_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n729_statement_begin_α:
                                                                                        jmp   n730_lit_string_α
n729_statement_begin_β:
                                                                                        jmp   n737_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n730_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx1028_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n731_assign_α
.Lx1028_0:
                        .quad            .Lx1028_0_s
.Lx1028_0_s:
                        .string          "startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n731_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1029_0]
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
                                                                                        jmp   n732_statement_end_α
.Lx1029_0:
                        .quad            .Lx1029_0_s
.Lx1029_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n732_statement_end_α:
                                                                                        jmp   n737_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n733_statement_begin_α:
                                                                                        jmp   n734_lit_string_α
n733_statement_begin_β:
                                                                                        jmp   n737_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n734_lit_string_α:
                        mov              qword ptr [rsp + 4240], 2                      # result
                        mov              dword ptr [rsp + 4244], 16
                        mov              rax, qword ptr [rip + .Lx1034_0]
                        mov              qword ptr [rsp + 4248], rax
                                                                                        jmp   n735_assign_α
.Lx1034_0:
                        .quad            .Lx1034_0_s
.Lx1034_0_s:
                        .string          "FAIL: startswith"
#-----------------------------------------------------------------------------------------------------------------------
n735_assign_α:
                        mov              rsi, qword ptr [rsp + 4240]
                        mov              rdx, qword ptr [rsp + 4248]
                        mov              rdi, qword ptr [rip + .Lx1035_0]
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
                                                                                        jmp   n736_statement_end_α
.Lx1035_0:
                        .quad            .Lx1035_0_s
.Lx1035_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n736_statement_end_α:
                                                                                        jmp   n737_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n737_statement_begin_α:
                                                                                        jmp   n738_statement_end_α
n737_statement_begin_β:
                                                                                        jmp   n739_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n738_statement_end_α:
                                                                                        jmp   n739_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n739_statement_begin_α:
                                                                                        jmp   n740_lit_string_α
n739_statement_begin_β:
                        add              rsp, 448
                                                                                        jmp   n748_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n740_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1044_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n741_lit_string_α
n740_lit_string_β:
                        add              rsp, 16
                        add              rsp, 448
                                                                                        jmp   n739_statement_begin_β
.Lx1044_0:
                        .quad            .Lx1044_0_s
.Lx1044_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n741_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1045_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n742_call_α
n741_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n740_lit_string_β
.Lx1045_0:
                        .quad            .Lx1045_0_s
.Lx1045_0_s:
                        .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n742_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_string
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 48
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 256]                      # endswith
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 264]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx1047_0]
                        mov              esi, 2
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx1047_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
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
                        lea              rcx, [rip + .Lx1047_6]
                        lea              rdx, [rip + .Lx1047_7]
                                                                                        jmp   rax
.Lx1047_6:
                        mov              rdi, qword ptr [r9 + 256]                      # endswith
                        mov              rsi, qword ptr [r9 + 264]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 256], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 264], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1047_2
.Lx1047_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 256], rax                      # endswith
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 264], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1047_2
.Lx1047_5:
                        add              rsp, 48
.Lx1047_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1047_240
                        add              rsp, 16
                                                                                        jmp   n741_lit_string_β
.Lx1047_240:
                                                                                        jmp   n743_statement_end_α
n742_call_β:
                                                                                        jmp   n741_lit_string_β
.Lx1047_0:
                        .quad            .Lx1047_0_s
.Lx1047_0_s:
                        .string          "endswith"
#-----------------------------------------------------------------------------------------------------------------------
n743_statement_end_α:
                                                                                        jmp   n744_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n744_statement_begin_α:
                                                                                        jmp   n745_lit_string_α
n744_statement_begin_β:
                                                                                        jmp   n752_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n745_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx1052_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n746_assign_α
.Lx1052_0:
                        .quad            .Lx1052_0_s
.Lx1052_0_s:
                        .string          "endswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n746_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1053_0]
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
                                                                                        jmp   n747_statement_end_α
.Lx1053_0:
                        .quad            .Lx1053_0_s
.Lx1053_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n747_statement_end_α:
                                                                                        jmp   n752_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n748_statement_begin_α:
                                                                                        jmp   n749_lit_string_α
n748_statement_begin_β:
                                                                                        jmp   n752_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n749_lit_string_α:
                        mov              qword ptr [rsp + 4368], 2                      # result
                        mov              dword ptr [rsp + 4372], 14
                        mov              rax, qword ptr [rip + .Lx1058_0]
                        mov              qword ptr [rsp + 4376], rax
                                                                                        jmp   n750_assign_α
.Lx1058_0:
                        .quad            .Lx1058_0_s
.Lx1058_0_s:
                        .string          "FAIL: endswith"
#-----------------------------------------------------------------------------------------------------------------------
n750_assign_α:
                        mov              rsi, qword ptr [rsp + 4368]
                        mov              rdx, qword ptr [rsp + 4376]
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
                                                                                        jmp   n752_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n752_statement_begin_α:
                                                                                        jmp   n753_statement_end_α
n752_statement_begin_β:
                                                                                        jmp   n754_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n753_statement_end_α:
                                                                                        jmp   n754_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n754_statement_begin_α:
                                                                                        jmp   n755_lit_string_α
n754_statement_begin_β:
                        add              rsp, 512
                                                                                        jmp   n763_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n755_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1068_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n756_lit_string_α
n755_lit_string_β:
                        add              rsp, 16
                        add              rsp, 512
                                                                                        jmp   n754_statement_begin_β
.Lx1068_0:
                        .quad            .Lx1068_0_s
.Lx1068_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n756_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1069_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n757_call_α
n756_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n755_lit_string_β
.Lx1069_0:
                        .quad            .Lx1069_0_s
.Lx1069_0_s:
                        .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n757_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_string
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 48
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 240]                      # startswith
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx1071_0]
                        mov              esi, 2
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx1071_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
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
                        lea              rcx, [rip + .Lx1071_6]
                        lea              rdx, [rip + .Lx1071_7]
                                                                                        jmp   rax
.Lx1071_6:
                        mov              rdi, qword ptr [r9 + 240]                      # startswith
                        mov              rsi, qword ptr [r9 + 248]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 240], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 248], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1071_2
.Lx1071_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 240], rax                      # startswith
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 248], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1071_2
.Lx1071_5:
                        add              rsp, 48
.Lx1071_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1071_240
                        add              rsp, 16
                                                                                        jmp   n756_lit_string_β
.Lx1071_240:
                                                                                        jmp   n758_statement_end_α
n757_call_β:
                                                                                        jmp   n756_lit_string_β
.Lx1071_0:
                        .quad            .Lx1071_0_s
.Lx1071_0_s:
                        .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n758_statement_end_α:
                                                                                        jmp   n759_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n759_statement_begin_α:
                                                                                        jmp   n760_lit_string_α
n759_statement_begin_β:
                                                                                        jmp   n767_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n760_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx1076_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n761_assign_α
.Lx1076_0:
                        .quad            .Lx1076_0_s
.Lx1076_0_s:
                        .string          "FAIL: startswith matched wrong"
#-----------------------------------------------------------------------------------------------------------------------
n761_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1077_0]
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
                                                                                        jmp   n762_statement_end_α
.Lx1077_0:
                        .quad            .Lx1077_0_s
.Lx1077_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n762_statement_end_α:
                                                                                        jmp   n767_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n763_statement_begin_α:
                                                                                        jmp   n764_lit_string_α
n763_statement_begin_β:
                                                                                        jmp   n767_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n764_lit_string_α:
                        mov              qword ptr [rsp + 4480], 2                      # result
                        mov              dword ptr [rsp + 4484], 16
                        mov              rax, qword ptr [rip + .Lx1082_0]
                        mov              qword ptr [rsp + 4488], rax
                                                                                        jmp   n765_assign_α
.Lx1082_0:
                        .quad            .Lx1082_0_s
.Lx1082_0_s:
                        .string          "no startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n765_assign_α:
                        mov              rsi, qword ptr [rsp + 4480]
                        mov              rdx, qword ptr [rsp + 4488]
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
                                                                                        jmp   n767_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n767_statement_begin_α:
                                                                                        jmp   n768_statement_end_α
n767_statement_begin_β:
                                                                                        jmp   n769_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n768_statement_end_α:
                                                                                        jmp   n769_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n769_statement_begin_α:
                                                                                        jmp   n770_lit_string_α
n769_statement_begin_β:
                                                                                        jmp   n775_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n770_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1092_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n771_lit_string_α
n770_lit_string_β:
                        add              rsp, 16
                        add              rsp, 576
                                                                                        jmp   n769_statement_begin_β
.Lx1092_0:
                        .quad            .Lx1092_0_s
.Lx1092_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n771_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1093_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n772_call_α
n771_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n770_lit_string_β
.Lx1093_0:
                        .quad            .Lx1093_0_s
.Lx1093_0_s:
                        .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n772_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_string
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [r9 + 272]                      # index
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 280]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx1095_0]
                        mov              esi, 3
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx1095_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
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
                        lea              rcx, [rip + .Lx1095_6]
                        lea              rdx, [rip + .Lx1095_7]
                                                                                        jmp   rax
.Lx1095_6:
                        mov              rdi, qword ptr [r9 + 272]                      # index
                        mov              rsi, qword ptr [r9 + 280]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 272], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 296], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1095_2
.Lx1095_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 296], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1095_2
.Lx1095_5:
                        add              rsp, 64
.Lx1095_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1095_240
                        add              rsp, 16
                                                                                        jmp   n771_lit_string_β
.Lx1095_240:
                                                                                        jmp   n773_assign_α
n772_call_β:
                                                                                        jmp   n771_lit_string_β
.Lx1095_0:
                        .quad            .Lx1095_0_s
.Lx1095_0_s:
                        .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n773_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1096_0]
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
                                                                                        jmp   n774_statement_end_α
.Lx1096_0:
                        .quad            .Lx1096_0_s
.Lx1096_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n774_statement_end_α:
                                                                                        jmp   n775_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n775_statement_begin_α:
                                                                                        jmp   n776_lit_string_α
n775_statement_begin_β:
                        add              rsp, 624
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n776_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1101_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n777_lit_string_α
n776_lit_string_β:
                        add              rsp, 16
                        add              rsp, 624
                                                                                        jmp   n775_statement_begin_β
.Lx1101_0:
                        .quad            .Lx1101_0_s
.Lx1101_0_s:
                        .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n777_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1102_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n778_call_α
n777_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n776_lit_string_β
.Lx1102_0:
                        .quad            .Lx1102_0_s
.Lx1102_0_s:
                        .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
n778_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_string
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [r9 + 272]                      # index
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 280]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx1104_0]
                        mov              esi, 3
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx1104_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
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
                        lea              rcx, [rip + .Lx1104_6]
                        lea              rdx, [rip + .Lx1104_7]
                                                                                        jmp   rax
.Lx1104_6:
                        mov              rdi, qword ptr [r9 + 272]                      # index
                        mov              rsi, qword ptr [r9 + 280]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 272], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 296], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1104_2
.Lx1104_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 296], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 232], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 64
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx1104_2
.Lx1104_5:
                        add              rsp, 64
.Lx1104_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1104_240
                        add              rsp, 16
                                                                                        jmp   n777_lit_string_β
.Lx1104_240:
                                                                                        jmp   n779_assign_α
n778_call_β:
                                                                                        jmp   n777_lit_string_β
.Lx1104_0:
                        .quad            .Lx1104_0_s
.Lx1104_0_s:
                        .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n779_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1105_0]
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
                                                                                        jmp   n780_statement_end_α
.Lx1105_0:
                        .quad            .Lx1105_0_s
.Lx1105_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n780_statement_end_α:
                        add              rsp, 672
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n781_goto_α:
                                                                                        jmp   n625_statement_begin_α
n781_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n782_goto_α:
                                                                                        jmp   n629_statement_begin_α
n782_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n783_goto_α:
                                                                                        jmp   n633_statement_begin_α
n783_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n784_goto_α:
                                                                                        jmp   n785_statement_begin_α
n784_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n785_statement_begin_α:
                                                                                        jmp   n786_var_α
n785_statement_begin_β:
                                                                                        jmp   n792_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n786_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1872], rax                    # result
                        mov              qword ptr [rsp + 1880], rdx
                                                                                        jmp   n787_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n787_lit_integer_α:
                        mov              qword ptr [rsp + 1856], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1115_0]
                        mov              qword ptr [rsp + 1864], rax
                                                                                        jmp   n788_coerce_numeric_α
.Lx1115_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n788_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 1872]
                        cmp              eax, 5
                                                                                        je    .Lx1117_1
                        cmp              eax, 3
                                                                                        jne   .Lx1117_0
                        mov              eax, dword ptr [rsp + 1856]
                        cmp              eax, 3
                                                                                        jne   .Lx1117_0
.Lx1117_1:
                        mov              rax, qword ptr [rsp + 1872]
                        mov              qword ptr [rsp + 1840], rax
                        mov              rax, qword ptr [rsp + 1880]
                        mov              qword ptr [rsp + 1848], rax
                                                                                        jmp   n789_coerce_numeric_α
.Lx1117_0:
                        lea              rdi, [rsp + 1872]
                        lea              rsi, [rsp + 1856]
                        lea              rdx, [rsp + 1840]
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
                                                                                        jmp   n789_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n789_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 1856]
                        cmp              eax, 5
                                                                                        je    .Lx1119_1
                        cmp              eax, 3
                                                                                        jne   .Lx1119_0
                        mov              eax, dword ptr [rsp + 1872]
                        cmp              eax, 3
                                                                                        jne   .Lx1119_0
.Lx1119_1:
                        mov              rax, qword ptr [rsp + 1856]
                        mov              qword ptr [rsp + 1824], rax
                        mov              rax, qword ptr [rsp + 1864]
                        mov              qword ptr [rsp + 1832], rax
                                                                                        jmp   n790_cmp_test_α
.Lx1119_0:
                        lea              rdi, [rsp + 1856]
                        lea              rsi, [rsp + 1872]
                        lea              rdx, [rsp + 1824]
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
                                                                                        jmp   n790_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n790_cmp_test_α:
                        lea              rdi, [rsp + 1840]
                        lea              rsi, [rsp + 1824]
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
                                                                                        jg    n785_statement_begin_β
                                                                                        jmp   n791_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n791_statement_end_α:
                                                                                        jmp   n813_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n792_statement_begin_α:
                                                                                        jmp   n793_var_α
n792_statement_begin_β:
                                                                                        jmp   n799_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n793_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 1968], rax                    # result
                        mov              qword ptr [rsp + 1976], rdx
                                                                                        jmp   n794_var_α
#-----------------------------------------------------------------------------------------------------------------------
n794_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1984], rax                    # result
                        mov              qword ptr [rsp + 1992], rdx
                                                                                        jmp   n795_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n795_lit_integer_α:
                        mov              qword ptr [rsp + 2000], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1128_0]
                        mov              qword ptr [rsp + 2008], rax
                                                                                        jmp   n796_call_α
.Lx1128_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n796_call_α:
                        mov              rax, qword ptr [rsp + 2000]
                        mov              qword ptr [rsp + 1936], rax
                        mov              rax, qword ptr [rsp + 2008]
                        mov              qword ptr [rsp + 1944], rax
                        mov              rax, qword ptr [rsp + 1984]
                        mov              qword ptr [rsp + 1920], rax
                        mov              rax, qword ptr [rsp + 1992]
                        mov              qword ptr [rsp + 1928], rax
                        mov              rax, qword ptr [rsp + 1968]
                        mov              qword ptr [rsp + 1904], rax
                        mov              rax, qword ptr [rsp + 1976]
                        mov              qword ptr [rsp + 1912], rax
                        .section         .rodata
.Lrkfn1130:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1130]
                        lea              rsi, [rsp + 1904]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                        cmp              eax, 104
                                                                                        je    n792_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n797_assign_α
n796_call_β:
                                                                                        jmp   n792_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n797_assign_α:
                        mov              rax, qword ptr [rsp + 1888]
                        mov              rdx, qword ptr [rsp + 1896]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              qword ptr [r9 + 168], rdx
                                                                                        jmp   n798_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n798_statement_end_α:
                                                                                        jmp   n799_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n799_statement_begin_α:
                                                                                        jmp   n800_var_α
n799_statement_begin_β:
                                                                                        jmp   n813_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n800_var_α:
                        mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 2144], rax                    # result
                        mov              qword ptr [rsp + 2152], rdx
                                                                                        jmp   n801_var_α
#-----------------------------------------------------------------------------------------------------------------------
n801_var_α:
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 2128], rax                    # result
                        mov              qword ptr [rsp + 2136], rdx
                                                                                        jmp   n802_coerce_string_α
#-----------------------------------------------------------------------------------------------------------------------
n802_coerce_string_α:
                        lea              rdi, [rsp + 2128]
                        lea              rsi, [rsp + 2112]
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
                                                                                        jmp   n803_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n803_match_begin_α:
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rsp                      # old____
                        mov              rdi, qword ptr [rsp + 2144]
                        mov              rsi, qword ptr [rsp + 2152]
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
.Lx1141_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n804_match_any_α
n803_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx1141_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx1141_1
                                                                                        jmp   .Lx1141_0
.Lx1141_1:
n803_match_begin_af:
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
                                                                                        jmp   n813_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n804_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n803_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rsp + 2120]
                        mov              r9d, dword ptr [rsp + 2116]
                        mov              edx, 0
.Lx1143_5:
                        cmp              edx, r9d
                                                                                        jge   n803_match_begin_β
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx1143_6
                        add              edx, 1
                                                                                        jmp   .Lx1143_5
.Lx1143_6:
                        add              r14d, 1
                                                                                        jmp   n805_match_end_α
n804_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n803_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n805_match_end_α:
                        mov              r8, r12
.Lx1145_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx1145_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx1145_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx1145_5
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
.Lx1145_1:
                        test             rax, rax
                                                                                        je    .Lx1145_2
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
                        lea              rcx, [rip + .Lx1145_3]
                        lea              rdx, [rip + .Lx1145_4]
                                                                                        jmp   rax
.Lx1145_3:
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
                                                                                        jmp   .Lx1145_1
.Lx1145_4:
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
                                                                                        jmp   .Lx1145_1
.Lx1145_2:
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
.Lx1145_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx1145_10
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
                                                                                        jmp   n806_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n806_statement_end_α:
                                                                                        jmp   n807_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n807_statement_begin_α:
                                                                                        jmp   n808_var_α
n807_statement_begin_β:
                                                                                        jmp   n785_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n808_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2176], rax                    # result
                        mov              qword ptr [rsp + 2184], rdx
                                                                                        jmp   n809_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n809_lit_integer_α:
                        mov              qword ptr [rsp + 2192], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1151_0]
                        mov              qword ptr [rsp + 2200], rax
                                                                                        jmp   n810_binop_α
.Lx1151_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n810_binop_α:
                        mov              eax, dword ptr [rsp + 2176]
                        cmp              eax, 3
                                                                                        jne   .Lx1152_0
                        mov              rax, qword ptr [rsp + 2184]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2160], 3
                        mov              qword ptr [rsp + 2168], rax
                                                                                        jmp   n811_assign_α
.Lx1152_0:
                        mov              rdi, qword ptr [rsp + 2176]
                        mov              rsi, qword ptr [rsp + 2184]
                        mov              rdx, qword ptr [rsp + 2192]
                        mov              rcx, qword ptr [rsp + 2200]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1152_240
                        add              rsp, 32
                                                                                        jmp   n807_statement_begin_β
.Lx1152_240:
                        mov              qword ptr [rsp + 2160], rax
                        mov              qword ptr [rsp + 2168], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n811_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n811_assign_α:
                        mov              rax, qword ptr [rsp + 2160]
                        mov              rdx, qword ptr [rsp + 2168]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n812_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n812_statement_end_α:
                                                                                        jmp   n785_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n813_statement_begin_α:
                                                                                        jmp   n814_var_α
n813_statement_begin_β:
                                                                                        jmp   n820_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n814_var_α:
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2288], rax                    # result
                        mov              qword ptr [rsp + 2296], rdx
                                                                                        jmp   n815_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n815_lit_integer_α:
                        mov              qword ptr [rsp + 2304], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1159_0]
                        mov              qword ptr [rsp + 2312], rax
                                                                                        jmp   n816_var_α
.Lx1159_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n816_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2320], rax                    # result
                        mov              qword ptr [rsp + 2328], rdx
                                                                                        jmp   n817_call_α
#-----------------------------------------------------------------------------------------------------------------------
n817_call_α:
                        mov              rax, qword ptr [rsp + 2320]
                        mov              qword ptr [rsp + 2256], rax
                        mov              rax, qword ptr [rsp + 2328]
                        mov              qword ptr [rsp + 2264], rax
                        mov              rax, qword ptr [rsp + 2304]
                        mov              qword ptr [rsp + 2240], rax
                        mov              rax, qword ptr [rsp + 2312]
                        mov              qword ptr [rsp + 2248], rax
                        mov              rax, qword ptr [rsp + 2288]
                        mov              qword ptr [rsp + 2224], rax
                        mov              rax, qword ptr [rsp + 2296]
                        mov              qword ptr [rsp + 2232], rax
                        .section         .rodata
.Lrkfn1162:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1162]
                        lea              rsi, [rsp + 2224]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        cmp              eax, 104
                                                                                        je    n813_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n818_assign_α
n817_call_β:
                                                                                        jmp   n813_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n818_assign_α:
                        mov              rax, qword ptr [rsp + 2208]
                        mov              rdx, qword ptr [rsp + 2216]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n819_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n819_statement_end_α:
                                                                                        jmp   n820_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n820_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx1167_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx1167_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n821_goto_α:
                                                                                        jmp   n813_statement_begin_α
n821_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n822_goto_α:
                                                                                        jmp   n637_statement_begin_α
n822_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n823_goto_α:
                                                                                        jmp   n641_statement_begin_α
n823_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n824_goto_α:
                                                                                        jmp   n645_statement_begin_α
n824_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n825_goto_α:
                                                                                        jmp   n649_statement_begin_α
n825_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n826_goto_α:
                                                                                        jmp   n653_statement_begin_α
n826_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n827_goto_α:
                                                                                        jmp   n657_statement_begin_α
n827_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n828_goto_α:
                                                                                        jmp   n661_statement_begin_α
n828_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n829_goto_α:
                                                                                        jmp   n718_statement_begin_α
n829_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n830_goto_α:
                                                                                        jmp   n722_statement_begin_α
n830_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n831_goto_α:
                                                                                        jmp   n733_statement_begin_α
n831_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n832_goto_α:
                                                                                        jmp   n737_statement_begin_α
n832_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n833_goto_α:
                                                                                        jmp   n748_statement_begin_α
n833_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n834_goto_α:
                                                                                        jmp   n752_statement_begin_α
n834_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n835_goto_α:
                                                                                        jmp   n759_statement_begin_α
n835_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n836_goto_α:
                                                                                        jmp   n767_statement_begin_α
n836_goto_β:
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
.S0:                    .string          "r"
.S1:                    .string          "ws"
.S2:                    .string          "PATV$0"
.S3:                    .string          "PATV$1"
.S4:                    .string          "PATV$2"
.S5:                    .string          "ix"
                        .text
                        .section         .note.GNU-stack,"",@progbits

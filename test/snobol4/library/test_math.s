                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_α:
proc_LBL__max_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_var_α
n0_statement_begin_β:
                                                                                        jmp   n10_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # max
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_var_α
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n3_coerce_numeric_α
n2_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx16_1
                        cmp              eax, 3
                                                                                        jne   .Lx16_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 3
                                                                                        jne   .Lx16_0
.Lx16_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n4_coerce_numeric_α
.Lx16_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 147
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
                                                                                        jmp   n4_coerce_numeric_α
n3_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n2_var_β
#-----------------------------------------------------------------------------------------------------------------------
n4_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx18_1
                        cmp              eax, 3
                                                                                        jne   .Lx18_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 3
                                                                                        jne   .Lx18_0
.Lx18_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n5_cmp_test_α
.Lx18_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 148
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
                                                                                        jmp   n5_cmp_test_α
n4_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n3_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n5_cmp_test_α:
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
                                                                                        js    .Lx20_240
                        add              rsp, 16
                                                                                        jmp   n4_coerce_numeric_β
.Lx20_240:
                        mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0
                                                                                        jmp   n6_var_α
n5_cmp_test_β:
                        add              rsp, 16
                                                                                        jmp   n4_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n7_binop_α
n6_var_β:
                        add              rsp, 16
                                                                                        jmp   n5_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n7_binop_α:
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
                                                                                        jmp   n8_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # max
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:
                                                                                        jmp   n10_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n10_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_β:
                                                                                        jmp   proc_LBL__max_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_α:
proc_LBL__min_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:
                                                                                        jmp   n29_var_α
n28_statement_begin_β:
                                                                                        jmp   n38_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # min
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n30_var_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n31_coerce_numeric_α
n30_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n28_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n31_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx44_1
                        cmp              eax, 3
                                                                                        jne   .Lx44_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 3
                                                                                        jne   .Lx44_0
.Lx44_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n32_coerce_numeric_α
.Lx44_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111
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
                                                                                        jmp   n32_coerce_numeric_α
n31_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n30_var_β
#-----------------------------------------------------------------------------------------------------------------------
n32_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx46_1
                        cmp              eax, 3
                                                                                        jne   .Lx46_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 3
                                                                                        jne   .Lx46_0
.Lx46_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n33_cmp_test_α
.Lx46_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112
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
                                                                                        jmp   n33_cmp_test_α
n32_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n31_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n33_cmp_test_α:
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
                                                                                        jg    .Lx48_240
                        add              rsp, 16
                                                                                        jmp   n32_coerce_numeric_β
.Lx48_240:
                        mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0
                                                                                        jmp   n34_var_α
n33_cmp_test_β:
                        add              rsp, 16
                                                                                        jmp   n32_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n34_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n35_binop_α
n34_var_β:
                        add              rsp, 16
                                                                                        jmp   n33_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n35_binop_α:
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
                                                                                        jmp   n36_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # min
                        mov              qword ptr [r9 + 40], rdx
                                                                                        jmp   n37_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:
                                                                                        jmp   n38_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n38_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_β:
                                                                                        jmp   proc_LBL__min_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_α:
proc_LBL__abs_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_begin_α:
                                                                                        jmp   n57_var_α
n56_statement_begin_β:
                                                                                        jmp   n67_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # abs
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n58_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n59_coerce_numeric_α
n58_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n56_statement_begin_β
.Lx71_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n59_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx73_1
                        cmp              eax, 3
                                                                                        jne   .Lx73_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx73_0
.Lx73_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n60_coerce_numeric_α
.Lx73_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 147
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
                                                                                        jmp   n60_coerce_numeric_α
n59_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n58_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n60_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx75_1
                        cmp              eax, 3
                                                                                        jne   .Lx75_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx75_0
.Lx75_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n61_cmp_test_α
.Lx75_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 148
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
                                                                                        jmp   n61_cmp_test_α
n60_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n59_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n61_cmp_test_α:
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
                                                                                        js    .Lx77_240
                        add              rsp, 16
                                                                                        jmp   n60_coerce_numeric_β
.Lx77_240:
                        mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0
                                                                                        jmp   n62_var_α
n61_cmp_test_β:
                        add              rsp, 16
                                                                                        jmp   n60_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # abs
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n63_unop_α
n62_var_β:
                        add              rsp, 16
                                                                                        jmp   n61_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n63_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n64_binop_α
n63_unop_β:
                        add              rsp, 16
                                                                                        jmp   n62_var_β
#-----------------------------------------------------------------------------------------------------------------------
n64_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # cmp_test
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # unop
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
                                                                                        jmp   n65_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n65_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # abs
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n66_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:
                                                                                        jmp   n67_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n67_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_β:
                                                                                        jmp   proc_LBL__abs_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_α:
proc_LBL__sign_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:
                                                                                        jmp   n87_var_α
n86_statement_begin_β:
                                                                                        jmp   n97_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n87_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n88_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx111_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n89_coerce_numeric_α
n88_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n86_statement_begin_β
.Lx111_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n89_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx113_1
                        cmp              eax, 3
                                                                                        jne   .Lx113_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx113_0
.Lx113_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n90_coerce_numeric_α
.Lx113_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 147
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
                                                                                        jmp   n90_coerce_numeric_α
n89_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n88_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n90_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx115_1
                        cmp              eax, 3
                                                                                        jne   .Lx115_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx115_0
.Lx115_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n91_cmp_test_α
.Lx115_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 148
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
                                                                                        jmp   n91_cmp_test_α
n90_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n89_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n91_cmp_test_α:
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
                                                                                        js    .Lx117_240
                        add              rsp, 16
                                                                                        jmp   n90_coerce_numeric_β
.Lx117_240:
                        mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0
                                                                                        jmp   n92_lit_integer_α
n91_cmp_test_β:
                        add              rsp, 16
                                                                                        jmp   n90_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n93_unop_α
n92_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n91_cmp_test_β
.Lx118_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n93_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n94_binop_α
n93_unop_β:
                        add              rsp, 16
                                                                                        jmp   n92_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n94_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # cmp_test
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # unop
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
                                                                                        jmp   n95_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n95_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax                       # sign
                        mov              qword ptr [r9 + 72], rdx
                                                                                        jmp   n96_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_end_α:
                                                                                        jmp   n107_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_begin_α:
                                                                                        jmp   n98_var_α
n97_statement_begin_β:
                                                                                        jmp   n107_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n98_var_α:
                        mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 656], rax                     # result
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n99_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n99_lit_integer_α:
                        mov              qword ptr [rsp + 640], 3                       # result
                        mov              rax, qword ptr [rip + .Lx127_0]
                        mov              qword ptr [rsp + 648], rax
                                                                                        jmp   n100_coerce_numeric_α
.Lx127_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n100_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 656]
                        cmp              eax, 5
                                                                                        je    .Lx129_1
                        cmp              eax, 3
                                                                                        jne   .Lx129_0
                        mov              eax, dword ptr [rsp + 640]
                        cmp              eax, 3
                                                                                        jne   .Lx129_0
.Lx129_1:
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 624], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 632], rax
                                                                                        jmp   n101_coerce_numeric_α
.Lx129_0:
                        lea              rdi, [rsp + 656]
                        lea              rsi, [rsp + 640]
                        lea              rdx, [rsp + 624]
                        mov              rcx, 111
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
                                                                                        jmp   n101_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n101_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 640]
                        cmp              eax, 5
                                                                                        je    .Lx131_1
                        cmp              eax, 3
                                                                                        jne   .Lx131_0
                        mov              eax, dword ptr [rsp + 656]
                        cmp              eax, 3
                                                                                        jne   .Lx131_0
.Lx131_1:
                        mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 616], rax
                                                                                        jmp   n102_cmp_test_α
.Lx131_0:
                        lea              rdi, [rsp + 640]
                        lea              rsi, [rsp + 656]
                        lea              rdx, [rsp + 608]
                        mov              rcx, 112
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
                                                                                        jmp   n102_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n102_cmp_test_α:
                        lea              rdi, [rsp + 624]
                        lea              rsi, [rsp + 608]
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
                                                                                        jle   n97_statement_begin_β
                        mov              qword ptr [rsp + 592], 0
                        mov              qword ptr [rsp + 600], 0
                                                                                        jmp   n103_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n103_lit_integer_α:
                        mov              qword ptr [rsp + 672], 3                       # result
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 680], rax
                                                                                        jmp   n104_binop_α
.Lx134_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n104_binop_α:
                        mov              rdi, qword ptr [rsp + 592]
                        mov              rsi, qword ptr [rsp + 600]
                        mov              rdx, qword ptr [rsp + 672]
                        mov              rcx, qword ptr [rsp + 680]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n105_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n105_assign_α:
                        mov              rax, qword ptr [rsp + 576]
                        mov              rdx, qword ptr [rsp + 584]
                        mov              qword ptr [r9 + 64], rax                       # sign
                        mov              qword ptr [r9 + 72], rdx
                                                                                        jmp   n106_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:
                                                                                        jmp   n107_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n107_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_β:
                                                                                        jmp   proc_LBL__sign_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_α:
proc_LBL__gcd_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_begin_α:
                                                                                        jmp   n142_var_α
n141_statement_begin_β:
                                                                                        jmp   n160_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n142_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n143_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n143_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx164_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n144_call_α
n143_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n141_statement_begin_β
.Lx164_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n144_call_α:
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
.Lrkfnzd166:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd166]
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
                                                                                        jne   .Lx165_240
                        add              rsp, 16
                                                                                        jmp   n143_lit_integer_β
.Lx165_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n145_statement_end_α
n144_call_β:
                        add              rsp, 16
                                                                                        jmp   n143_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_end_α:
                                                                                        jmp   n146_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n146_statement_begin_α:
                                                                                        jmp   n147_var_α
n146_statement_begin_β:
                                                                                        jmp   n152_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n147_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]                       # gcd
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n148_var_α
n147_var_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n146_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n148_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n149_call_α
n148_var_β:
                        add              rsp, 16
                                                                                        jmp   n147_var_β
#-----------------------------------------------------------------------------------------------------------------------
n149_call_α:
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
.Lrkfnzd174:            .string          "REMDR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd174]
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
                                                                                        jne   .Lx173_240
                        add              rsp, 16
                                                                                        jmp   n148_var_β
.Lx173_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n150_assign_α
n149_call_β:
                        add              rsp, 16
                                                                                        jmp   n148_var_β
#-----------------------------------------------------------------------------------------------------------------------
n150_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              qword ptr [r9 + 120], rdx
                                                                                        jmp   n151_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n151_statement_end_α:
                                                                                        jmp   n152_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_begin_α:
                                                                                        jmp   n153_var_α
n152_statement_begin_β:
                                                                                        jmp   n156_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n153_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n154_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n154_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              qword ptr [r9 + 88], rdx
                                                                                        jmp   n155_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_end_α:
                                                                                        jmp   n156_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_begin_α:
                                                                                        jmp   n157_var_α
n156_statement_begin_β:
                                                                                        jmp   n141_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n157_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n158_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n158_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              qword ptr [r9 + 104], rdx
                                                                                        jmp   n159_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n159_statement_end_α:
                        add              rsp, 128
                                                                                        jmp   n141_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n160_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_β:
                                                                                        jmp   proc_LBL__gcd_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_α:
proc_LBL__lcm_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n192_statement_begin_α:
                                                                                        jmp   n193_var_α
n192_statement_begin_β:
                                                                                        jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n193_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # a
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n194_var_α
#-----------------------------------------------------------------------------------------------------------------------
n194_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n195_call_α
n194_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n192_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n195_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx
                        lea              r10, [rip + .Lx212_6]
                        lea              r11, [rip + .Lx212_7]
                                                                                        jmp   gcd_shim
.Lx212_6:
                        mov              rax, qword ptr [r9 + 80]                       # gcd
                        mov              rdx, qword ptr [r9 + 88]
                                                                                        jmp   .Lx212_2
.Lx212_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx212_2
.Lx212_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx212_240
                        add              rsp, 16
                                                                                        jmp   n194_var_β
.Lx212_240:
                                                                                        jmp   n196_assign_α
n195_call_β:
                                                                                        jmp   n194_var_β
.Lx212_0:
                        .quad            .Lx212_0_s
.Lx212_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n196_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              qword ptr [r9 + 168], rdx
                                                                                        jmp   n197_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n197_statement_end_α:
                                                                                        jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_begin_α:
                                                                                        jmp   n199_var_α
n198_statement_begin_β:
                                                                                        jmp   n206_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n199_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # a
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n200_var_α
n199_var_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n198_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n200_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 160]                      # g
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n201_binop_α
n200_var_β:
                        add              rsp, 16
                                                                                        jmp   n199_var_β
#-----------------------------------------------------------------------------------------------------------------------
n201_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_div@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx220_240
                        add              rsp, 16
                                                                                        jmp   n200_var_β
.Lx220_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n202_var_α
n201_binop_β:
                        add              rsp, 16
                                                                                        jmp   n200_var_β
#-----------------------------------------------------------------------------------------------------------------------
n202_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n203_binop_α
n202_var_β:
                        add              rsp, 16
                                                                                        jmp   n201_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n203_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx222_240
                        add              rsp, 16
                                                                                        jmp   n202_var_β
.Lx222_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n204_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n204_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # lcm
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n205_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n205_statement_end_α:
                                                                                        jmp   n206_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n206_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_β:
                                                                                        jmp   proc_LBL__lcm_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_max_α
proc_max_α:
proc_max_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n228_save_restore_α:
                                                                                        jmp   n229_goto_deferred_α
max_shim:
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax                        # max
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   n229_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n229_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__max_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_max_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_max_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_max_β:
                                                                                        jmp   proc_max_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_max_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_max_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_min_α
proc_min_α:
proc_min_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n234_save_restore_α:
                                                                                        jmp   n235_goto_deferred_α
min_shim:
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax                       # min
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   n235_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n235_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__min_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_min_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_min_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_min_β:
                                                                                        jmp   proc_min_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_min_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_min_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_abs_α
proc_abs_α:
proc_abs_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n240_save_restore_α:
                                                                                        jmp   n241_goto_deferred_α
abs_shim:
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 48], rax                       # abs
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 56], rax
                                                                                        jmp   n241_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n241_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__abs_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_abs_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_abs_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_abs_β:
                                                                                        jmp   proc_abs_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_abs_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_abs_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_sign_α
proc_sign_α:
proc_sign_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n246_save_restore_α:
                                                                                        jmp   n247_goto_deferred_α
sign_shim:
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax                       # sign
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
                                                                                        jmp   n247_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n247_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__sign_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_sign_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_sign_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_sign_β:
                                                                                        jmp   proc_sign_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_sign_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_sign_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_gcd_α
proc_gcd_α:
proc_gcd_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n252_save_restore_α:
                                                                                        jmp   n253_goto_deferred_α
gcd_shim:
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
                                                                                        jmp   n253_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n253_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__gcd_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_gcd_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_gcd_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_gcd_β:
                                                                                        jmp   proc_gcd_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_gcd_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_gcd_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_lcm_α
proc_lcm_α:
proc_lcm_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n258_save_restore_α:
                                                                                        jmp   n259_goto_deferred_α
lcm_shim:
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 144], rax                      # a
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
                                                                                        jmp   n259_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n259_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__lcm_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_lcm_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_lcm_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_lcm_β:
                                                                                        jmp   proc_lcm_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_lcm_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_lcm_ω:
                                                                                        jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__max"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__max_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__min"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__min_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__abs"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__abs_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__sign"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__sign_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "LBL__gcd"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_LBL__gcd_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 2176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "LBL__lcm"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_LBL__lcm_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 2176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "max"
.Lgvan1:                .string          "x"
.Lgvan2:                .string          "min"
.Lgvan3:                .string          "abs"
.Lgvan4:                .string          "sign"
.Lgvan5:                .string          "gcd"
.Lgvan6:                .string          "b"
.Lgvan7:                .string          "r"
.Lgvan8:                .string          "lcm"
.Lgvan9:                .string          "a"
.Lgvan10:               .string          "g"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 11
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 11
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n264_statement_begin_α:
                                                                                        jmp   n265_statement_end_α
n264_statement_begin_β:
                                                                                        jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:
                                                                                        jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_begin_α:
                                                                                        jmp   n267_func_activate_α
n266_statement_begin_β:
                                                                                        jmp   n269_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx382_0]
                        mov              rsi, qword ptr [rip + .Lx382_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_max_α]
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
                                                                                        jmp   n268_statement_end_α
n267_func_activate_β:
                                                                                        jmp   n266_statement_begin_β
.Lx382_0:
                        .quad            .Lx382_0_s
.Lx382_0_s:
                        .string          "max"
.Lx382_1:
                        .quad            .Lx382_1_s
.Lx382_1_s:
                        .string          "max,x"
#-----------------------------------------------------------------------------------------------------------------------
n268_statement_end_α:
                                                                                        jmp   n269_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n269_statement_begin_α:
                                                                                        jmp   n270_statement_end_α
n269_statement_begin_β:
                                                                                        jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:
                                                                                        jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_begin_α:
                                                                                        jmp   n272_func_activate_α
n271_statement_begin_β:
                                                                                        jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n272_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx392_0]
                        mov              rsi, qword ptr [rip + .Lx392_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_min_α]
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
                                                                                        jmp   n273_statement_end_α
n272_func_activate_β:
                                                                                        jmp   n271_statement_begin_β
.Lx392_0:
                        .quad            .Lx392_0_s
.Lx392_0_s:
                        .string          "min"
.Lx392_1:
                        .quad            .Lx392_1_s
.Lx392_1_s:
                        .string          "min,x"
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_end_α:
                                                                                        jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_begin_α:
                                                                                        jmp   n275_statement_end_α
n274_statement_begin_β:
                                                                                        jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_end_α:
                                                                                        jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_begin_α:
                                                                                        jmp   n277_func_activate_α
n276_statement_begin_β:
                                                                                        jmp   n279_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n277_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx402_0]
                        mov              rsi, qword ptr [rip + .Lx402_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_abs_α]
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
                                                                                        jmp   n278_statement_end_α
n277_func_activate_β:
                                                                                        jmp   n276_statement_begin_β
.Lx402_0:
                        .quad            .Lx402_0_s
.Lx402_0_s:
                        .string          "abs"
.Lx402_1:
                        .quad            .Lx402_1_s
.Lx402_1_s:
                        .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n278_statement_end_α:
                                                                                        jmp   n279_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n279_statement_begin_α:
                                                                                        jmp   n280_statement_end_α
n279_statement_begin_β:
                                                                                        jmp   n281_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n280_statement_end_α:
                                                                                        jmp   n281_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n281_statement_begin_α:
                                                                                        jmp   n282_func_activate_α
n281_statement_begin_β:
                                                                                        jmp   n284_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n282_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx412_0]
                        mov              rsi, qword ptr [rip + .Lx412_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_sign_α]
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
                                                                                        jmp   n283_statement_end_α
n282_func_activate_β:
                                                                                        jmp   n281_statement_begin_β
.Lx412_0:
                        .quad            .Lx412_0_s
.Lx412_0_s:
                        .string          "sign"
.Lx412_1:
                        .quad            .Lx412_1_s
.Lx412_1_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n283_statement_end_α:
                                                                                        jmp   n284_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n284_statement_begin_α:
                                                                                        jmp   n285_statement_end_α
n284_statement_begin_β:
                                                                                        jmp   n286_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n285_statement_end_α:
                                                                                        jmp   n286_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n286_statement_begin_α:
                                                                                        jmp   n287_func_activate_α
n286_statement_begin_β:
                                                                                        jmp   n289_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n287_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx422_0]
                        mov              rsi, qword ptr [rip + .Lx422_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_gcd_α]
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
                                                                                        jmp   n288_statement_end_α
n287_func_activate_β:
                                                                                        jmp   n286_statement_begin_β
.Lx422_0:
                        .quad            .Lx422_0_s
.Lx422_0_s:
                        .string          "gcd"
.Lx422_1:
                        .quad            .Lx422_1_s
.Lx422_1_s:
                        .string          "gcd,b,r"
#-----------------------------------------------------------------------------------------------------------------------
n288_statement_end_α:
                                                                                        jmp   n289_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n289_statement_begin_α:
                                                                                        jmp   n290_statement_end_α
n289_statement_begin_β:
                                                                                        jmp   n291_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_end_α:
                                                                                        jmp   n291_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_begin_α:
                                                                                        jmp   n292_func_activate_α
n291_statement_begin_β:
                                                                                        jmp   n294_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n292_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx432_0]
                        mov              rsi, qword ptr [rip + .Lx432_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_lcm_α]
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
                                                                                        jmp   n293_statement_end_α
n292_func_activate_β:
                                                                                        jmp   n291_statement_begin_β
.Lx432_0:
                        .quad            .Lx432_0_s
.Lx432_0_s:
                        .string          "lcm"
.Lx432_1:
                        .quad            .Lx432_1_s
.Lx432_1_s:
                        .string          "a,b,g"
#-----------------------------------------------------------------------------------------------------------------------
n293_statement_end_α:
                                                                                        jmp   n294_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n294_statement_begin_α:
                                                                                        jmp   n295_statement_end_α
n294_statement_begin_β:
                                                                                        jmp   n296_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n295_statement_end_α:
                                                                                        jmp   n296_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n296_statement_begin_α:
                                                                                        jmp   n297_statement_end_α
n296_statement_begin_β:
                                                                                        jmp   n298_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n297_statement_end_α:
                                                                                        jmp   n298_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n298_statement_begin_α:
                                                                                        jmp   n299_lit_string_α
n298_statement_begin_β:
                                                                                        jmp   n303_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n299_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx445_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n300_lit_integer_α
.Lx445_0:
                        .quad            .Lx445_0_s
.Lx445_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n300_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx446_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n301_call_α
n300_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n298_statement_begin_β
.Lx446_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n301_call_α:
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
.Lrkfnzd448:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd448]
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
                                                                                        jne   .Lx447_240
                        add              rsp, 16
                                                                                        jmp   n300_lit_integer_β
.Lx447_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n302_statement_end_α
n301_call_β:
                        add              rsp, 16
                                                                                        jmp   n300_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n302_statement_end_α:
                                                                                        jmp   n303_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_begin_α:
                                                                                        jmp   n304_statement_end_α
n303_statement_begin_β:
                                                                                        jmp   n305_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_end_α:
                                                                                        jmp   n305_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n305_statement_begin_α:
                                                                                        jmp   n306_lit_integer_α
n305_statement_begin_β:
                                                                                        jmp   n311_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n306_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx457_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n307_lit_integer_α
n306_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n305_statement_begin_β
.Lx457_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n307_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx458_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n308_call_α
n307_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n306_lit_integer_β
.Lx458_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n308_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx
                        lea              r10, [rip + .Lx460_6]
                        lea              r11, [rip + .Lx460_7]
                                                                                        jmp   max_shim
.Lx460_6:
                        mov              rax, qword ptr [r9 + 0]                        # max
                        mov              rdx, qword ptr [r9 + 8]
                                                                                        jmp   .Lx460_2
.Lx460_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx460_2
.Lx460_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx460_240
                        add              rsp, 16
                                                                                        jmp   n307_lit_integer_β
.Lx460_240:
                                                                                        jmp   n309_assign_α
n308_call_β:
                                                                                        jmp   n307_lit_integer_β
.Lx460_0:
                        .quad            .Lx460_0_s
.Lx460_0_s:
                        .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n309_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx461_0]
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
                                                                                        jmp   n310_statement_end_α
.Lx461_0:
                        .quad            .Lx461_0_s
.Lx461_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n310_statement_end_α:
                                                                                        jmp   n311_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n311_statement_begin_α:
                                                                                        jmp   n312_lit_integer_α
n311_statement_begin_β:
                                                                                        jmp   n317_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n312_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx466_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n313_lit_integer_α
n312_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n311_statement_begin_β
.Lx466_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n313_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx467_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n314_call_α
n313_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n312_lit_integer_β
.Lx467_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n314_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx
                        lea              r10, [rip + .Lx469_6]
                        lea              r11, [rip + .Lx469_7]
                                                                                        jmp   min_shim
.Lx469_6:
                        mov              rax, qword ptr [r9 + 32]                       # min
                        mov              rdx, qword ptr [r9 + 40]
                                                                                        jmp   .Lx469_2
.Lx469_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx469_2
.Lx469_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx469_240
                        add              rsp, 16
                                                                                        jmp   n313_lit_integer_β
.Lx469_240:
                                                                                        jmp   n315_assign_α
n314_call_β:
                                                                                        jmp   n313_lit_integer_β
.Lx469_0:
                        .quad            .Lx469_0_s
.Lx469_0_s:
                        .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n315_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx470_0]
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
                                                                                        jmp   n316_statement_end_α
.Lx470_0:
                        .quad            .Lx470_0_s
.Lx470_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_end_α:
                                                                                        jmp   n317_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_begin_α:
                                                                                        jmp   n318_lit_real_α
n317_statement_begin_β:
                                                                                        jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n318_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx475_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n319_lit_real_α
.Lx475_0:
                        .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n319_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx476_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n320_call_α
.Lx476_0:
                        .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n320_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx
                        lea              r10, [rip + .Lx478_6]
                        lea              r11, [rip + .Lx478_7]
                                                                                        jmp   max_shim
.Lx478_6:
                        mov              rax, qword ptr [r9 + 0]                        # max
                        mov              rdx, qword ptr [r9 + 8]
                                                                                        jmp   .Lx478_2
.Lx478_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx478_2
.Lx478_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx478_240
                        add              rsp, 16
                        add              rsp, 176
                                                                                        jmp   n317_statement_begin_β
.Lx478_240:
                                                                                        jmp   n321_assign_α
n320_call_β:
                                                                                        jmp   n317_statement_begin_β
.Lx478_0:
                        .quad            .Lx478_0_s
.Lx478_0_s:
                        .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n321_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
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
                                                                                        jmp   n322_statement_end_α
.Lx479_0:
                        .quad            .Lx479_0_s
.Lx479_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:
                                                                                        jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n323_statement_begin_α:
                                                                                        jmp   n324_lit_real_α
n323_statement_begin_β:
                                                                                        jmp   n329_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx484_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n325_lit_real_α
.Lx484_0:
                        .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n325_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx485_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n326_call_α
.Lx485_0:
                        .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n326_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx
                        lea              r10, [rip + .Lx487_6]
                        lea              r11, [rip + .Lx487_7]
                                                                                        jmp   min_shim
.Lx487_6:
                        mov              rax, qword ptr [r9 + 32]                       # min
                        mov              rdx, qword ptr [r9 + 40]
                                                                                        jmp   .Lx487_2
.Lx487_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx487_2
.Lx487_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx487_240
                        add              rsp, 16
                        add              rsp, 224
                                                                                        jmp   n323_statement_begin_β
.Lx487_240:
                                                                                        jmp   n327_assign_α
n326_call_β:
                                                                                        jmp   n323_statement_begin_β
.Lx487_0:
                        .quad            .Lx487_0_s
.Lx487_0_s:
                        .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n327_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx488_0]
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
                                                                                        jmp   n328_statement_end_α
.Lx488_0:
                        .quad            .Lx488_0_s
.Lx488_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n328_statement_end_α:
                                                                                        jmp   n329_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n329_statement_begin_α:
                                                                                        jmp   n330_lit_integer_α
n329_statement_begin_β:
                                                                                        jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n330_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx493_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n331_unop_α
n330_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 240
                                                                                        jmp   n329_statement_begin_β
.Lx493_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n331_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n332_call_α
n331_unop_β:
                        add              rsp, 16
                                                                                        jmp   n330_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n332_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx496_6]
                        lea              r11, [rip + .Lx496_7]
                                                                                        jmp   abs_shim
.Lx496_6:
                        mov              rax, qword ptr [r9 + 48]                       # abs
                        mov              rdx, qword ptr [r9 + 56]
                                                                                        jmp   .Lx496_2
.Lx496_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx496_2
.Lx496_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx496_240
                        add              rsp, 16
                                                                                        jmp   n331_unop_β
.Lx496_240:
                                                                                        jmp   n333_assign_α
n332_call_β:
                                                                                        jmp   n331_unop_β
.Lx496_0:
                        .quad            .Lx496_0_s
.Lx496_0_s:
                        .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n333_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx497_0]
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
                                                                                        jmp   n334_statement_end_α
.Lx497_0:
                        .quad            .Lx497_0_s
.Lx497_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:
                                                                                        jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α:
                                                                                        jmp   n336_lit_integer_α
n335_statement_begin_β:
                                                                                        jmp   n340_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx502_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n337_call_α
n336_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 288
                                                                                        jmp   n335_statement_begin_β
.Lx502_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n337_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx504_6]
                        lea              r11, [rip + .Lx504_7]
                                                                                        jmp   sign_shim
.Lx504_6:
                        mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                                                                                        jmp   .Lx504_2
.Lx504_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx504_2
.Lx504_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx504_240
                        add              rsp, 16
                                                                                        jmp   n336_lit_integer_β
.Lx504_240:
                                                                                        jmp   n338_assign_α
n337_call_β:
                                                                                        jmp   n336_lit_integer_β
.Lx504_0:
                        .quad            .Lx504_0_s
.Lx504_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n338_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx505_0]
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
                                                                                        jmp   n339_statement_end_α
.Lx505_0:
                        .quad            .Lx505_0_s
.Lx505_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n339_statement_end_α:
                                                                                        jmp   n340_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n340_statement_begin_α:
                                                                                        jmp   n341_lit_integer_α
n340_statement_begin_β:
                                                                                        jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n341_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx510_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n342_call_α
n341_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 320
                                                                                        jmp   n340_statement_begin_β
.Lx510_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n342_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx512_6]
                        lea              r11, [rip + .Lx512_7]
                                                                                        jmp   sign_shim
.Lx512_6:
                        mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                                                                                        jmp   .Lx512_2
.Lx512_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx512_2
.Lx512_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx512_240
                        add              rsp, 16
                                                                                        jmp   n341_lit_integer_β
.Lx512_240:
                                                                                        jmp   n343_assign_α
n342_call_β:
                                                                                        jmp   n341_lit_integer_β
.Lx512_0:
                        .quad            .Lx512_0_s
.Lx512_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n343_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx513_0]
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
                                                                                        jmp   n344_statement_end_α
.Lx513_0:
                        .quad            .Lx513_0_s
.Lx513_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n344_statement_end_α:
                                                                                        jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n345_statement_begin_α:
                                                                                        jmp   n346_lit_integer_α
n345_statement_begin_β:
                                                                                        jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n346_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx518_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n347_unop_α
n346_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 352
                                                                                        jmp   n345_statement_begin_β
.Lx518_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n347_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n348_call_α
n347_unop_β:
                        add              rsp, 16
                                                                                        jmp   n346_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n348_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx521_6]
                        lea              r11, [rip + .Lx521_7]
                                                                                        jmp   sign_shim
.Lx521_6:
                        mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                                                                                        jmp   .Lx521_2
.Lx521_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx521_2
.Lx521_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx521_240
                        add              rsp, 16
                                                                                        jmp   n347_unop_β
.Lx521_240:
                                                                                        jmp   n349_assign_α
n348_call_β:
                                                                                        jmp   n347_unop_β
.Lx521_0:
                        .quad            .Lx521_0_s
.Lx521_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n349_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx522_0]
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
                                                                                        jmp   n350_statement_end_α
.Lx522_0:
                        .quad            .Lx522_0_s
.Lx522_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n350_statement_end_α:
                                                                                        jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n351_statement_begin_α:
                                                                                        jmp   n352_lit_integer_α
n351_statement_begin_β:
                                                                                        jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n352_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx527_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n353_lit_integer_α
n352_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 400
                                                                                        jmp   n351_statement_begin_β
.Lx527_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n353_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx528_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n354_call_α
n353_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n352_lit_integer_β
.Lx528_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n354_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx
                        lea              r10, [rip + .Lx530_6]
                        lea              r11, [rip + .Lx530_7]
                                                                                        jmp   gcd_shim
.Lx530_6:
                        mov              rax, qword ptr [r9 + 80]                       # gcd
                        mov              rdx, qword ptr [r9 + 88]
                                                                                        jmp   .Lx530_2
.Lx530_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx530_2
.Lx530_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx530_240
                        add              rsp, 16
                                                                                        jmp   n353_lit_integer_β
.Lx530_240:
                                                                                        jmp   n355_assign_α
n354_call_β:
                                                                                        jmp   n353_lit_integer_β
.Lx530_0:
                        .quad            .Lx530_0_s
.Lx530_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n355_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx531_0]
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
                                                                                        jmp   n356_statement_end_α
.Lx531_0:
                        .quad            .Lx531_0_s
.Lx531_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n356_statement_end_α:
                                                                                        jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n357_statement_begin_α:
                                                                                        jmp   n358_lit_integer_α
n357_statement_begin_β:
                                                                                        jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n358_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx536_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n359_lit_integer_α
n358_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 448
                                                                                        jmp   n357_statement_begin_β
.Lx536_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n359_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx537_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n360_call_α
n359_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n358_lit_integer_β
.Lx537_0:
                        .quad            75
#-----------------------------------------------------------------------------------------------------------------------
n360_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx
                        lea              r10, [rip + .Lx539_6]
                        lea              r11, [rip + .Lx539_7]
                                                                                        jmp   gcd_shim
.Lx539_6:
                        mov              rax, qword ptr [r9 + 80]                       # gcd
                        mov              rdx, qword ptr [r9 + 88]
                                                                                        jmp   .Lx539_2
.Lx539_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx539_2
.Lx539_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx539_240
                        add              rsp, 16
                                                                                        jmp   n359_lit_integer_β
.Lx539_240:
                                                                                        jmp   n361_assign_α
n360_call_β:
                                                                                        jmp   n359_lit_integer_β
.Lx539_0:
                        .quad            .Lx539_0_s
.Lx539_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n361_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx540_0]
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
                                                                                        jmp   n362_statement_end_α
.Lx540_0:
                        .quad            .Lx540_0_s
.Lx540_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_end_α:
                                                                                        jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n363_statement_begin_α:
                                                                                        jmp   n364_lit_integer_α
n363_statement_begin_β:
                        add              rsp, 496
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx545_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n365_lit_integer_α
n364_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 496
                                                                                        jmp   n363_statement_begin_β
.Lx545_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n365_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx546_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n366_call_α
n365_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n364_lit_integer_β
.Lx546_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n366_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx
                        lea              r10, [rip + .Lx548_6]
                        lea              r11, [rip + .Lx548_7]
                                                                                        jmp   lcm_shim
.Lx548_6:
                        mov              rax, qword ptr [r9 + 128]                      # lcm
                        mov              rdx, qword ptr [r9 + 136]
                                                                                        jmp   .Lx548_2
.Lx548_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx548_2
.Lx548_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx548_240
                        add              rsp, 16
                                                                                        jmp   n365_lit_integer_β
.Lx548_240:
                                                                                        jmp   n367_assign_α
n366_call_β:
                                                                                        jmp   n365_lit_integer_β
.Lx548_0:
                        .quad            .Lx548_0_s
.Lx548_0_s:
                        .string          "lcm"
#-----------------------------------------------------------------------------------------------------------------------
n367_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx549_0]
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
                                                                                        jmp   n368_statement_end_α
.Lx549_0:
                        .quad            .Lx549_0_s
.Lx549_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n368_statement_end_α:
                        add              rsp, 544
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n369_goto_α:
                                                                                        jmp   n269_statement_begin_α
n369_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n370_goto_α:
                                                                                        jmp   n274_statement_begin_α
n370_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n371_goto_α:
                                                                                        jmp   n279_statement_begin_α
n371_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n372_goto_α:
                                                                                        jmp   n284_statement_begin_α
n372_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n373_goto_α:
                                                                                        jmp   n289_statement_begin_α
n373_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n374_goto_α:
                                                                                        jmp   n294_statement_begin_α
n374_goto_β:
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

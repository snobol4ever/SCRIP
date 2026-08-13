                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_α:
proc_LBL__max_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_var_α
n0_statement_begin_β:
                                                                                        jmp   n10_RETURN_α
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
                                                                                        jmp   n10_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n10_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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
                                                                                        jmp   n38_RETURN_α
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
                                                                                        jmp   n38_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n38_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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
                                                                                        jmp   n67_RETURN_α
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
                                                                                        jmp   n67_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n67_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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
                                                                                        jmp   n107_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_begin_α:
                                                                                        jmp   n98_var_α
n97_statement_begin_β:
                                                                                        jmp   n107_RETURN_α
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
                                                                                        jmp   n107_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n107_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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
                                                                                        jmp   n160_RETURN_α
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
n160_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # var
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx212_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha]
                                                                                        jmp   rax
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
                                                                                        jmp   n206_RETURN_α
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
                                                                                        jmp   n206_RETURN_α
#-----------------------------------------------------------------------------------------------------------------------
n206_RETURN_α:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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
#-----------------------------------------------------------------------------------------------------------------------
n229_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__max_α]
                                                                                        jmp   rax
                                                                                        jmp   n230_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n230_save_restore_α:
max_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx236_2
                        mov              eax, 32
.Lx236_2:
                        sub              rsp, 64
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx236_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 0]                        # max
                        mov              qword ptr [r9 + 0], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx236_41
.Lx236_10:
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
.Lx236_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx236_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx236_42
.Lx236_11:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx236_42:
                        lea              r10, [rip + max_gamma]
                        lea              r11, [rip + max_omega]
                        lea              rax, [rip + proc_LBL__max_α]
                                                                                        jmp   rax
max_gamma:
                        mov              rdi, qword ptr [r9 + 0]                        # max
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx236_75
                        mov              eax, 32
.Lx236_75:
                        lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx236_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx236_110
.Lx236_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx236_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx236_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx236_111
.Lx236_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx236_111:
                        mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2
                                                                                        jbe   .Lx236_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx236_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
max_omega:
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx236_145
                        mov              eax, 32
.Lx236_145:
                        lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx236_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 0], rax                        # max
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx236_180
.Lx236_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx236_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx236_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx236_181
.Lx236_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx236_181:
                        mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2
                                                                                        jbe   .Lx236_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx236_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
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
n237_save_restore_α:
                                                                                        jmp   n238_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n238_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__min_α]
                                                                                        jmp   rax
                                                                                        jmp   n239_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n239_save_restore_α:
min_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx245_2
                        mov              eax, 32
.Lx245_2:
                        sub              rsp, 64
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx245_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 32]                       # min
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx245_41
.Lx245_10:
                        mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx245_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx245_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx245_42
.Lx245_11:
                        mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx245_42:
                        lea              r10, [rip + min_gamma]
                        lea              r11, [rip + min_omega]
                        lea              rax, [rip + proc_LBL__min_α]
                                                                                        jmp   rax
min_gamma:
                        mov              rdi, qword ptr [r9 + 32]                       # min
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx245_75
                        mov              eax, 32
.Lx245_75:
                        lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx245_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   .Lx245_110
.Lx245_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx245_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx245_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx245_111
.Lx245_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx245_111:
                        mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2
                                                                                        jbe   .Lx245_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx245_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
min_omega:
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx245_145
                        mov              eax, 32
.Lx245_145:
                        lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx245_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 32], rax                       # min
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   .Lx245_180
.Lx245_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx245_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx245_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax
                                                                                        jmp   .Lx245_181
.Lx245_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx245_181:
                        mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2
                                                                                        jbe   .Lx245_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx245_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
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
n246_save_restore_α:
                                                                                        jmp   n247_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n247_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__abs_α]
                                                                                        jmp   rax
                                                                                        jmp   n248_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n248_save_restore_α:
abs_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx254_2
                        mov              eax, 16
.Lx254_2:
                        sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx254_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 48]                       # abs
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 56], rdx
                                                                                        jmp   .Lx254_41
.Lx254_10:
                        mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx254_41:
                        lea              r10, [rip + abs_gamma]
                        lea              r11, [rip + abs_omega]
                        lea              rax, [rip + proc_LBL__abs_α]
                                                                                        jmp   rax
abs_gamma:
                        mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx254_75
                        mov              eax, 16
.Lx254_75:
                        lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx254_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 56], rax
                                                                                        jmp   .Lx254_110
.Lx254_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 56], rax
.Lx254_110:
                        mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1
                                                                                        jbe   .Lx254_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx254_3:
                        mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
abs_omega:
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx254_145
                        mov              eax, 16
.Lx254_145:
                        lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx254_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 56], rax
                                                                                        jmp   .Lx254_180
.Lx254_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 56], rax
.Lx254_180:
                        mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1
                                                                                        jbe   .Lx254_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx254_4:
                        mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
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
n255_save_restore_α:
                                                                                        jmp   n256_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n256_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__sign_α]
                                                                                        jmp   rax
                                                                                        jmp   n257_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n257_save_restore_α:
sign_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx263_2
                        mov              eax, 16
.Lx263_2:
                        sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx263_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 64]                       # sign
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [r8 + 56], rdx
                                                                                        jmp   .Lx263_41
.Lx263_10:
                        mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx263_41:
                        lea              r10, [rip + sign_gamma]
                        lea              r11, [rip + sign_omega]
                        lea              rax, [rip + proc_LBL__sign_α]
                                                                                        jmp   rax
sign_gamma:
                        mov              rdi, qword ptr [r9 + 64]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx263_75
                        mov              eax, 16
.Lx263_75:
                        lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx263_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax
                                                                                        jmp   .Lx263_110
.Lx263_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx263_110:
                        mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1
                                                                                        jbe   .Lx263_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx263_3:
                        mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
sign_omega:
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1
                                                                                        jbe   .Lx263_145
                        mov              eax, 16
.Lx263_145:
                        lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx263_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax
                                                                                        jmp   .Lx263_180
.Lx263_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx263_180:
                        mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1
                                                                                        jbe   .Lx263_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx263_4:
                        mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
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
n264_save_restore_α:
                                                                                        jmp   n265_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n265_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__gcd_α]
                                                                                        jmp   rax
                                                                                        jmp   n266_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n266_save_restore_α:
gcd_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx272_2
                        mov              eax, 32
.Lx272_2:
                        sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx272_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 80]                       # gcd
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx272_41
.Lx272_10:
                        mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx272_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx272_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 96]                       # b
                        mov              qword ptr [r9 + 96], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [r9 + 104], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx272_42
.Lx272_11:
                        mov              rax, qword ptr [r9 + 96]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
.Lx272_42:
                        lea              r10, [rip + gcd_gamma]
                        lea              r11, [rip + gcd_omega]
                        lea              rax, [rip + proc_LBL__gcd_α]
                                                                                        jmp   rax
gcd_gamma:
                        mov              rdi, qword ptr [r9 + 80]                       # gcd
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx272_75
                        mov              eax, 32
.Lx272_75:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx272_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 88], rax
                                                                                        jmp   .Lx272_110
.Lx272_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 88], rax
.Lx272_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx272_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax
                                                                                        jmp   .Lx272_111
.Lx272_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx272_111:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx272_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx272_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
gcd_omega:
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx272_145
                        mov              eax, 32
.Lx272_145:
                        lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx272_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 88], rax
                                                                                        jmp   .Lx272_180
.Lx272_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 88], rax
.Lx272_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx272_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax
                                                                                        jmp   .Lx272_181
.Lx272_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx272_181:
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2
                                                                                        jbe   .Lx272_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx272_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
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
n273_save_restore_α:
                                                                                        jmp   n274_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n274_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__lcm_α]
                                                                                        jmp   rax
                                                                                        jmp   n275_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n275_save_restore_α:
lcm_alpha:
                        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx281_2
                        mov              eax, 32
.Lx281_2:
                        sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 160]                      # g
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 160], 0
                        mov              qword ptr [r9 + 168], 0
                        mov              rax, qword ptr [r9 + 128]                      # lcm
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        mov              qword ptr [rsp + 48], rcx
                        cmp              rcx, 0
                                                                                        jbe   .Lx281_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 144]                      # a
                        mov              qword ptr [r9 + 144], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [r9 + 152], rax
                        mov              qword ptr [r8 + 72], rdx
                                                                                        jmp   .Lx281_41
.Lx281_10:
                        mov              rax, qword ptr [r9 + 144]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 144], 0
                        mov              qword ptr [r9 + 152], 0
.Lx281_41:
                        cmp              rcx, 1
                                                                                        jbe   .Lx281_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 96]                       # b
                        mov              qword ptr [r9 + 96], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [r9 + 104], rax
                        mov              qword ptr [r8 + 88], rdx
                                                                                        jmp   .Lx281_42
.Lx281_11:
                        mov              rax, qword ptr [r9 + 96]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
.Lx281_42:
                        lea              r10, [rip + lcm_gamma]
                        lea              r11, [rip + lcm_omega]
                        lea              rax, [rip + proc_LBL__lcm_α]
                                                                                        jmp   rax
lcm_gamma:
                        mov              rdi, qword ptr [r9 + 128]                      # lcm
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx281_75
                        mov              eax, 32
.Lx281_75:
                        lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 168], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx281_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 144], rax                      # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 152], rax
                                                                                        jmp   .Lx281_110
.Lx281_80:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 144], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 152], rax
.Lx281_110:
                        cmp              rcx, 1
                                                                                        jbe   .Lx281_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax
                                                                                        jmp   .Lx281_111
.Lx281_81:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx281_111:
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2
                                                                                        jbe   .Lx281_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx281_3:
                        mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi
                                                                                        jmp   rcx
lcm_omega:
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2
                                                                                        jbe   .Lx281_145
                        mov              eax, 32
.Lx281_145:
                        lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 128], rax                      # lcm
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 168], rax
                        cmp              rcx, 0
                                                                                        jbe   .Lx281_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 144], rax                      # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 152], rax
                                                                                        jmp   .Lx281_180
.Lx281_150:
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 144], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 152], rax
.Lx281_180:
                        cmp              rcx, 1
                                                                                        jbe   .Lx281_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax
                                                                                        jmp   .Lx281_181
.Lx281_151:
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx281_181:
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2
                                                                                        jbe   .Lx281_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx281_4:
                        mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   rcx
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
n282_statement_begin_α:
                                                                                        jmp   n283_statement_end_α
n282_statement_begin_β:
                                                                                        jmp   n284_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n283_statement_end_α:
                                                                                        jmp   n284_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n284_statement_begin_α:
                                                                                        jmp   n285_func_activate_α
n284_statement_begin_β:
                                                                                        jmp   n287_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n285_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx400_0]
                        mov              rsi, qword ptr [rip + .Lx400_1]
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
                                                                                        jmp   n286_statement_end_α
n285_func_activate_β:
                                                                                        jmp   n284_statement_begin_β
.Lx400_0:
                        .quad            .Lx400_0_s
.Lx400_0_s:
                        .string          "max"
.Lx400_1:
                        .quad            .Lx400_1_s
.Lx400_1_s:
                        .string          "max,x"
#-----------------------------------------------------------------------------------------------------------------------
n286_statement_end_α:
                                                                                        jmp   n287_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n287_statement_begin_α:
                                                                                        jmp   n288_statement_end_α
n287_statement_begin_β:
                                                                                        jmp   n289_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n288_statement_end_α:
                                                                                        jmp   n289_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n289_statement_begin_α:
                                                                                        jmp   n290_func_activate_α
n289_statement_begin_β:
                                                                                        jmp   n292_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n290_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx410_0]
                        mov              rsi, qword ptr [rip + .Lx410_1]
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
                                                                                        jmp   n291_statement_end_α
n290_func_activate_β:
                                                                                        jmp   n289_statement_begin_β
.Lx410_0:
                        .quad            .Lx410_0_s
.Lx410_0_s:
                        .string          "min"
.Lx410_1:
                        .quad            .Lx410_1_s
.Lx410_1_s:
                        .string          "min,x"
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_end_α:
                                                                                        jmp   n292_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n292_statement_begin_α:
                                                                                        jmp   n293_statement_end_α
n292_statement_begin_β:
                                                                                        jmp   n294_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n293_statement_end_α:
                                                                                        jmp   n294_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n294_statement_begin_α:
                                                                                        jmp   n295_func_activate_α
n294_statement_begin_β:
                                                                                        jmp   n297_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n295_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx420_0]
                        mov              rsi, qword ptr [rip + .Lx420_1]
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
                                                                                        jmp   n296_statement_end_α
n295_func_activate_β:
                                                                                        jmp   n294_statement_begin_β
.Lx420_0:
                        .quad            .Lx420_0_s
.Lx420_0_s:
                        .string          "abs"
.Lx420_1:
                        .quad            .Lx420_1_s
.Lx420_1_s:
                        .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n296_statement_end_α:
                                                                                        jmp   n297_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n297_statement_begin_α:
                                                                                        jmp   n298_statement_end_α
n297_statement_begin_β:
                                                                                        jmp   n299_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n298_statement_end_α:
                                                                                        jmp   n299_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n299_statement_begin_α:
                                                                                        jmp   n300_func_activate_α
n299_statement_begin_β:
                                                                                        jmp   n302_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n300_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx430_0]
                        mov              rsi, qword ptr [rip + .Lx430_1]
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
                                                                                        jmp   n301_statement_end_α
n300_func_activate_β:
                                                                                        jmp   n299_statement_begin_β
.Lx430_0:
                        .quad            .Lx430_0_s
.Lx430_0_s:
                        .string          "sign"
.Lx430_1:
                        .quad            .Lx430_1_s
.Lx430_1_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n301_statement_end_α:
                                                                                        jmp   n302_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n302_statement_begin_α:
                                                                                        jmp   n303_statement_end_α
n302_statement_begin_β:
                                                                                        jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_end_α:
                                                                                        jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_begin_α:
                                                                                        jmp   n305_func_activate_α
n304_statement_begin_β:
                                                                                        jmp   n307_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n305_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx440_0]
                        mov              rsi, qword ptr [rip + .Lx440_1]
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
                                                                                        jmp   n306_statement_end_α
n305_func_activate_β:
                                                                                        jmp   n304_statement_begin_β
.Lx440_0:
                        .quad            .Lx440_0_s
.Lx440_0_s:
                        .string          "gcd"
.Lx440_1:
                        .quad            .Lx440_1_s
.Lx440_1_s:
                        .string          "gcd,b,r"
#-----------------------------------------------------------------------------------------------------------------------
n306_statement_end_α:
                                                                                        jmp   n307_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n307_statement_begin_α:
                                                                                        jmp   n308_statement_end_α
n307_statement_begin_β:
                                                                                        jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n308_statement_end_α:
                                                                                        jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n309_statement_begin_α:
                                                                                        jmp   n310_func_activate_α
n309_statement_begin_β:
                                                                                        jmp   n312_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n310_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx450_0]
                        mov              rsi, qword ptr [rip + .Lx450_1]
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
                                                                                        jmp   n311_statement_end_α
n310_func_activate_β:
                                                                                        jmp   n309_statement_begin_β
.Lx450_0:
                        .quad            .Lx450_0_s
.Lx450_0_s:
                        .string          "lcm"
.Lx450_1:
                        .quad            .Lx450_1_s
.Lx450_1_s:
                        .string          "a,b,g"
#-----------------------------------------------------------------------------------------------------------------------
n311_statement_end_α:
                                                                                        jmp   n312_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n312_statement_begin_α:
                                                                                        jmp   n313_statement_end_α
n312_statement_begin_β:
                                                                                        jmp   n314_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n313_statement_end_α:
                                                                                        jmp   n314_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n314_statement_begin_α:
                                                                                        jmp   n315_statement_end_α
n314_statement_begin_β:
                                                                                        jmp   n316_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n315_statement_end_α:
                                                                                        jmp   n316_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_begin_α:
                                                                                        jmp   n317_lit_string_α
n316_statement_begin_β:
                                                                                        jmp   n321_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n317_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx463_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n318_lit_integer_α
.Lx463_0:
                        .quad            .Lx463_0_s
.Lx463_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n318_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx464_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n319_call_α
n318_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n316_statement_begin_β
.Lx464_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n319_call_α:
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
.Lrkfnzd466:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd466]
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
                                                                                        jne   .Lx465_240
                        add              rsp, 16
                                                                                        jmp   n318_lit_integer_β
.Lx465_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n320_statement_end_α
n319_call_β:
                        add              rsp, 16
                                                                                        jmp   n318_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_end_α:
                                                                                        jmp   n321_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n321_statement_begin_α:
                                                                                        jmp   n322_statement_end_α
n321_statement_begin_β:
                                                                                        jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:
                                                                                        jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n323_statement_begin_α:
                                                                                        jmp   n324_lit_integer_α
n323_statement_begin_β:
                                                                                        jmp   n329_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx475_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n325_lit_integer_α
n324_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n323_statement_begin_β
.Lx475_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n325_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx476_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n326_call_α
n325_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n324_lit_integer_β
.Lx476_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n326_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx478_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + max_alpha]
                                                                                        jmp   rax
.Lx478_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx478_240
                        add              rsp, 16
                                                                                        jmp   n325_lit_integer_β
.Lx478_240:
                                                                                        jmp   n327_assign_α
n326_call_β:
                                                                                        jmp   n325_lit_integer_β
.Lx478_0:
                        .quad            .Lx478_0_s
.Lx478_0_s:
                        .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n327_assign_α:
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
                                                                                        jmp   n328_statement_end_α
.Lx479_0:
                        .quad            .Lx479_0_s
.Lx479_0_s:
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
                        mov              rax, qword ptr [rip + .Lx484_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n331_lit_integer_α
n330_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n329_statement_begin_β
.Lx484_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n331_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx485_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n332_call_α
n331_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n330_lit_integer_β
.Lx485_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n332_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx487_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + min_alpha]
                                                                                        jmp   rax
.Lx487_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx487_240
                        add              rsp, 16
                                                                                        jmp   n331_lit_integer_β
.Lx487_240:
                                                                                        jmp   n333_assign_α
n332_call_β:
                                                                                        jmp   n331_lit_integer_β
.Lx487_0:
                        .quad            .Lx487_0_s
.Lx487_0_s:
                        .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n333_assign_α:
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
                                                                                        jmp   n334_statement_end_α
.Lx488_0:
                        .quad            .Lx488_0_s
.Lx488_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:
                                                                                        jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α:
                                                                                        jmp   n336_lit_real_α
n335_statement_begin_β:
                                                                                        jmp   n341_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx493_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n337_lit_real_α
.Lx493_0:
                        .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n337_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx494_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n338_call_α
.Lx494_0:
                        .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n338_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_real
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx496_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + max_alpha]
                                                                                        jmp   rax
.Lx496_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx496_240
                        add              rsp, 16
                        add              rsp, 176
                                                                                        jmp   n335_statement_begin_β
.Lx496_240:
                                                                                        jmp   n339_assign_α
n338_call_β:
                                                                                        jmp   n335_statement_begin_β
.Lx496_0:
                        .quad            .Lx496_0_s
.Lx496_0_s:
                        .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n339_assign_α:
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
                                                                                        jmp   n340_statement_end_α
.Lx497_0:
                        .quad            .Lx497_0_s
.Lx497_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n340_statement_end_α:
                                                                                        jmp   n341_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n341_statement_begin_α:
                                                                                        jmp   n342_lit_real_α
n341_statement_begin_β:
                                                                                        jmp   n347_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n342_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx502_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n343_lit_real_α
.Lx502_0:
                        .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n343_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx503_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n344_call_α
.Lx503_0:
                        .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n344_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_real
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx505_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + min_alpha]
                                                                                        jmp   rax
.Lx505_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx505_240
                        add              rsp, 16
                        add              rsp, 224
                                                                                        jmp   n341_statement_begin_β
.Lx505_240:
                                                                                        jmp   n345_assign_α
n344_call_β:
                                                                                        jmp   n341_statement_begin_β
.Lx505_0:
                        .quad            .Lx505_0_s
.Lx505_0_s:
                        .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n345_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx506_0]
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
                                                                                        jmp   n346_statement_end_α
.Lx506_0:
                        .quad            .Lx506_0_s
.Lx506_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n346_statement_end_α:
                                                                                        jmp   n347_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_begin_α:
                                                                                        jmp   n348_lit_integer_α
n347_statement_begin_β:
                                                                                        jmp   n353_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n348_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx511_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n349_unop_α
n348_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 240
                                                                                        jmp   n347_statement_begin_β
.Lx511_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n349_unop_α:
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
                                                                                        jmp   n350_call_α
n349_unop_β:
                        add              rsp, 16
                                                                                        jmp   n348_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n350_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # unop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx514_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + abs_alpha]
                                                                                        jmp   rax
.Lx514_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx514_240
                        add              rsp, 16
                                                                                        jmp   n349_unop_β
.Lx514_240:
                                                                                        jmp   n351_assign_α
n350_call_β:
                                                                                        jmp   n349_unop_β
.Lx514_0:
                        .quad            .Lx514_0_s
.Lx514_0_s:
                        .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n351_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx515_0]
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
                                                                                        jmp   n352_statement_end_α
.Lx515_0:
                        .quad            .Lx515_0_s
.Lx515_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n352_statement_end_α:
                                                                                        jmp   n353_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n353_statement_begin_α:
                                                                                        jmp   n354_lit_integer_α
n353_statement_begin_β:
                                                                                        jmp   n358_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n354_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx520_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n355_call_α
n354_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 288
                                                                                        jmp   n353_statement_begin_β
.Lx520_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n355_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx522_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha]
                                                                                        jmp   rax
.Lx522_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx522_240
                        add              rsp, 16
                                                                                        jmp   n354_lit_integer_β
.Lx522_240:
                                                                                        jmp   n356_assign_α
n355_call_β:
                                                                                        jmp   n354_lit_integer_β
.Lx522_0:
                        .quad            .Lx522_0_s
.Lx522_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n356_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx523_0]
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
                                                                                        jmp   n357_statement_end_α
.Lx523_0:
                        .quad            .Lx523_0_s
.Lx523_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n357_statement_end_α:
                                                                                        jmp   n358_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n358_statement_begin_α:
                                                                                        jmp   n359_lit_integer_α
n358_statement_begin_β:
                                                                                        jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n359_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx528_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n360_call_α
n359_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 320
                                                                                        jmp   n358_statement_begin_β
.Lx528_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n360_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx530_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha]
                                                                                        jmp   rax
.Lx530_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx530_240
                        add              rsp, 16
                                                                                        jmp   n359_lit_integer_β
.Lx530_240:
                                                                                        jmp   n361_assign_α
n360_call_β:
                                                                                        jmp   n359_lit_integer_β
.Lx530_0:
                        .quad            .Lx530_0_s
.Lx530_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n361_assign_α:
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
                                                                                        jmp   n362_statement_end_α
.Lx531_0:
                        .quad            .Lx531_0_s
.Lx531_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_end_α:
                                                                                        jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n363_statement_begin_α:
                                                                                        jmp   n364_lit_integer_α
n363_statement_begin_β:
                                                                                        jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx536_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n365_unop_α
n364_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 352
                                                                                        jmp   n363_statement_begin_β
.Lx536_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n365_unop_α:
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
                                                                                        jmp   n366_call_α
n365_unop_β:
                        add              rsp, 16
                                                                                        jmp   n364_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n366_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # unop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx539_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha]
                                                                                        jmp   rax
.Lx539_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx539_240
                        add              rsp, 16
                                                                                        jmp   n365_unop_β
.Lx539_240:
                                                                                        jmp   n367_assign_α
n366_call_β:
                                                                                        jmp   n365_unop_β
.Lx539_0:
                        .quad            .Lx539_0_s
.Lx539_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n367_assign_α:
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
                                                                                        jmp   n368_statement_end_α
.Lx540_0:
                        .quad            .Lx540_0_s
.Lx540_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n368_statement_end_α:
                                                                                        jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n369_statement_begin_α:
                                                                                        jmp   n370_lit_integer_α
n369_statement_begin_β:
                                                                                        jmp   n375_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n370_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx545_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n371_lit_integer_α
n370_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 400
                                                                                        jmp   n369_statement_begin_β
.Lx545_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n371_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx546_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n372_call_α
n371_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n370_lit_integer_β
.Lx546_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n372_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx548_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha]
                                                                                        jmp   rax
.Lx548_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx548_240
                        add              rsp, 16
                                                                                        jmp   n371_lit_integer_β
.Lx548_240:
                                                                                        jmp   n373_assign_α
n372_call_β:
                                                                                        jmp   n371_lit_integer_β
.Lx548_0:
                        .quad            .Lx548_0_s
.Lx548_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n373_assign_α:
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
                                                                                        jmp   n374_statement_end_α
.Lx549_0:
                        .quad            .Lx549_0_s
.Lx549_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n374_statement_end_α:
                                                                                        jmp   n375_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n375_statement_begin_α:
                                                                                        jmp   n376_lit_integer_α
n375_statement_begin_β:
                                                                                        jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n376_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx554_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n377_lit_integer_α
n376_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 448
                                                                                        jmp   n375_statement_begin_β
.Lx554_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n377_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx555_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n378_call_α
n377_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n376_lit_integer_β
.Lx555_0:
                        .quad            75
#-----------------------------------------------------------------------------------------------------------------------
n378_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx557_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha]
                                                                                        jmp   rax
.Lx557_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx557_240
                        add              rsp, 16
                                                                                        jmp   n377_lit_integer_β
.Lx557_240:
                                                                                        jmp   n379_assign_α
n378_call_β:
                                                                                        jmp   n377_lit_integer_β
.Lx557_0:
                        .quad            .Lx557_0_s
.Lx557_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n379_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx558_0]
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
                                                                                        jmp   n380_statement_end_α
.Lx558_0:
                        .quad            .Lx558_0_s
.Lx558_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n380_statement_end_α:
                                                                                        jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n381_statement_begin_α:
                                                                                        jmp   n382_lit_integer_α
n381_statement_begin_β:
                        add              rsp, 496
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n382_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx563_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n383_lit_integer_α
n382_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 496
                                                                                        jmp   n381_statement_begin_β
.Lx563_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n383_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx564_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n384_call_α
n383_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n382_lit_integer_β
.Lx564_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n384_call_α:
                        sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx566_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + lcm_alpha]
                                                                                        jmp   rax
.Lx566_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx566_240
                        add              rsp, 16
                                                                                        jmp   n383_lit_integer_β
.Lx566_240:
                                                                                        jmp   n385_assign_α
n384_call_β:
                                                                                        jmp   n383_lit_integer_β
.Lx566_0:
                        .quad            .Lx566_0_s
.Lx566_0_s:
                        .string          "lcm"
#-----------------------------------------------------------------------------------------------------------------------
n385_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx567_0]
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
                                                                                        jmp   n386_statement_end_α
.Lx567_0:
                        .quad            .Lx567_0_s
.Lx567_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n386_statement_end_α:
                        add              rsp, 544
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n387_goto_α:
                                                                                        jmp   n287_statement_begin_α
n387_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n388_goto_α:
                                                                                        jmp   n292_statement_begin_α
n388_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n389_goto_α:
                                                                                        jmp   n297_statement_begin_α
n389_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n390_goto_α:
                                                                                        jmp   n302_statement_begin_α
n390_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n391_goto_α:
                                                                                        jmp   n307_statement_begin_α
n391_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n392_goto_α:
                                                                                        jmp   n312_statement_begin_α
n392_goto_β:
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
                        .section         .rodata
.S0:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_α:
proc_LBL__max_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # max
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_var_α
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n3_coerce_numeric_α
n2_var_β:               add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx15_1
                        cmp              eax, 3;                    jne   .Lx15_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 3;                    jne   .Lx15_0
.Lx15_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_coerce_numeric_α
.Lx15_0:                lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n4_coerce_numeric_α
n3_coerce_numeric_β:    add              rsp, 16;                   jmp   n2_var_β
#-----------------------------------------------------------------------------------------------------------------------
n4_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx17_1
                        cmp              eax, 3;                    jne   .Lx17_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 3;                    jne   .Lx17_0
.Lx17_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n5_cmp_test_α
.Lx17_0:                lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n5_cmp_test_α
n4_coerce_numeric_β:    add              rsp, 16;                   jmp   n3_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n5_cmp_test_α:          sub              rsp, 16
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
                        test             eax, eax;                  js    .Lx19_240
                        add              rsp, 16;                   jmp   n4_coerce_numeric_β
.Lx19_240:              mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0;    jmp   n6_var_α
n5_cmp_test_β:          add              rsp, 16;                   jmp   n4_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n7_binop_α
n6_var_β:               add              rsp, 16;                   jmp   n5_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n7_binop_α:             sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n8_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:            mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # max
                        mov              qword ptr [r9 + 8], rdx;   jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     add              rsp, 112;                  jmp   RETURN
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
n25_statement_begin_α:                                              jmp   n26_var_α
n25_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # min
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n27_var_α
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n28_coerce_numeric_α
n27_var_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n25_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n28_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx40_1
                        cmp              eax, 3;                    jne   .Lx40_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 3;                    jne   .Lx40_0
.Lx40_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n29_coerce_numeric_α
.Lx40_0:                lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n29_coerce_numeric_α
n28_coerce_numeric_β:   add              rsp, 16;                   jmp   n27_var_β
#-----------------------------------------------------------------------------------------------------------------------
n29_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx42_1
                        cmp              eax, 3;                    jne   .Lx42_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 3;                    jne   .Lx42_0
.Lx42_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n30_cmp_test_α
.Lx42_0:                lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n30_cmp_test_α
n29_coerce_numeric_β:   add              rsp, 16;                   jmp   n28_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n30_cmp_test_α:         sub              rsp, 16
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
                        test             eax, eax;                  jg    .Lx44_240
                        add              rsp, 16;                   jmp   n29_coerce_numeric_β
.Lx44_240:              mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0;    jmp   n31_var_α
n30_cmp_test_β:         add              rsp, 16;                   jmp   n29_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n31_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n32_binop_α
n31_var_β:              add              rsp, 16;                   jmp   n30_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n32_binop_α:            sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n33_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # min
                        mov              qword ptr [r9 + 40], rdx;  jmp   n34_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    add              rsp, 112;                  jmp   RETURN
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
n50_statement_begin_α:                                              jmp   n51_var_α
n50_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # abs
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n53_coerce_numeric_α
n52_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n50_statement_begin_β
.Lx64_0:                .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n53_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx66_1
                        cmp              eax, 3;                    jne   .Lx66_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx66_0
.Lx66_1:                mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n54_coerce_numeric_α
.Lx66_0:                lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n54_coerce_numeric_α
n53_coerce_numeric_β:   add              rsp, 16;                   jmp   n52_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n54_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5;                    je    .Lx68_1
                        cmp              eax, 3;                    jne   .Lx68_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3;                    jne   .Lx68_0
.Lx68_1:                mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n55_cmp_test_α
.Lx68_0:                lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n55_cmp_test_α
n54_coerce_numeric_β:   add              rsp, 16;                   jmp   n53_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n55_cmp_test_α:         sub              rsp, 16
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
                        test             eax, eax;                  js    .Lx70_240
                        add              rsp, 16;                   jmp   n54_coerce_numeric_β
.Lx70_240:              mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0;    jmp   n56_var_α
n55_cmp_test_β:         add              rsp, 16;                   jmp   n54_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # abs
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n57_unop_α
n56_var_β:              add              rsp, 16;                   jmp   n55_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n57_unop_α:             sub              rsp, 16
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n58_binop_α
n57_unop_β:             add              rsp, 16;                   jmp   n56_var_β
#-----------------------------------------------------------------------------------------------------------------------
n58_binop_α:            sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n59_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # abs
                        mov              qword ptr [r9 + 56], rdx;  jmp   n60_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 128;                  jmp   RETURN
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
n77_statement_begin_α:                                              jmp   n78_var_α
n77_statement_begin_β:                                              jmp   n88_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n78_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n79_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n80_coerce_numeric_α
n79_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n77_statement_begin_β
.Lx101_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n80_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx103_1
                        cmp              eax, 3;                    jne   .Lx103_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx103_0
.Lx103_1:               mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n81_coerce_numeric_α
.Lx103_0:               lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n81_coerce_numeric_α
n80_coerce_numeric_β:   add              rsp, 16;                   jmp   n79_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n81_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5;                    je    .Lx105_1
                        cmp              eax, 3;                    jne   .Lx105_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3;                    jne   .Lx105_0
.Lx105_1:               mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n82_cmp_test_α
.Lx105_0:               lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n82_cmp_test_α
n81_coerce_numeric_β:   add              rsp, 16;                   jmp   n80_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n82_cmp_test_α:         sub              rsp, 16
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
                        test             eax, eax;                  js    .Lx107_240
                        add              rsp, 16;                   jmp   n81_coerce_numeric_β
.Lx107_240:             mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0;    jmp   n83_lit_integer_α
n82_cmp_test_β:         add              rsp, 16;                   jmp   n81_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx108_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n84_unop_α
n83_lit_integer_β:      add              rsp, 16;                   jmp   n82_cmp_test_β
.Lx108_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n84_unop_α:             sub              rsp, 16
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n85_binop_α
n84_unop_β:             add              rsp, 16;                   jmp   n83_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n85_binop_α:            sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n86_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n86_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax                       # sign
                        mov              qword ptr [r9 + 72], rdx;  jmp   n87_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_end_α:    add              rsp, 128;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_begin_α:                                              jmp   n89_var_α
n88_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n89_var_α:              mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 656], rax                     # result
                        mov              qword ptr [rsp + 664], rdx; jmp   n90_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_integer_α:      mov              qword ptr [rsp + 640], 3                       # result
                        mov              rax, qword ptr [rip + .Lx117_0]
                        mov              qword ptr [rsp + 648], rax; jmp   n91_coerce_numeric_α
.Lx117_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n91_coerce_numeric_α:   mov              eax, dword ptr [rsp + 656]
                        cmp              eax, 5;                    je    .Lx119_1
                        cmp              eax, 3;                    jne   .Lx119_0
                        mov              eax, dword ptr [rsp + 640]
                        cmp              eax, 3;                    jne   .Lx119_0
.Lx119_1:               mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 624], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 632], rax; jmp   n92_coerce_numeric_α
.Lx119_0:               lea              rdi, [rsp + 656]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n92_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n92_coerce_numeric_α:   mov              eax, dword ptr [rsp + 640]
                        cmp              eax, 5;                    je    .Lx121_1
                        cmp              eax, 3;                    jne   .Lx121_0
                        mov              eax, dword ptr [rsp + 656]
                        cmp              eax, 3;                    jne   .Lx121_0
.Lx121_1:               mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 616], rax; jmp   n93_cmp_test_α
.Lx121_0:               lea              rdi, [rsp + 640]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n93_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n93_cmp_test_α:         lea              rdi, [rsp + 624]
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
                        test             eax, eax;                  jle   n88_statement_begin_β
                        mov              qword ptr [rsp + 592], 0
                        mov              qword ptr [rsp + 600], 0;  jmp   n94_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_integer_α:      mov              qword ptr [rsp + 672], 3                       # result
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rsp + 680], rax; jmp   n95_binop_α
.Lx124_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n95_binop_α:            mov              rdi, qword ptr [rsp + 592]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n96_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n96_assign_α:           mov              rax, qword ptr [rsp + 576]
                        mov              rdx, qword ptr [rsp + 584]
                        mov              qword ptr [r9 + 64], rax                       # sign
                        mov              qword ptr [r9 + 72], rdx;  jmp   n97_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_end_α:                                                jmp   RETURN
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
n129_statement_begin_α:                                             jmp   n130_var_α
n129_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n130_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n131_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n131_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx151_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n132_call_α
n131_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n129_statement_begin_β
.Lx151_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n132_call_α:            sub              rsp, 16
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
.Lrkfnzd153:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd153]
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
                        cmp              eax, 104;                  jne   .Lx152_240
                        add              rsp, 16;                   jmp   n131_lit_integer_β
.Lx152_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n133_statement_end_α
n132_call_β:            add              rsp, 16;                   jmp   n131_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_end_α:                                               jmp   n134_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n134_statement_begin_α:                                             jmp   n135_var_α
n134_statement_begin_β:                                             jmp   n140_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n135_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]                       # gcd
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n136_var_α
n135_var_β:             add              rsp, 16
                        add              rsp, 48;                   jmp   n134_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n136_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n137_call_α
n136_var_β:             add              rsp, 16;                   jmp   n135_var_β
#-----------------------------------------------------------------------------------------------------------------------
n137_call_α:            sub              rsp, 16
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
.Lrkfnzd161:            .string          "REMDR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd161]
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
                        cmp              eax, 104;                  jne   .Lx160_240
                        add              rsp, 16;                   jmp   n136_var_β
.Lx160_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n138_assign_α
n137_call_β:            add              rsp, 16;                   jmp   n136_var_β
#-----------------------------------------------------------------------------------------------------------------------
n138_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              qword ptr [r9 + 120], rdx; jmp   n139_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_end_α:                                               jmp   n140_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_begin_α:                                             jmp   n141_var_α
n140_statement_begin_β:                                             jmp   n144_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n141_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n142_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n142_assign_α:          mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              qword ptr [r9 + 88], rdx;  jmp   n143_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_end_α:                                               jmp   n144_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_begin_α:                                             jmp   n145_var_α
n144_statement_begin_β:                                             jmp   n129_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n145_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n146_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n146_assign_α:          mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              qword ptr [r9 + 104], rdx; jmp   n147_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_end_α:   add              rsp, 128;                  jmp   n129_statement_begin_α
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
n177_statement_begin_α:                                             jmp   n178_var_α
n177_statement_begin_β:                                             jmp   n183_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n178_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # a
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n179_var_α
#-----------------------------------------------------------------------------------------------------------------------
n179_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n180_call_α
n179_var_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n177_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n180_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx196_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha];    jmp   rax
.Lx196_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx196_240
                        add              rsp, 16;                   jmp   n179_var_β
.Lx196_240:                                                         jmp   n181_assign_α
n180_call_β:                                                        jmp   n179_var_β
.Lx196_0:               .quad            .Lx196_0_s
.Lx196_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n181_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              qword ptr [r9 + 168], rdx; jmp   n182_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_end_α:                                               jmp   n183_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_begin_α:                                             jmp   n184_var_α
n183_statement_begin_β: add              rsp, 48;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n184_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # a
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n185_var_α
n184_var_β:             add              rsp, 16
                        add              rsp, 48;                   jmp   n183_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n185_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 160]                      # g
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n186_binop_α
n185_var_β:             add              rsp, 16;                   jmp   n184_var_β
#-----------------------------------------------------------------------------------------------------------------------
n186_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_div@PLT
                        cmp              eax, 104;                  jne   .Lx204_240
                        add              rsp, 16;                   jmp   n185_var_β
.Lx204_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n187_var_α
n186_binop_β:           add              rsp, 16;                   jmp   n185_var_β
#-----------------------------------------------------------------------------------------------------------------------
n187_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n188_binop_α
n187_var_β:             add              rsp, 16;                   jmp   n186_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n188_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                  jne   .Lx206_240
                        add              rsp, 16;                   jmp   n187_var_β
.Lx206_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n189_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n189_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # lcm
                        mov              qword ptr [r9 + 136], rdx; jmp   n190_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n190_statement_end_α:   add              rsp, 128;                  jmp   RETURN
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
n210_save_restore_α:                                                jmp   n211_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n211_goto_deferred_α:   lea              rax, [rip + proc_LBL__max_α]; jmp   rax
                                                                    jmp   n212_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n212_save_restore_α:
max_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx218_2
                        mov              eax, 32
.Lx218_2:               sub              rsp, 64
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx218_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 0]                        # max
                        mov              qword ptr [r9 + 0], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx218_41
.Lx218_10:              mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
.Lx218_41:              cmp              rcx, 1;                    jbe   .Lx218_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx218_42
.Lx218_11:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx218_42:              lea              r10, [rip + max_gamma]
                        lea              r11, [rip + max_omega]
                        lea              rax, [rip + proc_LBL__max_α]; jmp   rax
max_gamma:              mov              rdi, qword ptr [r9 + 0]                        # max
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx218_75
                        mov              eax, 32
.Lx218_75:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx218_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 8], rax;   jmp   .Lx218_110
.Lx218_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx218_110:             cmp              rcx, 1;                    jbe   .Lx218_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx218_111
.Lx218_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx218_111:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2;                    jbe   .Lx218_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx218_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
max_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx218_145
                        mov              eax, 32
.Lx218_145:             lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx218_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 0], rax                        # max
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 8], rax;   jmp   .Lx218_180
.Lx218_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx218_180:             cmp              rcx, 1;                    jbe   .Lx218_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx218_181
.Lx218_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx218_181:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2;                    jbe   .Lx218_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx218_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n219_save_restore_α:                                                jmp   n220_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n220_goto_deferred_α:   lea              rax, [rip + proc_LBL__min_α]; jmp   rax
                                                                    jmp   n221_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n221_save_restore_α:
min_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx227_2
                        mov              eax, 32
.Lx227_2:               sub              rsp, 64
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx227_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 32]                       # min
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx227_41
.Lx227_10:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx227_41:              cmp              rcx, 1;                    jbe   .Lx227_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx227_42
.Lx227_11:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx227_42:              lea              r10, [rip + min_gamma]
                        lea              r11, [rip + min_omega]
                        lea              rax, [rip + proc_LBL__min_α]; jmp   rax
min_gamma:              mov              rdi, qword ptr [r9 + 32]                       # min
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx227_75
                        mov              eax, 32
.Lx227_75:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx227_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx227_110
.Lx227_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx227_110:             cmp              rcx, 1;                    jbe   .Lx227_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx227_111
.Lx227_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx227_111:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2;                    jbe   .Lx227_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx227_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
min_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx227_145
                        mov              eax, 32
.Lx227_145:             lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx227_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 32], rax                       # min
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx227_180
.Lx227_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx227_180:             cmp              rcx, 1;                    jbe   .Lx227_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx227_181
.Lx227_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx227_181:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2;                    jbe   .Lx227_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx227_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n228_save_restore_α:                                                jmp   n229_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n229_goto_deferred_α:   lea              rax, [rip + proc_LBL__abs_α]; jmp   rax
                                                                    jmp   n230_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n230_save_restore_α:
abs_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx236_2
                        mov              eax, 16
.Lx236_2:               sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx236_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 48]                       # abs
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx236_41
.Lx236_10:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx236_41:              lea              r10, [rip + abs_gamma]
                        lea              r11, [rip + abs_omega]
                        lea              rax, [rip + proc_LBL__abs_α]; jmp   rax
abs_gamma:              mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx236_75
                        mov              eax, 16
.Lx236_75:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx236_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx236_110
.Lx236_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 56], rax
.Lx236_110:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx236_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx236_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
abs_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx236_145
                        mov              eax, 16
.Lx236_145:             lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx236_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx236_180
.Lx236_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 56], rax
.Lx236_180:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx236_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx236_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n237_save_restore_α:                                                jmp   n238_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n238_goto_deferred_α:   lea              rax, [rip + proc_LBL__sign_α]; jmp   rax
                                                                    jmp   n239_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n239_save_restore_α:
sign_alpha:             mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx245_2
                        mov              eax, 16
.Lx245_2:               sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx245_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 64]                       # sign
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx245_41
.Lx245_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx245_41:              lea              r10, [rip + sign_gamma]
                        lea              r11, [rip + sign_omega]
                        lea              rax, [rip + proc_LBL__sign_α]; jmp   rax
sign_gamma:             mov              rdi, qword ptr [r9 + 64]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx245_75
                        mov              eax, 16
.Lx245_75:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx245_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax;  jmp   .Lx245_110
.Lx245_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx245_110:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx245_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx245_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
sign_omega:             mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx245_145
                        mov              eax, 16
.Lx245_145:             lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx245_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax;  jmp   .Lx245_180
.Lx245_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx245_180:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx245_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx245_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n246_save_restore_α:                                                jmp   n247_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n247_goto_deferred_α:   lea              rax, [rip + proc_LBL__gcd_α]; jmp   rax
                                                                    jmp   n248_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n248_save_restore_α:
gcd_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx254_2
                        mov              eax, 32
.Lx254_2:               sub              rsp, 80
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
                        cmp              rcx, 0;                    jbe   .Lx254_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 80]                       # gcd
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx254_41
.Lx254_10:              mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx254_41:              cmp              rcx, 1;                    jbe   .Lx254_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 96]                       # b
                        mov              qword ptr [r9 + 96], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [r9 + 104], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx254_42
.Lx254_11:              mov              rax, qword ptr [r9 + 96]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
.Lx254_42:              lea              r10, [rip + gcd_gamma]
                        lea              r11, [rip + gcd_omega]
                        lea              rax, [rip + proc_LBL__gcd_α]; jmp   rax
gcd_gamma:              mov              rdi, qword ptr [r9 + 80]                       # gcd
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx254_75
                        mov              eax, 32
.Lx254_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0;                    jbe   .Lx254_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 88], rax;  jmp   .Lx254_110
.Lx254_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 88], rax
.Lx254_110:             cmp              rcx, 1;                    jbe   .Lx254_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax; jmp   .Lx254_111
.Lx254_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx254_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx254_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx254_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
gcd_omega:              mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx254_145
                        mov              eax, 32
.Lx254_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0;                    jbe   .Lx254_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 88], rax;  jmp   .Lx254_180
.Lx254_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 88], rax
.Lx254_180:             cmp              rcx, 1;                    jbe   .Lx254_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax; jmp   .Lx254_181
.Lx254_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx254_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx254_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx254_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n255_save_restore_α:                                                jmp   n256_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n256_goto_deferred_α:   lea              rax, [rip + proc_LBL__lcm_α]; jmp   rax
                                                                    jmp   n257_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n257_save_restore_α:
lcm_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx263_2
                        mov              eax, 32
.Lx263_2:               sub              rsp, 96
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
                        cmp              rcx, 0;                    jbe   .Lx263_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 144]                      # a
                        mov              qword ptr [r9 + 144], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [r9 + 152], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx263_41
.Lx263_10:              mov              rax, qword ptr [r9 + 144]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 144], 0
                        mov              qword ptr [r9 + 152], 0
.Lx263_41:              cmp              rcx, 1;                    jbe   .Lx263_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 96]                       # b
                        mov              qword ptr [r9 + 96], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [r9 + 104], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx263_42
.Lx263_11:              mov              rax, qword ptr [r9 + 96]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
.Lx263_42:              lea              r10, [rip + lcm_gamma]
                        lea              r11, [rip + lcm_omega]
                        lea              rax, [rip + proc_LBL__lcm_α]; jmp   rax
lcm_gamma:              mov              rdi, qword ptr [r9 + 128]                      # lcm
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx263_75
                        mov              eax, 32
.Lx263_75:              lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 168], rax
                        cmp              rcx, 0;                    jbe   .Lx263_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 144], rax                      # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 152], rax; jmp   .Lx263_110
.Lx263_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 144], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 152], rax
.Lx263_110:             cmp              rcx, 1;                    jbe   .Lx263_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax; jmp   .Lx263_111
.Lx263_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx263_111:             mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                    jbe   .Lx263_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx263_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
lcm_omega:              mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx263_145
                        mov              eax, 32
.Lx263_145:             lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 128], rax                      # lcm
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 168], rax
                        cmp              rcx, 0;                    jbe   .Lx263_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 144], rax                      # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 152], rax; jmp   .Lx263_180
.Lx263_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 144], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 152], rax
.Lx263_180:             cmp              rcx, 1;                    jbe   .Lx263_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax; jmp   .Lx263_181
.Lx263_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx263_181:             mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                    jbe   .Lx263_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx263_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n264_statement_begin_α:                                             jmp   n265_statement_end_α
n264_statement_begin_β:                                             jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:                                               jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_begin_α:                                             jmp   n267_func_activate_α
n266_statement_begin_β:                                             jmp   n269_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_func_activate_α:   mov              rdi, qword ptr [rip + .Lx383_0]
                        mov              rsi, qword ptr [rip + .Lx383_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n268_statement_end_α
n267_func_activate_β:                                               jmp   n266_statement_begin_β
.Lx383_0:               .quad            .Lx383_0_s
.Lx383_0_s:             .string          "max"
.Lx383_1:               .quad            .Lx383_1_s
.Lx383_1_s:             .string          "max,x"
#-----------------------------------------------------------------------------------------------------------------------
n268_statement_end_α:                                               jmp   n269_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n269_statement_begin_α:                                             jmp   n270_statement_end_α
n269_statement_begin_β:                                             jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:                                               jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_begin_α:                                             jmp   n272_func_activate_α
n271_statement_begin_β:                                             jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n272_func_activate_α:   mov              rdi, qword ptr [rip + .Lx393_0]
                        mov              rsi, qword ptr [rip + .Lx393_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n273_statement_end_α
n272_func_activate_β:                                               jmp   n271_statement_begin_β
.Lx393_0:               .quad            .Lx393_0_s
.Lx393_0_s:             .string          "min"
.Lx393_1:               .quad            .Lx393_1_s
.Lx393_1_s:             .string          "min,x"
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_end_α:                                               jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_begin_α:                                             jmp   n275_statement_end_α
n274_statement_begin_β:                                             jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_end_α:                                               jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_begin_α:                                             jmp   n277_func_activate_α
n276_statement_begin_β:                                             jmp   n279_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n277_func_activate_α:   mov              rdi, qword ptr [rip + .Lx403_0]
                        mov              rsi, qword ptr [rip + .Lx403_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n278_statement_end_α
n277_func_activate_β:                                               jmp   n276_statement_begin_β
.Lx403_0:               .quad            .Lx403_0_s
.Lx403_0_s:             .string          "abs"
.Lx403_1:               .quad            .Lx403_1_s
.Lx403_1_s:             .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n278_statement_end_α:                                               jmp   n279_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n279_statement_begin_α:                                             jmp   n280_statement_end_α
n279_statement_begin_β:                                             jmp   n281_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n280_statement_end_α:                                               jmp   n281_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n281_statement_begin_α:                                             jmp   n282_func_activate_α
n281_statement_begin_β:                                             jmp   n284_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n282_func_activate_α:   mov              rdi, qword ptr [rip + .Lx413_0]
                        mov              rsi, qword ptr [rip + .Lx413_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n283_statement_end_α
n282_func_activate_β:                                               jmp   n281_statement_begin_β
.Lx413_0:               .quad            .Lx413_0_s
.Lx413_0_s:             .string          "sign"
.Lx413_1:               .quad            .Lx413_1_s
.Lx413_1_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n283_statement_end_α:                                               jmp   n284_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n284_statement_begin_α:                                             jmp   n285_statement_end_α
n284_statement_begin_β:                                             jmp   n286_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n285_statement_end_α:                                               jmp   n286_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n286_statement_begin_α:                                             jmp   n287_func_activate_α
n286_statement_begin_β:                                             jmp   n289_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n287_func_activate_α:   mov              rdi, qword ptr [rip + .Lx423_0]
                        mov              rsi, qword ptr [rip + .Lx423_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n288_statement_end_α
n287_func_activate_β:                                               jmp   n286_statement_begin_β
.Lx423_0:               .quad            .Lx423_0_s
.Lx423_0_s:             .string          "gcd"
.Lx423_1:               .quad            .Lx423_1_s
.Lx423_1_s:             .string          "gcd,b,r"
#-----------------------------------------------------------------------------------------------------------------------
n288_statement_end_α:                                               jmp   n289_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n289_statement_begin_α:                                             jmp   n290_statement_end_α
n289_statement_begin_β:                                             jmp   n291_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_end_α:                                               jmp   n291_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_begin_α:                                             jmp   n292_func_activate_α
n291_statement_begin_β:                                             jmp   n294_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n292_func_activate_α:   mov              rdi, qword ptr [rip + .Lx433_0]
                        mov              rsi, qword ptr [rip + .Lx433_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n293_statement_end_α
n292_func_activate_β:                                               jmp   n291_statement_begin_β
.Lx433_0:               .quad            .Lx433_0_s
.Lx433_0_s:             .string          "lcm"
.Lx433_1:               .quad            .Lx433_1_s
.Lx433_1_s:             .string          "a,b,g"
#-----------------------------------------------------------------------------------------------------------------------
n293_statement_end_α:                                               jmp   n294_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n294_statement_begin_α:                                             jmp   n295_statement_end_α
n294_statement_begin_β:                                             jmp   n296_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n295_statement_end_α:                                               jmp   n296_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n296_statement_begin_α:                                             jmp   n297_statement_end_α
n296_statement_begin_β:                                             jmp   n298_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n297_statement_end_α:                                               jmp   n298_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n298_statement_begin_α:                                             jmp   n299_lit_string_α
n298_statement_begin_β:                                             jmp   n303_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n299_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx446_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n300_lit_integer_α
.Lx446_0:               .quad            .Lx446_0_s
.Lx446_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n300_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx447_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n301_call_α
n300_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n298_statement_begin_β
.Lx447_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n301_call_α:            sub              rsp, 16
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
.Lrkfnzd449:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd449]
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
                        cmp              eax, 104;                  jne   .Lx448_240
                        add              rsp, 16;                   jmp   n300_lit_integer_β
.Lx448_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n302_statement_end_α
n301_call_β:            add              rsp, 16;                   jmp   n300_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n302_statement_end_α:                                               jmp   n303_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_begin_α:                                             jmp   n304_statement_end_α
n303_statement_begin_β:                                             jmp   n305_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_end_α:                                               jmp   n305_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n305_statement_begin_α:                                             jmp   n306_lit_integer_α
n305_statement_begin_β:                                             jmp   n311_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n306_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx458_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n307_lit_integer_α
n306_lit_integer_β:     add              rsp, 16
                        add              rsp, 48;                   jmp   n305_statement_begin_β
.Lx458_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n307_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx459_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n308_call_α
n307_lit_integer_β:     add              rsp, 16;                   jmp   n306_lit_integer_β
.Lx459_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n308_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx461_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + max_alpha];    jmp   rax
.Lx461_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx461_240
                        add              rsp, 16;                   jmp   n307_lit_integer_β
.Lx461_240:                                                         jmp   n309_assign_α
n308_call_β:                                                        jmp   n307_lit_integer_β
.Lx461_0:               .quad            .Lx461_0_s
.Lx461_0_s:             .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n309_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx462_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n310_statement_end_α
.Lx462_0:               .quad            .Lx462_0_s
.Lx462_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n310_statement_end_α:                                               jmp   n311_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n311_statement_begin_α:                                             jmp   n312_lit_integer_α
n311_statement_begin_β:                                             jmp   n317_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n312_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx467_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n313_lit_integer_α
n312_lit_integer_β:     add              rsp, 16
                        add              rsp, 96;                   jmp   n311_statement_begin_β
.Lx467_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n313_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx468_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n314_call_α
n313_lit_integer_β:     add              rsp, 16;                   jmp   n312_lit_integer_β
.Lx468_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n314_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx470_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + min_alpha];    jmp   rax
.Lx470_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx470_240
                        add              rsp, 16;                   jmp   n313_lit_integer_β
.Lx470_240:                                                         jmp   n315_assign_α
n314_call_β:                                                        jmp   n313_lit_integer_β
.Lx470_0:               .quad            .Lx470_0_s
.Lx470_0_s:             .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n315_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx471_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n316_statement_end_α
.Lx471_0:               .quad            .Lx471_0_s
.Lx471_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_end_α:                                               jmp   n317_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_begin_α:                                             jmp   n318_lit_real_α
n317_statement_begin_β:                                             jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n318_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx476_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n319_lit_real_α
.Lx476_0:               .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n319_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx477_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n320_call_α
.Lx477_0:               .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n320_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx479_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + max_alpha];    jmp   rax
.Lx479_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx479_240
                        add              rsp, 16
                        add              rsp, 176;                  jmp   n317_statement_begin_β
.Lx479_240:                                                         jmp   n321_assign_α
n320_call_β:                                                        jmp   n317_statement_begin_β
.Lx479_0:               .quad            .Lx479_0_s
.Lx479_0_s:             .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n321_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx480_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n322_statement_end_α
.Lx480_0:               .quad            .Lx480_0_s
.Lx480_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:                                               jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n323_statement_begin_α:                                             jmp   n324_lit_real_α
n323_statement_begin_β:                                             jmp   n329_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx485_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n325_lit_real_α
.Lx485_0:               .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n325_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx486_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n326_call_α
.Lx486_0:               .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n326_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx488_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + min_alpha];    jmp   rax
.Lx488_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx488_240
                        add              rsp, 16
                        add              rsp, 224;                  jmp   n323_statement_begin_β
.Lx488_240:                                                         jmp   n327_assign_α
n326_call_β:                                                        jmp   n323_statement_begin_β
.Lx488_0:               .quad            .Lx488_0_s
.Lx488_0_s:             .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n327_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx489_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n328_statement_end_α
.Lx489_0:               .quad            .Lx489_0_s
.Lx489_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n328_statement_end_α:                                               jmp   n329_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n329_statement_begin_α:                                             jmp   n330_lit_integer_α
n329_statement_begin_β:                                             jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n330_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx494_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n331_unop_α
n330_lit_integer_β:     add              rsp, 16
                        add              rsp, 240;                  jmp   n329_statement_begin_β
.Lx494_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n331_unop_α:            sub              rsp, 16
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n332_call_α
n331_unop_β:            add              rsp, 16;                   jmp   n330_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n332_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # unop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx497_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + abs_alpha];    jmp   rax
.Lx497_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx497_240
                        add              rsp, 16;                   jmp   n331_unop_β
.Lx497_240:                                                         jmp   n333_assign_α
n332_call_β:                                                        jmp   n331_unop_β
.Lx497_0:               .quad            .Lx497_0_s
.Lx497_0_s:             .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n333_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx498_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n334_statement_end_α
.Lx498_0:               .quad            .Lx498_0_s
.Lx498_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:                                               jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α:                                             jmp   n336_lit_integer_α
n335_statement_begin_β:                                             jmp   n340_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx503_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n337_call_α
n336_lit_integer_β:     add              rsp, 16
                        add              rsp, 288;                  jmp   n335_statement_begin_β
.Lx503_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n337_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx505_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha];   jmp   rax
.Lx505_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx505_240
                        add              rsp, 16;                   jmp   n336_lit_integer_β
.Lx505_240:                                                         jmp   n338_assign_α
n337_call_β:                                                        jmp   n336_lit_integer_β
.Lx505_0:               .quad            .Lx505_0_s
.Lx505_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n338_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n339_statement_end_α
.Lx506_0:               .quad            .Lx506_0_s
.Lx506_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n339_statement_end_α:                                               jmp   n340_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n340_statement_begin_α:                                             jmp   n341_lit_integer_α
n340_statement_begin_β:                                             jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n341_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx511_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n342_call_α
n341_lit_integer_β:     add              rsp, 16
                        add              rsp, 320;                  jmp   n340_statement_begin_β
.Lx511_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n342_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx513_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha];   jmp   rax
.Lx513_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx513_240
                        add              rsp, 16;                   jmp   n341_lit_integer_β
.Lx513_240:                                                         jmp   n343_assign_α
n342_call_β:                                                        jmp   n341_lit_integer_β
.Lx513_0:               .quad            .Lx513_0_s
.Lx513_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n343_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx514_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n344_statement_end_α
.Lx514_0:               .quad            .Lx514_0_s
.Lx514_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n344_statement_end_α:                                               jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n345_statement_begin_α:                                             jmp   n346_lit_integer_α
n345_statement_begin_β:                                             jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n346_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx519_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n347_unop_α
n346_lit_integer_β:     add              rsp, 16
                        add              rsp, 352;                  jmp   n345_statement_begin_β
.Lx519_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n347_unop_α:            sub              rsp, 16
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n348_call_α
n347_unop_β:            add              rsp, 16;                   jmp   n346_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n348_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # unop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx522_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha];   jmp   rax
.Lx522_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx522_240
                        add              rsp, 16;                   jmp   n347_unop_β
.Lx522_240:                                                         jmp   n349_assign_α
n348_call_β:                                                        jmp   n347_unop_β
.Lx522_0:               .quad            .Lx522_0_s
.Lx522_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n349_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n350_statement_end_α
.Lx523_0:               .quad            .Lx523_0_s
.Lx523_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n350_statement_end_α:                                               jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n351_statement_begin_α:                                             jmp   n352_lit_integer_α
n351_statement_begin_β:                                             jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n352_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx528_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n353_lit_integer_α
n352_lit_integer_β:     add              rsp, 16
                        add              rsp, 400;                  jmp   n351_statement_begin_β
.Lx528_0:               .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n353_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx529_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n354_call_α
n353_lit_integer_β:     add              rsp, 16;                   jmp   n352_lit_integer_β
.Lx529_0:               .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n354_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx531_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha];    jmp   rax
.Lx531_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx531_240
                        add              rsp, 16;                   jmp   n353_lit_integer_β
.Lx531_240:                                                         jmp   n355_assign_α
n354_call_β:                                                        jmp   n353_lit_integer_β
.Lx531_0:               .quad            .Lx531_0_s
.Lx531_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n355_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx532_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n356_statement_end_α
.Lx532_0:               .quad            .Lx532_0_s
.Lx532_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n356_statement_end_α:                                               jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n357_statement_begin_α:                                             jmp   n358_lit_integer_α
n357_statement_begin_β:                                             jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n358_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx537_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n359_lit_integer_α
n358_lit_integer_β:     add              rsp, 16
                        add              rsp, 448;                  jmp   n357_statement_begin_β
.Lx537_0:               .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n359_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx538_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n360_call_α
n359_lit_integer_β:     add              rsp, 16;                   jmp   n358_lit_integer_β
.Lx538_0:               .quad            75
#-----------------------------------------------------------------------------------------------------------------------
n360_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx540_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha];    jmp   rax
.Lx540_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx540_240
                        add              rsp, 16;                   jmp   n359_lit_integer_β
.Lx540_240:                                                         jmp   n361_assign_α
n360_call_β:                                                        jmp   n359_lit_integer_β
.Lx540_0:               .quad            .Lx540_0_s
.Lx540_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n361_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx541_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n362_statement_end_α
.Lx541_0:               .quad            .Lx541_0_s
.Lx541_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_end_α:                                               jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n363_statement_begin_α:                                             jmp   n364_lit_integer_α
n363_statement_begin_β: add              rsp, 496;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx546_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n365_lit_integer_α
n364_lit_integer_β:     add              rsp, 16
                        add              rsp, 496;                  jmp   n363_statement_begin_β
.Lx546_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n365_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx547_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n366_call_α
n365_lit_integer_β:     add              rsp, 16;                   jmp   n364_lit_integer_β
.Lx547_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n366_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx549_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + lcm_alpha];    jmp   rax
.Lx549_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx549_240
                        add              rsp, 16;                   jmp   n365_lit_integer_β
.Lx549_240:                                                         jmp   n367_assign_α
n366_call_β:                                                        jmp   n365_lit_integer_β
.Lx549_0:               .quad            .Lx549_0_s
.Lx549_0_s:             .string          "lcm"
#-----------------------------------------------------------------------------------------------------------------------
n367_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx550_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n368_statement_end_α
.Lx550_0:               .quad            .Lx550_0_s
.Lx550_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n368_statement_end_α:   add              rsp, 544;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n369_goto_α:                                                        jmp   n269_statement_begin_α
n369_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n370_goto_α:                                                        jmp   n274_statement_begin_α
n370_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n371_goto_α:                                                        jmp   n279_statement_begin_α
n371_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n372_goto_α:                                                        jmp   n284_statement_begin_α
n372_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n373_goto_α:                                                        jmp   n289_statement_begin_α
n373_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n374_goto_α:                                                        jmp   n294_statement_begin_α
n374_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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

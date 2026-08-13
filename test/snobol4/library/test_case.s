                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_α:
proc_LBL__lwr_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # lwr
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n2_keyword_snobol4_α:   sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx10_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n3_keyword_snobol4_α
.Lx10_0:                .quad            .Lx10_0_s
.Lx10_0_s:              .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n3_keyword_snobol4_α:   sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx11_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n4_call_α
.Lx11_0:                .quad            .Lx11_0_s
.Lx11_0_s:              .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
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
.Lrkfnzd13:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd13]
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
                        cmp              eax, 104;                  jne   .Lx12_240
                        add              rsp, 16
                        add              rsp, 48;                   jmp   n0_statement_begin_β
.Lx12_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n5_assign_α
n4_call_β:              add              rsp, 16
                        add              rsp, 48;                   jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:            mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # lwr
                        mov              qword ptr [r9 + 8], rdx;   jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 64;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_β:
                                                                    jmp   proc_LBL__lwr_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_α:
proc_LBL__upr_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:                                              jmp   n18_var_α
n17_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # upr
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n19_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n19_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx27_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n20_keyword_snobol4_α
.Lx27_0:                .quad            .Lx27_0_s
.Lx27_0_s:              .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n20_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx28_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n21_call_α
.Lx28_0:                .quad            .Lx28_0_s
.Lx28_0_s:              .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n21_call_α:             sub              rsp, 16
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
.Lrkfnzd30:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd30]
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
                        cmp              eax, 104;                  jne   .Lx29_240
                        add              rsp, 16
                        add              rsp, 48;                   jmp   n17_statement_begin_β
.Lx29_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n22_assign_α
n21_call_β:             add              rsp, 16
                        add              rsp, 48;                   jmp   n17_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # upr
                        mov              qword ptr [r9 + 24], rdx;  jmp   n23_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:    add              rsp, 64;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_β:
                                                                    jmp   proc_LBL__upr_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_α:
proc_LBL__cap_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_begin_α:                                              jmp   n35_var_α
n34_statement_begin_β:                                              jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n35_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # cap
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n36_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n37_lit_integer_α
n36_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n34_statement_begin_β
.Lx54_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx55_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n38_call_α
n37_lit_integer_β:      add              rsp, 16;                   jmp   n36_lit_integer_β
.Lx55_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n38_call_α:             sub              rsp, 16
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
.Lrkfnzd57:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd57]
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
                        cmp              eax, 104;                  jne   .Lx56_240
                        add              rsp, 16;                   jmp   n37_lit_integer_β
.Lx56_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n39_keyword_snobol4_α
n38_call_β:             add              rsp, 16;                   jmp   n37_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n39_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx58_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n40_keyword_snobol4_α
.Lx58_0:                .quad            .Lx58_0_s
.Lx58_0_s:              .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n40_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx59_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n41_call_α
.Lx59_0:                .quad            .Lx59_0_s
.Lx59_0_s:              .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n41_call_α:             sub              rsp, 16
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
.Lrkfnzd61:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd61]
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
                        cmp              eax, 104;                  jne   .Lx60_240
                        add              rsp, 64;                   jmp   n37_lit_integer_β
.Lx60_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n42_var_α
n41_call_β:             add              rsp, 64;                   jmp   n37_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n42_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # cap
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n43_lit_integer_α
n42_var_β:              add              rsp, 80;                   jmp   n37_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx63_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n44_call_α
n43_lit_integer_β:      add              rsp, 16;                   jmp   n42_var_β
.Lx63_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n44_call_α:             sub              rsp, 16
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
.Lrkfnzd65:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd65]
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
                        cmp              eax, 104;                  jne   .Lx64_240
                        add              rsp, 16;                   jmp   n43_lit_integer_β
.Lx64_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n45_keyword_snobol4_α
n44_call_β:             add              rsp, 16;                   jmp   n43_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n45_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx66_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n46_keyword_snobol4_α
.Lx66_0:                .quad            .Lx66_0_s
.Lx66_0_s:              .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n46_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx67_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n47_call_α
.Lx67_0:                .quad            .Lx67_0_s
.Lx67_0_s:              .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:             sub              rsp, 16
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
.Lrkfnzd69:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd69]
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
                        cmp              eax, 104;                  jne   .Lx68_240
                        add              rsp, 64;                   jmp   n43_lit_integer_β
.Lx68_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n48_binop_α
n47_call_β:             add              rsp, 64;                   jmp   n43_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n48_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # call
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n49_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # cap
                        mov              qword ptr [r9 + 40], rdx;  jmp   n50_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:    add              rsp, 224;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_β:
                                                                    jmp   proc_LBL__cap_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_α:
proc_LBL__icase_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:                                              jmp   n75_var_α
n74_statement_begin_β:                                              jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n76_call_α
#-----------------------------------------------------------------------------------------------------------------------
n76_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd117:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd117]
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
                        cmp              eax, 104;                  jne   .Lx116_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n74_statement_begin_β
.Lx116_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n77_statement_end_α
n76_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n74_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:    add              rsp, 32;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:                                              jmp   n79_var_α
n78_statement_begin_β:                                              jmp   n96_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n79_var_α:              mov              rax, qword ptr [r9 + 64]                       # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 1072], rax                    # result
                        mov              qword ptr [rsp + 1080], rdx; jmp   n80_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_match_begin_α:      mov              qword ptr [rsp + 952], rsp                     # old____
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rdi, qword ptr [rsp + 1072]
                        mov              rsi, qword ptr [rsp + 1080]
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
.Lx124_0:               mov              r14d, dword ptr [rsp + 0]; jmp   n81_match_pos_α
n80_match_begin_β:      mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx124_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx124_1
                                                                    jmp   .Lx124_0
.Lx124_1:
n80_match_begin_af:     sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n96_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n81_match_pos_α:        mov              rax, 0
                        cmp              r14d, eax;                 jne   n80_match_begin_β
                                                                    jmp   n82_match_assign_save_α
n81_match_pos_β:                                                    jmp   n80_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n82_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n83_match_any_α
n82_match_assign_save_β: add              rsp, 16;                  jmp   n80_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n83_match_any_α:        mov              eax, r14d
                        cmp              eax, r15d;                 jl    .Lx129_239
                        add              rsp, 16;                   jmp   n80_match_begin_β
.Lx129_239:             movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C0]
                        cmp              byte ptr [rdi+rsi], 0;     jne   .Lx129_240
                        add              rsp, 16;                   jmp   n80_match_begin_β
.Lx129_240:             add              r14d, 1;                   jmp   n84_match_assign_cond_α
n83_match_any_β:        sub              r14d, 1
                        add              rsp, 16;                   jmp   n80_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n84_match_assign_cond_α: mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n85_match_end_α
n84_match_assign_cond_β: sub              r12, 24;                  jmp   n83_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n85_match_end_α:        mov              r8, r12
.Lx133_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx133_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 960], eax
                        mov              qword ptr [rsp + 984], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx133_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx133_5
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
.Lx133_1:               test             rax, rax;                  je    .Lx133_2
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
                        lea              rcx, [rip + .Lx133_3]
                        lea              rdx, [rip + .Lx133_4];     jmp   rax
.Lx133_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx133_1
.Lx133_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx133_1
.Lx133_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx133_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx133_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n86_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:       mov              qword ptr [rsp + 1008], 2                      # result
                        mov              dword ptr [rsp + 1012], 0
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 1016], rax; jmp   n87_match_replace_α
.Lx134_0:               .quad            .Lx134_0_s
.Lx134_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n87_match_replace_α:    mov              rdi, qword ptr [rip + .Lx136_0]
                        mov              rsi, qword ptr [rsp + 1072]
                        mov              rdx, qword ptr [rsp + 1080]
                        mov              ecx, dword ptr [rsp + 912]
                        mov              r8, qword ptr [rsp + 936]
                        lea              r9, [rsp + 1008]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx136_1
.Lx136_0:               .quad            .Lx136_0_s
.Lx136_0_s:             .string          "str"
.Lx136_1:                                                           jmp   n88_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:                                                jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:                                              jmp   n90_var_α
n89_statement_begin_β:                                              jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_var_α:              mov              rax, qword ptr [r9 + 48]                       # icase
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 1152], rax                    # result
                        mov              qword ptr [rsp + 1160], rdx; jmp   n91_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n91_assign_α:           mov              rax, qword ptr [rsp + 1152]
                        mov              rdx, qword ptr [rsp + 1160]
                        mov              qword ptr [r9 + 112], rax                      # PAT$0$V0
                        mov              qword ptr [r9 + 120], rdx; jmp   n92_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_string_α:       mov              qword ptr [rsp + 1136], 2                      # result
                        mov              dword ptr [rsp + 1140], 5
                        mov              rax, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rsp + 1144], rax; jmp   n93_call_α
.Lx143_0:               .quad            .Lx143_0_s
.Lx143_0_s:             .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n93_call_α:             mov              rax, qword ptr [rsp + 1136]
                        mov              qword ptr [rsp + 1104], rax
                        mov              rax, qword ptr [rsp + 1144]
                        mov              qword ptr [rsp + 1112], rax
                        .section         .rodata
.Lrkfn145:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn145]
                        lea              rsi, [rsp + 1104]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1088], rax
                        mov              qword ptr [rsp + 1096], rdx
                        cmp              eax, 104;                  je    n89_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n94_assign_α
n93_call_β:                                                         jmp   n89_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_α:           mov              rax, qword ptr [rsp + 1088]
                        mov              rdx, qword ptr [rsp + 1096]
                        mov              qword ptr [r9 + 48], rax                       # icase
                        mov              qword ptr [r9 + 56], rdx;  jmp   n95_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_end_α:                                                jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_begin_α:                                              jmp   n97_var_α
n96_statement_begin_β:                                              jmp   n107_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n97_var_α:              mov              rax, qword ptr [r9 + 64]                       # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 1344], rax                    # result
                        mov              qword ptr [rsp + 1352], rdx; jmp   n98_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n98_match_begin_α:      mov              qword ptr [rsp + 1224], rsp                    # old____
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rdi, qword ptr [rsp + 1344]
                        mov              rsi, qword ptr [rsp + 1352]
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
.Lx153_0:               mov              r14d, dword ptr [rsp + 0]; jmp   n99_match_pos_α
n98_match_begin_β:      mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx153_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx153_1
                                                                    jmp   .Lx153_0
.Lx153_1:
n98_match_begin_af:     sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n107_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n99_match_pos_α:        mov              rax, 0
                        cmp              r14d, eax;                 jne   n98_match_begin_β
                                                                    jmp   n100_match_assign_save_α
n99_match_pos_β:                                                    jmp   n98_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n100_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n101_match_len_α
n100_match_assign_save_β: add              rsp, 16;                 jmp   n98_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n101_match_len_α:       mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                 jle   .Lx157_240
                        add              rsp, 16;                   jmp   n98_match_begin_β
.Lx157_240:             add              r14d, 1;                   jmp   n102_match_assign_cond_α
n101_match_len_β:       sub              r14d, 1
                        add              rsp, 16;                   jmp   n98_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n102_match_assign_cond_α: mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n103_match_end_α
n102_match_assign_cond_β: sub              r12, 24;                 jmp   n101_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n103_match_end_α:       mov              r8, r12
.Lx161_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx161_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 1232], eax
                        mov              qword ptr [rsp + 1256], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx161_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx161_5
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
.Lx161_1:               test             rax, rax;                  je    .Lx161_2
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
                        lea              rcx, [rip + .Lx161_3]
                        lea              rdx, [rip + .Lx161_4];     jmp   rax
.Lx161_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx161_1
.Lx161_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx161_1
.Lx161_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx161_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx161_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n104_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_string_α:      mov              qword ptr [rsp + 1280], 2                      # result
                        mov              dword ptr [rsp + 1284], 0
                        mov              rax, qword ptr [rip + .Lx162_0]
                        mov              qword ptr [rsp + 1288], rax; jmp   n105_match_replace_α
.Lx162_0:               .quad            .Lx162_0_s
.Lx162_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n105_match_replace_α:   mov              rdi, qword ptr [rip + .Lx164_0]
                        mov              rsi, qword ptr [rsp + 1344]
                        mov              rdx, qword ptr [rsp + 1352]
                        mov              ecx, dword ptr [rsp + 1184]
                        mov              r8, qword ptr [rsp + 1208]
                        lea              r9, [rsp + 1280]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx164_1
.Lx164_0:               .quad            .Lx164_0_s
.Lx164_0_s:             .string          "str"
.Lx164_1:                                                           jmp   n106_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:                                               jmp   n107_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_begin_α:                                             jmp   n108_var_α
n107_statement_begin_β:                                             jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n108_var_α:             mov              rax, qword ptr [r9 + 48]                       # icase
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 1376], rax                    # result
                        mov              qword ptr [rsp + 1384], rdx; jmp   n109_var_α
#-----------------------------------------------------------------------------------------------------------------------
n109_var_α:             mov              rax, qword ptr [r9 + 96]                       # ch
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 1392], rax                    # result
                        mov              qword ptr [rsp + 1400], rdx; jmp   n110_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n110_binop_α:           mov              rdi, qword ptr [rsp + 1376]
                        mov              rsi, qword ptr [rsp + 1384]
                        mov              rdx, qword ptr [rsp + 1392]
                        mov              rcx, qword ptr [rsp + 1400]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n111_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n111_assign_α:          mov              rax, qword ptr [rsp + 1360]
                        mov              rdx, qword ptr [rsp + 1368]
                        mov              qword ptr [r9 + 48], rax                       # icase
                        mov              qword ptr [r9 + 56], rdx;  jmp   n112_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_end_α:                                               jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_β:
                                                                    jmp   proc_LBL__icase_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_lwr_α
proc_lwr_α:
proc_lwr_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n175_save_restore_α:                                                jmp   n176_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n176_goto_deferred_α:   lea              rax, [rip + proc_LBL__lwr_α]; jmp   rax
                                                                    jmp   n177_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n177_save_restore_α:
lwr_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx183_2
                        mov              eax, 16
.Lx183_2:               sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx183_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 0]                        # lwr
                        mov              qword ptr [r9 + 0], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx183_41
.Lx183_10:              mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
.Lx183_41:              lea              r10, [rip + lwr_gamma]
                        lea              r11, [rip + lwr_omega]
                        lea              rax, [rip + proc_LBL__lwr_α]; jmp   rax
lwr_gamma:              mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx183_75
                        mov              eax, 16
.Lx183_75:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx183_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 8], rax;   jmp   .Lx183_110
.Lx183_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx183_110:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx183_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx183_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
lwr_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx183_145
                        mov              eax, 16
.Lx183_145:             lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx183_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 8], rax;   jmp   .Lx183_180
.Lx183_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx183_180:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx183_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx183_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_lwr_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_lwr_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_lwr_β:
                                                                    jmp   proc_lwr_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_lwr_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_lwr_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_upr_α
proc_upr_α:
proc_upr_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n184_save_restore_α:                                                jmp   n185_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n185_goto_deferred_α:   lea              rax, [rip + proc_LBL__upr_α]; jmp   rax
                                                                    jmp   n186_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n186_save_restore_α:
upr_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx192_2
                        mov              eax, 16
.Lx192_2:               sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx192_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # upr
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx192_41
.Lx192_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx192_41:              lea              r10, [rip + upr_gamma]
                        lea              r11, [rip + upr_omega]
                        lea              rax, [rip + proc_LBL__upr_α]; jmp   rax
upr_gamma:              mov              rdi, qword ptr [r9 + 16]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx192_75
                        mov              eax, 16
.Lx192_75:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx192_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx192_110
.Lx192_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx192_110:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx192_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx192_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
upr_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx192_145
                        mov              eax, 16
.Lx192_145:             lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx192_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx192_180
.Lx192_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx192_180:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx192_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx192_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_upr_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_upr_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_upr_β:
                                                                    jmp   proc_upr_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_upr_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_upr_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_cap_α
proc_cap_α:
proc_cap_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n193_save_restore_α:                                                jmp   n194_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n194_goto_deferred_α:   lea              rax, [rip + proc_LBL__cap_α]; jmp   rax
                                                                    jmp   n195_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n195_save_restore_α:
cap_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx201_2
                        mov              eax, 16
.Lx201_2:               sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx201_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 32]                       # cap
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx201_41
.Lx201_10:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx201_41:              lea              r10, [rip + cap_gamma]
                        lea              r11, [rip + cap_omega]
                        lea              rax, [rip + proc_LBL__cap_α]; jmp   rax
cap_gamma:              mov              rdi, qword ptr [r9 + 32]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx201_75
                        mov              eax, 16
.Lx201_75:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx201_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx201_110
.Lx201_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx201_110:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx201_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx201_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
cap_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx201_145
                        mov              eax, 16
.Lx201_145:             lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx201_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx201_180
.Lx201_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx201_180:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx201_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx201_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_cap_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_cap_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_cap_β:
                                                                    jmp   proc_cap_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_cap_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_cap_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_icase_α
proc_icase_α:
proc_icase_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n202_save_restore_α:                                                jmp   n203_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n203_goto_deferred_α:   lea              rax, [rip + proc_LBL__icase_α]; jmp   rax
                                                                    jmp   n204_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n204_save_restore_α:
icase_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx210_2
                        mov              eax, 16
.Lx210_2:               sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 80]                       # letter
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              rax, qword ptr [r9 + 96]                       # ch
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 48]                       # icase
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 48], r10
                        mov              qword ptr [rsp + 56], r11
                        mov              qword ptr [rsp + 64], rcx
                        cmp              rcx, 0;                    jbe   .Lx210_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 64]                       # str
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx210_41
.Lx210_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx210_41:              lea              r10, [rip + icase_gamma]
                        lea              r11, [rip + icase_omega]
                        lea              rax, [rip + proc_LBL__icase_α]; jmp   rax
icase_gamma:            mov              rdi, qword ptr [r9 + 48]                       # icase
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 64]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx210_75
                        mov              eax, 16
.Lx210_75:              lea              r8, [rsp + 80]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ch
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax                       # letter
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        cmp              rcx, 0;                    jbe   .Lx210_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax                       # str
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax;  jmp   .Lx210_110
.Lx210_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx210_110:             mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              eax, 128
                        cmp              rcx, 1;                    jbe   .Lx210_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 112
.Lx210_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
icase_omega:            mov              rcx, qword ptr [rsp + 64]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx210_145
                        mov              eax, 16
.Lx210_145:             lea              r8, [rsp + 80]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax                       # icase
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax                       # ch
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax                       # letter
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        cmp              rcx, 0;                    jbe   .Lx210_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax                       # str
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax;  jmp   .Lx210_180
.Lx210_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx210_180:             mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              eax, 128
                        cmp              rcx, 1;                    jbe   .Lx210_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 112
.Lx210_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_icase_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_icase_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_icase_β:
                                                                    jmp   proc_icase_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_icase_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_icase_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
proc_PAT$0_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n211_match_defer_α:     sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]                      # PAT$0$V0
                        mov              rdx, qword ptr [r9 + 120]
                        cmp              eax, 8;                    jne   .Lx219_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                  jne   .Lx219_10
                        mov              rdi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             dtp_fn_of@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx219_10
.Lx219_9:               xor              eax, eax
.Lx219_10:              test             rax, rax;                  jz    .Lx219_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx219_4]
                        lea              r11, [rip + .Lx219_5];     jmp   rax
.Lx219_4:                                                           jmp   n212_match_alternate_α
.Lx219_5:               add              rsp, 16;                   jmp   proc_PAT$0_ω
.Lx219_0:               push             r14
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
.Lx219_2:               test             rax, rax;                  je    .Lx219_3
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
                        lea              rcx, [rip + .Lx219_7]
                        lea              rdx, [rip + .Lx219_8];     jmp   rax
.Lx219_7:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx219_2
.Lx219_8:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx219_2
.Lx219_3:               add              rsp, 8
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
                        test             eax, eax;                  jns   .Lx219_240
                        add              rsp, 16;                   jmp   proc_PAT$0_ω
.Lx219_240:             mov              r14d, eax
                        lea              rax, [rip + .Lx219_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n212_match_alternate_α
.Lx219_6:               add              rsp, 16
                        add              rsp, 16;                   jmp   proc_PAT$0_ω
n211_match_defer_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n212_match_alternate_α: sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx221_21]
                        mov              qword ptr [rsp + 16], rax; jmp   n216_var_α
.Lx221_21:              lea              rax, [rip + .Lx221_19]
                        mov              qword ptr [rsp + 16], rax; jmp   n213_var_α
n212_match_alternate_s0: lea              rax, [rip + .Lx221_40]
                        mov              qword ptr [rsp + 8], rax;  jmp   n212_match_alternate_as
n212_match_alternate_s1: lea              rax, [rip + .Lx221_41]
                        mov              qword ptr [rsp + 8], rax;  jmp   n212_match_alternate_as
.Lx221_40:                                                          jmp   n218_match_value_β
.Lx221_41:                                                          jmp   n215_match_value_β
n212_match_alternate_as:                                            jmp   proc_PAT$0_γ
n212_match_alternate_β: mov              rax, qword ptr [rsp + 8];  jmp   rax
n212_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16]; jmp   rax
.Lx221_19:              add              rsp, 32;                   jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
n213_var_α:             mov              rax, qword ptr [r9 + 80]                       # letter
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 176], rax                     # result
                        mov              qword ptr [rsp + 184], rdx; jmp   n214_call_α
n213_var_β:                                                         jmp   n212_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n214_call_α:            sub              rsp, 48
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 232]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx224_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + lwr_alpha];    jmp   rax
                        lea              r8, [rip + g_gc_pending]
                        mov              eax, dword ptr [r8 + 0]
                        test             eax, eax;                  jne   .Lx224_20
                        mov              rax, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx;   jmp   .Lx224_21
.Lx224_20:              mov              edi, 0
                        mov              rsi, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
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
.Lx224_21:              mov              rdi, qword ptr [rip + .Lx224_0]
                        mov              esi, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                  je    .Lx224_1
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
                        lea              rcx, [rip + .Lx224_3]
                        lea              rdx, [rip + .Lx224_4];     jmp   rax
.Lx224_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx224_2
.Lx224_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx224_2
.Lx224_1:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_faildescr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx224_2:               mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                        cmp              eax, 104;                  je    n212_match_alternate_af
                                                                    jmp   n215_match_value_α
n214_call_β:                                                        jmp   n212_match_alternate_af
.Lx224_0:               .quad            .Lx224_0_s
.Lx224_0_s:             .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n215_match_value_α:     lea              rdi, [rsp + 128]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                  jz    .Lx225_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx225_4]
                        lea              rdx, [rip + .Lx225_5];     jmp   rax
.Lx225_4:                                                           jmp   n212_match_alternate_s1
.Lx225_5:                                                           jmp   n212_match_alternate_af
.Lx225_0:               lea              rdi, [rsp + 128]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
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
                        test             eax, eax;                  js    n212_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx225_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n212_match_alternate_s1
.Lx225_6:               add              rsp, 16;                   jmp   n212_match_alternate_af
n215_match_value_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n216_var_α:             mov              rax, qword ptr [r9 + 80]                       # letter
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 112], rax                     # result
                        mov              qword ptr [rsp + 120], rdx; jmp   n217_call_α
n216_var_β:                                                         jmp   n212_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n217_call_α:            sub              rsp, 48
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx228_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + upr_alpha];    jmp   rax
                        lea              r8, [rip + g_gc_pending]
                        mov              eax, dword ptr [r8 + 0]
                        test             eax, eax;                  jne   .Lx228_20
                        mov              rax, qword ptr [rsp + 112]
                        mov              rdx, qword ptr [rsp + 120]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx;   jmp   .Lx228_21
.Lx228_20:              mov              edi, 0
                        mov              rsi, qword ptr [rsp + 112]
                        mov              rdx, qword ptr [rsp + 120]
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
.Lx228_21:              mov              rdi, qword ptr [rip + .Lx228_0]
                        mov              esi, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                  je    .Lx228_1
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
                        lea              rcx, [rip + .Lx228_3]
                        lea              rdx, [rip + .Lx228_4];     jmp   rax
.Lx228_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx228_2
.Lx228_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx228_2
.Lx228_1:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_faildescr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx228_2:               mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                        cmp              eax, 104;                  je    n212_match_alternate_af
                                                                    jmp   n218_match_value_α
n217_call_β:                                                        jmp   n212_match_alternate_af
.Lx228_0:               .quad            .Lx228_0_s
.Lx228_0_s:             .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n218_match_value_α:     lea              rdi, [rsp + 64]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                  jz    .Lx229_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx229_4]
                        lea              rdx, [rip + .Lx229_5];     jmp   rax
.Lx229_4:                                                           jmp   n212_match_alternate_s0
.Lx229_5:                                                           jmp   n212_match_alternate_af
.Lx229_0:               lea              rdi, [rsp + 64]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
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
                        test             eax, eax;                  js    n212_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx229_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n212_match_alternate_s0
.Lx229_6:               add              rsp, 16;                   jmp   n212_match_alternate_af
n218_match_value_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                    jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        sub              rsp, 8
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax;                       jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__lwr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__lwr_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2480
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__upr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__upr_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2480
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__cap"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__cap_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2480
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__icase"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__icase_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2480
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "PAT$0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + proc_PAT$0_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 208
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "lwr"
.Lgvan1:                .string          "upr"
.Lgvan2:                .string          "cap"
.Lgvan3:                .string          "icase"
.Lgvan4:                .string          "str"
.Lgvan5:                .string          "letter"
.Lgvan6:                .string          "ch"
.Lgvan7:                .string          "PAT$0$V0"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 8
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 8
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
n230_statement_begin_α:                                             jmp   n231_statement_end_α
n230_statement_begin_β:                                             jmp   n232_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_end_α:                                               jmp   n232_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'no match ok'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n232_statement_begin_α:                                             jmp   n233_func_activate_α
n232_statement_begin_β:                                             jmp   n235_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n233_func_activate_α:   mov              rdi, qword ptr [rip + .Lx393_0]
                        mov              rsi, qword ptr [rip + .Lx393_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_lwr_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n234_statement_end_α
n233_func_activate_β:                                               jmp   n232_statement_begin_β
.Lx393_0:               .quad            .Lx393_0_s
.Lx393_0_s:             .string          "lwr"
.Lx393_1:               .quad            .Lx393_1_s
.Lx393_1_s:             .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n234_statement_end_α:                                               jmp   n235_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_begin_α:                                             jmp   n236_statement_end_α
n235_statement_begin_β:                                             jmp   n237_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_end_α:                                               jmp   n237_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_begin_α:                                             jmp   n238_func_activate_α
n237_statement_begin_β:                                             jmp   n240_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n238_func_activate_α:   mov              rdi, qword ptr [rip + .Lx403_0]
                        mov              rsi, qword ptr [rip + .Lx403_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_upr_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n239_statement_end_α
n238_func_activate_β:                                               jmp   n237_statement_begin_β
.Lx403_0:               .quad            .Lx403_0_s
.Lx403_0_s:             .string          "upr"
.Lx403_1:               .quad            .Lx403_1_s
.Lx403_1_s:             .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_end_α:                                               jmp   n240_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_begin_α:                                             jmp   n241_statement_end_α
n240_statement_begin_β:                                             jmp   n242_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n241_statement_end_α:                                               jmp   n242_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n242_statement_begin_α:                                             jmp   n243_func_activate_α
n242_statement_begin_β:                                             jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n243_func_activate_α:   mov              rdi, qword ptr [rip + .Lx413_0]
                        mov              rsi, qword ptr [rip + .Lx413_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_cap_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n244_statement_end_α
n243_func_activate_β:                                               jmp   n242_statement_begin_β
.Lx413_0:               .quad            .Lx413_0_s
.Lx413_0_s:             .string          "cap"
.Lx413_1:               .quad            .Lx413_1_s
.Lx413_1_s:             .string          "cap"
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_end_α:                                               jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n245_statement_begin_α:                                             jmp   n246_statement_end_α
n245_statement_begin_β:                                             jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n246_statement_end_α:                                               jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n247_statement_begin_α:                                             jmp   n248_func_activate_α
n247_statement_begin_β:                                             jmp   n250_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n248_func_activate_α:   mov              rdi, qword ptr [rip + .Lx423_0]
                        mov              rsi, qword ptr [rip + .Lx423_1]
                        mov              edx, 3
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_icase_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n249_statement_end_α
n248_func_activate_β:                                               jmp   n247_statement_begin_β
.Lx423_0:               .quad            .Lx423_0_s
.Lx423_0_s:             .string          "icase"
.Lx423_1:               .quad            .Lx423_1_s
.Lx423_1_s:             .string          "str,letter,ch"
#-----------------------------------------------------------------------------------------------------------------------
n249_statement_end_α:                                               jmp   n250_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n250_statement_begin_α:                                             jmp   n251_statement_end_α
n250_statement_begin_β:                                             jmp   n252_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n251_statement_end_α:                                               jmp   n252_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n252_statement_begin_α:                                             jmp   n253_statement_end_α
n252_statement_begin_β:                                             jmp   n254_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n253_statement_end_α:                                               jmp   n254_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_begin_α:                                             jmp   n255_lit_string_α
n254_statement_begin_β:                                             jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n255_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx436_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n256_lit_integer_α
.Lx436_0:               .quad            .Lx436_0_s
.Lx436_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n256_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx437_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n257_call_α
n256_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n254_statement_begin_β
.Lx437_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n257_call_α:            sub              rsp, 16
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
.Lrkfnzd439:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd439]
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
                        cmp              eax, 104;                  jne   .Lx438_240
                        add              rsp, 16;                   jmp   n256_lit_integer_β
.Lx438_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n258_statement_end_α
n257_call_β:            add              rsp, 16;                   jmp   n256_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_end_α:                                               jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_begin_α:                                             jmp   n260_statement_end_α
n259_statement_begin_β:                                             jmp   n261_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n260_statement_end_α:                                               jmp   n261_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n261_statement_begin_α:                                             jmp   n262_lit_string_α
n261_statement_begin_β:                                             jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n262_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx448_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n263_call_α
n262_lit_string_β:      add              rsp, 16
                        add              rsp, 48;                   jmp   n261_statement_begin_β
.Lx448_0:               .quad            .Lx448_0_s
.Lx448_0_s:             .string          "HELLO WORLD"
#-----------------------------------------------------------------------------------------------------------------------
n263_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx450_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + lwr_alpha];    jmp   rax
.Lx450_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx450_240
                        add              rsp, 16;                   jmp   n262_lit_string_β
.Lx450_240:                                                         jmp   n264_assign_α
n263_call_β:                                                        jmp   n262_lit_string_β
.Lx450_0:               .quad            .Lx450_0_s
.Lx450_0_s:             .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n264_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx451_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n265_statement_end_α
.Lx451_0:               .quad            .Lx451_0_s
.Lx451_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:                                               jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_begin_α:                                             jmp   n267_lit_string_α
n266_statement_begin_β:                                             jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx456_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n268_call_α
n267_lit_string_β:      add              rsp, 16
                        add              rsp, 80;                   jmp   n266_statement_begin_β
.Lx456_0:               .quad            .Lx456_0_s
.Lx456_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n268_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx458_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + upr_alpha];    jmp   rax
.Lx458_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx458_240
                        add              rsp, 16;                   jmp   n267_lit_string_β
.Lx458_240:                                                         jmp   n269_assign_α
n268_call_β:                                                        jmp   n267_lit_string_β
.Lx458_0:               .quad            .Lx458_0_s
.Lx458_0_s:             .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n269_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx459_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n270_statement_end_α
.Lx459_0:               .quad            .Lx459_0_s
.Lx459_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:                                               jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_begin_α:                                             jmp   n272_lit_string_α
n271_statement_begin_β:                                             jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n272_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx464_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n273_call_α
n272_lit_string_β:      add              rsp, 16
                        add              rsp, 112;                  jmp   n271_statement_begin_β
.Lx464_0:               .quad            .Lx464_0_s
.Lx464_0_s:             .string          "hELLO wORLD"
#-----------------------------------------------------------------------------------------------------------------------
n273_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx466_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + cap_alpha];    jmp   rax
.Lx466_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx466_240
                        add              rsp, 16;                   jmp   n272_lit_string_β
.Lx466_240:                                                         jmp   n274_assign_α
n273_call_β:                                                        jmp   n272_lit_string_β
.Lx466_0:               .quad            .Lx466_0_s
.Lx466_0_s:             .string          "cap"
#-----------------------------------------------------------------------------------------------------------------------
n274_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx467_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n275_statement_end_α
.Lx467_0:               .quad            .Lx467_0_s
.Lx467_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_end_α:                                               jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_begin_α:                                             jmp   n277_statement_end_α
n276_statement_begin_β:                                             jmp   n278_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n277_statement_end_α:                                               jmp   n278_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n278_statement_begin_α:                                             jmp   n279_lit_string_α
n278_statement_begin_β:                                             jmp   n290_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n279_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx476_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n280_match_begin_α
.Lx476_0:               .quad            .Lx476_0_s
.Lx476_0_s:             .string          "Hello"
#-----------------------------------------------------------------------------------------------------------------------
n280_match_begin_α:     mov              rdi, qword ptr [rsp + 0]                       # lit_string
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 1760], r13                    # outer_Σ
                        mov              qword ptr [rsp + 1768], r14                    # outer_δ
                        mov              qword ptr [rsp + 1776], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 1784], rax                    # cap_gen
                        mov              qword ptr [rsp + 1752], rsp                    # old____
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
                        mov              qword ptr [rsp + 1728], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 1712], 0                      # start_δ
.Lx478_0:               mov              r14d, dword ptr [rsp + 1712]; jmp   n281_lit_string_α
n280_match_begin_β:     add              dword ptr [rsp + 1712], 1
                        mov              eax, dword ptr [rsp + 1712]
                        cmp              eax, r15d;                 jg    .Lx478_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx478_1
                                                                    jmp   .Lx478_0
.Lx478_1:
n280_match_begin_af:    sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 1728]
                        mov              r13, qword ptr [rsp + 1760]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 1768]                    # outer_δ
                        mov              r15, qword ptr [rsp + 1776]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 1784]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n290_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n281_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx479_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n282_call_α
.Lx479_0:               .quad            .Lx479_0_s
.Lx479_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n282_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx481_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + icase_alpha];  jmp   rax
.Lx481_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx481_240
                        add              rsp, 16;                   jmp   n280_match_begin_α
.Lx481_240:                                                         jmp   n283_match_value_α
n282_call_β:                                                        jmp   n280_match_begin_α
.Lx481_0:               .quad            .Lx481_0_s
.Lx481_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n283_match_value_α:     lea              rdi, [rsp + 0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                  jz    .Lx482_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx482_4]
                        lea              rdx, [rip + .Lx482_5];     jmp   rax
.Lx482_4:                                                           jmp   n284_match_end_α
.Lx482_5:                                                           jmp   n280_match_begin_β
.Lx482_0:               lea              rdi, [rsp + 0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
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
                        test             eax, eax;                  js    n280_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx482_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n284_match_end_α
.Lx482_6:               add              rsp, 16;                   jmp   n280_match_begin_β
n283_match_value_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n284_match_end_α:       mov              r8, r12
.Lx484_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx484_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx484_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx484_5
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
.Lx484_1:               test             rax, rax;                  je    .Lx484_2
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
                        lea              rcx, [rip + .Lx484_3]
                        lea              rdx, [rip + .Lx484_4];     jmp   rax
.Lx484_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx484_1
.Lx484_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx484_1
.Lx484_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx484_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx484_10
                        mov              r13, qword ptr [rsp + 1760]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 1768]                    # outer_δ
                        mov              r15, qword ptr [rsp + 1776]                    # outer_Δ
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n285_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n285_statement_end_α:                                               jmp   n286_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n286_statement_begin_α:                                             jmp   n287_lit_string_α
n286_statement_begin_β:                                             jmp   n290_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n287_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx489_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n288_assign_α
.Lx489_0:               .quad            .Lx489_0_s
.Lx489_0_s:             .string          "ok: icase hello"
#-----------------------------------------------------------------------------------------------------------------------
n288_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx490_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n289_statement_end_α
.Lx490_0:               .quad            .Lx490_0_s
.Lx490_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n289_statement_end_α:                                               jmp   n290_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_begin_α:                                             jmp   n291_statement_end_α
n290_statement_begin_β:                                             jmp   n292_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_end_α:                                               jmp   n292_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n292_statement_begin_α:                                             jmp   n293_lit_string_α
n292_statement_begin_β:                                             jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n293_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx499_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n294_match_begin_α
.Lx499_0:               .quad            .Lx499_0_s
.Lx499_0_s:             .string          "HELLO"
#-----------------------------------------------------------------------------------------------------------------------
n294_match_begin_α:     mov              rdi, qword ptr [rsp + 0]                       # lit_string
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 1952], r13                    # outer_Σ
                        mov              qword ptr [rsp + 1960], r14                    # outer_δ
                        mov              qword ptr [rsp + 1968], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 1976], rax                    # cap_gen
                        mov              qword ptr [rsp + 1944], rsp                    # old____
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
                        mov              qword ptr [rsp + 1920], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 1904], 0                      # start_δ
.Lx501_0:               mov              r14d, dword ptr [rsp + 1904]; jmp   n295_lit_string_α
n294_match_begin_β:     add              dword ptr [rsp + 1904], 1
                        mov              eax, dword ptr [rsp + 1904]
                        cmp              eax, r15d;                 jg    .Lx501_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx501_1
                                                                    jmp   .Lx501_0
.Lx501_1:
n294_match_begin_af:    sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 1920]
                        mov              r13, qword ptr [rsp + 1952]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 1960]                    # outer_δ
                        mov              r15, qword ptr [rsp + 1968]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 1976]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n295_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx502_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n296_call_α
.Lx502_0:               .quad            .Lx502_0_s
.Lx502_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n296_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx504_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + icase_alpha];  jmp   rax
.Lx504_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx504_240
                        add              rsp, 16;                   jmp   n294_match_begin_α
.Lx504_240:                                                         jmp   n297_match_value_α
n296_call_β:                                                        jmp   n294_match_begin_α
.Lx504_0:               .quad            .Lx504_0_s
.Lx504_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n297_match_value_α:     lea              rdi, [rsp + 0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                  jz    .Lx505_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx505_4]
                        lea              rdx, [rip + .Lx505_5];     jmp   rax
.Lx505_4:                                                           jmp   n298_match_end_α
.Lx505_5:                                                           jmp   n294_match_begin_β
.Lx505_0:               lea              rdi, [rsp + 0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
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
                        test             eax, eax;                  js    n294_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx505_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n298_match_end_α
.Lx505_6:               add              rsp, 16;                   jmp   n294_match_begin_β
n297_match_value_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n298_match_end_α:       mov              r8, r12
.Lx507_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx507_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx507_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx507_5
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
.Lx507_1:               test             rax, rax;                  je    .Lx507_2
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
                        lea              rcx, [rip + .Lx507_3]
                        lea              rdx, [rip + .Lx507_4];     jmp   rax
.Lx507_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx507_1
.Lx507_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx507_1
.Lx507_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx507_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx507_10
                        mov              r13, qword ptr [rsp + 1952]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 1960]                    # outer_δ
                        mov              r15, qword ptr [rsp + 1968]                    # outer_Δ
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n299_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n299_statement_end_α:                                               jmp   n300_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n300_statement_begin_α:                                             jmp   n301_lit_string_α
n300_statement_begin_β:                                             jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n301_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx512_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n302_assign_α
.Lx512_0:               .quad            .Lx512_0_s
.Lx512_0_s:             .string          "ok: icase HELLO"
#-----------------------------------------------------------------------------------------------------------------------
n302_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n303_statement_end_α
.Lx513_0:               .quad            .Lx513_0_s
.Lx513_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_end_α:                                               jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_begin_α:                                             jmp   n305_statement_end_α
n304_statement_begin_β:                                             jmp   n306_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n305_statement_end_α:                                               jmp   n306_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n306_statement_begin_α:                                             jmp   n307_lit_string_α
n306_statement_begin_β:                                             jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n307_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx522_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n308_match_begin_α
.Lx522_0:               .quad            .Lx522_0_s
.Lx522_0_s:             .string          "HeLLo"
#-----------------------------------------------------------------------------------------------------------------------
n308_match_begin_α:     mov              rdi, qword ptr [rsp + 0]                       # lit_string
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 2144], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2152], r14                    # outer_δ
                        mov              qword ptr [rsp + 2160], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2168], rax                    # cap_gen
                        mov              qword ptr [rsp + 2136], rsp                    # old____
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
                        mov              qword ptr [rsp + 2112], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2096], 0                      # start_δ
.Lx524_0:               mov              r14d, dword ptr [rsp + 2096]; jmp   n309_lit_string_α
n308_match_begin_β:     add              dword ptr [rsp + 2096], 1
                        mov              eax, dword ptr [rsp + 2096]
                        cmp              eax, r15d;                 jg    .Lx524_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx524_1
                                                                    jmp   .Lx524_0
.Lx524_1:
n308_match_begin_af:    sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2112]
                        mov              r13, qword ptr [rsp + 2144]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2152]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2160]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2168]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n309_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx525_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n310_call_α
.Lx525_0:               .quad            .Lx525_0_s
.Lx525_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n310_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx527_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + icase_alpha];  jmp   rax
.Lx527_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx527_240
                        add              rsp, 16;                   jmp   n308_match_begin_α
.Lx527_240:                                                         jmp   n311_match_value_α
n310_call_β:                                                        jmp   n308_match_begin_α
.Lx527_0:               .quad            .Lx527_0_s
.Lx527_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n311_match_value_α:     lea              rdi, [rsp + 0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                  jz    .Lx528_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx528_4]
                        lea              rdx, [rip + .Lx528_5];     jmp   rax
.Lx528_4:                                                           jmp   n312_match_end_α
.Lx528_5:                                                           jmp   n308_match_begin_β
.Lx528_0:               lea              rdi, [rsp + 0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
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
                        test             eax, eax;                  js    n308_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx528_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n312_match_end_α
.Lx528_6:               add              rsp, 16;                   jmp   n308_match_begin_β
n311_match_value_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n312_match_end_α:       mov              r8, r12
.Lx530_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx530_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx530_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx530_5
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
.Lx530_1:               test             rax, rax;                  je    .Lx530_2
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
                        lea              rcx, [rip + .Lx530_3]
                        lea              rdx, [rip + .Lx530_4];     jmp   rax
.Lx530_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx530_1
.Lx530_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx530_1
.Lx530_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx530_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx530_10
                        mov              r13, qword ptr [rsp + 2144]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2152]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2160]                    # outer_Δ
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n313_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n313_statement_end_α:                                               jmp   n314_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n314_statement_begin_α:                                             jmp   n315_lit_string_α
n314_statement_begin_β:                                             jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n315_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx535_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n316_assign_α
.Lx535_0:               .quad            .Lx535_0_s
.Lx535_0_s:             .string          "ok: icase HeLLo"
#-----------------------------------------------------------------------------------------------------------------------
n316_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx536_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n317_statement_end_α
.Lx536_0:               .quad            .Lx536_0_s
.Lx536_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_end_α:                                               jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n318_statement_begin_α:                                             jmp   n319_statement_end_α
n318_statement_begin_β:                                             jmp   n320_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n319_statement_end_α:                                               jmp   n320_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_begin_α:                                             jmp   n321_lit_string_α
n320_statement_begin_β: add              rsp, 336;                  jmp   n332_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n321_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx545_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n322_match_begin_α
n321_lit_string_β:      add              rsp, 16
                        add              rsp, 336;                  jmp   n332_statement_begin_α
.Lx545_0:               .quad            .Lx545_0_s
.Lx545_0_s:             .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n322_match_begin_α:     mov              rdi, qword ptr [rsp + 0]                       # lit_string
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 2336], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2344], r14                    # outer_δ
                        mov              qword ptr [rsp + 2352], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2360], rax                    # cap_gen
                        mov              qword ptr [rsp + 2328], rsp                    # old____
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
                        mov              qword ptr [rsp + 2304], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2288], 0                      # start_δ
.Lx547_0:               mov              r14d, dword ptr [rsp + 2288]; jmp   n323_lit_string_α
n322_match_begin_β:     add              dword ptr [rsp + 2288], 1
                        mov              eax, dword ptr [rsp + 2288]
                        cmp              eax, r15d;                 jg    .Lx547_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx547_1
                                                                    jmp   .Lx547_0
.Lx547_1:
n322_match_begin_af:    sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2304]
                        mov              r13, qword ptr [rsp + 2336]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2344]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2352]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2360]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n321_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n323_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx548_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n324_call_α
.Lx548_0:               .quad            .Lx548_0_s
.Lx548_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n324_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx550_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + icase_alpha];  jmp   rax
.Lx550_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx550_240
                        add              rsp, 16;                   jmp   n322_match_begin_α
.Lx550_240:                                                         jmp   n325_match_value_α
n324_call_β:                                                        jmp   n322_match_begin_α
.Lx550_0:               .quad            .Lx550_0_s
.Lx550_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n325_match_value_α:     lea              rdi, [rsp + 0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                  jz    .Lx551_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx551_4]
                        lea              rdx, [rip + .Lx551_5];     jmp   rax
.Lx551_4:                                                           jmp   n326_match_end_α
.Lx551_5:                                                           jmp   n322_match_begin_β
.Lx551_0:               lea              rdi, [rsp + 0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_value_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
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
                        test             eax, eax;                  js    n322_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx551_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n326_match_end_α
.Lx551_6:               add              rsp, 16;                   jmp   n322_match_begin_β
n325_match_value_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n326_match_end_α:       mov              r8, r12
.Lx553_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx553_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx553_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx553_5
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
.Lx553_1:               test             rax, rax;                  je    .Lx553_2
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
                        lea              rcx, [rip + .Lx553_3]
                        lea              rdx, [rip + .Lx553_4];     jmp   rax
.Lx553_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx553_1
.Lx553_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx553_1
.Lx553_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx553_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx553_10
                        mov              r13, qword ptr [rsp + 2336]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2344]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2352]                    # outer_Δ
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n327_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n327_statement_end_α:                                               jmp   n328_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n328_statement_begin_α:                                             jmp   n329_lit_string_α
n328_statement_begin_β: add              rsp, 384;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n329_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx558_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n330_assign_α
.Lx558_0:               .quad            .Lx558_0_s
.Lx558_0_s:             .string          "FAIL: icase matched wrong string"
#-----------------------------------------------------------------------------------------------------------------------
n330_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx559_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n331_statement_end_α
.Lx559_0:               .quad            .Lx559_0_s
.Lx559_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n331_statement_end_α:   add              rsp, 160;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n332_statement_begin_α:                                             jmp   n333_lit_string_α
n332_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n333_lit_string_α:      mov              qword ptr [rsp + 2448], 2                      # result
                        mov              dword ptr [rsp + 2452], 11
                        mov              rax, qword ptr [rip + .Lx564_0]
                        mov              qword ptr [rsp + 2456], rax; jmp   n334_assign_α
.Lx564_0:               .quad            .Lx564_0_s
.Lx564_0_s:             .string          "no match ok"
#-----------------------------------------------------------------------------------------------------------------------
n334_assign_α:          mov              rsi, qword ptr [rsp + 2448]
                        mov              rdx, qword ptr [rsp + 2456]
                        mov              rdi, qword ptr [rip + .Lx565_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n335_statement_end_α
.Lx565_0:               .quad            .Lx565_0_s
.Lx565_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_end_α:                                               jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n336_goto_α:                                                        jmp   n235_statement_begin_α
n336_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n337_goto_α:                                                        jmp   n240_statement_begin_α
n337_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n338_goto_α:                                                        jmp   n245_statement_begin_α
n338_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n339_goto_α:                                                        jmp   n340_statement_begin_α
n339_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n340_statement_begin_α:                                             jmp   n341_var_α
n340_statement_begin_β:                                             jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n341_var_α:             mov              rax, qword ptr [r9 + 64]                       # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 1344], rax                    # result
                        mov              qword ptr [rsp + 1352], rdx; jmp   n342_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n342_match_begin_α:     mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rsp                      # old____
                        mov              rdi, qword ptr [rsp + 1344]
                        mov              rsi, qword ptr [rsp + 1352]
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
.Lx576_0:               mov              r14d, dword ptr [rsp + 0]; jmp   n343_match_pos_α
n342_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx576_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx576_1
                                                                    jmp   .Lx576_0
.Lx576_1:
n342_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n343_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                 jne   n342_match_begin_β
                                                                    jmp   n344_match_assign_save_α
n343_match_pos_β:                                                   jmp   n342_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n344_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n345_match_len_α
n344_match_assign_save_β: add              rsp, 16;                 jmp   n342_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n345_match_len_α:       mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                 jle   .Lx580_240
                        add              rsp, 16;                   jmp   n342_match_begin_β
.Lx580_240:             add              r14d, 1;                   jmp   n346_match_assign_cond_α
n345_match_len_β:       sub              r14d, 1
                        add              rsp, 16;                   jmp   n342_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n346_match_assign_cond_α: mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n347_match_end_α
n346_match_assign_cond_β: sub              r12, 24;                 jmp   n345_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n347_match_end_α:       mov              r8, r12
.Lx584_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx584_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 1232], eax
                        mov              qword ptr [rsp + 1256], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx584_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx584_5
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
.Lx584_1:               test             rax, rax;                  je    .Lx584_2
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
                        lea              rcx, [rip + .Lx584_3]
                        lea              rdx, [rip + .Lx584_4];     jmp   rax
.Lx584_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx584_1
.Lx584_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx584_1
.Lx584_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx584_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx584_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n348_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n348_lit_string_α:      mov              qword ptr [rsp + 1280], 2                      # result
                        mov              dword ptr [rsp + 1284], 0
                        mov              rax, qword ptr [rip + .Lx585_0]
                        mov              qword ptr [rsp + 1288], rax; jmp   n349_match_replace_α
.Lx585_0:               .quad            .Lx585_0_s
.Lx585_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n349_match_replace_α:   mov              rdi, qword ptr [rip + .Lx587_0]
                        mov              rsi, qword ptr [rsp + 1344]
                        mov              rdx, qword ptr [rsp + 1352]
                        mov              ecx, dword ptr [rsp + 1184]
                        mov              r8, qword ptr [rsp + 1208]
                        lea              r9, [rsp + 1280]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx587_1
.Lx587_0:               .quad            .Lx587_0_s
.Lx587_0_s:             .string          "str"
.Lx587_1:                                                           jmp   n350_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n350_statement_end_α:                                               jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n351_statement_begin_α:                                             jmp   n352_var_α
n351_statement_begin_β:                                             jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n352_var_α:             mov              rax, qword ptr [r9 + 48]                       # icase
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 1376], rax                    # result
                        mov              qword ptr [rsp + 1384], rdx; jmp   n353_var_α
#-----------------------------------------------------------------------------------------------------------------------
n353_var_α:             mov              rax, qword ptr [r9 + 96]                       # ch
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 1392], rax                    # result
                        mov              qword ptr [rsp + 1400], rdx; jmp   n354_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n354_binop_α:           mov              rdi, qword ptr [rsp + 1376]
                        mov              rsi, qword ptr [rsp + 1384]
                        mov              rdx, qword ptr [rsp + 1392]
                        mov              rcx, qword ptr [rsp + 1400]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n355_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n355_assign_α:          mov              rax, qword ptr [rsp + 1360]
                        mov              rdx, qword ptr [rsp + 1368]
                        mov              qword ptr [r9 + 48], rax                       # icase
                        mov              qword ptr [r9 + 56], rdx;  jmp   n356_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n356_statement_end_α:                                               jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n357_statement_begin_α:                                             jmp   n358_var_α
n357_statement_begin_β:                                             jmp   n361_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n358_var_α:             mov              rax, qword ptr [r9 + 64]                       # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 880], rax                     # result
                        mov              qword ptr [rsp + 888], rdx; jmp   n359_call_α
#-----------------------------------------------------------------------------------------------------------------------
n359_call_α:            mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 848], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 856], rax
                        .section         .rodata
.Lrkfn602:              .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn602]
                        lea              rsi, [rsp + 848]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                        cmp              eax, 104;                  je    n357_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n360_statement_end_α
n359_call_β:                                                        jmp   n357_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n360_statement_end_α:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n361_statement_begin_α:                                             jmp   n362_var_α
n361_statement_begin_β:                                             jmp   n340_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n362_var_α:             mov              rax, qword ptr [r9 + 64]                       # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 1072], rax                    # result
                        mov              qword ptr [rsp + 1080], rdx; jmp   n363_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n363_match_begin_α:     mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rsp                      # old____
                        mov              rdi, qword ptr [rsp + 1072]
                        mov              rsi, qword ptr [rsp + 1080]
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
.Lx609_0:               mov              r14d, dword ptr [rsp + 0]; jmp   n364_match_pos_α
n363_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx609_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx609_1
                                                                    jmp   .Lx609_0
.Lx609_1:
n363_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n340_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n364_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                 jne   n363_match_begin_β
                                                                    jmp   n365_match_assign_save_α
n364_match_pos_β:                                                   jmp   n363_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n365_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n366_match_any_α
n365_match_assign_save_β: add              rsp, 16;                 jmp   n363_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n366_match_any_α:       mov              eax, r14d
                        cmp              eax, r15d;                 jl    .Lx614_239
                        add              rsp, 16;                   jmp   n363_match_begin_β
.Lx614_239:             movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C0]
                        cmp              byte ptr [rdi+rsi], 0;     jne   .Lx614_240
                        add              rsp, 16;                   jmp   n363_match_begin_β
.Lx614_240:             add              r14d, 1;                   jmp   n367_match_assign_cond_α
n366_match_any_β:       sub              r14d, 1
                        add              rsp, 16;                   jmp   n363_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n367_match_assign_cond_α: mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n368_match_end_α
n367_match_assign_cond_β: sub              r12, 24;                 jmp   n366_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n368_match_end_α:       mov              r8, r12
.Lx618_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx618_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 960], eax
                        mov              qword ptr [rsp + 984], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx618_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx618_5
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
.Lx618_1:               test             rax, rax;                  je    .Lx618_2
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
                        lea              rcx, [rip + .Lx618_3]
                        lea              rdx, [rip + .Lx618_4];     jmp   rax
.Lx618_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx618_1
.Lx618_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx618_1
.Lx618_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx618_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx618_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n369_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n369_lit_string_α:      mov              qword ptr [rsp + 1008], 2                      # result
                        mov              dword ptr [rsp + 1012], 0
                        mov              rax, qword ptr [rip + .Lx619_0]
                        mov              qword ptr [rsp + 1016], rax; jmp   n370_match_replace_α
.Lx619_0:               .quad            .Lx619_0_s
.Lx619_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n370_match_replace_α:   mov              rdi, qword ptr [rip + .Lx621_0]
                        mov              rsi, qword ptr [rsp + 1072]
                        mov              rdx, qword ptr [rsp + 1080]
                        mov              ecx, dword ptr [rsp + 912]
                        mov              r8, qword ptr [rsp + 936]
                        lea              r9, [rsp + 1008]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx621_1
.Lx621_0:               .quad            .Lx621_0_s
.Lx621_0_s:             .string          "str"
.Lx621_1:                                                           jmp   n371_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n371_statement_end_α:                                               jmp   n372_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n372_statement_begin_α:                                             jmp   n373_var_α
n372_statement_begin_β:                                             jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n373_var_α:             mov              rax, qword ptr [r9 + 48]                       # icase
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 1152], rax                    # result
                        mov              qword ptr [rsp + 1160], rdx; jmp   n374_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n374_assign_α:          mov              rax, qword ptr [rsp + 1152]
                        mov              rdx, qword ptr [rsp + 1160]
                        mov              qword ptr [r9 + 112], rax                      # PAT$0$V0
                        mov              qword ptr [r9 + 120], rdx; jmp   n375_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n375_lit_string_α:      mov              qword ptr [rsp + 1136], 2                      # result
                        mov              dword ptr [rsp + 1140], 5
                        mov              rax, qword ptr [rip + .Lx628_0]
                        mov              qword ptr [rsp + 1144], rax; jmp   n376_call_α
.Lx628_0:               .quad            .Lx628_0_s
.Lx628_0_s:             .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n376_call_α:            mov              rax, qword ptr [rsp + 1136]
                        mov              qword ptr [rsp + 1104], rax
                        mov              rax, qword ptr [rsp + 1144]
                        mov              qword ptr [rsp + 1112], rax
                        .section         .rodata
.Lrkfn630:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn630]
                        lea              rsi, [rsp + 1104]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1088], rax
                        mov              qword ptr [rsp + 1096], rdx
                        cmp              eax, 104;                  je    n372_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n377_assign_α
n376_call_β:                                                        jmp   n372_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n377_assign_α:          mov              rax, qword ptr [rsp + 1088]
                        mov              rdx, qword ptr [rsp + 1096]
                        mov              qword ptr [r9 + 48], rax                       # icase
                        mov              qword ptr [r9 + 56], rdx;  jmp   n378_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n378_statement_end_α:                                               jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n379_goto_α:                                                        jmp   n250_statement_begin_α
n379_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n380_goto_α:                                                        jmp   n290_statement_begin_α
n380_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n381_goto_α:                                                        jmp   n304_statement_begin_α
n381_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n382_goto_α:                                                        jmp   n318_statement_begin_α
n382_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n383_goto_α:                                                        jmp   n328_statement_begin_α
n383_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S3]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                lea              rdi, [rip + .S4]
                        call             rt_bomb@PLT
                        ud2
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
.S0:                    .string          "letter"
.S1:                    .string          "ch"
.S2:                    .string          "PAT$0$V0"
.S3:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
.S4:                    .string          "BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .rodata
.C0:                    .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                        .byte            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
                        .byte            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                        .byte            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .text
                        .section         .note.GNU-stack,"",@progbits

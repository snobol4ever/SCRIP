                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__pad_left_α:
proc_LBL__pad_left_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_call_α
#-----------------------------------------------------------------------------------------------------------------------
n2_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd33:             .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd33]
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
                        cmp              eax, 104;                  jne   .Lx32_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
.Lx32_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n3_lit_string_α
n2_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_binop_α
n3_lit_string_β:        add              rsp, 16
                        add              rsp, 32;                   jmp   n0_statement_begin_β
.Lx34_0:                .quad            .Lx34_0_s
.Lx34_0_s:              .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n4_binop_α:             sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n5_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:            mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              qword ptr [r9 + 56], rdx;  jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:                                                 jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                               jmp   n8_var_α
n7_statement_begin_β:   add              rsp, 64;                   jmp   n18_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n9_call_α
n8_var_β:               add              rsp, 16
                        add              rsp, 64;                   jmp   n7_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd43:             .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd43]
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
                        cmp              eax, 104;                  jne   .Lx42_240
                        add              rsp, 16;                   jmp   n8_var_β
.Lx42_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n10_var_α
n9_call_β:              add              rsp, 16;                   jmp   n8_var_β
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n11_coerce_numeric_α
n10_var_β:              add              rsp, 32;                   jmp   n8_var_β
#-----------------------------------------------------------------------------------------------------------------------
n11_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # call
                        cmp              eax, 5;                    je    .Lx46_1
                        cmp              eax, 3;                    jne   .Lx46_0
                        mov              eax, dword ptr [rsp + 16]                      # var
                        cmp              eax, 3;                    jne   .Lx46_0
.Lx46_1:                mov              rax, qword ptr [rsp + 32]                      # call
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # call
                        mov              qword ptr [rsp + 8], rax;  jmp   n12_coerce_numeric_α
.Lx46_0:                lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n12_coerce_numeric_α
n11_coerce_numeric_β:   add              rsp, 16;                   jmp   n10_var_β
#-----------------------------------------------------------------------------------------------------------------------
n12_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx48_1
                        cmp              eax, 3;                    jne   .Lx48_0
                        mov              eax, dword ptr [rsp + 48]                      # call
                        cmp              eax, 3;                    jne   .Lx48_0
.Lx48_1:                mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n13_cmp_test_α
.Lx48_0:                lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n13_cmp_test_α
n12_coerce_numeric_β:   add              rsp, 16;                   jmp   n11_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n13_cmp_test_α:         sub              rsp, 16
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
                        test             eax, eax;                  jns   .Lx50_240
                        add              rsp, 16;                   jmp   n12_coerce_numeric_β
.Lx50_240:              mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0;    jmp   n14_var_α
n13_cmp_test_β:         add              rsp, 16;                   jmp   n12_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n15_binop_α
n14_var_β:              add              rsp, 16;                   jmp   n13_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n15_binop_α:            sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n16_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              qword ptr [r9 + 8], rdx;   jmp   n17_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    add              rsp, 192;                  jmp   RETURN
#=======================================================================================================================
#         startswith('foobar', 'bar')     :S(bad_sw2)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:                                              jmp   n19_var_α
n18_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n19_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n20_var_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n21_var_α
n20_var_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n18_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n22_call_α
n21_var_β:              add              rsp, 16;                   jmp   n20_var_β
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd62:             .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd62]
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
                        cmp              eax, 104;                  jne   .Lx61_240
                        add              rsp, 16;                   jmp   n21_var_β
.Lx61_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n23_binop_α
n22_call_β:             add              rsp, 16;                   jmp   n21_var_β
#-----------------------------------------------------------------------------------------------------------------------
n23_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  jne   .Lx63_240
                        add              rsp, 32;                   jmp   n21_var_β
.Lx63_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n24_call_α
n23_binop_β:            add              rsp, 32;                   jmp   n21_var_β
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:             sub              rsp, 16
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
.Lrkfnzd65:             .string          "DUPL"
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
                        add              rsp, 16;                   jmp   n23_binop_β
.Lx64_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n25_var_α
n24_call_β:             add              rsp, 16;                   jmp   n23_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n26_binop_α
n25_var_β:              add              rsp, 32;                   jmp   n23_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n26_binop_α:            sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n27_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              qword ptr [r9 + 8], rdx;   jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    add              rsp, 128;                  jmp   RETURN
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
n71_statement_begin_α:                                              jmp   n72_var_α
n71_statement_begin_β:                                              jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n73_call_α
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd104:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd104]
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
                        cmp              eax, 104;                  jne   .Lx103_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n71_statement_begin_β
.Lx103_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n74_lit_string_α
n73_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n71_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx105_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n75_binop_α
n74_lit_string_β:       add              rsp, 16
                        add              rsp, 32;                   jmp   n71_statement_begin_β
.Lx105_0:               .quad            .Lx105_0_s
.Lx105_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n75_binop_α:            sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n76_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n76_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              qword ptr [r9 + 56], rdx;  jmp   n77_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:    add              rsp, 64;                   jmp   n78_statement_begin_α
#=======================================================================================================================
#         OUTPUT = index('foobar', 'xyz') ;* 0
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:                                              jmp   n79_var_α
n78_statement_begin_β:                                              jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n79_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n80_call_α
#-----------------------------------------------------------------------------------------------------------------------
n80_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd114:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd114]
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
                        cmp              eax, 104;                  jne   .Lx113_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n78_statement_begin_β
.Lx113_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n81_var_α
n80_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n78_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n81_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n82_coerce_numeric_α
n81_var_β:              add              rsp, 16
                        add              rsp, 32;                   jmp   n78_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n82_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # call
                        cmp              eax, 5;                    je    .Lx117_1
                        cmp              eax, 3;                    jne   .Lx117_0
                        mov              eax, dword ptr [rsp + 16]                      # var
                        cmp              eax, 3;                    jne   .Lx117_0
.Lx117_1:               mov              rax, qword ptr [rsp + 32]                      # call
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # call
                        mov              qword ptr [rsp + 8], rax;  jmp   n83_coerce_numeric_α
.Lx117_0:               lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n83_coerce_numeric_α
n82_coerce_numeric_β:   add              rsp, 16;                   jmp   n81_var_β
#-----------------------------------------------------------------------------------------------------------------------
n83_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx119_1
                        cmp              eax, 3;                    jne   .Lx119_0
                        mov              eax, dword ptr [rsp + 48]                      # call
                        cmp              eax, 3;                    jne   .Lx119_0
.Lx119_1:               mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n84_cmp_test_α
.Lx119_0:               lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n84_cmp_test_α
n83_coerce_numeric_β:   add              rsp, 16;                   jmp   n82_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n84_cmp_test_α:         sub              rsp, 16
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
                        test             eax, eax;                  jns   .Lx121_240
                        add              rsp, 16;                   jmp   n83_coerce_numeric_β
.Lx121_240:             mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0;    jmp   n85_var_α
n84_cmp_test_β:         add              rsp, 16;                   jmp   n83_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n85_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n86_binop_α
n85_var_β:              add              rsp, 16;                   jmp   n84_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n86_binop_α:            sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n87_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              qword ptr [r9 + 72], rdx;  jmp   n88_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    add              rsp, 128;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:                                              jmp   n90_var_α
n89_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n90_var_α:              mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 816], rax                     # result
                        mov              qword ptr [rsp + 824], rdx; jmp   n91_var_α
#-----------------------------------------------------------------------------------------------------------------------
n91_var_α:              mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 896], rax                     # result
                        mov              qword ptr [rsp + 904], rdx; jmp   n92_var_α
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:              mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 928], rax                     # result
                        mov              qword ptr [rsp + 936], rdx; jmp   n93_var_α
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:              mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 992], rax                     # result
                        mov              qword ptr [rsp + 1000], rdx; jmp   n94_call_α
#-----------------------------------------------------------------------------------------------------------------------
n94_call_α:             mov              rax, qword ptr [rsp + 992]
                        mov              qword ptr [rsp + 960], rax
                        mov              rax, qword ptr [rsp + 1000]
                        mov              qword ptr [rsp + 968], rax
                        .section         .rodata
.Lrkfn134:              .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn134]
                        lea              rsi, [rsp + 960]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                        cmp              eax, 104;                  je    n89_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n95_binop_α
n94_call_β:                                                         jmp   n89_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n95_binop_α:            mov              eax, dword ptr [rsp + 928]
                        cmp              eax, 3;                    jne   .Lx135_0
                        mov              eax, dword ptr [rsp + 944]
                        cmp              eax, 3;                    jne   .Lx135_0
                        mov              rax, qword ptr [rsp + 936]
                        mov              rcx, qword ptr [rsp + 952]
                        sub              rax, rcx
                        mov              qword ptr [rsp + 912], 3
                        mov              qword ptr [rsp + 920], rax; jmp   n96_call_α
.Lx135_0:               mov              rdi, qword ptr [rsp + 928]
                        mov              rsi, qword ptr [rsp + 936]
                        mov              rdx, qword ptr [rsp + 944]
                        mov              rcx, qword ptr [rsp + 952]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  je    n89_statement_begin_β
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n96_call_α
#-----------------------------------------------------------------------------------------------------------------------
n96_call_α:             mov              rax, qword ptr [rsp + 912]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 920]
                        mov              qword ptr [rsp + 872], rax
                        mov              rax, qword ptr [rsp + 896]
                        mov              qword ptr [rsp + 848], rax
                        mov              rax, qword ptr [rsp + 904]
                        mov              qword ptr [rsp + 856], rax
                        .section         .rodata
.Lrkfn137:              .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn137]
                        lea              rsi, [rsp + 848]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                        cmp              eax, 104;                  je    n89_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n97_binop_α
n96_call_β:                                                         jmp   n89_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n97_binop_α:            mov              rdi, qword ptr [rsp + 816]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n98_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n98_assign_α:           mov              rax, qword ptr [rsp + 800]
                        mov              rdx, qword ptr [rsp + 808]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              qword ptr [r9 + 72], rdx;  jmp   n99_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:                                                jmp   RETURN
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
n142_statement_begin_α:                                             jmp   n143_lit_string_α
n142_statement_begin_β:                                             jmp   n155_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n143_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx175_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n144_lit_integer_α
n143_lit_string_β:      add              rsp, 16;                   jmp   n142_statement_begin_β
.Lx175_0:               .quad            .Lx175_0_s
.Lx175_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n144_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n145_call_α
n144_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n142_statement_begin_β
.Lx176_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n145_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd62:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd62]
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
                        cmp              eax, 104;                  jne   .Lx177_240
                        add              rsp, 16;                   jmp   n144_lit_integer_β
.Lx177_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n146_binop_α
n145_call_β:            add              rsp, 16;                   jmp   n144_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n146_binop_α:           sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n147_lit_integer_α
n146_binop_β:           add              rsp, 32;                   jmp   n144_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n147_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n148_call_α
n147_lit_integer_β:     add              rsp, 16;                   jmp   n146_binop_β
.Lx179_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n148_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd65:         .string          "CHAR"
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
                        cmp              eax, 104;                  jne   .Lx180_240
                        add              rsp, 16;                   jmp   n147_lit_integer_β
.Lx180_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n149_binop_α
n148_call_β:            add              rsp, 16;                   jmp   n147_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n149_binop_α:           sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n150_lit_integer_α
n149_binop_β:           add              rsp, 32;                   jmp   n147_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx182_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n151_call_α
n150_lit_integer_β:     add              rsp, 16;                   jmp   n149_binop_β
.Lx182_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n151_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd68:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd68]
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
                        cmp              eax, 104;                  jne   .Lx183_240
                        add              rsp, 16;                   jmp   n150_lit_integer_β
.Lx183_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n152_binop_α
n151_call_β:            add              rsp, 16;                   jmp   n150_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n152_binop_α:           sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n153_assign_α
n152_binop_β:           add              rsp, 32;                   jmp   n150_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n153_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              qword ptr [r9 + 104], rdx; jmp   n154_statement_end_α
n153_assign_β:                                                      jmp   n142_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_end_α:                                               jmp   n155_statement_begin_α
n154_statement_end_β:                                               jmp   n155_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_begin_α:                                             jmp   n156_var_α
n155_statement_begin_β:                                             jmp   n169_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n156_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n157_match_begin_α
n156_var_β:             add              rsp, 16;                   jmp   n169_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n157_match_begin_α:     mov              rdi, qword ptr [rsp + 0]                       # var
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
.Lx192_0:               mov              r14d, dword ptr [rsp + 0]; jmp   n158_match_pos_α
n157_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx192_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx192_1
                                                                    jmp   .Lx192_0
.Lx192_1:
n157_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n169_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                 jne   n157_match_begin_β
                                                                    jmp   n159_match_alternate_α
n158_match_pos_β:                                                   jmp   n157_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n159_match_alternate_α: sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx195_21]
                        mov              qword ptr [rsp + 16], rax; jmp   n168_match_span_α
.Lx195_21:              lea              rax, [rip + .Lx195_19]
                        mov              qword ptr [rsp + 16], rax; jmp   n167_match_lit_α
n159_match_alternate_s0: lea              rax, [rip + .Lx195_40]
                        mov              qword ptr [rsp + 8], rax;  jmp   n159_match_alternate_as
n159_match_alternate_s1: lea              rax, [rip + .Lx195_41]
                        mov              qword ptr [rsp + 8], rax;  jmp   n159_match_alternate_as
.Lx195_40:                                                          jmp   n168_match_span_β
.Lx195_41:                                                          jmp   n167_match_lit_β
n159_match_alternate_as:                                            jmp   n160_match_assign_save_α
n159_match_alternate_β: mov              rax, qword ptr [rsp + 8];  jmp   rax
n159_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16]; jmp   rax
.Lx195_19:              add              rsp, 32;                   jmp   n158_match_pos_β
#-----------------------------------------------------------------------------------------------------------------------
n160_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n161_match_rem_α
n160_match_assign_save_β: add              rsp, 16;                 jmp   n159_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n161_match_rem_α:       sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                jmp   n162_match_assign_cond_α
n161_match_rem_β:       mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n159_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n162_match_assign_cond_α: mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n163_match_end_α
n162_match_assign_cond_β: sub              r12, 24;                 jmp   n161_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n163_match_end_α:       mov              r8, r12
.Lx202_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx202_9
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
.Lx202_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx202_5
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
.Lx202_1:               test             rax, rax;                  je    .Lx202_2
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
                        lea              rcx, [rip + .Lx202_3]
                        lea              rdx, [rip + .Lx202_4];     jmp   rax
.Lx202_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx202_1
.Lx202_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx202_1
.Lx202_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx202_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx202_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n164_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n164_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n165_match_replace_α
n164_lit_string_β:      add              rsp, 16;                   jmp   n169_statement_begin_α
.Lx203_0:               .quad            .Lx203_0_s
.Lx203_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n165_match_replace_α:   mov              rdi, qword ptr [rip + .Lx205_0]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx205_1
.Lx205_0:               .quad            .Lx205_0_s
.Lx205_0_s:             .string          "s"
.Lx205_1:                                                           jmp   n166_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n166_statement_end_α:                                               jmp   n169_statement_begin_α
n166_statement_end_β:                                               jmp   n169_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n167_match_lit_α:                                                   jmp   n159_match_alternate_s1
n167_match_lit_β:                                                   jmp   n159_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n168_match_span_α:      lea              rdi, [rip + .S1]
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
                        test             rax, rax;                  js    n159_match_alternate_af
                        mov              r8, qword ptr [rsp + 1440]
                        mov              r9d, dword ptr [rsp + 1448]
                        mov              dword ptr [rsp + 1440], 0
.Lx211_0:               mov              eax, r14d
                        add              eax, dword ptr [rsp + 1440]
                        cmp              eax, r15d;                 jge   .Lx211_1
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edx, 0
.Lx211_2:               cmp              edx, r9d;                  jge   .Lx211_1
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                  je    .Lx211_3
                        add              edx, 1;                    jmp   .Lx211_2
.Lx211_3:               add              dword ptr [rsp + 1440], 1; jmp   .Lx211_0
.Lx211_1:               mov              eax, dword ptr [rsp + 1440]
                        test             eax, eax;                  jle   n159_match_alternate_af
                        mov              edx, r14d
                        mov              dword ptr [rsp + 1444], edx
                        add              edx, eax
                        mov              r14d, edx;                 jmp   n159_match_alternate_s0
n168_match_span_β:      mov              r14d, dword ptr [rsp + 1444]; jmp   n159_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n169_statement_begin_α:                                             jmp   n170_var_α
n169_statement_begin_β: add              rsp, 224;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n170_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n171_assign_α
n170_var_β:             add              rsp, 16
                        add              rsp, 224;                  jmp   n169_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n171_assign_α:          mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax                       # ltrim
                        mov              qword ptr [r9 + 88], rdx;  jmp   n172_statement_end_α
n171_assign_β:                                                      jmp   n169_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:   add              rsp, 176;                  jmp   RETURN
n172_statement_end_β:   add              rsp, 240;                  jmp   RETURN
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
n218_statement_begin_α:                                             jmp   n219_lit_string_α
n218_statement_begin_β:                                             jmp   n231_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n219_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx273_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n220_lit_integer_α
.Lx273_0:               .quad            .Lx273_0_s
.Lx273_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n220_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n221_call_α
n220_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n218_statement_begin_β
.Lx274_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n221_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd93:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd93]
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
                        cmp              eax, 104;                  jne   .Lx275_240
                        add              rsp, 16;                   jmp   n220_lit_integer_β
.Lx275_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n222_binop_α
n221_call_β:            add              rsp, 16;                   jmp   n220_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n222_binop_α:           sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n223_lit_integer_α
n222_binop_β:           add              rsp, 32;                   jmp   n220_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n223_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n224_call_α
n223_lit_integer_β:     add              rsp, 16;                   jmp   n222_binop_β
.Lx277_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n224_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd96:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd96]
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
                        cmp              eax, 104;                  jne   .Lx278_240
                        add              rsp, 16;                   jmp   n223_lit_integer_β
.Lx278_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n225_binop_α
n224_call_β:            add              rsp, 16;                   jmp   n223_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n225_binop_α:           sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n226_lit_integer_α
n225_binop_β:           add              rsp, 32;                   jmp   n223_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n226_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx280_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n227_call_α
n226_lit_integer_β:     add              rsp, 16;                   jmp   n225_binop_β
.Lx280_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n227_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd99:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd99]
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
                        cmp              eax, 104;                  jne   .Lx281_240
                        add              rsp, 16;                   jmp   n226_lit_integer_β
.Lx281_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n228_binop_α
n227_call_β:            add              rsp, 16;                   jmp   n226_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n228_binop_α:           sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n229_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n229_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              qword ptr [r9 + 104], rdx; jmp   n230_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_end_α:                                               jmp   n231_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_begin_α:                                             jmp   n232_var_α
n231_statement_begin_β:                                             jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n232_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n233_call_α
n232_var_β:             add              rsp, 16
                        add              rsp, 160;                  jmp   n231_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n233_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd290:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd290]
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
                        cmp              eax, 104;                  jne   .Lx289_240
                        add              rsp, 16;                   jmp   n232_var_β
.Lx289_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n234_assign_α
n233_call_β:            add              rsp, 16;                   jmp   n232_var_β
#-----------------------------------------------------------------------------------------------------------------------
n234_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx; jmp   n235_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_end_α:                                               jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_begin_α:                                             jmp   n237_var_α
n236_statement_begin_β: add              rsp, 192;                  jmp   n243_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n237_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n238_lit_integer_α
n237_var_β:             add              rsp, 16
                        add              rsp, 192;                  jmp   n236_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n238_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n239_coerce_numeric_α
n238_lit_integer_β:     add              rsp, 16;                   jmp   n237_var_β
.Lx297_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n239_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx299_1
                        cmp              eax, 3;                    jne   .Lx299_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx299_0
.Lx299_1:               mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n240_coerce_numeric_α
.Lx299_0:               lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n240_coerce_numeric_α
n239_coerce_numeric_β:  add              rsp, 16;                   jmp   n238_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n240_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5;                    je    .Lx301_1
                        cmp              eax, 3;                    jne   .Lx301_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3;                    jne   .Lx301_0
.Lx301_1:               mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n241_cmp_test_α
.Lx301_0:               lea              rdi, [rsp + 32]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n241_cmp_test_α
n240_coerce_numeric_β:  add              rsp, 16;                   jmp   n239_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n241_cmp_test_α:        sub              rsp, 16
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
                        test             eax, eax;                  jle   .Lx303_240
                        add              rsp, 16;                   jmp   n240_coerce_numeric_β
.Lx303_240:                                                         jmp   n242_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n242_statement_end_α:                                               jmp   n264_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_begin_α:                                             jmp   n244_var_α
n243_statement_begin_β:                                             jmp   n250_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n244_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2016], rax                    # result
                        mov              qword ptr [rsp + 2024], rdx; jmp   n245_var_α
#-----------------------------------------------------------------------------------------------------------------------
n245_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2032], rax                    # result
                        mov              qword ptr [rsp + 2040], rdx; jmp   n246_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n246_lit_integer_α:     mov              qword ptr [rsp + 2048], 3                      # result
                        mov              rax, qword ptr [rip + .Lx310_0]
                        mov              qword ptr [rsp + 2056], rax; jmp   n247_call_α
.Lx310_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n247_call_α:            mov              rax, qword ptr [rsp + 2048]
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
.Lrkfn312:              .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn312]
                        lea              rsi, [rsp + 1952]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                        cmp              eax, 104;                  je    n243_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n248_assign_α
n247_call_β:                                                        jmp   n243_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n248_assign_α:          mov              rax, qword ptr [rsp + 1936]
                        mov              rdx, qword ptr [rsp + 1944]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              qword ptr [r9 + 168], rdx; jmp   n249_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n249_statement_end_α:                                               jmp   n250_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n250_statement_begin_α:                                             jmp   n251_var_α
n250_statement_begin_β:                                             jmp   n264_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n251_var_α:             mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 2192], rax                    # result
                        mov              qword ptr [rsp + 2200], rdx; jmp   n252_var_α
#-----------------------------------------------------------------------------------------------------------------------
n252_var_α:             mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 2176], rax                    # result
                        mov              qword ptr [rsp + 2184], rdx; jmp   n253_coerce_string_α
#-----------------------------------------------------------------------------------------------------------------------
n253_coerce_string_α:   lea              rdi, [rsp + 2176]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n254_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n254_match_begin_α:     mov              qword ptr [rsp + 2120], rsp                    # old____
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
.Lx323_0:               mov              r14d, dword ptr [rsp + 0]; jmp   n255_match_any_α
n254_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx323_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx323_1
                                                                    jmp   .Lx323_0
.Lx323_1:
n254_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n264_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n255_match_any_α:       mov              eax, r14d
                        cmp              eax, r15d;                 jge   n254_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rsp + 2168]
                        mov              r9d, dword ptr [rsp + 2164]
                        mov              edx, 0
.Lx325_5:               cmp              edx, r9d;                  jge   n254_match_begin_β
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                  je    .Lx325_6
                        add              edx, 1;                    jmp   .Lx325_5
.Lx325_6:               add              r14d, 1;                   jmp   n256_match_end_α
n255_match_any_β:       sub              r14d, 1;                   jmp   n254_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n256_match_end_α:       mov              r8, r12
.Lx327_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx327_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx327_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx327_5
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
.Lx327_1:               test             rax, rax;                  je    .Lx327_2
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
                        lea              rcx, [rip + .Lx327_3]
                        lea              rdx, [rip + .Lx327_4];     jmp   rax
.Lx327_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx327_1
.Lx327_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx327_1
.Lx327_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx327_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx327_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n257_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n257_statement_end_α:                                               jmp   n258_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_begin_α:                                             jmp   n259_var_α
n258_statement_begin_β:                                             jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n259_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2224], rax                    # result
                        mov              qword ptr [rsp + 2232], rdx; jmp   n260_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n260_lit_integer_α:     mov              qword ptr [rsp + 2240], 3                      # result
                        mov              rax, qword ptr [rip + .Lx333_0]
                        mov              qword ptr [rsp + 2248], rax; jmp   n261_binop_α
.Lx333_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n261_binop_α:           mov              eax, dword ptr [rsp + 2224]
                        cmp              eax, 3;                    jne   .Lx334_0
                        mov              rax, qword ptr [rsp + 2232]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2208], 3
                        mov              qword ptr [rsp + 2216], rax; jmp   n262_assign_α
.Lx334_0:               mov              rdi, qword ptr [rsp + 2224]
                        mov              rsi, qword ptr [rsp + 2232]
                        mov              rdx, qword ptr [rsp + 2240]
                        mov              rcx, qword ptr [rsp + 2248]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  jne   .Lx334_240
                        add              rsp, 32;                   jmp   n258_statement_begin_β
.Lx334_240:             mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n262_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n262_assign_α:          mov              rax, qword ptr [rsp + 2208]
                        mov              rdx, qword ptr [rsp + 2216]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx; jmp   n263_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n263_statement_end_α:                                               jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n264_statement_begin_α:                                             jmp   n265_var_α
n264_statement_begin_β: add              rsp, 272;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n265_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n266_lit_integer_α
n265_var_β:             add              rsp, 16
                        add              rsp, 272;                  jmp   n264_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n266_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx341_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n267_var_α
n266_lit_integer_β:     add              rsp, 16;                   jmp   n265_var_β
.Lx341_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n267_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n268_call_α
n267_var_β:             add              rsp, 16;                   jmp   n266_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n268_call_α:            sub              rsp, 16
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
.Lrkfnzd344:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd344]
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
                        cmp              eax, 104;                  jne   .Lx343_240
                        add              rsp, 16;                   jmp   n267_var_β
.Lx343_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n269_assign_α
n268_call_β:            add              rsp, 16;                   jmp   n267_var_β
#-----------------------------------------------------------------------------------------------------------------------
n269_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              qword ptr [r9 + 136], rdx; jmp   n270_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:   add              rsp, 336;                  jmp   RETURN
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
n348_statement_begin_α:                                             jmp   n349_var_α
n348_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n349_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n350_call_α
#-----------------------------------------------------------------------------------------------------------------------
n350_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # var
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx358_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + rtrim_alpha];  jmp   rax
.Lx358_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx358_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n348_statement_begin_β
.Lx358_240:                                                         jmp   n351_call_α
n350_call_β:                                                        jmp   n348_statement_begin_β
.Lx358_0:               .quad            .Lx358_0_s
.Lx358_0_s:             .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n351_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # call
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx360_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ltrim_alpha];  jmp   rax
.Lx360_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx360_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n348_statement_begin_β
.Lx360_240:                                                         jmp   n352_assign_α
n351_call_β:                                                        jmp   n348_statement_begin_β
.Lx360_0:               .quad            .Lx360_0_s
.Lx360_0_s:             .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n352_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 176], rax                      # trimws
                        mov              qword ptr [r9 + 184], rdx; jmp   n353_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n353_statement_end_α:   add              rsp, 48;                   jmp   RETURN
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
n364_statement_begin_α:                                             jmp   n365_var_α
n364_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n365_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n366_var_α
#-----------------------------------------------------------------------------------------------------------------------
n366_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n367_call_α
n366_var_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n364_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n367_call_α:            sub              rsp, 16
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
.Lrkfnzd375:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd375]
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
                        cmp              eax, 104;                  jne   .Lx374_240
                        add              rsp, 16;                   jmp   n366_var_β
.Lx374_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n368_assign_α
n367_call_β:            add              rsp, 16;                   jmp   n366_var_β
#-----------------------------------------------------------------------------------------------------------------------
n368_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax                      # repeat
                        mov              qword ptr [r9 + 200], rdx; jmp   n369_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n369_statement_end_α:   add              rsp, 48;                   jmp   RETURN
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
n379_statement_begin_α:                                             jmp   n380_var_α
n379_statement_begin_β:                                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n380_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n381_var_α
#-----------------------------------------------------------------------------------------------------------------------
n381_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n382_assign_α
n381_var_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n382_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx394_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n383_var_α
n382_assign_β:                                                      jmp   n381_var_β
.Lx394_0:               .quad            .Lx394_0_s
.Lx394_0_s:             .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n383_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n384_coerce_string_α
n383_var_β:             add              rsp, 16;                   jmp   n382_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n384_coerce_string_α:   sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n385_match_begin_α
n384_coerce_string_β:   add              rsp, 16;                   jmp   n383_var_β
#-----------------------------------------------------------------------------------------------------------------------
n385_match_begin_α:     mov              rdi, qword ptr [rsp + 48]                      # var
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
.Lx399_0:               mov              r14d, dword ptr [rsp + 2656]; jmp   n386_match_break_α
n385_match_begin_β:     add              dword ptr [rsp + 2656], 1
                        mov              eax, dword ptr [rsp + 2656]
                        cmp              eax, r15d;                 jg    .Lx399_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx399_1
                                                                    jmp   .Lx399_0
.Lx399_1:
n385_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n384_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n386_match_break_α:     sub              rsp, 16
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
                        cmp              eax, r15d;                 jl    .Lx401_240
                        add              rsp, 16;                   jmp   n385_match_begin_β
.Lx401_240:             mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax;                 jmp   n387_match_defer_α
n386_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16;                   jmp   n385_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n387_match_defer_α:     lea              rdi, [rip + .S2]
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
                        test             rax, rax;                  jz    .Lx402_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx402_4]
                        lea              r11, [rip + .Lx402_5];     jmp   rax
.Lx402_4:                                                           jmp   n388_match_end_α
.Lx402_5:                                                           jmp   n386_match_break_β
.Lx402_0:               push             r14
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
.Lx402_2:               test             rax, rax;                  je    .Lx402_3
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
                        lea              rcx, [rip + .Lx402_7]
                        lea              rdx, [rip + .Lx402_8];     jmp   rax
.Lx402_7:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx402_2
.Lx402_8:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx402_2
.Lx402_3:               add              rsp, 8
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
                        test             eax, eax;                  js    n386_match_break_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx402_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n388_match_end_α
.Lx402_6:               add              rsp, 16;                   jmp   n386_match_break_β
n387_match_defer_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n388_match_end_α:       mov              r8, r12
.Lx404_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx404_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx404_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx404_5
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
.Lx404_1:               test             rax, rax;                  je    .Lx404_2
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
                        lea              rcx, [rip + .Lx404_3]
                        lea              rdx, [rip + .Lx404_4];     jmp   rax
.Lx404_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx404_1
.Lx404_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx404_1
.Lx404_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx404_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx404_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n389_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n389_statement_end_α:   add              rsp, 64;                   jmp   RETURN
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
n407_statement_begin_α:                                             jmp   n408_var_α
n407_statement_begin_β:                                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n408_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n409_var_α
#-----------------------------------------------------------------------------------------------------------------------
n409_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n410_assign_α
n409_var_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n410_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx420_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n411_match_begin_α
n410_assign_β:                                                      jmp   n409_var_β
.Lx420_0:               .quad            .Lx420_0_s
.Lx420_0_s:             .string          "PATV$1"
#-----------------------------------------------------------------------------------------------------------------------
n411_match_begin_α:     mov              rdi, qword ptr [rsp + 16]                      # var
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
.Lx422_0:               mov              r14d, dword ptr [rsp + 2848]; jmp   n412_match_pos_α
n411_match_begin_β:     add              dword ptr [rsp + 2848], 1
                        mov              eax, dword ptr [rsp + 2848]
                        cmp              eax, r15d;                 jg    .Lx422_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx422_1
                                                                    jmp   .Lx422_0
.Lx422_1:
n411_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n410_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n412_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                 jne   n411_match_begin_β
                                                                    jmp   n413_match_defer_α
n412_match_pos_β:                                                   jmp   n411_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n413_match_defer_α:     lea              rdi, [rip + .S3]
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
                        test             rax, rax;                  jz    .Lx424_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx424_4]
                        lea              r11, [rip + .Lx424_5];     jmp   rax
.Lx424_4:                                                           jmp   n414_match_end_α
.Lx424_5:                                                           jmp   n411_match_begin_β
.Lx424_0:               push             r14
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
.Lx424_2:               test             rax, rax;                  je    .Lx424_3
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
                        lea              rdx, [rip + .Lx424_8];     jmp   rax
.Lx424_7:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx424_2
.Lx424_8:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx424_2
.Lx424_3:               add              rsp, 8
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
                        test             eax, eax;                  js    n411_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx424_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n414_match_end_α
.Lx424_6:               add              rsp, 16;                   jmp   n411_match_begin_β
n413_match_defer_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n414_match_end_α:       mov              r8, r12
.Lx426_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx426_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx426_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx426_5
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
.Lx426_1:               test             rax, rax;                  je    .Lx426_2
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
                        lea              rdx, [rip + .Lx426_4];     jmp   rax
.Lx426_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx426_1
.Lx426_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx426_1
.Lx426_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx426_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx426_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n415_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n415_statement_end_α:   add              rsp, 32;                   jmp   RETURN
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
n429_statement_begin_α:                                             jmp   n430_var_α
n429_statement_begin_β:                                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n430_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n431_var_α
#-----------------------------------------------------------------------------------------------------------------------
n431_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n432_assign_α
n431_var_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n432_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx442_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n433_match_begin_α
n432_assign_β:                                                      jmp   n431_var_β
.Lx442_0:               .quad            .Lx442_0_s
.Lx442_0_s:             .string          "PATV$2"
#-----------------------------------------------------------------------------------------------------------------------
n433_match_begin_α:     mov              rdi, qword ptr [rsp + 16]                      # var
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
.Lx444_0:               mov              r14d, dword ptr [rsp + 3008]; jmp   n434_match_defer_α
n433_match_begin_β:     add              dword ptr [rsp + 3008], 1
                        mov              eax, dword ptr [rsp + 3008]
                        cmp              eax, r15d;                 jg    .Lx444_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx444_1
                                                                    jmp   .Lx444_0
.Lx444_1:
n433_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n432_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n434_match_defer_α:     lea              rdi, [rip + .S4]
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
                        test             rax, rax;                  jz    .Lx445_0
                        mov              r8d, 1
                        lea              r10, [rip + .Lx445_4]
                        lea              r11, [rip + .Lx445_5];     jmp   rax
.Lx445_4:               lea              rcx, [rip + g_scan_hit_start]
                        mov              rax, qword ptr [rcx]
                        mov              dword ptr [rsp + 3008], eax; jmp   n435_match_rpos_α
.Lx445_5:                                                           jmp   n433_match_begin_β
.Lx445_0:               push             r14
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
.Lx445_2:               test             rax, rax;                  je    .Lx445_3
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
                        lea              rcx, [rip + .Lx445_7]
                        lea              rdx, [rip + .Lx445_8];     jmp   rax
.Lx445_7:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx445_2
.Lx445_8:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx445_2
.Lx445_3:               add              rsp, 8
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
                        test             eax, eax;                  js    n433_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx445_6]
                        sub              rsp, 8
                        push             rax;                       jmp   n435_match_rpos_α
.Lx445_6:               add              rsp, 16;                   jmp   n433_match_begin_β
n434_match_defer_β:                                                 jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n435_match_rpos_α:      mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx;                 jne   n433_match_begin_β
                                                                    jmp   n436_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n436_match_end_α:       mov              r8, r12
.Lx448_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx448_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx448_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx448_5
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
.Lx448_1:               test             rax, rax;                  je    .Lx448_2
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
                        lea              rcx, [rip + .Lx448_3]
                        lea              rdx, [rip + .Lx448_4];     jmp   rax
.Lx448_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx448_1
.Lx448_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx448_1
.Lx448_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx448_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx448_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n437_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n437_statement_end_α:   add              rsp, 32;                   jmp   RETURN
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
n451_statement_begin_α:                                             jmp   n452_lit_integer_α
n451_statement_begin_β:                                             jmp   n455_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n452_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx478_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n453_assign_α
.Lx478_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n453_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              qword ptr [r9 + 280], rdx; jmp   n454_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n454_statement_end_α:                                               jmp   n455_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n455_statement_begin_α:                                             jmp   n456_var_α
n455_statement_begin_β:                                             jmp   n459_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n456_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n457_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n457_assign_α:          mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              qword ptr [r9 + 296], rdx; jmp   n458_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n458_statement_end_α:                                               jmp   n459_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n459_statement_begin_α:                                             jmp   n460_var_α
n459_statement_begin_β: add              rsp, 32;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n460_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n461_var_α
n460_var_β:             add              rsp, 16
                        add              rsp, 32;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n461_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n462_coerce_string_α
n461_var_β:             add              rsp, 16;                   jmp   n460_var_β
#-----------------------------------------------------------------------------------------------------------------------
n462_coerce_string_α:   sub              rsp, 16
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n463_match_begin_α
n462_coerce_string_β:   add              rsp, 16;                   jmp   n461_var_β
#-----------------------------------------------------------------------------------------------------------------------
n463_match_begin_α:     mov              rdi, qword ptr [rsp + 32]                      # var
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
.Lx495_0:               mov              r14d, dword ptr [rsp + 0]; jmp   n464_match_assign_save_α
n463_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx495_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx495_1
                                                                    jmp   .Lx495_0
.Lx495_1:
n463_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n462_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n464_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n465_match_break_α
n464_match_assign_save_β: add              rsp, 16;                 jmp   n463_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n465_match_break_α:     sub              rsp, 16
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
                        cmp              eax, r15d;                 jl    .Lx499_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n463_match_begin_β
.Lx499_240:             mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax;                 jmp   n466_match_assign_cond_α
n465_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n463_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n466_match_assign_cond_α: mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S5]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n467_match_end_α
n466_match_assign_cond_β: sub              r12, 24;                 jmp   n465_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n467_match_end_α:       mov              r8, r12
.Lx503_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx503_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx503_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx503_5
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
.Lx503_1:               test             rax, rax;                  je    .Lx503_2
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
                        lea              rcx, [rip + .Lx503_3]
                        lea              rdx, [rip + .Lx503_4];     jmp   rax
.Lx503_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx503_1
.Lx503_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx503_1
.Lx503_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx503_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx503_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n468_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n468_statement_end_α:                                               jmp   n469_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n469_statement_begin_α:                                             jmp   n470_var_α
n469_statement_begin_β: add              rsp, 112;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n470_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n471_call_α
n470_var_β:             add              rsp, 16
                        add              rsp, 112;                  jmp   n469_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n471_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd510:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd510]
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
                        cmp              eax, 104;                  jne   .Lx509_240
                        add              rsp, 16;                   jmp   n470_var_β
.Lx509_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n472_lit_integer_α
n471_call_β:            add              rsp, 16;                   jmp   n470_var_β
#-----------------------------------------------------------------------------------------------------------------------
n472_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx511_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n473_binop_α
n472_lit_integer_β:     add              rsp, 32;                   jmp   n470_var_β
.Lx511_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n473_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                  jne   .Lx512_240
                        add              rsp, 16;                   jmp   n472_lit_integer_β
.Lx512_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n474_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n474_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              qword ptr [r9 + 280], rdx; jmp   n475_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n475_statement_end_α:   add              rsp, 80;                   jmp   RETURN
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
n516_save_restore_α:                                                jmp   n517_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n517_goto_deferred_α:   lea              rax, [rip + proc_LBL__pad_left_α]; jmp   rax
                                                                    jmp   n518_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n518_save_restore_α:
pad_left_alpha:         mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 48
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx524_2
                        mov              eax, 48
.Lx524_2:               sub              rsp, 96
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
                        cmp              rcx, 0;                    jbe   .Lx524_10
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx524_41
.Lx524_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx524_41:              cmp              rcx, 1;                    jbe   .Lx524_11
                        mov              rax, qword ptr [r8 + 96]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 96], rdx
                        mov              rax, qword ptr [r8 + 104]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 104], rdx; jmp   .Lx524_42
.Lx524_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx524_42:              cmp              rcx, 2;                    jbe   .Lx524_12
                        mov              rax, qword ptr [r8 + 112]
                        mov              rdx, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 112], rdx
                        mov              rax, qword ptr [r8 + 120]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 120], rdx; jmp   .Lx524_43
.Lx524_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx524_43:              lea              r10, [rip + pad_left_gamma]
                        lea              r11, [rip + pad_left_omega]
                        lea              rax, [rip + proc_LBL__pad_left_α]; jmp   rax
pad_left_gamma:         mov              rdi, qword ptr [r9 + 0]                        # pad_left
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx524_75
                        mov              eax, 48
.Lx524_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx524_80
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx524_110
.Lx524_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx524_110:             cmp              rcx, 1;                    jbe   .Lx524_81
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx524_111
.Lx524_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx524_111:             cmp              rcx, 2;                    jbe   .Lx524_82
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx524_112
.Lx524_82:              mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx524_112:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3;                    jbe   .Lx524_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx524_3:               mov              rcx, qword ptr [r8 + 64]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
pad_left_omega:         mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx524_145
                        mov              eax, 48
.Lx524_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx524_150
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx524_180
.Lx524_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx524_180:             cmp              rcx, 1;                    jbe   .Lx524_151
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx524_181
.Lx524_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx524_181:             cmp              rcx, 2;                    jbe   .Lx524_152
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx524_182
.Lx524_152:             mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx524_182:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3;                    jbe   .Lx524_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx524_4:               mov              rcx, qword ptr [r8 + 72]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n525_save_restore_α:                                                jmp   n526_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n526_goto_deferred_α:   lea              rax, [rip + proc_LBL__pad_right_α]; jmp   rax
                                                                    jmp   n527_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n527_save_restore_α:
pad_right_alpha:        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 48
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx533_2
                        mov              eax, 48
.Lx533_2:               sub              rsp, 96
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
                        cmp              rcx, 0;                    jbe   .Lx533_10
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx533_41
.Lx533_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx533_41:              cmp              rcx, 1;                    jbe   .Lx533_11
                        mov              rax, qword ptr [r8 + 96]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 96], rdx
                        mov              rax, qword ptr [r8 + 104]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 104], rdx; jmp   .Lx533_42
.Lx533_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx533_42:              cmp              rcx, 2;                    jbe   .Lx533_12
                        mov              rax, qword ptr [r8 + 112]
                        mov              rdx, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 112], rdx
                        mov              rax, qword ptr [r8 + 120]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 120], rdx; jmp   .Lx533_43
.Lx533_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx533_43:              lea              r10, [rip + pad_right_gamma]
                        lea              r11, [rip + pad_right_omega]
                        lea              rax, [rip + proc_LBL__pad_right_α]; jmp   rax
pad_right_gamma:        mov              rdi, qword ptr [r9 + 64]                       # pad_right
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx533_75
                        mov              eax, 48
.Lx533_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rcx, 0;                    jbe   .Lx533_80
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx533_110
.Lx533_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx533_110:             cmp              rcx, 1;                    jbe   .Lx533_81
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx533_111
.Lx533_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx533_111:             cmp              rcx, 2;                    jbe   .Lx533_82
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx533_112
.Lx533_82:              mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx533_112:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3;                    jbe   .Lx533_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx533_3:               mov              rcx, qword ptr [r8 + 64]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
pad_right_omega:        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx533_145
                        mov              eax, 48
.Lx533_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rcx, 0;                    jbe   .Lx533_150
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx533_180
.Lx533_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx533_180:             cmp              rcx, 1;                    jbe   .Lx533_151
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx533_181
.Lx533_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx533_181:             cmp              rcx, 2;                    jbe   .Lx533_152
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx533_182
.Lx533_152:             mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx533_182:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3;                    jbe   .Lx533_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx533_4:               mov              rcx, qword ptr [r8 + 72]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n534_save_restore_α:                                                jmp   n535_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n535_goto_deferred_α:   lea              rax, [rip + proc_LBL__ltrim_α]; jmp   rax
                                                                    jmp   n536_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n536_save_restore_α:
ltrim_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx542_2
                        mov              eax, 16
.Lx542_2:               sub              rsp, 96
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
                        cmp              rcx, 0;                    jbe   .Lx542_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx542_41
.Lx542_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx542_41:              lea              r10, [rip + ltrim_gamma]
                        lea              r11, [rip + ltrim_omega]
                        lea              rax, [rip + proc_LBL__ltrim_α]; jmp   rax
ltrim_gamma:            mov              rdi, qword ptr [r9 + 80]                       # ltrim
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 64]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx542_75
                        mov              eax, 16
.Lx542_75:              lea              r8, [rsp + 80]
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
                        cmp              rcx, 0;                    jbe   .Lx542_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx542_110
.Lx542_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx542_110:             mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              eax, 128
                        cmp              rcx, 1;                    jbe   .Lx542_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 112
.Lx542_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
ltrim_omega:            mov              rcx, qword ptr [rsp + 64]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx542_145
                        mov              eax, 16
.Lx542_145:             lea              r8, [rsp + 80]
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
                        cmp              rcx, 0;                    jbe   .Lx542_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx542_180
.Lx542_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx542_180:             mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              eax, 128
                        cmp              rcx, 1;                    jbe   .Lx542_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 112
.Lx542_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n543_save_restore_α:                                                jmp   n544_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n544_goto_deferred_α:   lea              rax, [rip + proc_LBL__rtrim_α]; jmp   rax
                                                                    jmp   n545_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n545_save_restore_α:
rtrim_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx551_2
                        mov              eax, 16
.Lx551_2:               sub              rsp, 112
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
                        cmp              rcx, 0;                    jbe   .Lx551_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx551_41
.Lx551_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx551_41:              lea              r10, [rip + rtrim_gamma]
                        lea              r11, [rip + rtrim_omega]
                        lea              rax, [rip + proc_LBL__rtrim_α]; jmp   rax
rtrim_gamma:            mov              rdi, qword ptr [r9 + 128]                      # rtrim
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx551_75
                        mov              eax, 16
.Lx551_75:              lea              r8, [rsp + 96]
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
                        cmp              rcx, 0;                    jbe   .Lx551_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx551_110
.Lx551_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx551_110:             mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 144
                        cmp              rcx, 1;                    jbe   .Lx551_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx551_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
rtrim_omega:            mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx551_145
                        mov              eax, 16
.Lx551_145:             lea              r8, [rsp + 96]
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
                        cmp              rcx, 0;                    jbe   .Lx551_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx551_180
.Lx551_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx551_180:             mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 144
                        cmp              rcx, 1;                    jbe   .Lx551_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx551_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n552_save_restore_α:                                                jmp   n553_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n553_goto_deferred_α:   lea              rax, [rip + proc_LBL__trimws_α]; jmp   rax
                                                                    jmp   n554_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n554_save_restore_α:
trimws_alpha:           mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx560_2
                        mov              eax, 16
.Lx560_2:               sub              rsp, 64
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
                        cmp              rcx, 0;                    jbe   .Lx560_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx560_41
.Lx560_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx560_41:              lea              r10, [rip + trimws_gamma]
                        lea              r11, [rip + trimws_omega]
                        lea              rax, [rip + proc_LBL__trimws_α]; jmp   rax
trimws_gamma:           mov              rdi, qword ptr [r9 + 176]                      # trimws
                        mov              rsi, qword ptr [r9 + 184]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx560_75
                        mov              eax, 16
.Lx560_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rcx, 0;                    jbe   .Lx560_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx560_110
.Lx560_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx560_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx560_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx560_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
trimws_omega:           mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx560_145
                        mov              eax, 16
.Lx560_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax                      # trimws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rcx, 0;                    jbe   .Lx560_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx560_180
.Lx560_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx560_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx560_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx560_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n561_save_restore_α:                                                jmp   n562_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n562_goto_deferred_α:   lea              rax, [rip + proc_LBL__repeat_α]; jmp   rax
                                                                    jmp   n563_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n563_save_restore_α:
repeat_alpha:           mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx569_2
                        mov              eax, 32
.Lx569_2:               sub              rsp, 80
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
                        cmp              rcx, 0;                    jbe   .Lx569_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx569_41
.Lx569_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx569_41:              cmp              rcx, 1;                    jbe   .Lx569_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx569_42
.Lx569_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx569_42:              lea              r10, [rip + repeat_gamma]
                        lea              r11, [rip + repeat_omega]
                        lea              rax, [rip + proc_LBL__repeat_α]; jmp   rax
repeat_gamma:           mov              rdi, qword ptr [r9 + 192]                      # repeat
                        mov              rsi, qword ptr [r9 + 200]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx569_75
                        mov              eax, 32
.Lx569_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rcx, 0;                    jbe   .Lx569_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx569_110
.Lx569_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx569_110:             cmp              rcx, 1;                    jbe   .Lx569_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx569_111
.Lx569_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx569_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx569_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx569_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
repeat_omega:           mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx569_145
                        mov              eax, 32
.Lx569_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax                      # repeat
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rcx, 0;                    jbe   .Lx569_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx569_180
.Lx569_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx569_180:             cmp              rcx, 1;                    jbe   .Lx569_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx569_181
.Lx569_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx569_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx569_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx569_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n570_save_restore_α:                                                jmp   n571_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n571_goto_deferred_α:   lea              rax, [rip + proc_LBL__contains_α]; jmp   rax
                                                                    jmp   n572_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n572_save_restore_α:
contains_alpha:         mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx578_2
                        mov              eax, 32
.Lx578_2:               sub              rsp, 80
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
                        cmp              rcx, 0;                    jbe   .Lx578_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx578_41
.Lx578_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx578_41:              cmp              rcx, 1;                    jbe   .Lx578_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx578_42
.Lx578_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx578_42:              lea              r10, [rip + contains_gamma]
                        lea              r11, [rip + contains_omega]
                        lea              rax, [rip + proc_LBL__contains_α]; jmp   rax
contains_gamma:         mov              rdi, qword ptr [r9 + 208]                      # contains
                        mov              rsi, qword ptr [r9 + 216]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx578_75
                        mov              eax, 32
.Lx578_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rcx, 0;                    jbe   .Lx578_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx578_110
.Lx578_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx578_110:             cmp              rcx, 1;                    jbe   .Lx578_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax; jmp   .Lx578_111
.Lx578_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx578_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx578_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx578_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
contains_omega:         mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx578_145
                        mov              eax, 32
.Lx578_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax                      # contains
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rcx, 0;                    jbe   .Lx578_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx578_180
.Lx578_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx578_180:             cmp              rcx, 1;                    jbe   .Lx578_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax; jmp   .Lx578_181
.Lx578_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx578_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx578_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx578_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n579_save_restore_α:                                                jmp   n580_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n580_goto_deferred_α:   lea              rax, [rip + proc_LBL__startswith_α]; jmp   rax
                                                                    jmp   n581_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n581_save_restore_α:
startswith_alpha:       mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx587_2
                        mov              eax, 32
.Lx587_2:               sub              rsp, 80
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
                        cmp              rcx, 0;                    jbe   .Lx587_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx587_41
.Lx587_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx587_41:              cmp              rcx, 1;                    jbe   .Lx587_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx587_42
.Lx587_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx587_42:              lea              r10, [rip + startswith_gamma]
                        lea              r11, [rip + startswith_omega]
                        lea              rax, [rip + proc_LBL__startswith_α]; jmp   rax
startswith_gamma:       mov              rdi, qword ptr [r9 + 240]                      # startswith
                        mov              rsi, qword ptr [r9 + 248]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx587_75
                        mov              eax, 32
.Lx587_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rcx, 0;                    jbe   .Lx587_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx587_110
.Lx587_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx587_110:             cmp              rcx, 1;                    jbe   .Lx587_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax; jmp   .Lx587_111
.Lx587_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx587_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx587_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx587_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
startswith_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx587_145
                        mov              eax, 32
.Lx587_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax                      # startswith
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rcx, 0;                    jbe   .Lx587_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx587_180
.Lx587_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx587_180:             cmp              rcx, 1;                    jbe   .Lx587_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax; jmp   .Lx587_181
.Lx587_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx587_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx587_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx587_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n588_save_restore_α:                                                jmp   n589_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n589_goto_deferred_α:   lea              rax, [rip + proc_LBL__endswith_α]; jmp   rax
                                                                    jmp   n590_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n590_save_restore_α:
endswith_alpha:         mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx596_2
                        mov              eax, 32
.Lx596_2:               sub              rsp, 80
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
                        cmp              rcx, 0;                    jbe   .Lx596_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx596_41
.Lx596_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx596_41:              cmp              rcx, 1;                    jbe   .Lx596_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx596_42
.Lx596_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx596_42:              lea              r10, [rip + endswith_gamma]
                        lea              r11, [rip + endswith_omega]
                        lea              rax, [rip + proc_LBL__endswith_α]; jmp   rax
endswith_gamma:         mov              rdi, qword ptr [r9 + 256]                      # endswith
                        mov              rsi, qword ptr [r9 + 264]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx596_75
                        mov              eax, 32
.Lx596_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rcx, 0;                    jbe   .Lx596_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx596_110
.Lx596_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx596_110:             cmp              rcx, 1;                    jbe   .Lx596_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax; jmp   .Lx596_111
.Lx596_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx596_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx596_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx596_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
endswith_omega:         mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx596_145
                        mov              eax, 32
.Lx596_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax                      # endswith
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rcx, 0;                    jbe   .Lx596_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx596_180
.Lx596_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx596_180:             cmp              rcx, 1;                    jbe   .Lx596_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax; jmp   .Lx596_181
.Lx596_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx596_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx596_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx596_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n597_save_restore_α:                                                jmp   n598_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n598_goto_deferred_α:   lea              rax, [rip + proc_LBL__index_α]; jmp   rax
                                                                    jmp   n599_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n599_save_restore_α:
index_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx605_2
                        mov              eax, 32
.Lx605_2:               sub              rsp, 96
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
                        cmp              rcx, 0;                    jbe   .Lx605_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx605_41
.Lx605_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx605_41:              cmp              rcx, 1;                    jbe   .Lx605_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx605_42
.Lx605_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx605_42:              lea              r10, [rip + index_gamma]
                        lea              r11, [rip + index_omega]
                        lea              rax, [rip + proc_LBL__index_α]; jmp   rax
index_gamma:            mov              rdi, qword ptr [r9 + 272]                      # index
                        mov              rsi, qword ptr [r9 + 280]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx605_75
                        mov              eax, 32
.Lx605_75:              lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 272], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 296], rax
                        cmp              rcx, 0;                    jbe   .Lx605_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx605_110
.Lx605_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx605_110:             cmp              rcx, 1;                    jbe   .Lx605_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax; jmp   .Lx605_111
.Lx605_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx605_111:             mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                    jbe   .Lx605_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx605_3:               mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
index_omega:            mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx605_145
                        mov              eax, 32
.Lx605_145:             lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 296], rax
                        cmp              rcx, 0;                    jbe   .Lx605_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx605_180
.Lx605_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx605_180:             cmp              rcx, 1;                    jbe   .Lx605_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax; jmp   .Lx605_181
.Lx605_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx605_181:             mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                    jbe   .Lx605_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx605_4:               mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
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
n606_statement_begin_α:                                             jmp   n607_statement_end_α
n606_statement_begin_β:                                             jmp   n608_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n607_statement_end_α:                                               jmp   n608_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'endswith ok'          :(ew1)
#-----------------------------------------------------------------------------------------------------------------------
n608_statement_begin_α:                                             jmp   n609_func_activate_α
n608_statement_begin_β:                                             jmp   n611_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n609_func_activate_α:   mov              rdi, qword ptr [rip + .Lx840_0]
                        mov              rsi, qword ptr [rip + .Lx840_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n610_statement_end_α
n609_func_activate_β:                                               jmp   n608_statement_begin_β
.Lx840_0:               .quad            .Lx840_0_s
.Lx840_0_s:             .string          "pad_left"
.Lx840_1:               .quad            .Lx840_1_s
.Lx840_1_s:             .string          "s,n,c"
#-----------------------------------------------------------------------------------------------------------------------
n610_statement_end_α:                                               jmp   n611_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n611_statement_begin_α:                                             jmp   n612_statement_end_α
n611_statement_begin_β:                                             jmp   n613_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n612_statement_end_α:                                               jmp   n613_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n613_statement_begin_α:                                             jmp   n614_func_activate_α
n613_statement_begin_β:                                             jmp   n616_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n614_func_activate_α:   mov              rdi, qword ptr [rip + .Lx850_0]
                        mov              rsi, qword ptr [rip + .Lx850_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n615_statement_end_α
n614_func_activate_β:                                               jmp   n613_statement_begin_β
.Lx850_0:               .quad            .Lx850_0_s
.Lx850_0_s:             .string          "pad_right"
.Lx850_1:               .quad            .Lx850_1_s
.Lx850_1_s:             .string          "s,n,c"
#-----------------------------------------------------------------------------------------------------------------------
n615_statement_end_α:                                               jmp   n616_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n616_statement_begin_α:                                             jmp   n617_statement_end_α
n616_statement_begin_β:                                             jmp   n618_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n617_statement_end_α:                                               jmp   n618_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n618_statement_begin_α:                                             jmp   n619_func_activate_α
n618_statement_begin_β:                                             jmp   n621_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n619_func_activate_α:   mov              rdi, qword ptr [rip + .Lx860_0]
                        mov              rsi, qword ptr [rip + .Lx860_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n620_statement_end_α
n619_func_activate_β:                                               jmp   n618_statement_begin_β
.Lx860_0:               .quad            .Lx860_0_s
.Lx860_0_s:             .string          "ltrim"
.Lx860_1:               .quad            .Lx860_1_s
.Lx860_1_s:             .string          "s,ws,r"
#-----------------------------------------------------------------------------------------------------------------------
n620_statement_end_α:                                               jmp   n621_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n621_statement_begin_α:                                             jmp   n622_statement_end_α
n621_statement_begin_β:                                             jmp   n623_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n622_statement_end_α:                                               jmp   n623_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n623_statement_begin_α:                                             jmp   n624_func_activate_α
n623_statement_begin_β:                                             jmp   n626_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n624_func_activate_α:   mov              rdi, qword ptr [rip + .Lx870_0]
                        mov              rsi, qword ptr [rip + .Lx870_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n625_statement_end_α
n624_func_activate_β:                                               jmp   n623_statement_begin_β
.Lx870_0:               .quad            .Lx870_0_s
.Lx870_0_s:             .string          "rtrim"
.Lx870_1:               .quad            .Lx870_1_s
.Lx870_1_s:             .string          "s,ws,i,ch"
#-----------------------------------------------------------------------------------------------------------------------
n625_statement_end_α:                                               jmp   n626_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n626_statement_begin_α:                                             jmp   n627_statement_end_α
n626_statement_begin_β:                                             jmp   n628_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n627_statement_end_α:                                               jmp   n628_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n628_statement_begin_α:                                             jmp   n629_func_activate_α
n628_statement_begin_β:                                             jmp   n631_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n629_func_activate_α:   mov              rdi, qword ptr [rip + .Lx880_0]
                        mov              rsi, qword ptr [rip + .Lx880_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n630_statement_end_α
n629_func_activate_β:                                               jmp   n628_statement_begin_β
.Lx880_0:               .quad            .Lx880_0_s
.Lx880_0_s:             .string          "trimws"
.Lx880_1:               .quad            .Lx880_1_s
.Lx880_1_s:             .string          "s"
#-----------------------------------------------------------------------------------------------------------------------
n630_statement_end_α:                                               jmp   n631_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n631_statement_begin_α:                                             jmp   n632_statement_end_α
n631_statement_begin_β:                                             jmp   n633_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n632_statement_end_α:                                               jmp   n633_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n633_statement_begin_α:                                             jmp   n634_func_activate_α
n633_statement_begin_β:                                             jmp   n636_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n634_func_activate_α:   mov              rdi, qword ptr [rip + .Lx890_0]
                        mov              rsi, qword ptr [rip + .Lx890_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n635_statement_end_α
n634_func_activate_β:                                               jmp   n633_statement_begin_β
.Lx890_0:               .quad            .Lx890_0_s
.Lx890_0_s:             .string          "repeat"
.Lx890_1:               .quad            .Lx890_1_s
.Lx890_1_s:             .string          "s,n"
#-----------------------------------------------------------------------------------------------------------------------
n635_statement_end_α:                                               jmp   n636_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n636_statement_begin_α:                                             jmp   n637_statement_end_α
n636_statement_begin_β:                                             jmp   n638_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n637_statement_end_α:                                               jmp   n638_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n638_statement_begin_α:                                             jmp   n639_func_activate_α
n638_statement_begin_β:                                             jmp   n641_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n639_func_activate_α:   mov              rdi, qword ptr [rip + .Lx900_0]
                        mov              rsi, qword ptr [rip + .Lx900_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n640_statement_end_α
n639_func_activate_β:                                               jmp   n638_statement_begin_β
.Lx900_0:               .quad            .Lx900_0_s
.Lx900_0_s:             .string          "contains"
.Lx900_1:               .quad            .Lx900_1_s
.Lx900_1_s:             .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n640_statement_end_α:                                               jmp   n641_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n641_statement_begin_α:                                             jmp   n642_statement_end_α
n641_statement_begin_β:                                             jmp   n643_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n642_statement_end_α:                                               jmp   n643_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n643_statement_begin_α:                                             jmp   n644_func_activate_α
n643_statement_begin_β:                                             jmp   n646_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n644_func_activate_α:   mov              rdi, qword ptr [rip + .Lx910_0]
                        mov              rsi, qword ptr [rip + .Lx910_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n645_statement_end_α
n644_func_activate_β:                                               jmp   n643_statement_begin_β
.Lx910_0:               .quad            .Lx910_0_s
.Lx910_0_s:             .string          "startswith"
.Lx910_1:               .quad            .Lx910_1_s
.Lx910_1_s:             .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n645_statement_end_α:                                               jmp   n646_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n646_statement_begin_α:                                             jmp   n647_statement_end_α
n646_statement_begin_β:                                             jmp   n648_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n647_statement_end_α:                                               jmp   n648_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n648_statement_begin_α:                                             jmp   n649_func_activate_α
n648_statement_begin_β:                                             jmp   n651_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n649_func_activate_α:   mov              rdi, qword ptr [rip + .Lx920_0]
                        mov              rsi, qword ptr [rip + .Lx920_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n650_statement_end_α
n649_func_activate_β:                                               jmp   n648_statement_begin_β
.Lx920_0:               .quad            .Lx920_0_s
.Lx920_0_s:             .string          "endswith"
.Lx920_1:               .quad            .Lx920_1_s
.Lx920_1_s:             .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n650_statement_end_α:                                               jmp   n651_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n651_statement_begin_α:                                             jmp   n652_statement_end_α
n651_statement_begin_β:                                             jmp   n653_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n652_statement_end_α:                                               jmp   n653_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n653_statement_begin_α:                                             jmp   n654_func_activate_α
n653_statement_begin_β:                                             jmp   n656_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n654_func_activate_α:   mov              rdi, qword ptr [rip + .Lx930_0]
                        mov              rsi, qword ptr [rip + .Lx930_1]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n655_statement_end_α
n654_func_activate_β:                                               jmp   n653_statement_begin_β
.Lx930_0:               .quad            .Lx930_0_s
.Lx930_0_s:             .string          "index"
.Lx930_1:               .quad            .Lx930_1_s
.Lx930_1_s:             .string          "s,t,ix"
#-----------------------------------------------------------------------------------------------------------------------
n655_statement_end_α:                                               jmp   n656_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n656_statement_begin_α:                                             jmp   n657_statement_end_α
n656_statement_begin_β:                                             jmp   n658_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n657_statement_end_α:                                               jmp   n658_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n658_statement_begin_α:                                             jmp   n659_statement_end_α
n658_statement_begin_β:                                             jmp   n660_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n659_statement_end_α:                                               jmp   n660_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n660_statement_begin_α:                                             jmp   n661_lit_string_α
n660_statement_begin_β:                                             jmp   n665_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n661_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx943_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n662_lit_integer_α
.Lx943_0:               .quad            .Lx943_0_s
.Lx943_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n662_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx944_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n663_call_α
n662_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n660_statement_begin_β
.Lx944_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n663_call_α:            sub              rsp, 16
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
.Lrkfnzd946:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd946]
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
                        cmp              eax, 104;                  jne   .Lx945_240
                        add              rsp, 16;                   jmp   n662_lit_integer_β
.Lx945_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n664_statement_end_α
n663_call_β:            add              rsp, 16;                   jmp   n662_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n664_statement_end_α:                                               jmp   n665_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n665_statement_begin_α:                                             jmp   n666_statement_end_α
n665_statement_begin_β:                                             jmp   n667_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n666_statement_end_α:                                               jmp   n667_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n667_statement_begin_α:                                             jmp   n668_lit_string_α
n667_statement_begin_β:                                             jmp   n674_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n668_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx955_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n669_lit_integer_α
n668_lit_string_β:      add              rsp, 16
                        add              rsp, 48;                   jmp   n667_statement_begin_β
.Lx955_0:               .quad            .Lx955_0_s
.Lx955_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n669_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx956_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n670_lit_string_α
n669_lit_integer_β:     add              rsp, 16;                   jmp   n668_lit_string_β
.Lx956_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n670_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx957_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n671_call_α
n670_lit_string_β:      add              rsp, 16;                   jmp   n669_lit_integer_β
.Lx957_0:               .quad            .Lx957_0_s
.Lx957_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n671_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx959_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + pad_left_alpha]; jmp   rax
.Lx959_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx959_240
                        add              rsp, 16;                   jmp   n670_lit_string_β
.Lx959_240:                                                         jmp   n672_assign_α
n671_call_β:                                                        jmp   n670_lit_string_β
.Lx959_0:               .quad            .Lx959_0_s
.Lx959_0_s:             .string          "pad_left"
#-----------------------------------------------------------------------------------------------------------------------
n672_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx960_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n673_statement_end_α
.Lx960_0:               .quad            .Lx960_0_s
.Lx960_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n673_statement_end_α:                                               jmp   n674_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n674_statement_begin_α:                                             jmp   n675_lit_string_α
n674_statement_begin_β:                                             jmp   n681_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n675_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx965_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n676_lit_integer_α
n675_lit_string_β:      add              rsp, 16
                        add              rsp, 112;                  jmp   n674_statement_begin_β
.Lx965_0:               .quad            .Lx965_0_s
.Lx965_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n676_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx966_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n677_lit_string_α
n676_lit_integer_β:     add              rsp, 16;                   jmp   n675_lit_string_β
.Lx966_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n677_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx967_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n678_call_α
n677_lit_string_β:      add              rsp, 16;                   jmp   n676_lit_integer_β
.Lx967_0:               .quad            .Lx967_0_s
.Lx967_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n678_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx969_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + pad_right_alpha]; jmp   rax
.Lx969_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx969_240
                        add              rsp, 16;                   jmp   n677_lit_string_β
.Lx969_240:                                                         jmp   n679_assign_α
n678_call_β:                                                        jmp   n677_lit_string_β
.Lx969_0:               .quad            .Lx969_0_s
.Lx969_0_s:             .string          "pad_right"
#-----------------------------------------------------------------------------------------------------------------------
n679_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n680_statement_end_α
.Lx970_0:               .quad            .Lx970_0_s
.Lx970_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n680_statement_end_α:                                               jmp   n681_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n681_statement_begin_α:                                             jmp   n682_lit_string_α
n681_statement_begin_β:                                             jmp   n686_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n682_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx975_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n683_call_α
n682_lit_string_β:      add              rsp, 16
                        add              rsp, 176;                  jmp   n681_statement_begin_β
.Lx975_0:               .quad            .Lx975_0_s
.Lx975_0_s:             .string          "   hello"
#-----------------------------------------------------------------------------------------------------------------------
n683_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx977_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ltrim_alpha];  jmp   rax
.Lx977_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx977_240
                        add              rsp, 16;                   jmp   n682_lit_string_β
.Lx977_240:                                                         jmp   n684_assign_α
n683_call_β:                                                        jmp   n682_lit_string_β
.Lx977_0:               .quad            .Lx977_0_s
.Lx977_0_s:             .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n684_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n685_statement_end_α
.Lx978_0:               .quad            .Lx978_0_s
.Lx978_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n685_statement_end_α:                                               jmp   n686_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n686_statement_begin_α:                                             jmp   n687_lit_string_α
n686_statement_begin_β:                                             jmp   n691_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n687_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx983_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n688_call_α
n687_lit_string_β:      add              rsp, 16
                        add              rsp, 208;                  jmp   n686_statement_begin_β
.Lx983_0:               .quad            .Lx983_0_s
.Lx983_0_s:             .string          "hello   "
#-----------------------------------------------------------------------------------------------------------------------
n688_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx985_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + rtrim_alpha];  jmp   rax
.Lx985_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx985_240
                        add              rsp, 16;                   jmp   n687_lit_string_β
.Lx985_240:                                                         jmp   n689_assign_α
n688_call_β:                                                        jmp   n687_lit_string_β
.Lx985_0:               .quad            .Lx985_0_s
.Lx985_0_s:             .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n689_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx986_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n690_statement_end_α
.Lx986_0:               .quad            .Lx986_0_s
.Lx986_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n690_statement_end_α:                                               jmp   n691_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n691_statement_begin_α:                                             jmp   n692_lit_string_α
n691_statement_begin_β:                                             jmp   n696_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n692_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 9
                        mov              rax, qword ptr [rip + .Lx991_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n693_call_α
n692_lit_string_β:      add              rsp, 16
                        add              rsp, 240;                  jmp   n691_statement_begin_β
.Lx991_0:               .quad            .Lx991_0_s
.Lx991_0_s:             .string          "  hello  "
#-----------------------------------------------------------------------------------------------------------------------
n693_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx993_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + trimws_alpha]; jmp   rax
.Lx993_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx993_240
                        add              rsp, 16;                   jmp   n692_lit_string_β
.Lx993_240:                                                         jmp   n694_assign_α
n693_call_β:                                                        jmp   n692_lit_string_β
.Lx993_0:               .quad            .Lx993_0_s
.Lx993_0_s:             .string          "trimws"
#-----------------------------------------------------------------------------------------------------------------------
n694_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx994_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n695_statement_end_α
.Lx994_0:               .quad            .Lx994_0_s
.Lx994_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n695_statement_end_α:                                               jmp   n696_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n696_statement_begin_α:                                             jmp   n697_lit_string_α
n696_statement_begin_β:                                             jmp   n702_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n697_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx999_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n698_lit_integer_α
n697_lit_string_β:      add              rsp, 16
                        add              rsp, 272;                  jmp   n696_statement_begin_β
.Lx999_0:               .quad            .Lx999_0_s
.Lx999_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n698_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1000_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n699_call_α
n698_lit_integer_β:     add              rsp, 16;                   jmp   n697_lit_string_β
.Lx1000_0:              .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n699_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx1002_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + repeat_alpha]; jmp   rax
.Lx1002_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx1002_240
                        add              rsp, 16;                   jmp   n698_lit_integer_β
.Lx1002_240:                                                        jmp   n700_assign_α
n699_call_β:                                                        jmp   n698_lit_integer_β
.Lx1002_0:              .quad            .Lx1002_0_s
.Lx1002_0_s:            .string          "repeat"
#-----------------------------------------------------------------------------------------------------------------------
n700_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1003_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n701_statement_end_α
.Lx1003_0:              .quad            .Lx1003_0_s
.Lx1003_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n701_statement_end_α:                                               jmp   n702_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n702_statement_begin_α:                                             jmp   n703_statement_end_α
n702_statement_begin_β:                                             jmp   n704_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n703_statement_end_α:                                               jmp   n704_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n704_statement_begin_α:                                             jmp   n705_lit_string_α
n704_statement_begin_β: add              rsp, 320;                  jmp   n713_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n705_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1012_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n706_lit_string_α
n705_lit_string_β:      add              rsp, 16
                        add              rsp, 320;                  jmp   n704_statement_begin_β
.Lx1012_0:              .quad            .Lx1012_0_s
.Lx1012_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n706_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1013_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n707_call_α
n706_lit_string_β:      add              rsp, 16;                   jmp   n705_lit_string_β
.Lx1013_0:              .quad            .Lx1013_0_s
.Lx1013_0_s:            .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n707_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx1015_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + contains_alpha]; jmp   rax
.Lx1015_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx1015_240
                        add              rsp, 16;                   jmp   n706_lit_string_β
.Lx1015_240:                                                        jmp   n708_statement_end_α
n707_call_β:                                                        jmp   n706_lit_string_β
.Lx1015_0:              .quad            .Lx1015_0_s
.Lx1015_0_s:            .string          "contains"
#-----------------------------------------------------------------------------------------------------------------------
n708_statement_end_α:                                               jmp   n709_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n709_statement_begin_α:                                             jmp   n710_lit_string_α
n709_statement_begin_β:                                             jmp   n717_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n710_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx1020_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n711_assign_α
.Lx1020_0:              .quad            .Lx1020_0_s
.Lx1020_0_s:            .string          "contains ok"
#-----------------------------------------------------------------------------------------------------------------------
n711_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1021_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n712_statement_end_α
.Lx1021_0:              .quad            .Lx1021_0_s
.Lx1021_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n712_statement_end_α:                                               jmp   n717_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n713_statement_begin_α:                                             jmp   n714_lit_string_α
n713_statement_begin_β:                                             jmp   n717_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n714_lit_string_α:      mov              qword ptr [rsp + 4224], 2                      # result
                        mov              dword ptr [rsp + 4228], 14
                        mov              rax, qword ptr [rip + .Lx1026_0]
                        mov              qword ptr [rsp + 4232], rax; jmp   n715_assign_α
.Lx1026_0:              .quad            .Lx1026_0_s
.Lx1026_0_s:            .string          "FAIL: contains"
#-----------------------------------------------------------------------------------------------------------------------
n715_assign_α:          mov              rsi, qword ptr [rsp + 4224]
                        mov              rdx, qword ptr [rsp + 4232]
                        mov              rdi, qword ptr [rip + .Lx1027_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n716_statement_end_α
.Lx1027_0:              .quad            .Lx1027_0_s
.Lx1027_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n716_statement_end_α:                                               jmp   n717_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n717_statement_begin_α:                                             jmp   n718_statement_end_α
n717_statement_begin_β:                                             jmp   n719_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n718_statement_end_α:                                               jmp   n719_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n719_statement_begin_α:                                             jmp   n720_lit_string_α
n719_statement_begin_β: add              rsp, 384;                  jmp   n728_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n720_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1036_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n721_lit_string_α
n720_lit_string_β:      add              rsp, 16
                        add              rsp, 384;                  jmp   n719_statement_begin_β
.Lx1036_0:              .quad            .Lx1036_0_s
.Lx1036_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n721_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1037_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n722_call_α
n721_lit_string_β:      add              rsp, 16;                   jmp   n720_lit_string_β
.Lx1037_0:              .quad            .Lx1037_0_s
.Lx1037_0_s:            .string          "foo"
#-----------------------------------------------------------------------------------------------------------------------
n722_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx1039_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + startswith_alpha]; jmp   rax
.Lx1039_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx1039_240
                        add              rsp, 16;                   jmp   n721_lit_string_β
.Lx1039_240:                                                        jmp   n723_statement_end_α
n722_call_β:                                                        jmp   n721_lit_string_β
.Lx1039_0:              .quad            .Lx1039_0_s
.Lx1039_0_s:            .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n723_statement_end_α:                                               jmp   n724_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n724_statement_begin_α:                                             jmp   n725_lit_string_α
n724_statement_begin_β:                                             jmp   n732_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n725_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx1044_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n726_assign_α
.Lx1044_0:              .quad            .Lx1044_0_s
.Lx1044_0_s:            .string          "startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n726_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1045_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n727_statement_end_α
.Lx1045_0:              .quad            .Lx1045_0_s
.Lx1045_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n727_statement_end_α:                                               jmp   n732_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n728_statement_begin_α:                                             jmp   n729_lit_string_α
n728_statement_begin_β:                                             jmp   n732_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n729_lit_string_α:      mov              qword ptr [rsp + 4352], 2                      # result
                        mov              dword ptr [rsp + 4356], 16
                        mov              rax, qword ptr [rip + .Lx1050_0]
                        mov              qword ptr [rsp + 4360], rax; jmp   n730_assign_α
.Lx1050_0:              .quad            .Lx1050_0_s
.Lx1050_0_s:            .string          "FAIL: startswith"
#-----------------------------------------------------------------------------------------------------------------------
n730_assign_α:          mov              rsi, qword ptr [rsp + 4352]
                        mov              rdx, qword ptr [rsp + 4360]
                        mov              rdi, qword ptr [rip + .Lx1051_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n731_statement_end_α
.Lx1051_0:              .quad            .Lx1051_0_s
.Lx1051_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n731_statement_end_α:                                               jmp   n732_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n732_statement_begin_α:                                             jmp   n733_statement_end_α
n732_statement_begin_β:                                             jmp   n734_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n733_statement_end_α:                                               jmp   n734_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n734_statement_begin_α:                                             jmp   n735_lit_string_α
n734_statement_begin_β: add              rsp, 448;                  jmp   n743_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n735_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1060_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n736_lit_string_α
n735_lit_string_β:      add              rsp, 16
                        add              rsp, 448;                  jmp   n734_statement_begin_β
.Lx1060_0:              .quad            .Lx1060_0_s
.Lx1060_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n736_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1061_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n737_call_α
n736_lit_string_β:      add              rsp, 16;                   jmp   n735_lit_string_β
.Lx1061_0:              .quad            .Lx1061_0_s
.Lx1061_0_s:            .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n737_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx1063_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + endswith_alpha]; jmp   rax
.Lx1063_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx1063_240
                        add              rsp, 16;                   jmp   n736_lit_string_β
.Lx1063_240:                                                        jmp   n738_statement_end_α
n737_call_β:                                                        jmp   n736_lit_string_β
.Lx1063_0:              .quad            .Lx1063_0_s
.Lx1063_0_s:            .string          "endswith"
#-----------------------------------------------------------------------------------------------------------------------
n738_statement_end_α:                                               jmp   n739_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n739_statement_begin_α:                                             jmp   n740_lit_string_α
n739_statement_begin_β:                                             jmp   n747_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n740_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx1068_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n741_assign_α
.Lx1068_0:              .quad            .Lx1068_0_s
.Lx1068_0_s:            .string          "endswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n741_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1069_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n742_statement_end_α
.Lx1069_0:              .quad            .Lx1069_0_s
.Lx1069_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n742_statement_end_α:                                               jmp   n747_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n743_statement_begin_α:                                             jmp   n744_lit_string_α
n743_statement_begin_β:                                             jmp   n747_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n744_lit_string_α:      mov              qword ptr [rsp + 4480], 2                      # result
                        mov              dword ptr [rsp + 4484], 14
                        mov              rax, qword ptr [rip + .Lx1074_0]
                        mov              qword ptr [rsp + 4488], rax; jmp   n745_assign_α
.Lx1074_0:              .quad            .Lx1074_0_s
.Lx1074_0_s:            .string          "FAIL: endswith"
#-----------------------------------------------------------------------------------------------------------------------
n745_assign_α:          mov              rsi, qword ptr [rsp + 4480]
                        mov              rdx, qword ptr [rsp + 4488]
                        mov              rdi, qword ptr [rip + .Lx1075_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n746_statement_end_α
.Lx1075_0:              .quad            .Lx1075_0_s
.Lx1075_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n746_statement_end_α:                                               jmp   n747_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n747_statement_begin_α:                                             jmp   n748_statement_end_α
n747_statement_begin_β:                                             jmp   n749_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n748_statement_end_α:                                               jmp   n749_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n749_statement_begin_α:                                             jmp   n750_lit_string_α
n749_statement_begin_β: add              rsp, 512;                  jmp   n758_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n750_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1084_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n751_lit_string_α
n750_lit_string_β:      add              rsp, 16
                        add              rsp, 512;                  jmp   n749_statement_begin_β
.Lx1084_0:              .quad            .Lx1084_0_s
.Lx1084_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n751_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1085_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n752_call_α
n751_lit_string_β:      add              rsp, 16;                   jmp   n750_lit_string_β
.Lx1085_0:              .quad            .Lx1085_0_s
.Lx1085_0_s:            .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n752_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx1087_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + startswith_alpha]; jmp   rax
.Lx1087_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx1087_240
                        add              rsp, 16;                   jmp   n751_lit_string_β
.Lx1087_240:                                                        jmp   n753_statement_end_α
n752_call_β:                                                        jmp   n751_lit_string_β
.Lx1087_0:              .quad            .Lx1087_0_s
.Lx1087_0_s:            .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n753_statement_end_α:                                               jmp   n754_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n754_statement_begin_α:                                             jmp   n755_lit_string_α
n754_statement_begin_β:                                             jmp   n762_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n755_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx1092_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n756_assign_α
.Lx1092_0:              .quad            .Lx1092_0_s
.Lx1092_0_s:            .string          "FAIL: startswith matched wrong"
#-----------------------------------------------------------------------------------------------------------------------
n756_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1093_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n757_statement_end_α
.Lx1093_0:              .quad            .Lx1093_0_s
.Lx1093_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n757_statement_end_α:                                               jmp   n762_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n758_statement_begin_α:                                             jmp   n759_lit_string_α
n758_statement_begin_β:                                             jmp   n762_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n759_lit_string_α:      mov              qword ptr [rsp + 4592], 2                      # result
                        mov              dword ptr [rsp + 4596], 16
                        mov              rax, qword ptr [rip + .Lx1098_0]
                        mov              qword ptr [rsp + 4600], rax; jmp   n760_assign_α
.Lx1098_0:              .quad            .Lx1098_0_s
.Lx1098_0_s:            .string          "no startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n760_assign_α:          mov              rsi, qword ptr [rsp + 4592]
                        mov              rdx, qword ptr [rsp + 4600]
                        mov              rdi, qword ptr [rip + .Lx1099_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n761_statement_end_α
.Lx1099_0:              .quad            .Lx1099_0_s
.Lx1099_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n761_statement_end_α:                                               jmp   n762_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n762_statement_begin_α:                                             jmp   n763_statement_end_α
n762_statement_begin_β:                                             jmp   n764_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n763_statement_end_α:                                               jmp   n764_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n764_statement_begin_α:                                             jmp   n765_lit_string_α
n764_statement_begin_β:                                             jmp   n770_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n765_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1108_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n766_lit_string_α
n765_lit_string_β:      add              rsp, 16
                        add              rsp, 576;                  jmp   n764_statement_begin_β
.Lx1108_0:              .quad            .Lx1108_0_s
.Lx1108_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n766_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1109_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n767_call_α
n766_lit_string_β:      add              rsp, 16;                   jmp   n765_lit_string_β
.Lx1109_0:              .quad            .Lx1109_0_s
.Lx1109_0_s:            .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n767_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx1111_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + index_alpha];  jmp   rax
.Lx1111_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx1111_240
                        add              rsp, 16;                   jmp   n766_lit_string_β
.Lx1111_240:                                                        jmp   n768_assign_α
n767_call_β:                                                        jmp   n766_lit_string_β
.Lx1111_0:              .quad            .Lx1111_0_s
.Lx1111_0_s:            .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n768_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1112_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n769_statement_end_α
.Lx1112_0:              .quad            .Lx1112_0_s
.Lx1112_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n769_statement_end_α:                                               jmp   n770_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n770_statement_begin_α:                                             jmp   n771_lit_string_α
n770_statement_begin_β: add              rsp, 624;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n771_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1117_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n772_lit_string_α
n771_lit_string_β:      add              rsp, 16
                        add              rsp, 624;                  jmp   n770_statement_begin_β
.Lx1117_0:              .quad            .Lx1117_0_s
.Lx1117_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n772_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1118_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n773_call_α
n772_lit_string_β:      add              rsp, 16;                   jmp   n771_lit_string_β
.Lx1118_0:              .quad            .Lx1118_0_s
.Lx1118_0_s:            .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
n773_call_α:            sub              rsp, 16
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
                        lea              rax, [rip + .Lx1120_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + index_alpha];  jmp   rax
.Lx1120_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx1120_240
                        add              rsp, 16;                   jmp   n772_lit_string_β
.Lx1120_240:                                                        jmp   n774_assign_α
n773_call_β:                                                        jmp   n772_lit_string_β
.Lx1120_0:              .quad            .Lx1120_0_s
.Lx1120_0_s:            .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n774_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1121_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n775_statement_end_α
.Lx1121_0:              .quad            .Lx1121_0_s
.Lx1121_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n775_statement_end_α:   add              rsp, 672;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n776_goto_α:                                                        jmp   n611_statement_begin_α
n776_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n777_goto_α:                                                        jmp   n616_statement_begin_α
n777_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n778_goto_α:                                                        jmp   n621_statement_begin_α
n778_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n779_goto_α:                                                        jmp   n780_statement_begin_α
n779_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n780_statement_begin_α:                                             jmp   n781_var_α
n780_statement_begin_β:                                             jmp   n787_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n781_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1920], rax                    # result
                        mov              qword ptr [rsp + 1928], rdx; jmp   n782_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n782_lit_integer_α:     mov              qword ptr [rsp + 1904], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1131_0]
                        mov              qword ptr [rsp + 1912], rax; jmp   n783_coerce_numeric_α
.Lx1131_0:              .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n783_coerce_numeric_α:  mov              eax, dword ptr [rsp + 1920]
                        cmp              eax, 5;                    je    .Lx1133_1
                        cmp              eax, 3;                    jne   .Lx1133_0
                        mov              eax, dword ptr [rsp + 1904]
                        cmp              eax, 3;                    jne   .Lx1133_0
.Lx1133_1:              mov              rax, qword ptr [rsp + 1920]
                        mov              qword ptr [rsp + 1888], rax
                        mov              rax, qword ptr [rsp + 1928]
                        mov              qword ptr [rsp + 1896], rax; jmp   n784_coerce_numeric_α
.Lx1133_0:              lea              rdi, [rsp + 1920]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n784_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n784_coerce_numeric_α:  mov              eax, dword ptr [rsp + 1904]
                        cmp              eax, 5;                    je    .Lx1135_1
                        cmp              eax, 3;                    jne   .Lx1135_0
                        mov              eax, dword ptr [rsp + 1920]
                        cmp              eax, 3;                    jne   .Lx1135_0
.Lx1135_1:              mov              rax, qword ptr [rsp + 1904]
                        mov              qword ptr [rsp + 1872], rax
                        mov              rax, qword ptr [rsp + 1912]
                        mov              qword ptr [rsp + 1880], rax; jmp   n785_cmp_test_α
.Lx1135_0:              lea              rdi, [rsp + 1904]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n785_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n785_cmp_test_α:        lea              rdi, [rsp + 1888]
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
                        test             eax, eax;                  jg    n780_statement_begin_β
                                                                    jmp   n786_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n786_statement_end_α:                                               jmp   n808_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n787_statement_begin_α:                                             jmp   n788_var_α
n787_statement_begin_β:                                             jmp   n794_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n788_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2016], rax                    # result
                        mov              qword ptr [rsp + 2024], rdx; jmp   n789_var_α
#-----------------------------------------------------------------------------------------------------------------------
n789_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2032], rax                    # result
                        mov              qword ptr [rsp + 2040], rdx; jmp   n790_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n790_lit_integer_α:     mov              qword ptr [rsp + 2048], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1144_0]
                        mov              qword ptr [rsp + 2056], rax; jmp   n791_call_α
.Lx1144_0:              .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n791_call_α:            mov              rax, qword ptr [rsp + 2048]
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
.Lrkfn1146:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1146]
                        lea              rsi, [rsp + 1952]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                        cmp              eax, 104;                  je    n787_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n792_assign_α
n791_call_β:                                                        jmp   n787_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n792_assign_α:          mov              rax, qword ptr [rsp + 1936]
                        mov              rdx, qword ptr [rsp + 1944]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              qword ptr [r9 + 168], rdx; jmp   n793_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n793_statement_end_α:                                               jmp   n794_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n794_statement_begin_α:                                             jmp   n795_var_α
n794_statement_begin_β:                                             jmp   n808_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n795_var_α:             mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 2192], rax                    # result
                        mov              qword ptr [rsp + 2200], rdx; jmp   n796_var_α
#-----------------------------------------------------------------------------------------------------------------------
n796_var_α:             mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 2176], rax                    # result
                        mov              qword ptr [rsp + 2184], rdx; jmp   n797_coerce_string_α
#-----------------------------------------------------------------------------------------------------------------------
n797_coerce_string_α:   lea              rdi, [rsp + 2176]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n798_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n798_match_begin_α:     mov              qword ptr [rsp + 48], r13                      # outer_Σ
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
.Lx1157_0:              mov              r14d, dword ptr [rsp + 0]; jmp   n799_match_any_α
n798_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx1157_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx1157_1
                                                                    jmp   .Lx1157_0
.Lx1157_1:
n798_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n808_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n799_match_any_α:       mov              eax, r14d
                        cmp              eax, r15d;                 jge   n798_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rsp + 2168]
                        mov              r9d, dword ptr [rsp + 2164]
                        mov              edx, 0
.Lx1159_5:              cmp              edx, r9d;                  jge   n798_match_begin_β
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                  je    .Lx1159_6
                        add              edx, 1;                    jmp   .Lx1159_5
.Lx1159_6:              add              r14d, 1;                   jmp   n800_match_end_α
n799_match_any_β:       sub              r14d, 1;                   jmp   n798_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n800_match_end_α:       mov              r8, r12
.Lx1161_9:              sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx1161_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx1161_5:              sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx1161_5
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
.Lx1161_1:              test             rax, rax;                  je    .Lx1161_2
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
                        lea              rcx, [rip + .Lx1161_3]
                        lea              rdx, [rip + .Lx1161_4];    jmp   rax
.Lx1161_3:              mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx1161_1
.Lx1161_4:              mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx1161_1
.Lx1161_2:              mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx1161_10:             sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx1161_10
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n801_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n801_statement_end_α:                                               jmp   n802_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n802_statement_begin_α:                                             jmp   n803_var_α
n802_statement_begin_β:                                             jmp   n780_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n803_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2224], rax                    # result
                        mov              qword ptr [rsp + 2232], rdx; jmp   n804_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n804_lit_integer_α:     mov              qword ptr [rsp + 2240], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1167_0]
                        mov              qword ptr [rsp + 2248], rax; jmp   n805_binop_α
.Lx1167_0:              .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n805_binop_α:           mov              eax, dword ptr [rsp + 2224]
                        cmp              eax, 3;                    jne   .Lx1168_0
                        mov              rax, qword ptr [rsp + 2232]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2208], 3
                        mov              qword ptr [rsp + 2216], rax; jmp   n806_assign_α
.Lx1168_0:              mov              rdi, qword ptr [rsp + 2224]
                        mov              rsi, qword ptr [rsp + 2232]
                        mov              rdx, qword ptr [rsp + 2240]
                        mov              rcx, qword ptr [rsp + 2248]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  jne   .Lx1168_240
                        add              rsp, 32;                   jmp   n802_statement_begin_β
.Lx1168_240:            mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n806_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n806_assign_α:          mov              rax, qword ptr [rsp + 2208]
                        mov              rdx, qword ptr [rsp + 2216]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx; jmp   n807_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n807_statement_end_α:                                               jmp   n780_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n808_statement_begin_α:                                             jmp   n809_var_α
n808_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n809_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2336], rax                    # result
                        mov              qword ptr [rsp + 2344], rdx; jmp   n810_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n810_lit_integer_α:     mov              qword ptr [rsp + 2352], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1175_0]
                        mov              qword ptr [rsp + 2360], rax; jmp   n811_var_α
.Lx1175_0:              .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n811_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2368], rax                    # result
                        mov              qword ptr [rsp + 2376], rdx; jmp   n812_call_α
#-----------------------------------------------------------------------------------------------------------------------
n812_call_α:            mov              rax, qword ptr [rsp + 2368]
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
.Lrkfn1178:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1178]
                        lea              rsi, [rsp + 2272]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2256], rax
                        mov              qword ptr [rsp + 2264], rdx
                        cmp              eax, 104;                  je    n808_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n813_assign_α
n812_call_β:                                                        jmp   n808_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n813_assign_α:          mov              rax, qword ptr [rsp + 2256]
                        mov              rdx, qword ptr [rsp + 2264]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              qword ptr [r9 + 136], rdx; jmp   n814_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n814_statement_end_α:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n815_goto_α:                                                        jmp   n808_statement_begin_α
n815_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n816_goto_α:                                                        jmp   n626_statement_begin_α
n816_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n817_goto_α:                                                        jmp   n631_statement_begin_α
n817_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n818_goto_α:                                                        jmp   n636_statement_begin_α
n818_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n819_goto_α:                                                        jmp   n641_statement_begin_α
n819_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n820_goto_α:                                                        jmp   n646_statement_begin_α
n820_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n821_goto_α:                                                        jmp   n651_statement_begin_α
n821_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n822_goto_α:                                                        jmp   n656_statement_begin_α
n822_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n823_goto_α:                                                        jmp   n713_statement_begin_α
n823_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n824_goto_α:                                                        jmp   n717_statement_begin_α
n824_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n825_goto_α:                                                        jmp   n728_statement_begin_α
n825_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n826_goto_α:                                                        jmp   n732_statement_begin_α
n826_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n827_goto_α:                                                        jmp   n743_statement_begin_α
n827_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n828_goto_α:                                                        jmp   n747_statement_begin_α
n828_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n829_goto_α:                                                        jmp   n754_statement_begin_α
n829_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n830_goto_α:                                                        jmp   n762_statement_begin_α
n830_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S6]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                lea              rdi, [rip + .S7]
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
.S0:                    .string          "r"
.S1:                    .string          "ws"
.S2:                    .string          "PATV$0"
.S3:                    .string          "PATV$1"
.S4:                    .string          "PATV$2"
.S5:                    .string          "ix"
.S6:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
.S7:                    .string          "BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits

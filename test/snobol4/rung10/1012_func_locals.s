                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lfunc_α:
proc_LBL__lfunc_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_statement_end_α
n0_statement_begin_β:                                               jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                 jmp   n2_statement_begin_α
#=======================================================================================================================
#         IDENT(a, 'p')              :s(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_var_α
n2_statement_begin_β:                                               jmp   n48_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n4_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx59_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n5_call_α
n4_lit_string_β:        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx59_0:                .quad            .Lx59_0_s
.Lx59_0_s:              .string          "p"
#-----------------------------------------------------------------------------------------------------------------------
n5_call_α:              sub              rsp, 16
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
.Lrkfnzd61:             .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd61]
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
                        cmp              eax, 104;                  jne   .Lx60_240
                        add              rsp, 16;                   jmp   n4_lit_string_β
.Lx60_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n6_statement_end_α
n5_call_β:              add              rsp, 16;                   jmp   n4_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 48;                   jmp   n7_statement_begin_α
#=======================================================================================================================
# e001    IDENT(b, 'q')              :s(e002)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                               jmp   n8_var_α
n7_statement_begin_β:                                               jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n9_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx67_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n10_call_α
n9_lit_string_β:        add              rsp, 16
                        add              rsp, 16;                   jmp   n7_statement_begin_β
.Lx67_0:                .quad            .Lx67_0_s
.Lx67_0_s:              .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n10_call_α:             sub              rsp, 16
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
.Lrkfnzd69:             .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd69]
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
                        cmp              eax, 104;                  jne   .Lx68_240
                        add              rsp, 16;                   jmp   n9_lit_string_β
.Lx68_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n11_statement_end_α
n10_call_β:             add              rsp, 16;                   jmp   n9_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:                                                jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:                                              jmp   n13_statement_end_α
n12_statement_begin_β:  add              rsp, 48;                   jmp   n14_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:    add              rsp, 48;                   jmp   n14_statement_begin_α
#=======================================================================================================================
#         DIFFER(d)                  :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:                                              jmp   n15_var_α
n14_statement_begin_β:                                              jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n16_call_α
#-----------------------------------------------------------------------------------------------------------------------
n16_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd80:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd80]
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
                        cmp              eax, 104;                  jne   .Lx79_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n14_statement_begin_β
.Lx79_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n17_statement_end_α
n16_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n14_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    add              rsp, 32;                   jmp   n18_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/003: local d should be null' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:                                              jmp   n19_lit_string_α
n18_statement_begin_β:                                              jmp   proc_LBL__lfunc_γ
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n20_assign_α
.Lx85_0:                .quad            .Lx85_0_s
.Lx85_0_s:              .string          "FAIL 1012/003: local d should be null"
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx86_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n21_statement_end_α
.Lx86_0:                .quad            .Lx86_0_s
.Lx86_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__lfunc_γ
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                              jmp   n23_statement_end_α
n22_statement_begin_β:                                              jmp   n24_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:                                                jmp   n24_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:                                              jmp   n25_lit_string_α
n24_statement_begin_β:                                              jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n26_assign_α
.Lx95_0:                .quad            .Lx95_0_s
.Lx95_0_s:              .string          "aa"
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:           mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              qword ptr [r9 + 24], rdx;  jmp   n27_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:    add              rsp, 16;                   jmp   n28_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                              jmp   n29_lit_string_α
n28_statement_begin_β:                                              jmp   n32_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n30_assign_α
.Lx101_0:               .quad            .Lx101_0_s
.Lx101_0_s:             .string          "bb"
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_α:           mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              qword ptr [r9 + 40], rdx;  jmp   n31_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:    add              rsp, 16;                   jmp   n32_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:                                              jmp   n33_lit_string_α
n32_statement_begin_β:                                              jmp   n36_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n34_assign_α
.Lx107_0:               .quad            .Lx107_0_s
.Lx107_0_s:             .string          "dd"
#-----------------------------------------------------------------------------------------------------------------------
n34_assign_α:           mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax                       # d
                        mov              qword ptr [r9 + 72], rdx;  jmp   n35_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:    add              rsp, 16;                   jmp   n36_statement_begin_α
#=======================================================================================================================
#         lfunc = a b d                                  :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:                                              jmp   n37_var_α
n36_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n37_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n38_var_α
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n39_binop_α
n38_var_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n36_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n39_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n40_var_α
n39_binop_β:            add              rsp, 16;                   jmp   n38_var_β
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n41_binop_α
n40_var_β:              add              rsp, 16;                   jmp   n39_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n41_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n42_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n42_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # lfunc
                        mov              qword ptr [r9 + 8], rdx;   jmp   n43_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:    add              rsp, 80;                   jmp   RETURN
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/002: arg b should be q'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:                                              jmp   n45_lit_string_α
n44_statement_begin_β:                                              jmp   proc_LBL__lfunc_γ
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n46_assign_α
.Lx123_0:               .quad            .Lx123_0_s
.Lx123_0_s:             .string          "FAIL 1012/002: arg b should be q"
#-----------------------------------------------------------------------------------------------------------------------
n46_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx124_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n47_statement_end_α
.Lx124_0:               .quad            .Lx124_0_s
.Lx124_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__lfunc_γ
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/001: arg a should be p'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:                                              jmp   n49_lit_string_α
n48_statement_begin_β:                                              jmp   proc_LBL__lfunc_γ
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n50_assign_α
.Lx129_0:               .quad            .Lx129_0_s
.Lx129_0_s:             .string          "FAIL 1012/001: arg a should be p"
#-----------------------------------------------------------------------------------------------------------------------
n50_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx130_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n51_statement_end_α
.Lx130_0:               .quad            .Lx130_0_s
.Lx130_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__lfunc_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lfunc_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lfunc_β:
                                                                    jmp   proc_LBL__lfunc_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lfunc_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lfunc_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__checklocal_α:
proc_LBL__checklocal_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_begin_α:                                             jmp   n134_statement_end_α
n133_statement_begin_β:                                             jmp   n135_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n134_statement_end_α:                                               jmp   n135_statement_begin_α
#=======================================================================================================================
#         DIFFER(x)                  :f(e007_inner)
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_begin_α:                                             jmp   n136_var_α
n135_statement_begin_β:                                             jmp   n143_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n136_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]                      # x
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n137_call_α
#-----------------------------------------------------------------------------------------------------------------------
n137_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd157:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd157]
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
                        cmp              eax, 104;                  jne   .Lx156_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n135_statement_begin_β
.Lx156_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n138_statement_end_α
n137_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n135_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_end_α:   add              rsp, 32;                   jmp   n139_statement_begin_α
#=======================================================================================================================
#         checklocal = 'local-not-null'                  :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_begin_α:                                             jmp   n140_lit_string_α
n139_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n140_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx162_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n141_assign_α
.Lx162_0:               .quad            .Lx162_0_s
.Lx162_0_s:             .string          "local-not-null"
#-----------------------------------------------------------------------------------------------------------------------
n141_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # checklocal
                        mov              qword ptr [r9 + 120], rdx; jmp   n142_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n142_statement_end_α:   add              rsp, 16;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_begin_α:                                             jmp   n144_statement_end_α
n143_statement_begin_β:                                             jmp   n145_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_end_α:                                               jmp   n145_statement_begin_α
#=======================================================================================================================
#         checklocal =                                   :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_begin_α:                                             jmp   n146_lit_string_α
n145_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n146_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n147_assign_α
.Lx172_0:               .quad            .Lx172_0_s
.Lx172_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n147_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # checklocal
                        mov              qword ptr [r9 + 120], rdx; jmp   n148_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:   add              rsp, 16;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__checklocal_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__checklocal_β:
                                                                    jmp   proc_LBL__checklocal_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__checklocal_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__checklocal_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_lfunc_α
proc_lfunc_α:
proc_lfunc_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n176_save_restore_α:                                                jmp   n177_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n177_goto_deferred_α:   lea              rax, [rip + proc_LBL__lfunc_α]; jmp   rax
                                                                    jmp   n178_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n178_save_restore_α:
lfunc_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 48
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx184_2
                        mov              eax, 48
.Lx184_2:               sub              rsp, 144
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 64]                       # d
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
                        mov              rax, qword ptr [r9 + 80]                       # e
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              rax, qword ptr [r9 + 96]                       # f
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 0]                        # lfunc
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 64], r10
                        mov              qword ptr [rsp + 72], r11
                        mov              qword ptr [rsp + 80], rcx
                        cmp              rcx, 0;                    jbe   .Lx184_10
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # a
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx184_41
.Lx184_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx184_41:              cmp              rcx, 1;                    jbe   .Lx184_11
                        mov              rax, qword ptr [r8 + 96]
                        mov              rdx, qword ptr [r9 + 32]                       # b
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 96], rdx
                        mov              rax, qword ptr [r8 + 104]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 104], rdx; jmp   .Lx184_42
.Lx184_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx184_42:              cmp              rcx, 2;                    jbe   .Lx184_12
                        mov              rax, qword ptr [r8 + 112]
                        mov              rdx, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 112], rdx
                        mov              rax, qword ptr [r8 + 120]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 120], rdx; jmp   .Lx184_43
.Lx184_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx184_43:              lea              r10, [rip + lfunc_gamma]
                        lea              r11, [rip + lfunc_omega]
                        lea              rax, [rip + proc_LBL__lfunc_α]; jmp   rax
lfunc_gamma:            mov              rdi, qword ptr [r9 + 0]                        # lfunc
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx184_75
                        mov              eax, 48
.Lx184_75:              lea              r8, [rsp + 96]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 96], rax                       # f
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 80], rax                       # e
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # d
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rcx, 0;                    jbe   .Lx184_80
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx184_110
.Lx184_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx184_110:             cmp              rcx, 1;                    jbe   .Lx184_81
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx184_111
.Lx184_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx184_111:             cmp              rcx, 2;                    jbe   .Lx184_82
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx184_112
.Lx184_82:              mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx184_112:             mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 176
                        cmp              rcx, 3;                    jbe   .Lx184_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx184_3:               mov              rcx, qword ptr [r8 + 64]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
lfunc_omega:            mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx184_145
                        mov              eax, 48
.Lx184_145:             lea              r8, [rsp + 96]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 0], rax                        # lfunc
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 96], rax                       # f
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 80], rax                       # e
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # d
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rcx, 0;                    jbe   .Lx184_150
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx184_180
.Lx184_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx184_180:             cmp              rcx, 1;                    jbe   .Lx184_151
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx184_181
.Lx184_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx184_181:             cmp              rcx, 2;                    jbe   .Lx184_152
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx184_182
.Lx184_152:             mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx184_182:             mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 176
                        cmp              rcx, 3;                    jbe   .Lx184_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx184_4:               mov              rcx, qword ptr [r8 + 72]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_lfunc_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_lfunc_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_lfunc_β:
                                                                    jmp   proc_lfunc_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_lfunc_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_lfunc_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_checklocal_α
proc_checklocal_α:
proc_checklocal_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n185_save_restore_α:                                                jmp   n186_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n186_goto_deferred_α:   lea              rax, [rip + proc_LBL__checklocal_α]; jmp   rax
                                                                    jmp   n187_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n187_save_restore_α:
checklocal_alpha:       mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx193_2
                        mov              eax, 16
.Lx193_2:               sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 112]                      # checklocal
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx193_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 128]                      # x
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx193_41
.Lx193_10:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx193_41:              lea              r10, [rip + checklocal_gamma]
                        lea              r11, [rip + checklocal_omega]
                        lea              rax, [rip + proc_LBL__checklocal_α]; jmp   rax
checklocal_gamma:       mov              rdi, qword ptr [r9 + 112]                      # checklocal
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx193_75
                        mov              eax, 16
.Lx193_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0;                    jbe   .Lx193_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 128], rax                      # x
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 136], rax; jmp   .Lx193_110
.Lx193_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 136], rax
.Lx193_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx193_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx193_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
checklocal_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx193_145
                        mov              eax, 16
.Lx193_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax                      # checklocal
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0;                    jbe   .Lx193_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 128], rax                      # x
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 136], rax; jmp   .Lx193_180
.Lx193_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 136], rax
.Lx193_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx193_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx193_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_checklocal_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_checklocal_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_checklocal_β:
                                                                    jmp   proc_checklocal_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_checklocal_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_checklocal_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__lfunc"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__lfunc_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1184
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__checklocal"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__checklocal_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1184
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "lfunc"
.Lgvan1:                .string          "a"
.Lgvan2:                .string          "b"
.Lgvan3:                .string          "c"
.Lgvan4:                .string          "d"
.Lgvan5:                .string          "e"
.Lgvan6:                .string          "f"
.Lgvan7:                .string          "checklocal"
.Lgvan8:                .string          "x"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 9
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 9
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_begin_α:                                             jmp   n195_statement_end_α
n194_statement_begin_β:                                             jmp   n196_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n195_statement_end_α:                                               jmp   n196_statement_begin_α
#=======================================================================================================================
#         DEFINE('lfunc(a,b,c)d,e,f')               :(lfunc_end)
#-----------------------------------------------------------------------------------------------------------------------
n196_statement_begin_α:                                             jmp   n197_func_activate_α
n196_statement_begin_β:                                             jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n197_func_activate_α:   mov              rdi, qword ptr [rip + .Lx331_0]
                        mov              rsi, qword ptr [rip + .Lx331_1]
                        mov              edx, 6
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + proc_lfunc_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n198_statement_end_α
n197_func_activate_β:                                               jmp   n196_statement_begin_β
.Lx331_0:               .quad            .Lx331_0_s
.Lx331_0_s:             .string          "lfunc"
.Lx331_1:               .quad            .Lx331_1_s
.Lx331_1_s:             .string          "a,b,c,d,e,f"
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_end_α:                                               jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_begin_α:                                             jmp   n200_statement_end_α
n199_statement_begin_β:                                             jmp   n201_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n200_statement_end_α:                                               jmp   n201_statement_begin_α
#=======================================================================================================================
#         a = 'global_a'
#-----------------------------------------------------------------------------------------------------------------------
n201_statement_begin_α:                                             jmp   n202_lit_string_α
n201_statement_begin_β:                                             jmp   n205_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n202_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx340_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n203_assign_α
.Lx340_0:               .quad            .Lx340_0_s
.Lx340_0_s:             .string          "global_a"
#-----------------------------------------------------------------------------------------------------------------------
n203_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              qword ptr [r9 + 24], rdx;  jmp   n204_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n204_statement_end_α:   add              rsp, 16;                   jmp   n205_statement_begin_α
#=======================================================================================================================
#         d = 'global_d'
#-----------------------------------------------------------------------------------------------------------------------
n205_statement_begin_α:                                             jmp   n206_lit_string_α
n205_statement_begin_β:                                             jmp   n209_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n206_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx346_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n207_assign_α
.Lx346_0:               .quad            .Lx346_0_s
.Lx346_0_s:             .string          "global_d"
#-----------------------------------------------------------------------------------------------------------------------
n207_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax                       # d
                        mov              qword ptr [r9 + 72], rdx;  jmp   n208_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n208_statement_end_α:   add              rsp, 16;                   jmp   n209_statement_begin_α
#=======================================================================================================================
#         d = 'global_d'
#-----------------------------------------------------------------------------------------------------------------------
n209_statement_begin_α:                                             jmp   n210_statement_end_α
n209_statement_begin_β:                                             jmp   n211_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_end_α:                                               jmp   n211_statement_begin_α
#=======================================================================================================================
#         DIFFER(lfunc('p', 'q', 'r'), 'aabbdd')                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n211_statement_begin_α:                                             jmp   n212_lit_string_α
n211_statement_begin_β:                                             jmp   n223_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n212_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx356_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n213_lit_string_α
.Lx356_0:               .quad            .Lx356_0_s
.Lx356_0_s:             .string          "p"
#-----------------------------------------------------------------------------------------------------------------------
n213_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx357_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n214_lit_string_α
n213_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n211_statement_begin_β
.Lx357_0:               .quad            .Lx357_0_s
.Lx357_0_s:             .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n214_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx358_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n215_call_α
n214_lit_string_β:      add              rsp, 16;                   jmp   n213_lit_string_β
.Lx358_0:               .quad            .Lx358_0_s
.Lx358_0_s:             .string          "r"
#-----------------------------------------------------------------------------------------------------------------------
n215_call_α:            sub              rsp, 16
                        sub              rsp, 80
                        mov              rax, qword ptr [rsp + 128]                     # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                        mov              eax, 3
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx360_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + lfunc_alpha];  jmp   rax
.Lx360_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx360_240
                        add              rsp, 16;                   jmp   n214_lit_string_β
.Lx360_240:                                                         jmp   n216_lit_string_α
n215_call_β:                                                        jmp   n214_lit_string_β
.Lx360_0:               .quad            .Lx360_0_s
.Lx360_0_s:             .string          "lfunc"
#-----------------------------------------------------------------------------------------------------------------------
n216_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx361_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n217_call_α
n216_lit_string_β:      add              rsp, 32;                   jmp   n214_lit_string_β
.Lx361_0:               .quad            .Lx361_0_s
.Lx361_0_s:             .string          "aabbdd"
#-----------------------------------------------------------------------------------------------------------------------
n217_call_α:            sub              rsp, 16
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
.Lrkfnzd363:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd363]
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
                        cmp              eax, 104;                  jne   .Lx362_240
                        add              rsp, 16;                   jmp   n216_lit_string_β
.Lx362_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n218_statement_end_α
n217_call_β:            add              rsp, 16;                   jmp   n216_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n218_statement_end_α:   add              rsp, 96;                   jmp   n219_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/004: lfunc return value'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_begin_α:                                             jmp   n220_lit_string_α
n219_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n220_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx368_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n221_assign_α
.Lx368_0:               .quad            .Lx368_0_s
.Lx368_0_s:             .string          "FAIL 1012/004: lfunc return value"
#-----------------------------------------------------------------------------------------------------------------------
n221_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx369_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n222_statement_end_α
.Lx369_0:               .quad            .Lx369_0_s
.Lx369_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n222_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_begin_α:                                             jmp   n224_statement_end_α
n223_statement_begin_β:                                             jmp   n225_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n224_statement_end_α:                                               jmp   n225_statement_begin_α
#=======================================================================================================================
#         IDENT(a, 'global_a')                           :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
n225_statement_begin_α:                                             jmp   n226_var_α
n225_statement_begin_β:                                             jmp   n262_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n226_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n227_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n227_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx379_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n228_call_α
n227_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n225_statement_begin_β
.Lx379_0:               .quad            .Lx379_0_s
.Lx379_0_s:             .string          "global_a"
#-----------------------------------------------------------------------------------------------------------------------
n228_call_α:            sub              rsp, 16
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
.Lrkfnzd381:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd381]
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
                        cmp              eax, 104;                  jne   .Lx380_240
                        add              rsp, 16;                   jmp   n227_lit_string_β
.Lx380_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n229_statement_end_α
n228_call_β:            add              rsp, 16;                   jmp   n227_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n229_statement_end_α:                                               jmp   n230_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_begin_α:                                             jmp   n231_statement_end_α
n230_statement_begin_β: add              rsp, 48;                   jmp   n232_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_end_α:   add              rsp, 48;                   jmp   n232_statement_begin_α
#=======================================================================================================================
#         IDENT(d, 'global_d')                           :s(e006)
#-----------------------------------------------------------------------------------------------------------------------
n232_statement_begin_α:                                             jmp   n233_var_α
n232_statement_begin_β:                                             jmp   n258_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n233_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n234_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n234_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx391_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n235_call_α
n234_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n232_statement_begin_β
.Lx391_0:               .quad            .Lx391_0_s
.Lx391_0_s:             .string          "global_d"
#-----------------------------------------------------------------------------------------------------------------------
n235_call_α:            sub              rsp, 16
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
.Lrkfnzd393:            .string          "IDENT"
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
                        cmp              eax, 104;                  jne   .Lx392_240
                        add              rsp, 16;                   jmp   n234_lit_string_β
.Lx392_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n236_statement_end_α
n235_call_β:            add              rsp, 16;                   jmp   n234_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_end_α:                                               jmp   n237_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_begin_α:                                             jmp   n238_statement_end_α
n237_statement_begin_β: add              rsp, 48;                   jmp   n239_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n238_statement_end_α:   add              rsp, 48;                   jmp   n239_statement_begin_α
#=======================================================================================================================
#         DEFINE('checklocal()x')                        :(cl_end)
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_begin_α:                                             jmp   n240_func_activate_α
n239_statement_begin_β:                                             jmp   n242_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n240_func_activate_α:   mov              rdi, qword ptr [rip + .Lx403_0]
                        mov              rsi, qword ptr [rip + .Lx403_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_checklocal_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n241_statement_end_α
n240_func_activate_β:                                               jmp   n239_statement_begin_β
.Lx403_0:               .quad            .Lx403_0_s
.Lx403_0_s:             .string          "checklocal"
.Lx403_1:               .quad            .Lx403_1_s
.Lx403_1_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n241_statement_end_α:                                               jmp   n242_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n242_statement_begin_α:                                             jmp   n243_statement_end_α
n242_statement_begin_β:                                             jmp   n244_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_end_α:                                               jmp   n244_statement_begin_α
#=======================================================================================================================
#         DIFFER(checklocal())       :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_begin_α:                                             jmp   n245_call_α
n244_statement_begin_β:                                             jmp   n252_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n245_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx413_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + checklocal_alpha]; jmp   rax
.Lx413_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx413_240
                        add              rsp, 16;                   jmp   n244_statement_begin_β
.Lx413_240:                                                         jmp   n246_call_α
n245_call_β:                                                        jmp   n244_statement_begin_β
.Lx413_0:               .quad            .Lx413_0_s
.Lx413_0_s:             .string          "checklocal"
#-----------------------------------------------------------------------------------------------------------------------
n246_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd415:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd415]
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
                        cmp              eax, 104;                  jne   .Lx414_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n244_statement_begin_β
.Lx414_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n247_statement_end_α
n246_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n244_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n247_statement_end_α:   add              rsp, 32;                   jmp   n248_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/007: local null on fresh call' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n248_statement_begin_α:                                             jmp   n249_lit_string_α
n248_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n249_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx420_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n250_assign_α
.Lx420_0:               .quad            .Lx420_0_s
.Lx420_0_s:             .string          "FAIL 1012/007: local null on fresh call"
#-----------------------------------------------------------------------------------------------------------------------
n250_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx421_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n251_statement_end_α
.Lx421_0:               .quad            .Lx421_0_s
.Lx421_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n251_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n252_statement_begin_α:                                             jmp   n253_statement_end_α
n252_statement_begin_β:                                             jmp   n254_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n253_statement_end_α:                                               jmp   n254_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1012_func_locals (7/7)'
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_begin_α:                                             jmp   n255_lit_string_α
n254_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n255_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx430_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n256_assign_α
.Lx430_0:               .quad            .Lx430_0_s
.Lx430_0_s:             .string          "PASS 1012_func_locals (7/7)"
#-----------------------------------------------------------------------------------------------------------------------
n256_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx431_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n257_statement_end_α
.Lx431_0:               .quad            .Lx431_0_s
.Lx431_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n257_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/006: global d not clobbered' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_begin_α:                                             jmp   n259_lit_string_α
n258_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n259_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx436_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n260_assign_α
.Lx436_0:               .quad            .Lx436_0_s
.Lx436_0_s:             .string          "FAIL 1012/006: global d not clobbered"
#-----------------------------------------------------------------------------------------------------------------------
n260_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx437_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n261_statement_end_α
.Lx437_0:               .quad            .Lx437_0_s
.Lx437_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n261_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/005: global a not clobbered' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n262_statement_begin_α:                                             jmp   n263_lit_string_α
n262_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n263_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx442_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n264_assign_α
.Lx442_0:               .quad            .Lx442_0_s
.Lx442_0_s:             .string          "FAIL 1012/005: global a not clobbered"
#-----------------------------------------------------------------------------------------------------------------------
n264_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx443_0]
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
.Lx443_0:               .quad            .Lx443_0_s
.Lx443_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n266_goto_α:                                                        jmp   n267_statement_begin_α
n266_goto_β:                                                        jmp   main_ω
#=======================================================================================================================
# e001    IDENT(b, 'q')              :s(e002)
#-----------------------------------------------------------------------------------------------------------------------
n267_statement_begin_α:                                             jmp   n268_var_α
n267_statement_begin_β:                                             jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n268_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n269_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n269_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx450_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n270_call_α
n269_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n267_statement_begin_β
.Lx450_0:               .quad            .Lx450_0_s
.Lx450_0_s:             .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n270_call_α:            sub              rsp, 16
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
.Lrkfnzd452:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd452]
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
                        cmp              eax, 104;                  jne   .Lx451_240
                        add              rsp, 16;                   jmp   n269_lit_string_β
.Lx451_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n271_statement_end_α
n270_call_β:            add              rsp, 16;                   jmp   n269_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_end_α:                                               jmp   n272_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n272_statement_begin_α:                                             jmp   n273_statement_end_α
n272_statement_begin_β: add              rsp, 48;                   jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_end_α:   add              rsp, 48;                   jmp   n274_statement_begin_α
#=======================================================================================================================
#         DIFFER(d)                  :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_begin_α:                                             jmp   n275_var_α
n274_statement_begin_β:                                             jmp   n282_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n275_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n276_call_α
#-----------------------------------------------------------------------------------------------------------------------
n276_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd463:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd463]
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
                        cmp              eax, 104;                  jne   .Lx462_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n274_statement_begin_β
.Lx462_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n277_statement_end_α
n276_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n274_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n277_statement_end_α:   add              rsp, 32;                   jmp   n278_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/003: local d should be null' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n278_statement_begin_α:                                             jmp   n279_lit_string_α
n278_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n279_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx468_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n280_assign_α
.Lx468_0:               .quad            .Lx468_0_s
.Lx468_0_s:             .string          "FAIL 1012/003: local d should be null"
#-----------------------------------------------------------------------------------------------------------------------
n280_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx469_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n281_statement_end_α
.Lx469_0:               .quad            .Lx469_0_s
.Lx469_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n281_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n282_statement_begin_α:                                             jmp   n283_statement_end_α
n282_statement_begin_β:                                             jmp   n284_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n283_statement_end_α:                                               jmp   n284_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n284_statement_begin_α:                                             jmp   n285_lit_string_α
n284_statement_begin_β:                                             jmp   n288_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n285_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx478_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n286_assign_α
.Lx478_0:               .quad            .Lx478_0_s
.Lx478_0_s:             .string          "aa"
#-----------------------------------------------------------------------------------------------------------------------
n286_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              qword ptr [r9 + 24], rdx;  jmp   n287_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n287_statement_end_α:   add              rsp, 16;                   jmp   n288_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n288_statement_begin_α:                                             jmp   n289_lit_string_α
n288_statement_begin_β:                                             jmp   n292_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n289_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx484_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n290_assign_α
.Lx484_0:               .quad            .Lx484_0_s
.Lx484_0_s:             .string          "bb"
#-----------------------------------------------------------------------------------------------------------------------
n290_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              qword ptr [r9 + 40], rdx;  jmp   n291_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_end_α:   add              rsp, 16;                   jmp   n292_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n292_statement_begin_α:                                             jmp   n293_lit_string_α
n292_statement_begin_β:                                             jmp   n296_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n293_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx490_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n294_assign_α
.Lx490_0:               .quad            .Lx490_0_s
.Lx490_0_s:             .string          "dd"
#-----------------------------------------------------------------------------------------------------------------------
n294_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax                       # d
                        mov              qword ptr [r9 + 72], rdx;  jmp   n295_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n295_statement_end_α:   add              rsp, 16;                   jmp   n296_statement_begin_α
#=======================================================================================================================
#         lfunc = a b d                                  :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n296_statement_begin_α:                                             jmp   n297_var_α
n296_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n297_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n298_var_α
#-----------------------------------------------------------------------------------------------------------------------
n298_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n299_binop_α
n298_var_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n296_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n299_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n300_var_α
n299_binop_β:           add              rsp, 16;                   jmp   n298_var_β
#-----------------------------------------------------------------------------------------------------------------------
n300_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n301_binop_α
n300_var_β:             add              rsp, 16;                   jmp   n299_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n301_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n302_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n302_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # lfunc
                        mov              qword ptr [r9 + 8], rdx;   jmp   n303_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_end_α:                                               jmp   RETURN
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/002: arg b should be q'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_begin_α:                                             jmp   n305_lit_string_α
n304_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n305_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx506_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n306_assign_α
.Lx506_0:               .quad            .Lx506_0_s
.Lx506_0_s:             .string          "FAIL 1012/002: arg b should be q"
#-----------------------------------------------------------------------------------------------------------------------
n306_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx507_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n307_statement_end_α
.Lx507_0:               .quad            .Lx507_0_s
.Lx507_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n307_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n308_goto_α:                                                        jmp   n272_statement_begin_α
n308_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n309_goto_α:                                                        jmp   n282_statement_begin_α
n309_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n310_goto_α:                                                        jmp   n199_statement_begin_α
n310_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n311_goto_α:                                                        jmp   n223_statement_begin_α
n311_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n312_goto_α:                                                        jmp   n230_statement_begin_α
n312_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n313_goto_α:                                                        jmp   n237_statement_begin_α
n313_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n314_goto_α:                                                        jmp   n315_statement_begin_α
n314_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n315_statement_begin_α:                                             jmp   n316_statement_end_α
n315_statement_begin_β:                                             jmp   n317_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_end_α:                                               jmp   n317_statement_begin_α
#=======================================================================================================================
#         checklocal =                                   :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_begin_α:                                             jmp   n318_lit_string_α
n317_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n318_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx523_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n319_assign_α
.Lx523_0:               .quad            .Lx523_0_s
.Lx523_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n319_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # checklocal
                        mov              qword ptr [r9 + 120], rdx; jmp   n320_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_end_α:   add              rsp, 16;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n321_goto_α:                                                        jmp   n242_statement_begin_α
n321_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n322_goto_α:                                                        jmp   n252_statement_begin_α
n322_goto_β:                                                        jmp   main_ω
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

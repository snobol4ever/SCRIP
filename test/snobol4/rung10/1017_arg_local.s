                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__jlab_α:
proc_LBL__jlab_α_body:
#=======================================================================================================================
# jlab    DEFINE('jlab(a,b,c)d,e,f')
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_func_activate_α
n0_statement_begin_β:                                               jmp   n3_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_func_activate_α:     mov              rdi, qword ptr [rip + .Lx116_0]
                        mov              rsi, qword ptr [rip + .Lx116_1]
                        mov              edx, 6
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + proc_jlab_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n2_statement_end_α
n1_func_activate_β:                                                 jmp   n0_statement_begin_β
.Lx116_0:               .quad            .Lx116_0_s
.Lx116_0_s:             .string          "jlab"
.Lx116_1:               .quad            .Lx116_1_s
.Lx116_1_s:             .string          "a,b,c,d,e,f"
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_end_α:                                                 jmp   n3_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_begin_α:                                               jmp   n4_statement_end_α
n3_statement_begin_β:                                               jmp   n5_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:                                                 jmp   n5_statement_begin_α
#=======================================================================================================================
#         DIFFER(ARG(.jlab, 1), 'A')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_begin_α:                                               jmp   n6_lit_string_α
n5_statement_begin_β:                                               jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n7_call_α
.Lx125_0:               .quad            .Lx125_0_s
.Lx125_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd127:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd127]
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
                        cmp              eax, 104;                  jne   .Lx126_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n5_statement_begin_β
.Lx126_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n8_lit_integer_α
n7_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n5_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx128_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n9_call_α
n8_lit_integer_β:       add              rsp, 16
                        add              rsp, 32;                   jmp   n5_statement_begin_β
.Lx128_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:              sub              rsp, 16
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
.Lbynamefnzd10:         .string          "ARG"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd10]
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
                        cmp              eax, 104;                  jne   .Lx129_240
                        add              rsp, 16;                   jmp   n8_lit_integer_β
.Lx129_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n10_lit_string_α
n9_call_β:              add              rsp, 16;                   jmp   n8_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx130_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n11_call_α
n10_lit_string_β:       add              rsp, 32;                   jmp   n8_lit_integer_β
.Lx130_0:               .quad            .Lx130_0_s
.Lx130_0_s:             .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
n11_call_α:             sub              rsp, 16
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
.Lrkfnzd132:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd132]
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
                        cmp              eax, 104;                  jne   .Lx131_240
                        add              rsp, 16;                   jmp   n10_lit_string_β
.Lx131_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n12_statement_end_α
n11_call_β:             add              rsp, 16;                   jmp   n10_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:    add              rsp, 96;                   jmp   n13_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/001: arg(.jlab,1) = A'     :(END)
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:                                              jmp   n14_lit_string_α
n13_statement_begin_β:                                              jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n15_assign_α
.Lx137_0:               .quad            .Lx137_0_s
.Lx137_0_s:             .string          "FAIL 1017/001: arg(.jlab,1) = A"
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx138_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n16_statement_end_α
.Lx138_0:               .quad            .Lx138_0_s
.Lx138_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:                                              jmp   n18_statement_end_α
n17_statement_begin_β:                                              jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:                                                jmp   n19_statement_begin_α
#=======================================================================================================================
#         DIFFER(ARG(.jlab, 3), 'C')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_begin_α:                                              jmp   n20_lit_string_α
n19_statement_begin_β:                                              jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx147_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n21_call_α
.Lx147_0:               .quad            .Lx147_0_s
.Lx147_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n21_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd149:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd149]
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
                        cmp              eax, 104;                  jne   .Lx148_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n19_statement_begin_β
.Lx148_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n22_lit_integer_α
n21_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n19_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n23_call_α
n22_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n19_statement_begin_β
.Lx150_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:             sub              rsp, 16
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
.Lbynamefnzd24:         .string          "ARG"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd24]
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
                        cmp              eax, 104;                  jne   .Lx151_240
                        add              rsp, 16;                   jmp   n22_lit_integer_β
.Lx151_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n24_lit_string_α
n23_call_β:             add              rsp, 16;                   jmp   n22_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx152_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n25_call_α
n24_lit_string_β:       add              rsp, 32;                   jmp   n22_lit_integer_β
.Lx152_0:               .quad            .Lx152_0_s
.Lx152_0_s:             .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:             sub              rsp, 16
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
.Lrkfnzd154:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd154]
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
                        cmp              eax, 104;                  jne   .Lx153_240
                        add              rsp, 16;                   jmp   n24_lit_string_β
.Lx153_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n26_statement_end_α
n25_call_β:             add              rsp, 16;                   jmp   n24_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:    add              rsp, 96;                   jmp   n27_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/002: arg(.jlab,3) = C'     :(END)
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_begin_α:                                              jmp   n28_lit_string_α
n27_statement_begin_β:                                              jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx159_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n29_assign_α
.Lx159_0:               .quad            .Lx159_0_s
.Lx159_0_s:             .string          "FAIL 1017/002: arg(.jlab,3) = C"
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx160_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n30_statement_end_α
.Lx160_0:               .quad            .Lx160_0_s
.Lx160_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:                                              jmp   n32_statement_end_α
n31_statement_begin_β:                                              jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:                                                jmp   n33_statement_begin_α
#=======================================================================================================================
#         ARG(.jlab, 0)              :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                              jmp   n34_lit_string_α
n33_statement_begin_β:                                              jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n35_call_α
.Lx169_0:               .quad            .Lx169_0_s
.Lx169_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd171:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd171]
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
                        cmp              eax, 104;                  jne   .Lx170_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n33_statement_begin_β
.Lx170_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n36_lit_integer_α
n35_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n33_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n37_call_α
n36_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n33_statement_begin_β
.Lx172_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n37_call_α:             sub              rsp, 16
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
.Lbynamefnzd38:         .string          "ARG"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd38]
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
                        cmp              eax, 104;                  jne   .Lx173_240
                        add              rsp, 16;                   jmp   n36_lit_integer_β
.Lx173_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n38_statement_end_α
n37_call_β:             add              rsp, 16;                   jmp   n36_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:    add              rsp, 64;                   jmp   n39_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/003: arg(.jlab,0) OOB should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:                                              jmp   n40_lit_string_α
n39_statement_begin_β:                                              jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n41_assign_α
.Lx178_0:               .quad            .Lx178_0_s
.Lx178_0_s:             .string          "FAIL 1017/003: arg(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx179_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n42_statement_end_α
.Lx179_0:               .quad            .Lx179_0_s
.Lx179_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:                                              jmp   n44_statement_end_α
n43_statement_begin_β:                                              jmp   n45_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:                                                jmp   n45_statement_begin_α
#=======================================================================================================================
#         ARG(.jlab, 4)              :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:                                              jmp   n46_lit_string_α
n45_statement_begin_β:                                              jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx188_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n47_call_α
.Lx188_0:               .quad            .Lx188_0_s
.Lx188_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd190:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd190]
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
                        cmp              eax, 104;                  jne   .Lx189_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n45_statement_begin_β
.Lx189_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n48_lit_integer_α
n47_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n45_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n49_call_α
n48_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n45_statement_begin_β
.Lx191_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n49_call_α:             sub              rsp, 16
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
.Lbynamefnzd50:         .string          "ARG"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd50]
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
                        cmp              eax, 104;                  jne   .Lx192_240
                        add              rsp, 16;                   jmp   n48_lit_integer_β
.Lx192_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n50_statement_end_α
n49_call_β:             add              rsp, 16;                   jmp   n48_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:    add              rsp, 64;                   jmp   n51_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/004: arg(.jlab,4) OOB should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:                                              jmp   n52_lit_string_α
n51_statement_begin_β:                                              jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n53_assign_α
.Lx197_0:               .quad            .Lx197_0_s
.Lx197_0_s:             .string          "FAIL 1017/004: arg(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx198_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n54_statement_end_α
.Lx198_0:               .quad            .Lx198_0_s
.Lx198_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:                                              jmp   n56_statement_end_α
n55_statement_begin_β:                                              jmp   n57_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:                                                jmp   n57_statement_begin_α
#=======================================================================================================================
#         DIFFER(LOCAL(.jlab, 1), 'D')                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                              jmp   n58_lit_string_α
n57_statement_begin_β:                                              jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx207_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n59_call_α
.Lx207_0:               .quad            .Lx207_0_s
.Lx207_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n59_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd209:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd209]
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
                        cmp              eax, 104;                  jne   .Lx208_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n57_statement_begin_β
.Lx208_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n60_lit_integer_α
n59_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n57_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx210_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n61_call_α
n60_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n57_statement_begin_β
.Lx210_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n61_call_α:             sub              rsp, 16
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
.Lbynamefnzd62:         .string          "LOCAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd62]
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
                        cmp              eax, 104;                  jne   .Lx211_240
                        add              rsp, 16;                   jmp   n60_lit_integer_β
.Lx211_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n62_lit_string_α
n61_call_β:             add              rsp, 16;                   jmp   n60_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx212_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n63_call_α
n62_lit_string_β:       add              rsp, 32;                   jmp   n60_lit_integer_β
.Lx212_0:               .quad            .Lx212_0_s
.Lx212_0_s:             .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n63_call_α:             sub              rsp, 16
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
.Lrkfnzd214:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd214]
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
                        cmp              eax, 104;                  jne   .Lx213_240
                        add              rsp, 16;                   jmp   n62_lit_string_β
.Lx213_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n64_statement_end_α
n63_call_β:             add              rsp, 16;                   jmp   n62_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    add              rsp, 96;                   jmp   n65_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/005: local(.jlab,1) = D'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:                                              jmp   n66_lit_string_α
n65_statement_begin_β:                                              jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx219_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n67_assign_α
.Lx219_0:               .quad            .Lx219_0_s
.Lx219_0_s:             .string          "FAIL 1017/005: local(.jlab,1) = D"
#-----------------------------------------------------------------------------------------------------------------------
n67_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx220_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n68_statement_end_α
.Lx220_0:               .quad            .Lx220_0_s
.Lx220_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_begin_α:                                              jmp   n70_statement_end_α
n69_statement_begin_β:                                              jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:                                                jmp   n71_statement_begin_α
#=======================================================================================================================
#         DIFFER(LOCAL(.jlab, 3), 'F')                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:                                              jmp   n72_lit_string_α
n71_statement_begin_β:                                              jmp   n83_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx229_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n73_call_α
.Lx229_0:               .quad            .Lx229_0_s
.Lx229_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd231:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd231]
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
                        cmp              eax, 104;                  jne   .Lx230_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n71_statement_begin_β
.Lx230_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n74_lit_integer_α
n73_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n71_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n75_call_α
n74_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n71_statement_begin_β
.Lx232_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n75_call_α:             sub              rsp, 16
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
.Lbynamefnzd76:         .string          "LOCAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd76]
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
                        cmp              eax, 104;                  jne   .Lx233_240
                        add              rsp, 16;                   jmp   n74_lit_integer_β
.Lx233_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n76_lit_string_α
n75_call_β:             add              rsp, 16;                   jmp   n74_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx234_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n77_call_α
n76_lit_string_β:       add              rsp, 32;                   jmp   n74_lit_integer_β
.Lx234_0:               .quad            .Lx234_0_s
.Lx234_0_s:             .string          "F"
#-----------------------------------------------------------------------------------------------------------------------
n77_call_α:             sub              rsp, 16
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
.Lrkfnzd236:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd236]
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
                        cmp              eax, 104;                  jne   .Lx235_240
                        add              rsp, 16;                   jmp   n76_lit_string_β
.Lx235_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n78_statement_end_α
n77_call_β:             add              rsp, 16;                   jmp   n76_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:    add              rsp, 96;                   jmp   n79_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/006: local(.jlab,3) = F'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:                                              jmp   n80_lit_string_α
n79_statement_begin_β:                                              jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx241_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n81_assign_α
.Lx241_0:               .quad            .Lx241_0_s
.Lx241_0_s:             .string          "FAIL 1017/006: local(.jlab,3) = F"
#-----------------------------------------------------------------------------------------------------------------------
n81_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx242_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n82_statement_end_α
.Lx242_0:               .quad            .Lx242_0_s
.Lx242_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_begin_α:                                              jmp   n84_statement_end_α
n83_statement_begin_β:                                              jmp   n85_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_end_α:                                                jmp   n85_statement_begin_α
#=======================================================================================================================
#         LOCAL(.jlab, 0)            :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_begin_α:                                              jmp   n86_lit_string_α
n85_statement_begin_β:                                              jmp   n95_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx251_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n87_call_α
.Lx251_0:               .quad            .Lx251_0_s
.Lx251_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n87_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd253:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd253]
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
                        cmp              eax, 104;                  jne   .Lx252_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n85_statement_begin_β
.Lx252_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n88_lit_integer_α
n87_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n85_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx254_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n89_call_α
n88_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n85_statement_begin_β
.Lx254_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n89_call_α:             sub              rsp, 16
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
.Lbynamefnzd90:         .string          "LOCAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd90]
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
                        cmp              eax, 104;                  jne   .Lx255_240
                        add              rsp, 16;                   jmp   n88_lit_integer_β
.Lx255_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n90_statement_end_α
n89_call_β:             add              rsp, 16;                   jmp   n88_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:    add              rsp, 64;                   jmp   n91_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/007: local(.jlab,0) OOB should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:                                              jmp   n92_lit_string_α
n91_statement_begin_β:                                              jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n93_assign_α
.Lx260_0:               .quad            .Lx260_0_s
.Lx260_0_s:             .string          "FAIL 1017/007: local(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx261_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n94_statement_end_α
.Lx261_0:               .quad            .Lx261_0_s
.Lx261_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_end_α:    add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_begin_α:                                              jmp   n96_statement_end_α
n95_statement_begin_β:                                              jmp   n97_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_end_α:                                                jmp   n97_statement_begin_α
#=======================================================================================================================
#         LOCAL(.jlab, 4)            :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_begin_α:                                              jmp   n98_lit_string_α
n97_statement_begin_β:                                              jmp   n107_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n98_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx270_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n99_call_α
.Lx270_0:               .quad            .Lx270_0_s
.Lx270_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n99_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd272:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd272]
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
                        cmp              eax, 104;                  jne   .Lx271_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n97_statement_begin_β
.Lx271_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n100_lit_integer_α
n99_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n97_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx273_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n101_call_α
n100_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n97_statement_begin_β
.Lx273_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n101_call_α:            sub              rsp, 16
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
.Lbynamefnzd102:        .string          "LOCAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd102]
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
                        cmp              eax, 104;                  jne   .Lx274_240
                        add              rsp, 16;                   jmp   n100_lit_integer_β
.Lx274_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n102_statement_end_α
n101_call_β:            add              rsp, 16;                   jmp   n100_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   add              rsp, 64;                   jmp   n103_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/008: local(.jlab,4) OOB should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                             jmp   n104_lit_string_α
n103_statement_begin_β:                                             jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx279_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n105_assign_α
.Lx279_0:               .quad            .Lx279_0_s
.Lx279_0_s:             .string          "FAIL 1017/008: local(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n105_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx280_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n106_statement_end_α
.Lx280_0:               .quad            .Lx280_0_s
.Lx280_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:   add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_begin_α:                                             jmp   n108_statement_end_α
n107_statement_begin_β:                                             jmp   n109_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:                                               jmp   n109_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1017_arg_local (8/8)'
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_begin_α:                                             jmp   n110_lit_string_α
n109_statement_begin_β:                                             jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n111_assign_α
.Lx289_0:               .quad            .Lx289_0_s
.Lx289_0_s:             .string          "PASS 1017_arg_local (8/8)"
#-----------------------------------------------------------------------------------------------------------------------
n111_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx290_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n112_statement_end_α
.Lx290_0:               .quad            .Lx290_0_s
.Lx290_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_end_α:   add              rsp, 16;                   jmp   proc_LBL__jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__jlab_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__jlab_β:
                                                                    jmp   proc_LBL__jlab_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__jlab_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__jlab_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_jlab_α
proc_jlab_α:
proc_jlab_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n293_save_restore_α:                                                jmp   n294_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n294_goto_deferred_α:   lea              rax, [rip + proc_LBL__jlab_α]; jmp   rax
                                                                    jmp   n295_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n295_save_restore_α:
jlab_alpha:             mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 48
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx301_2
                        mov              eax, 48
.Lx301_2:               sub              rsp, 144
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
                        mov              rax, qword ptr [r9 + 0]                        # jlab
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 64], r10
                        mov              qword ptr [rsp + 72], r11
                        mov              qword ptr [rsp + 80], rcx
                        cmp              rcx, 0;                    jbe   .Lx301_10
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # a
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx301_41
.Lx301_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx301_41:              cmp              rcx, 1;                    jbe   .Lx301_11
                        mov              rax, qword ptr [r8 + 96]
                        mov              rdx, qword ptr [r9 + 32]                       # b
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 96], rdx
                        mov              rax, qword ptr [r8 + 104]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 104], rdx; jmp   .Lx301_42
.Lx301_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx301_42:              cmp              rcx, 2;                    jbe   .Lx301_12
                        mov              rax, qword ptr [r8 + 112]
                        mov              rdx, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 112], rdx
                        mov              rax, qword ptr [r8 + 120]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 120], rdx; jmp   .Lx301_43
.Lx301_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx301_43:              lea              r10, [rip + jlab_gamma]
                        lea              r11, [rip + jlab_omega]
                        lea              rax, [rip + proc_LBL__jlab_α]; jmp   rax
jlab_gamma:             mov              rdi, qword ptr [r9 + 0]                        # jlab
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx301_75
                        mov              eax, 48
.Lx301_75:              lea              r8, [rsp + 96]
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
                        cmp              rcx, 0;                    jbe   .Lx301_80
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx301_110
.Lx301_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx301_110:             cmp              rcx, 1;                    jbe   .Lx301_81
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx301_111
.Lx301_81:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx301_111:             cmp              rcx, 2;                    jbe   .Lx301_82
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx301_112
.Lx301_82:              mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx301_112:             mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 176
                        cmp              rcx, 3;                    jbe   .Lx301_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx301_3:               mov              rcx, qword ptr [r8 + 64]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
jlab_omega:             mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                    jbe   .Lx301_145
                        mov              eax, 48
.Lx301_145:             lea              r8, [rsp + 96]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 0], rax                        # jlab
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
                        cmp              rcx, 0;                    jbe   .Lx301_150
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx301_180
.Lx301_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx301_180:             cmp              rcx, 1;                    jbe   .Lx301_151
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx301_181
.Lx301_151:             mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx301_181:             cmp              rcx, 2;                    jbe   .Lx301_152
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx301_182
.Lx301_152:             mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx301_182:             mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 176
                        cmp              rcx, 3;                    jbe   .Lx301_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx301_4:               mov              rcx, qword ptr [r8 + 72]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_jlab_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_jlab_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_jlab_β:
                                                                    jmp   proc_jlab_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_jlab_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_jlab_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__jlab"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__jlab_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1648
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "jlab"
.Lgvan1:                .string          "a"
.Lgvan2:                .string          "b"
.Lgvan3:                .string          "c"
.Lgvan4:                .string          "d"
.Lgvan5:                .string          "e"
.Lgvan6:                .string          "f"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 7
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 7
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n302_statement_begin_α:                                             jmp   n303_statement_end_α
n302_statement_begin_β:                                             jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_end_α:                                               jmp   n304_statement_begin_α
#=======================================================================================================================
# jlab    DEFINE('jlab(a,b,c)d,e,f')
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_begin_α:                                             jmp   n305_func_activate_α
n304_statement_begin_β:                                             jmp   n307_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n305_func_activate_α:   mov              rdi, qword ptr [rip + .Lx432_0]
                        mov              rsi, qword ptr [rip + .Lx432_1]
                        mov              edx, 6
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + proc_jlab_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n306_statement_end_α
n305_func_activate_β:                                               jmp   n304_statement_begin_β
.Lx432_0:               .quad            .Lx432_0_s
.Lx432_0_s:             .string          "jlab"
.Lx432_1:               .quad            .Lx432_1_s
.Lx432_1_s:             .string          "a,b,c,d,e,f"
#-----------------------------------------------------------------------------------------------------------------------
n306_statement_end_α:                                               jmp   n307_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n307_statement_begin_α:                                             jmp   n308_statement_end_α
n307_statement_begin_β:                                             jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n308_statement_end_α:                                               jmp   n309_statement_begin_α
#=======================================================================================================================
#         DIFFER(ARG(.jlab, 1), 'A')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n309_statement_begin_α:                                             jmp   n310_lit_string_α
n309_statement_begin_β:                                             jmp   n321_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n310_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx441_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n311_call_α
.Lx441_0:               .quad            .Lx441_0_s
.Lx441_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n311_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd443:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd443]
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
                        cmp              eax, 104;                  jne   .Lx442_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n309_statement_begin_β
.Lx442_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n312_lit_integer_α
n311_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n309_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n312_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx444_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n313_call_α
n312_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n309_statement_begin_β
.Lx444_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n313_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        lea              rdi, [rip + .Lbynamefnzd10]
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
                        cmp              eax, 104;                  jne   .Lx445_240
                        add              rsp, 16;                   jmp   n312_lit_integer_β
.Lx445_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n314_lit_string_α
n313_call_β:            add              rsp, 16;                   jmp   n312_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n314_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx446_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n315_call_α
n314_lit_string_β:      add              rsp, 32;                   jmp   n312_lit_integer_β
.Lx446_0:               .quad            .Lx446_0_s
.Lx446_0_s:             .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
n315_call_α:            sub              rsp, 16
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
.Lrkfnzd448:            .string          "DIFFER"
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
                        cmp              eax, 104;                  jne   .Lx447_240
                        add              rsp, 16;                   jmp   n314_lit_string_β
.Lx447_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n316_statement_end_α
n315_call_β:            add              rsp, 16;                   jmp   n314_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_end_α:   add              rsp, 96;                   jmp   n317_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/001: arg(.jlab,1) = A'     :(END)
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_begin_α:                                             jmp   n318_lit_string_α
n317_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n318_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx453_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n319_assign_α
.Lx453_0:               .quad            .Lx453_0_s
.Lx453_0_s:             .string          "FAIL 1017/001: arg(.jlab,1) = A"
#-----------------------------------------------------------------------------------------------------------------------
n319_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx454_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n320_statement_end_α
.Lx454_0:               .quad            .Lx454_0_s
.Lx454_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n321_statement_begin_α:                                             jmp   n322_statement_end_α
n321_statement_begin_β:                                             jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:                                               jmp   n323_statement_begin_α
#=======================================================================================================================
#         DIFFER(ARG(.jlab, 3), 'C')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n323_statement_begin_α:                                             jmp   n324_lit_string_α
n323_statement_begin_β:                                             jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx463_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n325_call_α
.Lx463_0:               .quad            .Lx463_0_s
.Lx463_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n325_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd465:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd465]
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
                        cmp              eax, 104;                  jne   .Lx464_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n323_statement_begin_β
.Lx464_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n326_lit_integer_α
n325_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n323_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n326_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx466_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n327_call_α
n326_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n323_statement_begin_β
.Lx466_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n327_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        lea              rdi, [rip + .Lbynamefnzd24]
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
                        cmp              eax, 104;                  jne   .Lx467_240
                        add              rsp, 16;                   jmp   n326_lit_integer_β
.Lx467_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n328_lit_string_α
n327_call_β:            add              rsp, 16;                   jmp   n326_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n328_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx468_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n329_call_α
n328_lit_string_β:      add              rsp, 32;                   jmp   n326_lit_integer_β
.Lx468_0:               .quad            .Lx468_0_s
.Lx468_0_s:             .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n329_call_α:            sub              rsp, 16
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
.Lrkfnzd470:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd470]
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
                        cmp              eax, 104;                  jne   .Lx469_240
                        add              rsp, 16;                   jmp   n328_lit_string_β
.Lx469_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n330_statement_end_α
n329_call_β:            add              rsp, 16;                   jmp   n328_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n330_statement_end_α:   add              rsp, 96;                   jmp   n331_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/002: arg(.jlab,3) = C'     :(END)
#-----------------------------------------------------------------------------------------------------------------------
n331_statement_begin_α:                                             jmp   n332_lit_string_α
n331_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n332_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx475_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n333_assign_α
.Lx475_0:               .quad            .Lx475_0_s
.Lx475_0_s:             .string          "FAIL 1017/002: arg(.jlab,3) = C"
#-----------------------------------------------------------------------------------------------------------------------
n333_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx476_0]
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
.Lx476_0:               .quad            .Lx476_0_s
.Lx476_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α:                                             jmp   n336_statement_end_α
n335_statement_begin_β:                                             jmp   n337_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_statement_end_α:                                               jmp   n337_statement_begin_α
#=======================================================================================================================
#         ARG(.jlab, 0)              :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n337_statement_begin_α:                                             jmp   n338_lit_string_α
n337_statement_begin_β:                                             jmp   n347_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n338_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx485_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n339_call_α
.Lx485_0:               .quad            .Lx485_0_s
.Lx485_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n339_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd487:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd487]
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
                        cmp              eax, 104;                  jne   .Lx486_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n337_statement_begin_β
.Lx486_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n340_lit_integer_α
n339_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n337_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n340_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx488_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n341_call_α
n340_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n337_statement_begin_β
.Lx488_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n341_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        lea              rdi, [rip + .Lbynamefnzd38]
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
                        cmp              eax, 104;                  jne   .Lx489_240
                        add              rsp, 16;                   jmp   n340_lit_integer_β
.Lx489_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n342_statement_end_α
n341_call_β:            add              rsp, 16;                   jmp   n340_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n342_statement_end_α:   add              rsp, 64;                   jmp   n343_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/003: arg(.jlab,0) OOB should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n343_statement_begin_α:                                             jmp   n344_lit_string_α
n343_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n344_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx494_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n345_assign_α
.Lx494_0:               .quad            .Lx494_0_s
.Lx494_0_s:             .string          "FAIL 1017/003: arg(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n345_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx495_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n346_statement_end_α
.Lx495_0:               .quad            .Lx495_0_s
.Lx495_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n346_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_begin_α:                                             jmp   n348_statement_end_α
n347_statement_begin_β:                                             jmp   n349_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n348_statement_end_α:                                               jmp   n349_statement_begin_α
#=======================================================================================================================
#         ARG(.jlab, 4)              :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n349_statement_begin_α:                                             jmp   n350_lit_string_α
n349_statement_begin_β:                                             jmp   n359_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n350_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx504_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n351_call_α
.Lx504_0:               .quad            .Lx504_0_s
.Lx504_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n351_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd506:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd506]
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
                        cmp              eax, 104;                  jne   .Lx505_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n349_statement_begin_β
.Lx505_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n352_lit_integer_α
n351_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n349_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n352_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx507_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n353_call_α
n352_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n349_statement_begin_β
.Lx507_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n353_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        lea              rdi, [rip + .Lbynamefnzd50]
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
                        cmp              eax, 104;                  jne   .Lx508_240
                        add              rsp, 16;                   jmp   n352_lit_integer_β
.Lx508_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n354_statement_end_α
n353_call_β:            add              rsp, 16;                   jmp   n352_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n354_statement_end_α:   add              rsp, 64;                   jmp   n355_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/004: arg(.jlab,4) OOB should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n355_statement_begin_α:                                             jmp   n356_lit_string_α
n355_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n356_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx513_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n357_assign_α
.Lx513_0:               .quad            .Lx513_0_s
.Lx513_0_s:             .string          "FAIL 1017/004: arg(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n357_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n358_statement_end_α
.Lx514_0:               .quad            .Lx514_0_s
.Lx514_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n358_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n359_statement_begin_α:                                             jmp   n360_statement_end_α
n359_statement_begin_β:                                             jmp   n361_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n360_statement_end_α:                                               jmp   n361_statement_begin_α
#=======================================================================================================================
#         DIFFER(LOCAL(.jlab, 1), 'D')                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n361_statement_begin_α:                                             jmp   n362_lit_string_α
n361_statement_begin_β:                                             jmp   n373_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n362_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx523_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n363_call_α
.Lx523_0:               .quad            .Lx523_0_s
.Lx523_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n363_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd525:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd525]
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
                        cmp              eax, 104;                  jne   .Lx524_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n361_statement_begin_β
.Lx524_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n364_lit_integer_α
n363_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n361_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx526_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n365_call_α
n364_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n361_statement_begin_β
.Lx526_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n365_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        lea              rdi, [rip + .Lbynamefnzd62]
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
                        cmp              eax, 104;                  jne   .Lx527_240
                        add              rsp, 16;                   jmp   n364_lit_integer_β
.Lx527_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n366_lit_string_α
n365_call_β:            add              rsp, 16;                   jmp   n364_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n366_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx528_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n367_call_α
n366_lit_string_β:      add              rsp, 32;                   jmp   n364_lit_integer_β
.Lx528_0:               .quad            .Lx528_0_s
.Lx528_0_s:             .string          "D"
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
.Lrkfnzd530:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd530]
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
                        cmp              eax, 104;                  jne   .Lx529_240
                        add              rsp, 16;                   jmp   n366_lit_string_β
.Lx529_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n368_statement_end_α
n367_call_β:            add              rsp, 16;                   jmp   n366_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n368_statement_end_α:   add              rsp, 96;                   jmp   n369_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/005: local(.jlab,1) = D'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n369_statement_begin_α:                                             jmp   n370_lit_string_α
n369_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n370_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx535_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n371_assign_α
.Lx535_0:               .quad            .Lx535_0_s
.Lx535_0_s:             .string          "FAIL 1017/005: local(.jlab,1) = D"
#-----------------------------------------------------------------------------------------------------------------------
n371_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n372_statement_end_α
.Lx536_0:               .quad            .Lx536_0_s
.Lx536_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n372_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n373_statement_begin_α:                                             jmp   n374_statement_end_α
n373_statement_begin_β:                                             jmp   n375_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n374_statement_end_α:                                               jmp   n375_statement_begin_α
#=======================================================================================================================
#         DIFFER(LOCAL(.jlab, 3), 'F')                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n375_statement_begin_α:                                             jmp   n376_lit_string_α
n375_statement_begin_β:                                             jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n376_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx545_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n377_call_α
.Lx545_0:               .quad            .Lx545_0_s
.Lx545_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n377_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd547:            .string          "SNO$NAME"
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
                        cmp              eax, 104;                  jne   .Lx546_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n375_statement_begin_β
.Lx546_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n378_lit_integer_α
n377_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n375_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n378_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx548_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n379_call_α
n378_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n375_statement_begin_β
.Lx548_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n379_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        lea              rdi, [rip + .Lbynamefnzd76]
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
                        cmp              eax, 104;                  jne   .Lx549_240
                        add              rsp, 16;                   jmp   n378_lit_integer_β
.Lx549_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n380_lit_string_α
n379_call_β:            add              rsp, 16;                   jmp   n378_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n380_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx550_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n381_call_α
n380_lit_string_β:      add              rsp, 32;                   jmp   n378_lit_integer_β
.Lx550_0:               .quad            .Lx550_0_s
.Lx550_0_s:             .string          "F"
#-----------------------------------------------------------------------------------------------------------------------
n381_call_α:            sub              rsp, 16
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
.Lrkfnzd552:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd552]
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
                        cmp              eax, 104;                  jne   .Lx551_240
                        add              rsp, 16;                   jmp   n380_lit_string_β
.Lx551_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n382_statement_end_α
n381_call_β:            add              rsp, 16;                   jmp   n380_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n382_statement_end_α:   add              rsp, 96;                   jmp   n383_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/006: local(.jlab,3) = F'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n383_statement_begin_α:                                             jmp   n384_lit_string_α
n383_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n384_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx557_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n385_assign_α
.Lx557_0:               .quad            .Lx557_0_s
.Lx557_0_s:             .string          "FAIL 1017/006: local(.jlab,3) = F"
#-----------------------------------------------------------------------------------------------------------------------
n385_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n386_statement_end_α
.Lx558_0:               .quad            .Lx558_0_s
.Lx558_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n386_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n387_statement_begin_α:                                             jmp   n388_statement_end_α
n387_statement_begin_β:                                             jmp   n389_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n388_statement_end_α:                                               jmp   n389_statement_begin_α
#=======================================================================================================================
#         LOCAL(.jlab, 0)            :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n389_statement_begin_α:                                             jmp   n390_lit_string_α
n389_statement_begin_β:                                             jmp   n399_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n390_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx567_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n391_call_α
.Lx567_0:               .quad            .Lx567_0_s
.Lx567_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n391_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd569:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd569]
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
                        cmp              eax, 104;                  jne   .Lx568_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n389_statement_begin_β
.Lx568_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n392_lit_integer_α
n391_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n389_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n392_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx570_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n393_call_α
n392_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n389_statement_begin_β
.Lx570_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n393_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        lea              rdi, [rip + .Lbynamefnzd90]
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
                        cmp              eax, 104;                  jne   .Lx571_240
                        add              rsp, 16;                   jmp   n392_lit_integer_β
.Lx571_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n394_statement_end_α
n393_call_β:            add              rsp, 16;                   jmp   n392_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n394_statement_end_α:   add              rsp, 64;                   jmp   n395_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/007: local(.jlab,0) OOB should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n395_statement_begin_α:                                             jmp   n396_lit_string_α
n395_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n396_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx576_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n397_assign_α
.Lx576_0:               .quad            .Lx576_0_s
.Lx576_0_s:             .string          "FAIL 1017/007: local(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n397_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx577_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n398_statement_end_α
.Lx577_0:               .quad            .Lx577_0_s
.Lx577_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n398_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n399_statement_begin_α:                                             jmp   n400_statement_end_α
n399_statement_begin_β:                                             jmp   n401_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n400_statement_end_α:                                               jmp   n401_statement_begin_α
#=======================================================================================================================
#         LOCAL(.jlab, 4)            :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n401_statement_begin_α:                                             jmp   n402_lit_string_α
n401_statement_begin_β:                                             jmp   n411_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n402_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx586_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n403_call_α
.Lx586_0:               .quad            .Lx586_0_s
.Lx586_0_s:             .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n403_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd588:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd588]
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
                        cmp              eax, 104;                  jne   .Lx587_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n401_statement_begin_β
.Lx587_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n404_lit_integer_α
n403_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n401_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n404_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx589_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n405_call_α
n404_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n401_statement_begin_β
.Lx589_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n405_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        lea              rdi, [rip + .Lbynamefnzd102]
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
                        cmp              eax, 104;                  jne   .Lx590_240
                        add              rsp, 16;                   jmp   n404_lit_integer_β
.Lx590_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n406_statement_end_α
n405_call_β:            add              rsp, 16;                   jmp   n404_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n406_statement_end_α:   add              rsp, 64;                   jmp   n407_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1017/008: local(.jlab,4) OOB should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n407_statement_begin_α:                                             jmp   n408_lit_string_α
n407_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n408_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx595_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n409_assign_α
.Lx595_0:               .quad            .Lx595_0_s
.Lx595_0_s:             .string          "FAIL 1017/008: local(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n409_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx596_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n410_statement_end_α
.Lx596_0:               .quad            .Lx596_0_s
.Lx596_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n410_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n411_statement_begin_α:                                             jmp   n412_statement_end_α
n411_statement_begin_β:                                             jmp   n413_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n412_statement_end_α:                                               jmp   n413_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1017_arg_local (8/8)'
#-----------------------------------------------------------------------------------------------------------------------
n413_statement_begin_α:                                             jmp   n414_lit_string_α
n413_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n414_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx605_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n415_assign_α
.Lx605_0:               .quad            .Lx605_0_s
.Lx605_0_s:             .string          "PASS 1017_arg_local (8/8)"
#-----------------------------------------------------------------------------------------------------------------------
n415_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx606_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n416_statement_end_α
.Lx606_0:               .quad            .Lx606_0_s
.Lx606_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n416_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n417_goto_α:                                                        jmp   n321_statement_begin_α
n417_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n418_goto_α:                                                        jmp   n335_statement_begin_α
n418_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n419_goto_α:                                                        jmp   n347_statement_begin_α
n419_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n420_goto_α:                                                        jmp   n359_statement_begin_α
n420_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n421_goto_α:                                                        jmp   n373_statement_begin_α
n421_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n422_goto_α:                                                        jmp   n387_statement_begin_α
n422_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n423_goto_α:                                                        jmp   n399_statement_begin_α
n423_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n424_goto_α:                                                        jmp   n411_statement_begin_α
n424_goto_β:                                                        jmp   main_ω
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

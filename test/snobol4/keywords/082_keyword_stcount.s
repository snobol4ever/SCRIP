                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "X"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 1
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 1
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         X = 1
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n1_call_α
.Lx39_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n1_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd41:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd41]
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
                        cmp              eax, 104;                  jne   .Lx40_240
                        add              rsp, 16;                   jmp   n2_statement_begin_α
.Lx40_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_statement_begin_α
n1_call_β:              add              rsp, 16;                   jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_integer_α
n2_statement_begin_β:   add              rsp, 32;                   jmp   n6_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_assign_α
.Lx44_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:            mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # X
                        mov              qword ptr [r9 + 8], rdx;   jmp   n5_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     add              rsp, 48;                   jmp   n6_lit_integer_α
#=======================================================================================================================
#         X = 2
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx48_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n7_call_α
.Lx48_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd50:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd50]
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
                        cmp              eax, 104;                  jne   .Lx49_240
                        add              rsp, 16;                   jmp   n8_statement_begin_α
.Lx49_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n8_statement_begin_α
n7_call_β:              add              rsp, 16;                   jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:                                               jmp   n9_lit_integer_α
n8_statement_begin_β:   add              rsp, 32;                   jmp   n12_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n10_assign_α
.Lx53_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:           mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # X
                        mov              qword ptr [r9 + 8], rdx;   jmp   n11_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    add              rsp, 48;                   jmp   n12_lit_integer_α
#=======================================================================================================================
#         GT(&STNO, 1)                                                :S(YES)F(NO)
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx57_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n13_call_α
.Lx57_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd59:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd59]
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
                        cmp              eax, 104;                  jne   .Lx58_240
                        add              rsp, 16;                   jmp   n14_statement_begin_α
.Lx58_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n14_statement_begin_α
n13_call_β:             add              rsp, 16;                   jmp   n14_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:                                              jmp   n15_keyword_snobol4_α
n14_statement_begin_β:  add              rsp, 32;                   jmp   n31_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n15_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx62_0]
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n16_lit_integer_α
.Lx62_0:                .quad            .Lx62_0_s
.Lx62_0_s:              .string          "STNO"
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx63_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n17_coerce_numeric_α
n16_lit_integer_β:      add              rsp, 16
                        add              rsp, 48;                   jmp   n14_statement_begin_β
.Lx63_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n17_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # keyword_snobol4
                        cmp              eax, 5;                    je    .Lx65_1
                        cmp              eax, 3;                    jne   .Lx65_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx65_0
.Lx65_1:                mov              rax, qword ptr [rsp + 32]                      # keyword_snobol4
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # keyword_snobol4
                        mov              qword ptr [rsp + 8], rax;  jmp   n18_coerce_numeric_α
.Lx65_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                                # lit_integer
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n18_coerce_numeric_α
n17_coerce_numeric_β:   add              rsp, 16;                   jmp   n16_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n18_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5;                    je    .Lx67_1
                        cmp              eax, 3;                    jne   .Lx67_0
                        mov              eax, dword ptr [rsp + 48]                      # keyword_snobol4
                        cmp              eax, 3;                    jne   .Lx67_0
.Lx67_1:                mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n19_cmp_test_α
.Lx67_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                                # keyword_snobol4
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n19_cmp_test_α
n18_coerce_numeric_β:   add              rsp, 16;                   jmp   n17_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n19_cmp_test_α:         sub              rsp, 16
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
                        test             eax, eax;                  jg    .Lx69_240
                        add              rsp, 16;                   jmp   n18_coerce_numeric_β
.Lx69_240:                                                          jmp   n20_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:    add              rsp, 112;                  jmp   n21_lit_integer_α
#=======================================================================================================================
# YES     OUTPUT = 'stno ok'
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx72_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n22_call_α
.Lx72_0:                .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd74:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd74]
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
                        cmp              eax, 104;                  jne   .Lx73_240
                        add              rsp, 16;                   jmp   n23_statement_begin_α
.Lx73_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n23_statement_begin_α
n22_call_β:             add              rsp, 16;                   jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                              jmp   n24_lit_string_α
n23_statement_begin_β:  add              rsp, 32;                   jmp   n27_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx77_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n25_assign_α
.Lx77_0:                .quad            .Lx77_0_s
.Lx77_0_s:              .string          "stno ok"
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx78_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n26_statement_end_α
.Lx78_0:                .quad            .Lx78_0_s
.Lx78_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:    add              rsp, 48;                   jmp   n27_lit_integer_α
#=======================================================================================================================
#         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx81_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n28_call_α
.Lx81_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n28_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd83:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd83]
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
                        cmp              eax, 104;                  jne   .Lx82_240
                        add              rsp, 16;                   jmp   n29_statement_begin_α
.Lx82_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n29_statement_begin_α
n28_call_β:             add              rsp, 16;                   jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                              jmp   n30_statement_end_α
n29_statement_begin_β:  add              rsp, 32;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    add              rsp, 32;                   jmp   main_γ
#=======================================================================================================================
# NO      OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n32_call_α
.Lx88_0:                .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n32_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd90:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd90]
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
                        cmp              eax, 104;                  jne   .Lx89_240
                        add              rsp, 16;                   jmp   n33_statement_begin_α
.Lx89_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n33_statement_begin_α
n32_call_β:             add              rsp, 16;                   jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                              jmp   n34_lit_string_α
n33_statement_begin_β:  add              rsp, 32;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx93_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n35_assign_α
.Lx93_0:                .quad            .Lx93_0_s
.Lx93_0_s:              .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx94_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n36_statement_end_α
.Lx94_0:                .quad            .Lx94_0_s
.Lx94_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    add              rsp, 48;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_α:                                                         jmp   n21_lit_integer_α
n37_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n38_goto_α:                                                         jmp   n31_lit_integer_α
n38_goto_β:                                                         jmp   main_ω
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

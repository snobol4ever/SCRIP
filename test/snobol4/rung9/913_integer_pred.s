                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_statement_end_α
n0_statement_begin_β:                                               jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                 jmp   n2_statement_begin_α
#=======================================================================================================================
#         INTEGER('abc')             :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_string_α
n2_statement_begin_β:                                               jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_call_α
.Lx45_0:                .quad            .Lx45_0_s
.Lx45_0_s:              .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd47:             .string          "INTEGER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd47]
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
                        cmp              eax, 104;                  jne   .Lx46_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx46_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n5_statement_end_α
n4_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     add              rsp, 32;                   jmp   n6_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 913/001: integer(string) should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:                                               jmp   n7_lit_string_α
n6_statement_begin_β:                                               jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 41
                        mov              rax, qword ptr [rip + .Lx52_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n8_assign_α
.Lx52_0:                .quad            .Lx52_0_s
.Lx52_0_s:              .string          "FAIL 913/001: integer(string) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:            mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx53_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n9_statement_end_α
.Lx53_0:                .quad            .Lx53_0_s
.Lx53_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:                                              jmp   n11_statement_end_α
n10_statement_begin_β:                                              jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:                                                jmp   n12_statement_begin_α
#=======================================================================================================================
#         INTEGER(12)                :s(e002)
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:                                              jmp   n13_lit_integer_α
n12_statement_begin_β:                                              jmp   n32_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n14_call_α
.Lx62_0:                .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd64:             .string          "INTEGER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd64]
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
                        cmp              eax, 104;                  jne   .Lx63_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n12_statement_begin_β
.Lx63_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n15_statement_end_α
n14_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:                                                jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:                                              jmp   n17_statement_end_α
n16_statement_begin_β:  add              rsp, 32;                   jmp   n18_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    add              rsp, 32;                   jmp   n18_statement_begin_α
#=======================================================================================================================
#         INTEGER('12')              :s(e003)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:                                              jmp   n19_lit_string_α
n18_statement_begin_β:                                              jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n20_call_α
.Lx73_0:                .quad            .Lx73_0_s
.Lx73_0_s:              .string          "12"
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd75:             .string          "INTEGER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd75]
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
                        cmp              eax, 104;                  jne   .Lx74_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n18_statement_begin_β
.Lx74_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n21_statement_end_α
n20_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n18_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:                                                jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                              jmp   n23_statement_end_α
n22_statement_begin_β:  add              rsp, 32;                   jmp   n24_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:    add              rsp, 32;                   jmp   n24_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 913_integer_pred (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:                                              jmp   n25_lit_string_α
n24_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n26_assign_α
.Lx84_0:                .quad            .Lx84_0_s
.Lx84_0_s:              .string          "PASS 913_integer_pred (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx85_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n27_statement_end_α
.Lx85_0:                .quad            .Lx85_0_s
.Lx85_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#=======================================================================================================================
#         OUTPUT = 'FAIL 913/003: integer(numeric-string) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                              jmp   n29_lit_string_α
n28_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 52
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n30_assign_α
.Lx90_0:                .quad            .Lx90_0_s
.Lx90_0_s:              .string          "FAIL 913/003: integer(numeric-string) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx91_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n31_statement_end_α
.Lx91_0:                .quad            .Lx91_0_s
.Lx91_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#=======================================================================================================================
#         OUTPUT = 'FAIL 913/002: integer(12) should succeed'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:                                              jmp   n33_lit_string_α
n32_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 40
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n34_assign_α
.Lx96_0:                .quad            .Lx96_0_s
.Lx96_0_s:              .string          "FAIL 913/002: integer(12) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n34_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx97_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n35_statement_end_α
.Lx97_0:                .quad            .Lx97_0_s
.Lx97_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n36_goto_α:                                                         jmp   n10_statement_begin_α
n36_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_α:                                                         jmp   n16_statement_begin_α
n37_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n38_goto_α:                                                         jmp   n22_statement_begin_α
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

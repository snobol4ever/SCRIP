                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "a"
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
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_statement_end_α
n0_statement_begin_β:                                               jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                 jmp   n2_statement_begin_α
#=======================================================================================================================
#         DIFFER(REPLACE('axxbyyy', 'xy', '01'), 'a00b111')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_string_α
n2_statement_begin_β:                                               jmp   n14_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_lit_string_α
.Lx64_0:                .quad            .Lx64_0_s
.Lx64_0_s:              .string          "axxbyyy"
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n5_lit_string_α
n4_lit_string_β:        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx65_0:                .quad            .Lx65_0_s
.Lx65_0_s:              .string          "xy"
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx66_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n6_call_α
n5_lit_string_β:        add              rsp, 16;                   jmp   n4_lit_string_β
.Lx66_0:                .quad            .Lx66_0_s
.Lx66_0_s:              .string          "01"
#-----------------------------------------------------------------------------------------------------------------------
n6_call_α:              sub              rsp, 16
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
.Lrkfnzd68:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd68]
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
                        cmp              eax, 104;                  jne   .Lx67_240
                        add              rsp, 16;                   jmp   n5_lit_string_β
.Lx67_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n7_lit_string_α
n6_call_β:              add              rsp, 16;                   jmp   n5_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx69_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n8_call_α
n7_lit_string_β:        add              rsp, 32;                   jmp   n5_lit_string_β
.Lx69_0:                .quad            .Lx69_0_s
.Lx69_0_s:              .string          "a00b111"
#-----------------------------------------------------------------------------------------------------------------------
n8_call_α:              sub              rsp, 16
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
.Lrkfnzd71:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd71]
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
                        cmp              eax, 104;                  jne   .Lx70_240
                        add              rsp, 16;                   jmp   n7_lit_string_β
.Lx70_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n9_statement_end_α
n8_call_β:              add              rsp, 16;                   jmp   n7_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     add              rsp, 96;                   jmp   n10_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 810/001: xy->01 mapping'             :(END)
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:                                              jmp   n11_lit_string_α
n10_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n12_assign_α
.Lx76_0:                .quad            .Lx76_0_s
.Lx76_0_s:              .string          "FAIL 810/001: xy->01 mapping"
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx77_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n13_statement_end_α
.Lx77_0:                .quad            .Lx77_0_s
.Lx77_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:                                              jmp   n15_statement_end_α
n14_statement_begin_β:                                              jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:                                                jmp   n16_statement_begin_α
#=======================================================================================================================
#         a = REPLACE(&ALPHABET, 'xy', 'ab')
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:                                              jmp   n17_keyword_snobol4_α
n16_statement_begin_β:                                              jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx86_0]
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n18_lit_string_α
.Lx86_0:                .quad            .Lx86_0_s
.Lx86_0_s:              .string          "ALPHABET"
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n19_lit_string_α
n18_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n16_statement_begin_β
.Lx87_0:                .quad            .Lx87_0_s
.Lx87_0_s:              .string          "xy"
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n20_call_α
n19_lit_string_β:       add              rsp, 16;                   jmp   n18_lit_string_β
.Lx88_0:                .quad            .Lx88_0_s
.Lx88_0_s:              .string          "ab"
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:             sub              rsp, 16
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
.Lrkfnzd90:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd90]
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
                        cmp              eax, 104;                  jne   .Lx89_240
                        add              rsp, 16;                   jmp   n19_lit_string_β
.Lx89_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n21_assign_α
n20_call_β:             add              rsp, 16;                   jmp   n19_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # a
                        mov              qword ptr [r9 + 8], rdx;   jmp   n22_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:    add              rsp, 64;                   jmp   n23_statement_begin_α
#=======================================================================================================================
#         DIFFER(REPLACE('axy', &ALPHABET, a), 'aab')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                              jmp   n24_lit_string_α
n23_statement_begin_β:                                              jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n25_keyword_snobol4_α
.Lx96_0:                .quad            .Lx96_0_s
.Lx96_0_s:              .string          "axy"
#-----------------------------------------------------------------------------------------------------------------------
n25_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx97_0]
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n26_var_α
.Lx97_0:                .quad            .Lx97_0_s
.Lx97_0_s:              .string          "ALPHABET"
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n27_call_α
n26_var_β:              add              rsp, 16
                        add              rsp, 32;                   jmp   n23_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n27_call_α:             sub              rsp, 16
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
.Lrkfnzd100:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd100]
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
                        cmp              eax, 104;                  jne   .Lx99_240
                        add              rsp, 16;                   jmp   n26_var_β
.Lx99_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n28_lit_string_α
n27_call_β:             add              rsp, 16;                   jmp   n26_var_β
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n29_call_α
n28_lit_string_β:       add              rsp, 32;                   jmp   n26_var_β
.Lx101_0:               .quad            .Lx101_0_s
.Lx101_0_s:             .string          "aab"
#-----------------------------------------------------------------------------------------------------------------------
n29_call_α:             sub              rsp, 16
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
.Lrkfnzd103:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd103]
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
                        cmp              eax, 104;                  jne   .Lx102_240
                        add              rsp, 16;                   jmp   n28_lit_string_β
.Lx102_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n30_statement_end_α
n29_call_β:             add              rsp, 16;                   jmp   n28_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    add              rsp, 96;                   jmp   n31_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 810/002: alphabet translation'       :(END)
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:                                              jmp   n32_lit_string_α
n31_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx108_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n33_assign_α
.Lx108_0:               .quad            .Lx108_0_s
.Lx108_0_s:             .string          "FAIL 810/002: alphabet translation"
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx109_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n34_statement_end_α
.Lx109_0:               .quad            .Lx109_0_s
.Lx109_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:                                              jmp   n36_statement_end_α
n35_statement_begin_β:                                              jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:                                                jmp   n37_statement_begin_α
#=======================================================================================================================
#         DIFFER(REPLACE('hello', 'aeiou', 'aeiou'), 'hello')                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:                                              jmp   n38_lit_string_α
n37_statement_begin_β:                                              jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n39_lit_string_α
.Lx118_0:               .quad            .Lx118_0_s
.Lx118_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx119_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n40_lit_string_α
n39_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n37_statement_begin_β
.Lx119_0:               .quad            .Lx119_0_s
.Lx119_0_s:             .string          "aeiou"
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n41_call_α
n40_lit_string_β:       add              rsp, 16;                   jmp   n39_lit_string_β
.Lx120_0:               .quad            .Lx120_0_s
.Lx120_0_s:             .string          "aeiou"
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
.Lrkfnzd122:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd122]
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
                        cmp              eax, 104;                  jne   .Lx121_240
                        add              rsp, 16;                   jmp   n40_lit_string_β
.Lx121_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n42_lit_string_α
n41_call_β:             add              rsp, 16;                   jmp   n40_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n43_call_α
n42_lit_string_β:       add              rsp, 32;                   jmp   n40_lit_string_β
.Lx123_0:               .quad            .Lx123_0_s
.Lx123_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n43_call_α:             sub              rsp, 16
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
.Lrkfnzd125:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd125]
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
                        cmp              eax, 104;                  jne   .Lx124_240
                        add              rsp, 16;                   jmp   n42_lit_string_β
.Lx124_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n44_statement_end_α
n43_call_β:             add              rsp, 16;                   jmp   n42_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:    add              rsp, 96;                   jmp   n45_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 810/003: identity replace'           :(END)
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:                                              jmp   n46_lit_string_α
n45_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx130_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n47_assign_α
.Lx130_0:               .quad            .Lx130_0_s
.Lx130_0_s:             .string          "FAIL 810/003: identity replace"
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx131_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n48_statement_end_α
.Lx131_0:               .quad            .Lx131_0_s
.Lx131_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:                                              jmp   n50_statement_end_α
n49_statement_begin_β:                                              jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:                                                jmp   n51_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 810_replace (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:                                              jmp   n52_lit_string_α
n51_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 22
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n53_assign_α
.Lx140_0:               .quad            .Lx140_0_s
.Lx140_0_s:             .string          "PASS 810_replace (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx141_0]
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
.Lx141_0:               .quad            .Lx141_0_s
.Lx141_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n55_goto_α:                                                         jmp   n14_statement_begin_α
n55_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n56_goto_α:                                                         jmp   n35_statement_begin_α
n56_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n57_goto_α:                                                         jmp   n49_statement_begin_α
n57_goto_β:                                                         jmp   main_ω
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

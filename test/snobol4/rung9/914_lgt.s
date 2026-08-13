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
#         LGT('abc', 'xyz')          :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_string_α
n2_statement_begin_β:                                               jmp   n11_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx72_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_lit_string_α
.Lx72_0:                .quad            .Lx72_0_s
.Lx72_0_s:              .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n5_cmp_test_α
n4_lit_string_β:        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx73_0:                .quad            .Lx73_0_s
.Lx73_0_s:              .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
n5_cmp_test_α:          sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # lit_string
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
                        test             eax, eax;                  jg    .Lx75_240
                        add              rsp, 16;                   jmp   n4_lit_string_β
.Lx75_240:                                                          jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 48;                   jmp   n7_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 914/001: lgt(abc,xyz) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                               jmp   n8_lit_string_α
n7_statement_begin_β:                                               jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n9_assign_α
.Lx80_0:                .quad            .Lx80_0_s
.Lx80_0_s:              .string          "FAIL 914/001: lgt(abc,xyz) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:            mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx81_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n10_statement_end_α
.Lx81_0:                .quad            .Lx81_0_s
.Lx81_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:                                              jmp   n12_statement_end_α
n11_statement_begin_β:                                              jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:                                                jmp   n13_statement_begin_α
#=======================================================================================================================
#         LGT('abc', 'abc')          :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:                                              jmp   n14_lit_string_α
n13_statement_begin_β:                                              jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n15_lit_string_α
.Lx90_0:                .quad            .Lx90_0_s
.Lx90_0_s:              .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n16_cmp_test_α
n15_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n13_statement_begin_β
.Lx91_0:                .quad            .Lx91_0_s
.Lx91_0_s:              .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n16_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # lit_string
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
                        test             eax, eax;                  jg    .Lx93_240
                        add              rsp, 16;                   jmp   n15_lit_string_β
.Lx93_240:                                                          jmp   n17_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    add              rsp, 48;                   jmp   n18_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 914/002: lgt(abc,abc) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:                                              jmp   n19_lit_string_α
n18_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n20_assign_α
.Lx98_0:                .quad            .Lx98_0_s
.Lx98_0_s:              .string          "FAIL 914/002: lgt(abc,abc) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx99_0]
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
.Lx99_0:                .quad            .Lx99_0_s
.Lx99_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                              jmp   n23_statement_end_α
n22_statement_begin_β:                                              jmp   n24_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:                                                jmp   n24_statement_begin_α
#=======================================================================================================================
#         LGT('xyz', 'abc')          :s(e003)
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:                                              jmp   n25_lit_string_α
n24_statement_begin_β:                                              jmp   n57_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx108_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n26_lit_string_α
.Lx108_0:               .quad            .Lx108_0_s
.Lx108_0_s:             .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n27_cmp_test_α
n26_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n24_statement_begin_β
.Lx109_0:               .quad            .Lx109_0_s
.Lx109_0_s:             .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n27_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # lit_string
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
                        test             eax, eax;                  jg    .Lx111_240
                        add              rsp, 16;                   jmp   n26_lit_string_β
.Lx111_240:                                                         jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:                                                jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                              jmp   n30_statement_end_α
n29_statement_begin_β:  add              rsp, 48;                   jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    add              rsp, 48;                   jmp   n31_statement_begin_α
#=======================================================================================================================
#         LGT('', 'abc')             :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:                                              jmp   n32_lit_string_α
n31_statement_begin_β:                                              jmp   n40_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n33_lit_string_α
.Lx120_0:               .quad            .Lx120_0_s
.Lx120_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n34_cmp_test_α
n33_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n31_statement_begin_β
.Lx121_0:               .quad            .Lx121_0_s
.Lx121_0_s:             .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n34_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # lit_string
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
                        test             eax, eax;                  jg    .Lx123_240
                        add              rsp, 16;                   jmp   n33_lit_string_β
.Lx123_240:                                                         jmp   n35_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:    add              rsp, 48;                   jmp   n36_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 914/004: lgt(null,abc) should fail'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:                                              jmp   n37_lit_string_α
n36_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx128_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n38_assign_α
.Lx128_0:               .quad            .Lx128_0_s
.Lx128_0_s:             .string          "FAIL 914/004: lgt(null,abc) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n38_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx129_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n39_statement_end_α
.Lx129_0:               .quad            .Lx129_0_s
.Lx129_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_begin_α:                                              jmp   n41_statement_end_α
n40_statement_begin_β:                                              jmp   n42_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_end_α:                                                jmp   n42_statement_begin_α
#=======================================================================================================================
#         LGT('abc', '')             :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:                                              jmp   n43_lit_string_α
n42_statement_begin_β:                                              jmp   n53_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n44_lit_string_α
.Lx138_0:               .quad            .Lx138_0_s
.Lx138_0_s:             .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n45_cmp_test_α
n44_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n42_statement_begin_β
.Lx139_0:               .quad            .Lx139_0_s
.Lx139_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n45_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # lit_string
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
                        test             eax, eax;                  jg    .Lx141_240
                        add              rsp, 16;                   jmp   n44_lit_string_β
.Lx141_240:                                                         jmp   n46_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_end_α:                                                jmp   n47_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_begin_α:                                              jmp   n48_statement_end_α
n47_statement_begin_β:  add              rsp, 48;                   jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    add              rsp, 48;                   jmp   n49_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 914_lgt (5/5)'
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:                                              jmp   n50_lit_string_α
n49_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 18
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n51_assign_α
.Lx150_0:               .quad            .Lx150_0_s
.Lx150_0_s:             .string          "PASS 914_lgt (5/5)"
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx151_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n52_statement_end_α
.Lx151_0:               .quad            .Lx151_0_s
.Lx151_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#=======================================================================================================================
#         OUTPUT = 'FAIL 914/005: lgt(abc,null) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:                                              jmp   n54_lit_string_α
n53_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 42
                        mov              rax, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n55_assign_α
.Lx156_0:               .quad            .Lx156_0_s
.Lx156_0_s:             .string          "FAIL 914/005: lgt(abc,null) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx157_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n56_statement_end_α
.Lx157_0:               .quad            .Lx157_0_s
.Lx157_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#=======================================================================================================================
#         OUTPUT = 'FAIL 914/003: lgt(xyz,abc) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                              jmp   n58_lit_string_α
n57_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 41
                        mov              rax, qword ptr [rip + .Lx162_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n59_assign_α
.Lx162_0:               .quad            .Lx162_0_s
.Lx162_0_s:             .string          "FAIL 914/003: lgt(xyz,abc) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx163_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n60_statement_end_α
.Lx163_0:               .quad            .Lx163_0_s
.Lx163_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n61_goto_α:                                                         jmp   n11_statement_begin_α
n61_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n62_goto_α:                                                         jmp   n22_statement_begin_α
n62_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n63_goto_α:                                                         jmp   n29_statement_begin_α
n63_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n64_goto_α:                                                         jmp   n40_statement_begin_α
n64_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n65_goto_α:                                                         jmp   n47_statement_begin_α
n65_goto_β:                                                         jmp   main_ω
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

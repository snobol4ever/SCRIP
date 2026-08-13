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
#         DIFFER(2.0 + 3.0, 5.0)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_real_α
n2_statement_begin_β:                                               jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_real_α:          sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_lit_real_α
.Lx97_0:                .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_real_α:          sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n5_binop_α
.Lx98_0:                .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n5_binop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                  jne   .Lx99_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n2_statement_begin_β
.Lx99_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n6_lit_real_α
n5_binop_β:             add              rsp, 16
                        add              rsp, 32;                   jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_real_α:          sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx100_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n7_call_α
.Lx100_0:               .quad            4617315517961601024
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:              sub              rsp, 16
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
.Lrkfnzd102:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd102]
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
                        cmp              eax, 104;                  jne   .Lx101_240
                        add              rsp, 32;                   jmp   n5_binop_β
.Lx101_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n8_statement_end_α
n7_call_β:              add              rsp, 32;                   jmp   n5_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:     add              rsp, 80;                   jmp   n9_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/001: 2.0+3.0'              :(END)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:                                               jmp   n10_lit_string_α
n9_statement_begin_β:                                               jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n11_assign_α
.Lx107_0:               .quad            .Lx107_0_s
.Lx107_0_s:             .string          "FAIL 412/001: 2.0+3.0"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx108_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n12_statement_end_α
.Lx108_0:               .quad            .Lx108_0_s
.Lx108_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:                                              jmp   n14_statement_end_α
n13_statement_begin_β:                                              jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:                                                jmp   n15_statement_begin_α
#=======================================================================================================================
#         DIFFER(3.0 - 1.0, 2.0)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                              jmp   n16_lit_real_α
n15_statement_begin_β:                                              jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx117_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n17_lit_real_α
.Lx117_0:               .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n18_binop_α
.Lx118_0:               .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  jne   .Lx119_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n15_statement_begin_β
.Lx119_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n19_lit_real_α
n18_binop_β:            add              rsp, 16
                        add              rsp, 32;                   jmp   n15_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n20_call_α
.Lx120_0:               .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:             sub              rsp, 16
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
.Lrkfnzd122:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd122]
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
                        cmp              eax, 104;                  jne   .Lx121_240
                        add              rsp, 32;                   jmp   n18_binop_β
.Lx121_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n21_statement_end_α
n20_call_β:             add              rsp, 32;                   jmp   n18_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    add              rsp, 80;                   jmp   n22_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/002: 3.0-1.0'              :(END)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                              jmp   n23_lit_string_α
n22_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx127_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n24_assign_α
.Lx127_0:               .quad            .Lx127_0_s
.Lx127_0_s:             .string          "FAIL 412/002: 3.0-1.0"
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx128_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n25_statement_end_α
.Lx128_0:               .quad            .Lx128_0_s
.Lx128_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                              jmp   n27_statement_end_α
n26_statement_begin_β:                                              jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:                                                jmp   n28_statement_begin_α
#=======================================================================================================================
#         DIFFER(3.0 * 2.0, 6.0)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                              jmp   n29_lit_real_α
n28_statement_begin_β:                                              jmp   n39_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n30_lit_real_α
.Lx137_0:               .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n31_binop_α
.Lx138_0:               .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n31_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                  jne   .Lx139_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n28_statement_begin_β
.Lx139_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n32_lit_real_α
n31_binop_β:            add              rsp, 16
                        add              rsp, 32;                   jmp   n28_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n33_call_α
.Lx140_0:               .quad            4618441417868443648
#-----------------------------------------------------------------------------------------------------------------------
n33_call_α:             sub              rsp, 16
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
.Lrkfnzd142:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd142]
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
                        cmp              eax, 104;                  jne   .Lx141_240
                        add              rsp, 32;                   jmp   n31_binop_β
.Lx141_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n34_statement_end_α
n33_call_β:             add              rsp, 32;                   jmp   n31_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    add              rsp, 80;                   jmp   n35_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/003: 3.0*2.0'              :(END)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:                                              jmp   n36_lit_string_α
n35_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx147_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n37_assign_α
.Lx147_0:               .quad            .Lx147_0_s
.Lx147_0_s:             .string          "FAIL 412/003: 3.0*2.0"
#-----------------------------------------------------------------------------------------------------------------------
n37_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx148_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n38_statement_end_α
.Lx148_0:               .quad            .Lx148_0_s
.Lx148_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:                                              jmp   n40_statement_end_α
n39_statement_begin_β:                                              jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:                                                jmp   n41_statement_begin_α
#=======================================================================================================================
#         DIFFER(3.0 / 2.0, 1.5)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:                                              jmp   n42_lit_real_α
n41_statement_begin_β:                                              jmp   n52_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n43_lit_real_α
.Lx157_0:               .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx158_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n44_binop_α
.Lx158_0:               .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n44_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_div@PLT
                        cmp              eax, 104;                  jne   .Lx159_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n41_statement_begin_β
.Lx159_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n45_lit_real_α
n44_binop_β:            add              rsp, 16
                        add              rsp, 32;                   jmp   n41_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n46_call_α
.Lx160_0:               .quad            4609434218613702656
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:             sub              rsp, 16
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
.Lrkfnzd162:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd162]
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
                        cmp              eax, 104;                  jne   .Lx161_240
                        add              rsp, 32;                   jmp   n44_binop_β
.Lx161_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n47_statement_end_α
n46_call_β:             add              rsp, 32;                   jmp   n44_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:    add              rsp, 80;                   jmp   n48_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/004: 3.0/2.0'              :(END)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:                                              jmp   n49_lit_string_α
n48_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n50_assign_α
.Lx167_0:               .quad            .Lx167_0_s
.Lx167_0_s:             .string          "FAIL 412/004: 3.0/2.0"
#-----------------------------------------------------------------------------------------------------------------------
n50_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx168_0]
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
.Lx168_0:               .quad            .Lx168_0_s
.Lx168_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:                                              jmp   n53_statement_end_α
n52_statement_begin_β:                                              jmp   n54_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:                                                jmp   n54_statement_begin_α
#=======================================================================================================================
#         DIFFER(3.0 ** 3, 27.0)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:                                              jmp   n55_lit_real_α
n54_statement_begin_β:                                              jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx177_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n56_lit_integer_α
.Lx177_0:               .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n57_binop_α
n56_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n54_statement_begin_β
.Lx178_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n57_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_pow@PLT
                        cmp              eax, 104;                  jne   .Lx179_240
                        add              rsp, 16;                   jmp   n56_lit_integer_β
.Lx179_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n58_lit_real_α
n57_binop_β:            add              rsp, 16;                   jmp   n56_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx180_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n59_call_α
.Lx180_0:               .quad            4628293042053316608
#-----------------------------------------------------------------------------------------------------------------------
n59_call_α:             sub              rsp, 16
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
.Lrkfnzd182:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd182]
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
                        cmp              eax, 104;                  jne   .Lx181_240
                        add              rsp, 32;                   jmp   n57_binop_β
.Lx181_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n60_statement_end_α
n59_call_β:             add              rsp, 32;                   jmp   n57_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 80;                   jmp   n61_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/005: 3.0**3'               :(END)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:                                              jmp   n62_lit_string_α
n61_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n63_assign_α
.Lx187_0:               .quad            .Lx187_0_s
.Lx187_0_s:             .string          "FAIL 412/005: 3.0**3"
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx188_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n64_statement_end_α
.Lx188_0:               .quad            .Lx188_0_s
.Lx188_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:                                              jmp   n66_statement_end_α
n65_statement_begin_β:                                              jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:                                                jmp   n67_statement_begin_α
#=======================================================================================================================
#         DIFFER(-1.0, 0.0 - 1.0)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:                                              jmp   n68_lit_real_α
n67_statement_begin_β:                                              jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n69_unop_α
.Lx197_0:               .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n69_unop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_real
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n70_lit_real_α
n69_unop_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n67_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n71_lit_real_α
.Lx199_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n72_binop_α
.Lx200_0:               .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n72_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  jne   .Lx201_240
                        add              rsp, 48;                   jmp   n69_unop_β
.Lx201_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n73_call_α
n72_binop_β:            add              rsp, 48;                   jmp   n69_unop_β
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:             sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd203:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd203]
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
                        cmp              eax, 104;                  jne   .Lx202_240
                        add              rsp, 16;                   jmp   n72_binop_β
.Lx202_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n74_statement_end_α
n73_call_β:             add              rsp, 16;                   jmp   n72_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_end_α:    add              rsp, 96;                   jmp   n75_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/006: unary minus on real'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_begin_α:                                              jmp   n76_lit_string_α
n75_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n77_assign_α
.Lx208_0:               .quad            .Lx208_0_s
.Lx208_0_s:             .string          "FAIL 412/006: unary minus on real"
#-----------------------------------------------------------------------------------------------------------------------
n77_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx209_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n78_statement_end_α
.Lx209_0:               .quad            .Lx209_0_s
.Lx209_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:                                              jmp   n80_statement_end_α
n79_statement_begin_β:                                              jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:                                                jmp   n81_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 412_arith_real (6/6)'
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:                                              jmp   n82_lit_string_α
n81_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx218_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n83_assign_α
.Lx218_0:               .quad            .Lx218_0_s
.Lx218_0_s:             .string          "PASS 412_arith_real (6/6)"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx219_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n84_statement_end_α
.Lx219_0:               .quad            .Lx219_0_s
.Lx219_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n85_goto_α:                                                         jmp   n13_statement_begin_α
n85_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n86_goto_α:                                                         jmp   n26_statement_begin_α
n86_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n87_goto_α:                                                         jmp   n39_statement_begin_α
n87_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n88_goto_α:                                                         jmp   n52_statement_begin_α
n88_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n89_goto_α:                                                         jmp   n65_statement_begin_α
n89_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n90_goto_α:                                                         jmp   n79_statement_begin_α
n90_goto_β:                                                         jmp   main_ω
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

                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
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
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         <stmt 2, line 41: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_define_α
n2_statement_begin_β:                                                         jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_define_α:            mov              rdi, qword ptr [rip + .Lx197_0]
                        mov              rsi, qword ptr [rip + .Lx197_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n5_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx197_0]
                        lea              rsi, [rip + max_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n4_statement_end_α
n3_define_β:                                                                  jmp   n2_statement_begin_β
.Lx197_0:               .quad            .Lx197_0_s
.Lx197_0_s:             .string          "max"
.Lx197_1:               .quad            .Lx197_1_s
.Lx197_1_s:             .string          "max,x"
                                                                              jmp   .Lx198_245
#-----------------------------------------------------------------------------------------------------------------------
max_α:                  sub              rsp, 64
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx198_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 0]              # max
                        mov              qword ptr [r9 + 0], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx198_41
.Lx198_10:              mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
.Lx198_41:              cmp              rdx, 1;                              jbe   .Lx198_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx198_42
.Lx198_11:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx198_42:              lea              r10, [rip + max_γ]
                        lea              r11, [rip + max_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n5_statement_begin_α];   jmp   rax
max_γ:                  mov              rdi, qword ptr [r9 + 0]              # max
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx198_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 8], rax;             jmp   .Lx198_110
.Lx198_80:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
.Lx198_110:             cmp              rdx, 1;                              jbe   .Lx198_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx198_111
.Lx198_81:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx198_111:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
max_ω:                  mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx198_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 0], rax              # max
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 8], rax;             jmp   .Lx198_180
.Lx198_150:             mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
.Lx198_180:             cmp              rdx, 1;                              jbe   .Lx198_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx198_181
.Lx198_151:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx198_181:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx198_245:
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:                                                           jmp   n15_statement_begin_α
#=======================================================================================================================
# max  <stmt 3, line 42: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_begin_α:                                                         jmp   n6_var_α
n5_statement_begin_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]              # max
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_coerce_numeric_α
n7_var_β:               add              rsp, 16
                        add              rsp, 16;                             jmp   n5_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n8_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx206_1
                        cmp              eax, 3;                              jne   .Lx206_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 3;                              jne   .Lx206_0
.Lx206_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_coerce_numeric_α
.Lx206_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 147
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n9_coerce_numeric_α
n8_coerce_numeric_β:    add              rsp, 16;                             jmp   n7_var_β
#-----------------------------------------------------------------------------------------------------------------------
n9_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx208_1
                        cmp              eax, 3;                              jne   .Lx208_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 3;                              jne   .Lx208_0
.Lx208_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_cmp_test_α
.Lx208_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 148
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n10_cmp_test_α
n9_coerce_numeric_β:    add              rsp, 16;                             jmp   n8_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n10_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                      # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            js    .Lx210_240
                        add              rsp, 16;                             jmp   n9_coerce_numeric_β
.Lx210_240:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n11_var_α
n10_cmp_test_β:         add              rsp, 16;                             jmp   n9_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n12_binop_α
n11_var_β:              add              rsp, 16;                             jmp   n10_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n12_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # cmp_test
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n13_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # max
                        mov              qword ptr [r9 + 8], rdx;             jmp   n14_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:    add              rsp, 112;                            jmp   RETURN
#=======================================================================================================================
# max_end  <stmt 4, line 44: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                                        jmp   n16_statement_end_α
n15_statement_begin_β:                                                        jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:                                                          jmp   n17_statement_begin_α
#=======================================================================================================================
#         <stmt 5, line 45: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:                                                        jmp   n18_define_α
n17_statement_begin_β:                                                        jmp   n30_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n18_define_α:           mov              rdi, qword ptr [rip + .Lx223_0]
                        mov              rsi, qword ptr [rip + .Lx223_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n20_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx223_0]
                        lea              rsi, [rip + min_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n19_statement_end_α
n18_define_β:                                                                 jmp   n17_statement_begin_β
.Lx223_0:               .quad            .Lx223_0_s
.Lx223_0_s:             .string          "min"
.Lx223_1:               .quad            .Lx223_1_s
.Lx223_1_s:             .string          "min,x"
                                                                              jmp   .Lx224_245
#-----------------------------------------------------------------------------------------------------------------------
min_α:                  sub              rsp, 64
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx224_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # min
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx224_41
.Lx224_10:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx224_41:              cmp              rdx, 1;                              jbe   .Lx224_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx224_42
.Lx224_11:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx224_42:              lea              r10, [rip + min_γ]
                        lea              r11, [rip + min_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n20_statement_begin_α];  jmp   rax
min_γ:                  mov              rdi, qword ptr [r9 + 32]             # min
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx224_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx224_110
.Lx224_80:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 40], rax
.Lx224_110:             cmp              rdx, 1;                              jbe   .Lx224_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx224_111
.Lx224_81:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx224_111:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
min_ω:                  mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx224_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # min
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx224_180
.Lx224_150:             mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 40], rax
.Lx224_180:             cmp              rdx, 1;                              jbe   .Lx224_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx224_181
.Lx224_151:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx224_181:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx224_245:
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:                                                          jmp   n30_statement_begin_α
#=======================================================================================================================
# min  <stmt 6, line 46: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:                                                        jmp   n21_var_α
n20_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # min
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n22_var_α
#-----------------------------------------------------------------------------------------------------------------------
n22_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n23_coerce_numeric_α
n22_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n20_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n23_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx232_1
                        cmp              eax, 3;                              jne   .Lx232_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 3;                              jne   .Lx232_0
.Lx232_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_coerce_numeric_α
.Lx232_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n24_coerce_numeric_α
n23_coerce_numeric_β:   add              rsp, 16;                             jmp   n22_var_β
#-----------------------------------------------------------------------------------------------------------------------
n24_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx234_1
                        cmp              eax, 3;                              jne   .Lx234_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 3;                              jne   .Lx234_0
.Lx234_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n25_cmp_test_α
.Lx234_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n25_cmp_test_α
n24_coerce_numeric_β:   add              rsp, 16;                             jmp   n23_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n25_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                      # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jg    .Lx236_240
                        add              rsp, 16;                             jmp   n24_coerce_numeric_β
.Lx236_240:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n26_var_α
n25_cmp_test_β:         add              rsp, 16;                             jmp   n24_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n27_binop_α
n26_var_β:              add              rsp, 16;                             jmp   n25_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n27_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # cmp_test
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n28_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # min
                        mov              qword ptr [r9 + 40], rdx;            jmp   n29_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:    add              rsp, 112;                            jmp   RETURN
#=======================================================================================================================
# min_end  <stmt 7, line 48: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:                                                        jmp   n31_statement_end_α
n30_statement_begin_β:                                                        jmp   n32_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:                                                          jmp   n32_statement_begin_α
#=======================================================================================================================
#         <stmt 8, line 49: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:                                                        jmp   n33_define_α
n32_statement_begin_β:                                                        jmp   n46_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_define_α:           mov              rdi, qword ptr [rip + .Lx249_0]
                        mov              rsi, qword ptr [rip + .Lx249_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n35_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx249_0]
                        lea              rsi, [rip + abs_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n34_statement_end_α
n33_define_β:                                                                 jmp   n32_statement_begin_β
.Lx249_0:               .quad            .Lx249_0_s
.Lx249_0_s:             .string          "abs"
.Lx249_1:               .quad            .Lx249_1_s
.Lx249_1_s:             .string          "abs"
                                                                              jmp   .Lx250_245
#-----------------------------------------------------------------------------------------------------------------------
abs_α:                  sub              rsp, 48
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx250_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 48]             # abs
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx250_41
.Lx250_10:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx250_41:              lea              r10, [rip + abs_γ]
                        lea              r11, [rip + abs_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n35_statement_begin_α];  jmp   rax
abs_γ:                  mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx250_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx250_110
.Lx250_80:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx250_110:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
abs_ω:                  mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx250_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx250_180
.Lx250_150:             mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx250_180:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx250_245:
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:                                                          jmp   n46_statement_begin_α
#=======================================================================================================================
# abs  <stmt 9, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:                                                        jmp   n36_var_α
n35_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n36_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # abs
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n37_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx256_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n38_coerce_numeric_α
n37_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n35_statement_begin_β
.Lx256_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n38_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx258_1
                        cmp              eax, 3;                              jne   .Lx258_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              eax, 3;                              jne   .Lx258_0
.Lx258_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_coerce_numeric_α
.Lx258_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 147
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n39_coerce_numeric_α
n38_coerce_numeric_β:   add              rsp, 16;                             jmp   n37_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n39_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              eax, 5;                              je    .Lx260_1
                        cmp              eax, 3;                              jne   .Lx260_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              eax, 3;                              jne   .Lx260_0
.Lx260_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n40_cmp_test_α
.Lx260_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 148
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n40_cmp_test_α
n39_coerce_numeric_β:   add              rsp, 16;                             jmp   n38_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n40_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                      # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            js    .Lx262_240
                        add              rsp, 16;                             jmp   n39_coerce_numeric_β
.Lx262_240:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n41_var_α
n40_cmp_test_β:         add              rsp, 16;                             jmp   n39_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # abs
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n42_unop_α
n41_var_β:              add              rsp, 16;                             jmp   n40_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n42_unop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # var
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_num_neg@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n43_binop_α
n42_unop_β:             add              rsp, 16;                             jmp   n41_var_β
#-----------------------------------------------------------------------------------------------------------------------
n43_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # cmp_test
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # unop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n44_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n44_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # abs
                        mov              qword ptr [r9 + 56], rdx;            jmp   n45_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:    add              rsp, 128;                            jmp   RETURN
#=======================================================================================================================
# abs_end  <stmt 10, line 52: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:                                                        jmp   n47_statement_end_α
n46_statement_begin_β:                                                        jmp   n48_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:                                                          jmp   n48_statement_begin_α
#=======================================================================================================================
#         <stmt 11, line 53: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:                                                        jmp   n49_define_α
n48_statement_begin_β:                                                        jmp   n72_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n49_define_α:           mov              rdi, qword ptr [rip + .Lx276_0]
                        mov              rsi, qword ptr [rip + .Lx276_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n51_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx276_0]
                        lea              rsi, [rip + sign_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n50_statement_end_α
n49_define_β:                                                                 jmp   n48_statement_begin_β
.Lx276_0:               .quad            .Lx276_0_s
.Lx276_0_s:             .string          "sign"
.Lx276_1:               .quad            .Lx276_1_s
.Lx276_1_s:             .string          "sign"
                                                                              jmp   .Lx277_245
#-----------------------------------------------------------------------------------------------------------------------
sign_α:                 sub              rsp, 48
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx277_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # sign
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx277_41
.Lx277_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx277_41:              lea              r10, [rip + sign_γ]
                        lea              r11, [rip + sign_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n51_statement_begin_α];  jmp   rax
sign_γ:                 mov              rdi, qword ptr [r9 + 64]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx277_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx277_110
.Lx277_80:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 72], rax
.Lx277_110:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
sign_ω:                 mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx277_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx277_180
.Lx277_150:             mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 72], rax
.Lx277_180:             mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx277_245:
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:                                                          jmp   n72_statement_begin_α
#=======================================================================================================================
# sign  <stmt 12, line 54: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:                                                        jmp   n52_var_α
n51_statement_begin_β:                                                        jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # sign
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n53_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx283_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n54_coerce_numeric_α
n53_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n51_statement_begin_β
.Lx283_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n54_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx285_1
                        cmp              eax, 3;                              jne   .Lx285_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              eax, 3;                              jne   .Lx285_0
.Lx285_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n55_coerce_numeric_α
.Lx285_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 147
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n55_coerce_numeric_α
n54_coerce_numeric_β:   add              rsp, 16;                             jmp   n53_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n55_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              eax, 5;                              je    .Lx287_1
                        cmp              eax, 3;                              jne   .Lx287_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              eax, 3;                              jne   .Lx287_0
.Lx287_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n56_cmp_test_α
.Lx287_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 148
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n56_cmp_test_α
n55_coerce_numeric_β:   add              rsp, 16;                             jmp   n54_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n56_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                      # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            js    .Lx289_240
                        add              rsp, 16;                             jmp   n55_coerce_numeric_β
.Lx289_240:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n57_lit_integer_α
n56_cmp_test_β:         add              rsp, 16;                             jmp   n55_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n58_unop_α
n57_lit_integer_β:      add              rsp, 16;                             jmp   n56_cmp_test_β
.Lx290_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n58_unop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_num_neg@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n59_binop_α
n58_unop_β:             add              rsp, 16;                             jmp   n57_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n59_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # cmp_test
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # unop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n60_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # sign
                        mov              qword ptr [r9 + 72], rdx;            jmp   n61_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:    add              rsp, 128;                            jmp   RETURN
#=======================================================================================================================
#         <stmt 13, line 55: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_begin_α:                                                        jmp   n63_var_α
n62_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n63_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # sign
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n64_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx299_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n65_coerce_numeric_α
n64_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n62_statement_begin_β
.Lx299_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n65_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx301_1
                        cmp              eax, 3;                              jne   .Lx301_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              eax, 3;                              jne   .Lx301_0
.Lx301_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n66_coerce_numeric_α
.Lx301_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n66_coerce_numeric_α
n65_coerce_numeric_β:   add              rsp, 16;                             jmp   n64_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n66_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              eax, 5;                              je    .Lx303_1
                        cmp              eax, 3;                              jne   .Lx303_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              eax, 3;                              jne   .Lx303_0
.Lx303_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n67_cmp_test_α
.Lx303_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n67_cmp_test_α
n66_coerce_numeric_β:   add              rsp, 16;                             jmp   n65_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n67_cmp_test_α:         sub              rsp, 16
                        lea              rdi, [rsp + 32]                      # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jg    .Lx305_240
                        add              rsp, 16;                             jmp   n66_coerce_numeric_β
.Lx305_240:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n68_lit_integer_α
n67_cmp_test_β:         add              rsp, 16;                             jmp   n66_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx306_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n69_binop_α
n68_lit_integer_β:      add              rsp, 16;                             jmp   n67_cmp_test_β
.Lx306_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n69_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # cmp_test
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n70_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # sign
                        mov              qword ptr [r9 + 72], rdx;            jmp   n71_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:    add              rsp, 112;                            jmp   RETURN
#=======================================================================================================================
# sign_end  <stmt 14, line 57: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:                                                        jmp   n73_statement_end_α
n72_statement_begin_β:                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:                                                          jmp   n74_statement_begin_α
#=======================================================================================================================
#         <stmt 15, line 58: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:                                                        jmp   n75_define_α
n74_statement_begin_β:                                                        jmp   n96_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_define_α:           mov              rdi, qword ptr [rip + .Lx318_0]
                        mov              rsi, qword ptr [rip + .Lx318_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n77_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx318_0]
                        lea              rsi, [rip + gcd_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n76_statement_end_α
n75_define_β:                                                                 jmp   n74_statement_begin_β
.Lx318_0:               .quad            .Lx318_0_s
.Lx318_0_s:             .string          "gcd"
.Lx318_1:               .quad            .Lx318_1_s
.Lx318_1_s:             .string          "gcd,b,r"
                                                                              jmp   .Lx319_245
#-----------------------------------------------------------------------------------------------------------------------
gcd_α:                  sub              rsp, 80
                        mov              rax, qword ptr [r9 + 112]            # r
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx319_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 80]             # gcd
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx319_41
.Lx319_10:              mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx319_41:              cmp              rdx, 1;                              jbe   .Lx319_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 96]             # b
                        mov              qword ptr [r9 + 96], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 104]
                        mov              qword ptr [r9 + 104], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx319_42
.Lx319_11:              mov              rax, qword ptr [r9 + 96]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
.Lx319_42:              lea              r10, [rip + gcd_γ]
                        lea              r11, [rip + gcd_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n77_statement_begin_α];  jmp   rax
gcd_γ:                  mov              rdi, qword ptr [r9 + 80]             # gcd
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax            # r
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx319_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # gcd
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx319_110
.Lx319_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
.Lx319_110:             cmp              rdx, 1;                              jbe   .Lx319_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 96], rax             # b
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 104], rax;           jmp   .Lx319_111
.Lx319_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 104], rax
.Lx319_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 80
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
gcd_ω:                  mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax            # r
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx319_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # gcd
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx319_180
.Lx319_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
.Lx319_180:             cmp              rdx, 1;                              jbe   .Lx319_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 96], rax             # b
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 104], rax;           jmp   .Lx319_181
.Lx319_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 104], rax
.Lx319_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx319_245:
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_end_α:                                                          jmp   n96_statement_begin_α
#=======================================================================================================================
# gcd  <stmt 16, line 59: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_begin_α:                                                        jmp   n78_var_α
n77_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n78_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]             # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n79_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx325_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n80_call_α
n79_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n77_statement_begin_β
.Lx325_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n80_call_α:             sub              rsp, 16
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
.Lrkfnzd327:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd327]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx326_240
                        add              rsp, 16;                             jmp   n79_lit_integer_β
.Lx326_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n81_statement_end_α
n80_call_β:             add              rsp, 16;                             jmp   n79_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_end_α:    add              rsp, 48;                             jmp   n82_statement_begin_α
#=======================================================================================================================
#         <stmt 17, line 60: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_begin_α:                                                        jmp   n83_var_α
n82_statement_begin_β:                                                        jmp   n88_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n83_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # gcd
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n84_var_α
#-----------------------------------------------------------------------------------------------------------------------
n84_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]             # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n85_call_α
n84_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n82_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n85_call_α:             sub              rsp, 16
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
.Lrkfnzd335:            .string          "REMDR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd335]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx334_240
                        add              rsp, 16;                             jmp   n84_var_β
.Lx334_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_assign_α
n85_call_β:             add              rsp, 16;                             jmp   n84_var_β
#-----------------------------------------------------------------------------------------------------------------------
n86_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # r
                        mov              qword ptr [r9 + 120], rdx;           jmp   n87_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_end_α:    add              rsp, 48;                             jmp   n88_statement_begin_α
#=======================================================================================================================
#         <stmt 18, line 61: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_begin_α:                                                        jmp   n89_var_α
n88_statement_begin_β:                                                        jmp   n92_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n89_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]             # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n90_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n90_assign_α:           mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # gcd
                        mov              qword ptr [r9 + 88], rdx;            jmp   n91_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:    add              rsp, 16;                             jmp   n92_statement_begin_α
#=======================================================================================================================
#         <stmt 19, line 62: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:                                                        jmp   n93_var_α
n92_statement_begin_β:                                                        jmp   n77_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]            # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n94_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_α:           mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # b
                        mov              qword ptr [r9 + 104], rdx;           jmp   n95_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_end_α:    add              rsp, 16;                             jmp   n77_statement_begin_α
#=======================================================================================================================
# gcd_end  <stmt 20, line 64: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_begin_α:                                                        jmp   n97_statement_end_α
n96_statement_begin_β:                                                        jmp   n98_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_end_α:                                                          jmp   n98_statement_begin_α
#=======================================================================================================================
#         <stmt 21, line 65: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_begin_α:                                                        jmp   n99_define_α
n98_statement_begin_β:                                                        jmp   n115_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n99_define_α:           mov              rdi, qword ptr [rip + .Lx358_0]
                        mov              rsi, qword ptr [rip + .Lx358_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n101_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx358_0]
                        lea              rsi, [rip + lcm_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n100_statement_end_α
n99_define_β:                                                                 jmp   n98_statement_begin_β
.Lx358_0:               .quad            .Lx358_0_s
.Lx358_0_s:             .string          "lcm"
.Lx358_1:               .quad            .Lx358_1_s
.Lx358_1_s:             .string          "a,b,g"
                                                                              jmp   .Lx359_245
#-----------------------------------------------------------------------------------------------------------------------
lcm_α:                  sub              rsp, 96
                        mov              rax, qword ptr [r9 + 160]            # g
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 160], 0
                        mov              qword ptr [r9 + 168], 0
                        mov              rax, qword ptr [r9 + 128]            # lcm
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        mov              qword ptr [rsp + 48], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx359_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 144]            # a
                        mov              qword ptr [r9 + 144], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 152]
                        mov              qword ptr [r9 + 152], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx359_41
.Lx359_10:              mov              rax, qword ptr [r9 + 144]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 144], 0
                        mov              qword ptr [r9 + 152], 0
.Lx359_41:              cmp              rdx, 1;                              jbe   .Lx359_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 96]             # b
                        mov              qword ptr [r9 + 96], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 104]
                        mov              qword ptr [r9 + 104], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx359_42
.Lx359_11:              mov              rax, qword ptr [r9 + 96]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
.Lx359_42:              lea              r10, [rip + lcm_γ]
                        lea              r11, [rip + lcm_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n101_statement_begin_α]; jmp   rax
lcm_γ:                  mov              rdi, qword ptr [r9 + 128]            # lcm
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 160], rax            # g
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 168], rax
                        cmp              rdx, 0;                              jbe   .Lx359_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 144], rax            # a
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 152], rax;           jmp   .Lx359_110
.Lx359_80:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 144], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 152], rax
.Lx359_110:             cmp              rdx, 1;                              jbe   .Lx359_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 96], rax             # b
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 104], rax;           jmp   .Lx359_111
.Lx359_81:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 104], rax
.Lx359_111:             mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
lcm_ω:                  mov              rcx, qword ptr [rsp + 48]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 128], rax            # lcm
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 160], rax            # g
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 168], rax
                        cmp              rdx, 0;                              jbe   .Lx359_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 144], rax            # a
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 152], rax;           jmp   .Lx359_180
.Lx359_150:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 144], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 152], rax
.Lx359_180:             cmp              rdx, 1;                              jbe   .Lx359_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 96], rax             # b
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 104], rax;           jmp   .Lx359_181
.Lx359_151:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 104], rax
.Lx359_181:             mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx359_245:
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_end_α:                                                         jmp   n115_statement_begin_α
#=======================================================================================================================
# lcm  <stmt 22, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_begin_α:                                                       jmp   n102_var_α
n101_statement_begin_β:                                                       jmp   n107_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n102_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # a
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n103_var_α
#-----------------------------------------------------------------------------------------------------------------------
n103_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]             # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n104_call_α
n103_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n101_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n104_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig367z]
                        lea              rax, [rip + gcd_α];                  jmp   rax
.Lsig367z:              .quad            2
                        .quad            .Lx367_2
                        .quad            .Lx367_2
                        .quad            32
                        .quad            16
.Lx367_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx367_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx367_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx367_240
                        add              rsp, 16;                             jmp   n103_var_β
.Lx367_240:                                                                   jmp   n105_assign_α
n104_call_β:                                                                  jmp   n103_var_β
.Lx367_0:               .quad            .Lx367_0_s
.Lx367_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n105_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 160], rax            # g
                        mov              qword ptr [r9 + 168], rdx;           jmp   n106_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:   add              rsp, 48;                             jmp   n107_statement_begin_α
#=======================================================================================================================
#         <stmt 23, line 67: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_begin_α:                                                       jmp   n108_var_α
n107_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n108_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # a
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n109_var_α
#-----------------------------------------------------------------------------------------------------------------------
n109_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 160]            # g
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n110_binop_α
n109_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n107_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n110_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_div@PLT
                        cmp              eax, 104;                            jne   .Lx375_240
                        add              rsp, 16;                             jmp   n109_var_β
.Lx375_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n111_var_α
n110_binop_β:           add              rsp, 16;                             jmp   n109_var_β
#-----------------------------------------------------------------------------------------------------------------------
n111_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]             # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n112_binop_α
n111_var_β:             add              rsp, 16;                             jmp   n110_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n112_binop_α:           sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # binop
                        mov              ecx, dword ptr [rsp + 16]            # var
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx377_2
                        mov              rax, qword ptr [rsp + 40]            # binop
                        mov              rdx, qword ptr [rsp + 24]            # var
                        imul             rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx377_7
.Lx377_2:               and              edx, 1;                              jz    .Lx377_0
                        mov              rsi, qword ptr [rsp + 40]            # binop
                        mov              rdi, qword ptr [rsp + 24]            # var
                        cmp              eax, 5;                              je    .Lx377_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx377_4
.Lx377_3:               movq             xmm0, rsi
.Lx377_4:               cmp              ecx, 5;                              je    .Lx377_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx377_6
.Lx377_5:               movq             xmm1, rdi
.Lx377_6:               mulsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx377_7:                                                                     jmp   n113_assign_α
.Lx377_0:               mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_mul@PLT
                        cmp              eax, 104;                            jne   .Lx377_240
                        add              rsp, 16;                             jmp   n111_var_β
.Lx377_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n113_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # lcm
                        mov              qword ptr [r9 + 136], rdx;           jmp   n114_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_end_α:   add              rsp, 80;                             jmp   RETURN
#=======================================================================================================================
# lcm_end  <stmt 24, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_begin_α:                                                       jmp   n116_statement_end_α
n115_statement_begin_β:                                                       jmp   n117_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_end_α:                                                         jmp   n117_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_begin_α:                                                       jmp   n118_statement_end_α
n117_statement_begin_β:                                                       jmp   n119_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n118_statement_end_α:                                                         jmp   n119_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_begin_α:                                                       jmp   n120_lit_integer_α
n119_statement_begin_β:                                                       jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx391_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n121_keyword_assign_snobol4_α
.Lx391_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n121_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx392_0]
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_write_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              eax, 104;                            jne   .Lx392_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n119_statement_begin_β
.Lx392_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n122_statement_end_α
.Lx392_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_end_α:                                                         jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_begin_α:                                                       jmp   n124_statement_end_α
n123_statement_begin_β: add              rsp, 32;                             jmp   n125_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_end_α:   add              rsp, 32;                             jmp   n125_statement_begin_α
#=======================================================================================================================
#         <stmt 28, line 73: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_begin_α:                                                       jmp   n126_lit_integer_α
n125_statement_begin_β:                                                       jmp   n131_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n126_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx401_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n127_lit_integer_α
.Lx401_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx402_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n128_call_α
n127_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n125_statement_begin_β
.Lx402_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n128_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig404z]
                        lea              rax, [rip + max_α];                  jmp   rax
.Lsig404z:              .quad            2
                        .quad            .Lx404_2
                        .quad            .Lx404_2
                        .quad            32
                        .quad            16
.Lx404_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx404_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx404_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx404_240
                        add              rsp, 16;                             jmp   n127_lit_integer_β
.Lx404_240:                                                                   jmp   n129_assign_α
n128_call_β:                                                                  jmp   n127_lit_integer_β
.Lx404_0:               .quad            .Lx404_0_s
.Lx404_0_s:             .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n129_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx405_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n130_statement_end_α
.Lx405_0:               .quad            .Lx405_0_s
.Lx405_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_end_α:   add              rsp, 48;                             jmp   n131_statement_begin_α
#=======================================================================================================================
#         <stmt 29, line 74: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_begin_α:                                                       jmp   n132_lit_integer_α
n131_statement_begin_β:                                                       jmp   n137_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx410_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n133_lit_integer_α
.Lx410_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx411_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n134_call_α
n133_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n131_statement_begin_β
.Lx411_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n134_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig413z]
                        lea              rax, [rip + min_α];                  jmp   rax
.Lsig413z:              .quad            2
                        .quad            .Lx413_2
                        .quad            .Lx413_2
                        .quad            32
                        .quad            16
.Lx413_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx413_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx413_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx413_240
                        add              rsp, 16;                             jmp   n133_lit_integer_β
.Lx413_240:                                                                   jmp   n135_assign_α
n134_call_β:                                                                  jmp   n133_lit_integer_β
.Lx413_0:               .quad            .Lx413_0_s
.Lx413_0_s:             .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n135_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx414_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n136_statement_end_α
.Lx414_0:               .quad            .Lx414_0_s
.Lx414_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_end_α:   add              rsp, 48;                             jmp   n137_statement_begin_α
#=======================================================================================================================
#         <stmt 30, line 75: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_begin_α:                                                       jmp   n138_lit_real_α
n137_statement_begin_β:                                                       jmp   n143_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n138_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx419_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n139_lit_real_α
.Lx419_0:               .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n139_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx420_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n140_call_α
.Lx420_0:               .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n140_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig422z]
                        lea              rax, [rip + max_α];                  jmp   rax
.Lsig422z:              .quad            2
                        .quad            .Lx422_2
                        .quad            .Lx422_2
                        .quad            32
                        .quad            16
.Lx422_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx422_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx422_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx422_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n137_statement_begin_β
.Lx422_240:                                                                   jmp   n141_assign_α
n140_call_β:                                                                  jmp   n137_statement_begin_β
.Lx422_0:               .quad            .Lx422_0_s
.Lx422_0_s:             .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n141_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx423_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n142_statement_end_α
.Lx423_0:               .quad            .Lx423_0_s
.Lx423_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n142_statement_end_α:   add              rsp, 48;                             jmp   n143_statement_begin_α
#=======================================================================================================================
#         <stmt 31, line 76: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_begin_α:                                                       jmp   n144_lit_real_α
n143_statement_begin_β:                                                       jmp   n149_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n144_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx428_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n145_lit_real_α
.Lx428_0:               .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n145_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx429_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n146_call_α
.Lx429_0:               .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n146_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig431z]
                        lea              rax, [rip + min_α];                  jmp   rax
.Lsig431z:              .quad            2
                        .quad            .Lx431_2
                        .quad            .Lx431_2
                        .quad            32
                        .quad            16
.Lx431_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx431_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx431_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx431_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n143_statement_begin_β
.Lx431_240:                                                                   jmp   n147_assign_α
n146_call_β:                                                                  jmp   n143_statement_begin_β
.Lx431_0:               .quad            .Lx431_0_s
.Lx431_0_s:             .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n147_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx432_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n148_statement_end_α
.Lx432_0:               .quad            .Lx432_0_s
.Lx432_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:   add              rsp, 48;                             jmp   n149_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 77: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_begin_α:                                                       jmp   n150_lit_integer_α
n149_statement_begin_β:                                                       jmp   n155_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx437_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n151_unop_α
.Lx437_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n151_unop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_num_neg@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n152_call_α
n151_unop_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n149_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n152_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig440z]
                        lea              rax, [rip + abs_α];                  jmp   rax
.Lsig440z:              .quad            1
                        .quad            .Lx440_2
                        .quad            .Lx440_2
                        .quad            16
.Lx440_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx440_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx440_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx440_240
                        add              rsp, 16;                             jmp   n151_unop_β
.Lx440_240:                                                                   jmp   n153_assign_α
n152_call_β:                                                                  jmp   n151_unop_β
.Lx440_0:               .quad            .Lx440_0_s
.Lx440_0_s:             .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n153_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx441_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n154_statement_end_α
.Lx441_0:               .quad            .Lx441_0_s
.Lx441_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_end_α:   add              rsp, 48;                             jmp   n155_statement_begin_α
#=======================================================================================================================
#         <stmt 33, line 78: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_begin_α:                                                       jmp   n156_lit_integer_α
n155_statement_begin_β:                                                       jmp   n160_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n156_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx446_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n157_call_α
.Lx446_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n157_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig448z]
                        lea              rax, [rip + sign_α];                 jmp   rax
.Lsig448z:              .quad            1
                        .quad            .Lx448_2
                        .quad            .Lx448_2
                        .quad            16
.Lx448_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx448_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx448_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx448_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n155_statement_begin_β
.Lx448_240:                                                                   jmp   n158_assign_α
n157_call_β:                                                                  jmp   n155_statement_begin_β
.Lx448_0:               .quad            .Lx448_0_s
.Lx448_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n158_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx449_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n159_statement_end_α
.Lx449_0:               .quad            .Lx449_0_s
.Lx449_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n159_statement_end_α:   add              rsp, 32;                             jmp   n160_statement_begin_α
#=======================================================================================================================
#         <stmt 34, line 79: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_begin_α:                                                       jmp   n161_lit_integer_α
n160_statement_begin_β:                                                       jmp   n165_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n161_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx454_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n162_call_α
.Lx454_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n162_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig456z]
                        lea              rax, [rip + sign_α];                 jmp   rax
.Lsig456z:              .quad            1
                        .quad            .Lx456_2
                        .quad            .Lx456_2
                        .quad            16
.Lx456_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx456_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx456_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx456_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n160_statement_begin_β
.Lx456_240:                                                                   jmp   n163_assign_α
n162_call_β:                                                                  jmp   n160_statement_begin_β
.Lx456_0:               .quad            .Lx456_0_s
.Lx456_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n163_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx457_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n164_statement_end_α
.Lx457_0:               .quad            .Lx457_0_s
.Lx457_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_end_α:   add              rsp, 32;                             jmp   n165_statement_begin_α
#=======================================================================================================================
#         <stmt 35, line 80: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n165_statement_begin_α:                                                       jmp   n166_lit_integer_α
n165_statement_begin_β:                                                       jmp   n171_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n166_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx462_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n167_unop_α
.Lx462_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n167_unop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_num_neg@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n168_call_α
n167_unop_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n165_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n168_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig465z]
                        lea              rax, [rip + sign_α];                 jmp   rax
.Lsig465z:              .quad            1
                        .quad            .Lx465_2
                        .quad            .Lx465_2
                        .quad            16
.Lx465_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx465_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx465_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx465_240
                        add              rsp, 16;                             jmp   n167_unop_β
.Lx465_240:                                                                   jmp   n169_assign_α
n168_call_β:                                                                  jmp   n167_unop_β
.Lx465_0:               .quad            .Lx465_0_s
.Lx465_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n169_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx466_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n170_statement_end_α
.Lx466_0:               .quad            .Lx466_0_s
.Lx466_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_end_α:   add              rsp, 48;                             jmp   n171_statement_begin_α
#=======================================================================================================================
#         <stmt 36, line 81: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n171_statement_begin_α:                                                       jmp   n172_lit_integer_α
n171_statement_begin_β:                                                       jmp   n177_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n172_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx471_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n173_lit_integer_α
.Lx471_0:               .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n173_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx472_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n174_call_α
n173_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n171_statement_begin_β
.Lx472_0:               .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n174_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig474z]
                        lea              rax, [rip + gcd_α];                  jmp   rax
.Lsig474z:              .quad            2
                        .quad            .Lx474_2
                        .quad            .Lx474_2
                        .quad            32
                        .quad            16
.Lx474_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx474_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx474_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx474_240
                        add              rsp, 16;                             jmp   n173_lit_integer_β
.Lx474_240:                                                                   jmp   n175_assign_α
n174_call_β:                                                                  jmp   n173_lit_integer_β
.Lx474_0:               .quad            .Lx474_0_s
.Lx474_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n175_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx475_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n176_statement_end_α
.Lx475_0:               .quad            .Lx475_0_s
.Lx475_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_end_α:   add              rsp, 48;                             jmp   n177_statement_begin_α
#=======================================================================================================================
#         <stmt 37, line 82: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_begin_α:                                                       jmp   n178_lit_integer_α
n177_statement_begin_β:                                                       jmp   n183_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n178_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx480_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n179_lit_integer_α
.Lx480_0:               .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n179_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx481_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n180_call_α
n179_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n177_statement_begin_β
.Lx481_0:               .quad            75
#-----------------------------------------------------------------------------------------------------------------------
n180_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig483z]
                        lea              rax, [rip + gcd_α];                  jmp   rax
.Lsig483z:              .quad            2
                        .quad            .Lx483_2
                        .quad            .Lx483_2
                        .quad            32
                        .quad            16
.Lx483_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx483_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx483_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx483_240
                        add              rsp, 16;                             jmp   n179_lit_integer_β
.Lx483_240:                                                                   jmp   n181_assign_α
n180_call_β:                                                                  jmp   n179_lit_integer_β
.Lx483_0:               .quad            .Lx483_0_s
.Lx483_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n181_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx484_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n182_statement_end_α
.Lx484_0:               .quad            .Lx484_0_s
.Lx484_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_end_α:   add              rsp, 48;                             jmp   n183_statement_begin_α
#=======================================================================================================================
#         <stmt 38, line 83: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_begin_α:                                                       jmp   n184_lit_integer_α
n183_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n184_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx489_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n185_lit_integer_α
.Lx489_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n185_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx490_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n186_call_α
n185_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n183_statement_begin_β
.Lx490_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n186_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig492z]
                        lea              rax, [rip + lcm_α];                  jmp   rax
.Lsig492z:              .quad            2
                        .quad            .Lx492_2
                        .quad            .Lx492_2
                        .quad            32
                        .quad            16
.Lx492_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx492_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx492_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx492_240
                        add              rsp, 16;                             jmp   n185_lit_integer_β
.Lx492_240:                                                                   jmp   n187_assign_α
n186_call_β:                                                                  jmp   n185_lit_integer_β
.Lx492_0:               .quad            .Lx492_0_s
.Lx492_0_s:             .string          "lcm"
#-----------------------------------------------------------------------------------------------------------------------
n187_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx493_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n188_statement_end_α
.Lx493_0:               .quad            .Lx493_0_s
.Lx493_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_end_α:   add              rsp, 48;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 pop              rcx
                        add              rsp, 8;                              jmp   rcx
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
module_init:
                        sub              rsp, 8
                        .section         .rodata
.Lseala6:               .string          "max"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            max_α
                        lea              rdi, [rip + .Lseala6]
                        mov              rsi, qword ptr [rip + max_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala7:               .string          "min"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            min_α
                        lea              rdi, [rip + .Lseala7]
                        mov              rsi, qword ptr [rip + min_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala8:               .string          "abs"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            abs_α
                        lea              rdi, [rip + .Lseala8]
                        mov              rsi, qword ptr [rip + abs_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala9:               .string          "sign"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            sign_α
                        lea              rdi, [rip + .Lseala9]
                        mov              rsi, qword ptr [rip + sign_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala10:              .string          "gcd"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            gcd_α
                        lea              rdi, [rip + .Lseala10]
                        mov              rsi, qword ptr [rip + gcd_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala11:              .string          "lcm"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            lcm_α
                        lea              rdi, [rip + .Lseala11]
                        mov              rsi, qword ptr [rip + lcm_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits

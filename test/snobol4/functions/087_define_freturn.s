                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
                        mov              edi, 2
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 2
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "ispos"
.Lgvan1:                .string          "x"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         DEFINE('ispos(x)')                                          :(ispos_end)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 1;                              jmp   n1_define_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_define_α:            mov              r11, 2
                        mov              rdi, qword ptr [rip + .Lx46_0]
                        mov              rsi, qword ptr [rip + .Lx46_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n3_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx46_0]
                        lea              rsi, [rip + ispos_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n2_statement_end_α
n1_define_β:            mov              r11, 2;                              jmp   n0_statement_begin_β
.Lx46_0:                .quad            .Lx46_0_s
.Lx46_0_s:              .string          "ispos"
.Lx46_1:                .quad            .Lx46_1_s
.Lx46_1_s:              .string          "x"
                                                                              jmp   .Lx47_245
#-----------------------------------------------------------------------------------------------------------------------
ispos_α:                sub              rsp, 64
                        mov              rax, qword ptr [r9 + 0]              # ispos
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx47_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx47_41
.Lx47_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx47_41:               lea              rcx, [rip + ispos_γ]
                        lea              rax, [rip + ispos_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n3_statement_begin_α];   jmp   rax
ispos_γ:                mov              rdi, qword ptr [r9 + 0]              # ispos
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx47_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx47_110
.Lx47_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx47_110:              mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
ispos_ω:                mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax              # ispos
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx47_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx47_180
.Lx47_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx47_180:              mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx47_245:
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_end_α:     mov              r11, 3
                        mov              r10, 1;                              jmp   n10_statement_begin_α
#=======================================================================================================================
# ispos   GT(x, 0)                                                   :S(RETURN)F(FRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_begin_α:   mov              r11, 4
                        mov              r10, 2;                              jmp   n4_var_α
n3_statement_begin_β:   mov              r11, 4;                              jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:               sub              rsp, 16
                        mov              r11, 5
                        mov              rax, qword ptr [r9 + 16]             # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n5_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:       sub              rsp, 16
                        mov              r11, 6
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n6_coerce_numeric_α
n5_lit_integer_β:       mov              r11, 6
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n3_statement_begin_β
.Lx53_0:                .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n6_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 7
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx55_1
                        cmp              al, 3;                               jne   .Lx55_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx55_0
.Lx55_1:                mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n7_coerce_numeric_α
.Lx55_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n7_coerce_numeric_α
n6_coerce_numeric_β:    mov              r11, 7
                        add              rsp, 16;                             jmp   n5_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n7_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 8
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx57_1
                        cmp              al, 3;                               jne   .Lx57_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx57_0
.Lx57_1:                mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n8_cmp_test_α
.Lx57_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n8_cmp_test_α
n7_coerce_numeric_β:    mov              r11, 8
                        add              rsp, 16;                             jmp   n6_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n8_cmp_test_α:          sub              rsp, 16
                        mov              r11, 9
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx59_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jg    .Lx59_239
                        add              rsp, 16;                             jmp   n7_coerce_numeric_β
.Lx59_239:                                                                    jmp   n9_statement_end_α
.Lx59_0:                lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jg    .Lx59_240
                        add              rsp, 16;                             jmp   n7_coerce_numeric_β
.Lx59_240:                                                                    jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     mov              r11, 10
                        mov              r10, 2
                        add              rsp, 80;                             jmp   RETURN
#=======================================================================================================================
# ispos_end  <stmt 3, line 5: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:  mov              r11, 11
                        mov              r10, 3;                              jmp   n11_statement_end_α
n10_statement_begin_β:  mov              r11, 11;                             jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    mov              r11, 12
                        mov              r10, 3;                              jmp   n12_statement_begin_α
#=======================================================================================================================
#         ispos(5)                                                    :S(A)F(B)
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:  mov              r11, 13
                        mov              r10, 4;                              jmp   n13_lit_integer_α
n12_statement_begin_β:  mov              r11, 13;                             jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:      sub              rsp, 16
                        mov              r11, 14
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx68_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n14_call_α
.Lx68_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:             sub              rsp, 16
                        mov              r11, 15
                        lea              rcx, [rip + .Lsig70z]
                        lea              rax, [rip + ispos_α];                jmp   rax
.Lsig70z:               .quad            1
                        .quad            .Lx70_2
                        .quad            .Lx70_2
                        .quad            16
.Lx70_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx70_29
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
.Lx70_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx70_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n12_statement_begin_β
.Lx70_240:                                                                    jmp   n15_statement_end_α
n14_call_β:             mov              r11, 15;                             jmp   n12_statement_begin_β
.Lx70_0:                .quad            .Lx70_0_s
.Lx70_0_s:              .string          "ispos"
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    mov              r11, 16
                        mov              r10, 4
                        add              rsp, 32;                             jmp   n16_statement_begin_α
#=======================================================================================================================
# A       OUTPUT = 'positive'
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:  mov              r11, 17
                        mov              r10, 5;                              jmp   n17_lit_string_α
n16_statement_begin_β:  mov              r11, 17;                             jmp   n20_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:       sub              rsp, 16
                        mov              r11, 18
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_assign_α
.Lx75_0:                .quad            .Lx75_0_s
.Lx75_0_s:              .string          "positive"
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:           mov              r11, 19
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n19_statement_end_α
.Lx76_0:                .quad            .Lx76_0_s
.Lx76_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:    mov              r11, 20
                        mov              r10, 5
                        add              rsp, 16;                             jmp   n20_statement_begin_α
#=======================================================================================================================
#         :(NEXT)
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:  mov              r11, 21
                        mov              r10, 6;                              jmp   n21_statement_end_α
n20_statement_begin_β:  mov              r11, 21;                             jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    mov              r11, 22
                        mov              r10, 6;                              jmp   n26_statement_begin_α
#=======================================================================================================================
# B       OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:  mov              r11, 23
                        mov              r10, 7;                              jmp   n23_lit_string_α
n22_statement_begin_β:  mov              r11, 23;                             jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:       sub              rsp, 16
                        mov              r11, 24
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_assign_α
.Lx85_0:                .quad            .Lx85_0_s
.Lx85_0_s:              .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:           mov              r11, 25
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n25_statement_end_α
.Lx86_0:                .quad            .Lx86_0_s
.Lx86_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    mov              r11, 26
                        mov              r10, 7
                        add              rsp, 16;                             jmp   n26_statement_begin_α
#=======================================================================================================================
# NEXT    ispos(-3)                                                   :S(C)F(D)
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:  mov              r11, 27
                        mov              r10, 8;                              jmp   n27_lit_integer_α
n26_statement_begin_β:  mov              r11, 27;                             jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:      sub              rsp, 16
                        mov              r11, 28
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n28_unop_α
.Lx91_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n28_unop_α:             sub              rsp, 16
                        mov              r11, 29
                        mov              rdi, qword ptr [rsp + 16]            # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_num_neg@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_call_α
n28_unop_β:             mov              r11, 29
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n26_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n29_call_α:             sub              rsp, 16
                        mov              r11, 30
                        lea              rcx, [rip + .Lsig94z]
                        lea              rax, [rip + ispos_α];                jmp   rax
.Lsig94z:               .quad            1
                        .quad            .Lx94_2
                        .quad            .Lx94_2
                        .quad            16
.Lx94_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx94_29
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
.Lx94_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx94_240
                        add              rsp, 16;                             jmp   n28_unop_β
.Lx94_240:                                                                    jmp   n30_statement_end_α
n29_call_β:             mov              r11, 30;                             jmp   n28_unop_β
.Lx94_0:                .quad            .Lx94_0_s
.Lx94_0_s:              .string          "ispos"
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    mov              r11, 31
                        mov              r10, 8
                        add              rsp, 48;                             jmp   n31_statement_begin_α
#=======================================================================================================================
# C       OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:  mov              r11, 32
                        mov              r10, 9;                              jmp   n32_lit_string_α
n31_statement_begin_β:  mov              r11, 32;                             jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:       sub              rsp, 16
                        mov              r11, 33
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n33_assign_α
.Lx99_0:                .quad            .Lx99_0_s
.Lx99_0_s:              .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:           mov              r11, 34
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx100_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n34_statement_end_α
.Lx100_0:               .quad            .Lx100_0_s
.Lx100_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    mov              r11, 35
                        mov              r10, 9
                        add              rsp, 16;                             jmp   n35_statement_begin_α
#=======================================================================================================================
#         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:  mov              r11, 36
                        mov              r10, 10;                             jmp   n36_statement_end_α
n35_statement_begin_β:  mov              r11, 36;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    mov              r11, 37
                        mov              r10, 10;                             jmp   main_γ
#=======================================================================================================================
# D       OUTPUT = 'not positive'
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:  mov              r11, 38
                        mov              r10, 11;                             jmp   n38_lit_string_α
n37_statement_begin_β:  mov              r11, 38;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              r11, 39
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 12
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_assign_α
.Lx109_0:               .quad            .Lx109_0_s
.Lx109_0_s:             .string          "not positive"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:           mov              r11, 40
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n40_statement_end_α
.Lx110_0:               .quad            .Lx110_0_s
.Lx110_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:    mov              r11, 41
                        mov              r10, 11
                        add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 42
                        pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                mov              r11, 43
                        add              rsp, 8
                        pop              rcx;                                 jmp   rcx
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
.Lseala1:               .string          "ispos"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            ispos_α
                        lea              rdi, [rip + .Lseala1]
                        mov              rsi, qword ptr [rip + ispos_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits

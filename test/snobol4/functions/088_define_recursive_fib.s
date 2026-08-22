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
.Lgvan0:                .string          "fib"
.Lgvan1:                .string          "n"
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
#         DEFINE('fib(n)')                                            :(fib_end)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 1;                              jmp   n1_define_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_define_α:            mov              r11, 2
                        mov              rdi, qword ptr [rip + .Lx52_0]
                        mov              rsi, qword ptr [rip + .Lx52_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n3_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx52_0]
                        lea              rsi, [rip + fib_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n2_statement_end_α
n1_define_β:            mov              r11, 2;                              jmp   n0_statement_begin_β
.Lx52_0:                .quad            .Lx52_0_s
.Lx52_0_s:              .string          "fib"
.Lx52_1:                .quad            .Lx52_1_s
.Lx52_1_s:              .string          "n"
                                                                              jmp   .Lx53_245
#-----------------------------------------------------------------------------------------------------------------------
fib_α:                  sub              rsp, 64
                        mov              rax, qword ptr [r9 + 0]              # fib
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx53_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # n
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx53_41
.Lx53_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx53_41:               lea              rcx, [rip + fib_γ]
                        lea              rax, [rip + fib_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n3_statement_begin_α];   jmp   rax
fib_γ:                  mov              rdi, qword ptr [r9 + 0]              # fib
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx53_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # n
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx53_110
.Lx53_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx53_110:              mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
fib_ω:                  mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax              # fib
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx53_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # n
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx53_180
.Lx53_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx53_180:              mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx53_245:
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_end_α:     mov              r11, 3
                        mov              r10, 1;                              jmp   n26_statement_begin_α
#=======================================================================================================================
# fib     LE(n, 1)                                                   :S(base)
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_begin_α:   mov              r11, 4
                        mov              r10, 2;                              jmp   n4_var_α
n3_statement_begin_β:   mov              r11, 4;                              jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:               sub              rsp, 16
                        mov              r11, 5
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n5_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:       sub              rsp, 16
                        mov              r11, 6
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx59_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n6_coerce_numeric_α
n5_lit_integer_β:       mov              r11, 6
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n3_statement_begin_β
.Lx59_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n6_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 7
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx61_1
                        cmp              al, 3;                               jne   .Lx61_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx61_0
.Lx61_1:                mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n7_coerce_numeric_α
.Lx61_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 118
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
                        cmp              al, 5;                               je    .Lx63_1
                        cmp              al, 3;                               jne   .Lx63_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx63_0
.Lx63_1:                mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n8_cmp_test_α
.Lx63_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 119
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
                        cmp              dl, 3;                               jne   .Lx65_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jle   .Lx65_239
                        add              rsp, 16;                             jmp   n7_coerce_numeric_β
.Lx65_239:                                                                    jmp   n9_statement_end_α
.Lx65_0:                lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jle   .Lx65_240
                        add              rsp, 16;                             jmp   n7_coerce_numeric_β
.Lx65_240:                                                                    jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     mov              r11, 10
                        mov              r10, 2
                        add              rsp, 80;                             jmp   n22_statement_begin_α
#=======================================================================================================================
#         fib = fib(n - 1) + fib(n - 2)                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:  mov              r11, 11
                        mov              r10, 3;                              jmp   n11_var_α
n10_statement_begin_β:  mov              r11, 11;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:              sub              rsp, 16
                        mov              r11, 12
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n12_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:      sub              rsp, 16
                        mov              r11, 13
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n13_binop_α
n12_lit_integer_β:      mov              r11, 13
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n10_statement_begin_β
.Lx71_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n13_binop_α:            sub              rsp, 16
                        mov              r11, 14
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx72_2
                        sub              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx72_7
.Lx72_2:                mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx72_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx72_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx72_4
.Lx72_3:                movq             xmm0, rsi
.Lx72_4:                cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx72_7:                                                                      jmp   n14_call_α
.Lx72_0:                mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx72_240
                        add              rsp, 16;                             jmp   n12_lit_integer_β
.Lx72_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n14_call_α
n13_binop_β:            mov              r11, 14
                        add              rsp, 16;                             jmp   n12_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:             sub              rsp, 16
                        mov              r11, 15
                        lea              rcx, [rip + .Lsig74z]
                        lea              rax, [rip + fib_α];                  jmp   rax
.Lsig74z:               .quad            1
                        .quad            .Lx74_2
                        .quad            .Lx74_2
                        .quad            16
.Lx74_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx74_29
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
.Lx74_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx74_240
                        add              rsp, 16;                             jmp   n13_binop_β
.Lx74_240:                                                                    jmp   n15_var_α
n14_call_β:             mov              r11, 15;                             jmp   n13_binop_β
.Lx74_0:                .quad            .Lx74_0_s
.Lx74_0_s:              .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:              sub              rsp, 16
                        mov              r11, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n16_lit_integer_α
n15_var_β:              mov              r11, 16
                        add              rsp, 32;                             jmp   n13_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:      sub              rsp, 16
                        mov              r11, 17
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_binop_α
n16_lit_integer_β:      mov              r11, 17
                        add              rsp, 16;                             jmp   n15_var_β
.Lx76_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n17_binop_α:            sub              rsp, 16
                        mov              r11, 18
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx77_2
                        sub              rax, 2
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx77_7
.Lx77_2:                mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx77_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 2
                        cmp              al, 5;                               je    .Lx77_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx77_4
.Lx77_3:                movq             xmm0, rsi
.Lx77_4:                cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx77_7:                                                                      jmp   n18_call_α
.Lx77_0:                mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx77_240
                        add              rsp, 16;                             jmp   n16_lit_integer_β
.Lx77_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n18_call_α
n17_binop_β:            mov              r11, 18
                        add              rsp, 16;                             jmp   n16_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:             sub              rsp, 16
                        mov              r11, 19
                        lea              rcx, [rip + .Lsig79z]
                        lea              rax, [rip + fib_α];                  jmp   rax
.Lsig79z:               .quad            1
                        .quad            .Lx79_2
                        .quad            .Lx79_2
                        .quad            16
.Lx79_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx79_29
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
.Lx79_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx79_240
                        add              rsp, 16;                             jmp   n17_binop_β
.Lx79_240:                                                                    jmp   n19_binop_α
n18_call_β:             mov              r11, 19;                             jmp   n17_binop_β
.Lx79_0:                .quad            .Lx79_0_s
.Lx79_0_s:              .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n19_binop_α:            sub              rsp, 16
                        mov              r11, 20
                        mov              eax, dword ptr [rsp + 80]            # call
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx80_2
                        mov              rax, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 24]
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx80_7
.Lx80_2:                and              edx, 1;                              jz    .Lx80_0
                        mov              rsi, qword ptr [rsp + 88]            # call
                        mov              rdi, qword ptr [rsp + 24]
                        cmp              al, 5;                               je    .Lx80_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx80_4
.Lx80_3:                movq             xmm0, rsi
.Lx80_4:                cmp              cl, 5;                               je    .Lx80_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx80_6
.Lx80_5:                movq             xmm1, rdi
.Lx80_6:                addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx80_7:                                                                      jmp   n20_assign_α
.Lx80_0:                mov              rdi, qword ptr [rsp + 80]            # call
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_add@PLT
                        cmp              al, 104;                             jne   .Lx80_240
                        add              rsp, 32;                             jmp   n17_binop_β
.Lx80_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n20_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:           mov              r11, 21
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # fib
                        mov              qword ptr [r9 + 8], rdx;             jmp   n21_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    mov              r11, 22
                        mov              r10, 3
                        add              rsp, 144;                            jmp   RETURN
#=======================================================================================================================
# base    fib = n                                                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:  mov              r11, 23
                        mov              r10, 4;                              jmp   n23_var_α
n22_statement_begin_β:  mov              r11, 23;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n23_var_α:              sub              rsp, 16
                        mov              r11, 24
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n24_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:           mov              r11, 25
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # fib
                        mov              qword ptr [r9 + 8], rdx;             jmp   n25_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    mov              r11, 26
                        mov              r10, 4
                        add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# fib_end  <stmt 5, line 7: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:  mov              r11, 27
                        mov              r10, 5;                              jmp   n27_statement_end_α
n26_statement_begin_β:  mov              r11, 27;                             jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:    mov              r11, 28
                        mov              r10, 5;                              jmp   n28_statement_begin_α
#=======================================================================================================================
#         OUTPUT = fib(0)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:  mov              r11, 29
                        mov              r10, 6;                              jmp   n29_lit_integer_α
n28_statement_begin_β:  mov              r11, 29;                             jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:      sub              rsp, 16
                        mov              r11, 30
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_call_α
.Lx96_0:                .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n30_call_α:             sub              rsp, 16
                        mov              r11, 31
                        lea              rcx, [rip + .Lsig98z]
                        lea              rax, [rip + fib_α];                  jmp   rax
.Lsig98z:               .quad            1
                        .quad            .Lx98_2
                        .quad            .Lx98_2
                        .quad            16
.Lx98_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx98_29
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
.Lx98_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx98_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n28_statement_begin_β
.Lx98_240:                                                                    jmp   n31_assign_α
n30_call_β:             mov              r11, 31;                             jmp   n28_statement_begin_β
.Lx98_0:                .quad            .Lx98_0_s
.Lx98_0_s:              .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:           mov              r11, 32
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n32_statement_end_α
.Lx99_0:                .quad            .Lx99_0_s
.Lx99_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    mov              r11, 33
                        mov              r10, 6
                        add              rsp, 32;                             jmp   n33_statement_begin_α
#=======================================================================================================================
#         OUTPUT = fib(1)
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:  mov              r11, 34
                        mov              r10, 7;                              jmp   n34_lit_integer_α
n33_statement_begin_β:  mov              r11, 34;                             jmp   n38_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:      sub              rsp, 16
                        mov              r11, 35
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx104_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n35_call_α
.Lx104_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:             sub              rsp, 16
                        mov              r11, 36
                        lea              rcx, [rip + .Lsig106z]
                        lea              rax, [rip + fib_α];                  jmp   rax
.Lsig106z:              .quad            1
                        .quad            .Lx106_2
                        .quad            .Lx106_2
                        .quad            16
.Lx106_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx106_29
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
.Lx106_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx106_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n33_statement_begin_β
.Lx106_240:                                                                   jmp   n36_assign_α
n35_call_β:             mov              r11, 36;                             jmp   n33_statement_begin_β
.Lx106_0:               .quad            .Lx106_0_s
.Lx106_0_s:             .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_α:           mov              r11, 37
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n37_statement_end_α
.Lx107_0:               .quad            .Lx107_0_s
.Lx107_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:    mov              r11, 38
                        mov              r10, 7
                        add              rsp, 32;                             jmp   n38_statement_begin_α
#=======================================================================================================================
#         OUTPUT = fib(6)
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_begin_α:  mov              r11, 39
                        mov              r10, 8;                              jmp   n39_lit_integer_α
n38_statement_begin_β:  mov              r11, 39;                             jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:      sub              rsp, 16
                        mov              r11, 40
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx112_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n40_call_α
.Lx112_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n40_call_α:             sub              rsp, 16
                        mov              r11, 41
                        lea              rcx, [rip + .Lsig114z]
                        lea              rax, [rip + fib_α];                  jmp   rax
.Lsig114z:              .quad            1
                        .quad            .Lx114_2
                        .quad            .Lx114_2
                        .quad            16
.Lx114_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx114_29
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
.Lx114_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx114_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n38_statement_begin_β
.Lx114_240:                                                                   jmp   n41_assign_α
n40_call_β:             mov              r11, 41;                             jmp   n38_statement_begin_β
.Lx114_0:               .quad            .Lx114_0_s
.Lx114_0_s:             .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:           mov              r11, 42
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx115_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n42_statement_end_α
.Lx115_0:               .quad            .Lx115_0_s
.Lx115_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:    mov              r11, 43
                        mov              r10, 8
                        add              rsp, 32;                             jmp   n43_statement_begin_α
#=======================================================================================================================
#         OUTPUT = fib(10)
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:  mov              r11, 44
                        mov              r10, 9;                              jmp   n44_lit_integer_α
n43_statement_begin_β:  mov              r11, 44;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_integer_α:      sub              rsp, 16
                        mov              r11, 45
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n45_call_α
.Lx120_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n45_call_α:             sub              rsp, 16
                        mov              r11, 46
                        lea              rcx, [rip + .Lsig122z]
                        lea              rax, [rip + fib_α];                  jmp   rax
.Lsig122z:              .quad            1
                        .quad            .Lx122_2
                        .quad            .Lx122_2
                        .quad            16
.Lx122_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx122_29
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
.Lx122_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx122_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n43_statement_begin_β
.Lx122_240:                                                                   jmp   n46_assign_α
n45_call_β:             mov              r11, 46;                             jmp   n43_statement_begin_β
.Lx122_0:               .quad            .Lx122_0_s
.Lx122_0_s:             .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n46_assign_α:           mov              r11, 47
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n47_statement_end_α
.Lx123_0:               .quad            .Lx123_0_s
.Lx123_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:    mov              r11, 48
                        mov              r10, 9
                        add              rsp, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 49
                        pop              rcx
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
.Lseala1:               .string          "fib"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            fib_α
                        lea              rdi, [rip + .Lseala1]
                        mov              rsi, qword ptr [rip + fib_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits

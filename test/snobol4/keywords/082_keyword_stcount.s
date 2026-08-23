                        .intel_syntax    noprefix
                        .text
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
                        .section         .rodata
.Lgvan0:                .string          "X"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         X = 1
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:       sub              rsp, 16
                        mov              r11, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx37_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n1_call_α
.Lx37_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n1_call_α:              sub              rsp, 16
                        mov              r11, 2
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd39:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd39]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx38_240
                        add              rsp, 16;                             jmp   n2_statement_begin_α
.Lx38_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n2_statement_begin_α
n1_call_β:              mov              r11, 2
                        add              rsp, 16;                             jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 1;                              jmp   n3_lit_integer_α
n2_statement_begin_β:   mov              r11, 3
                        add              rsp, 32;                             jmp   n6_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              r11, 4
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_assign_α
.Lx42_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:            mov              r11, 5
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # X
                        mov              qword ptr [r9 + 8], rdx;             jmp   n5_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     mov              r11, 6
                        mov              r10, 1
                        add              rsp, 48;                             jmp   n6_lit_integer_α
#=======================================================================================================================
#         X = 2
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:       sub              rsp, 16
                        mov              r11, 7
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n7_call_α
.Lx46_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:              sub              rsp, 16
                        mov              r11, 8
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd48:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd48]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx47_240
                        add              rsp, 16;                             jmp   n8_statement_begin_α
.Lx47_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_statement_begin_α
n7_call_β:              mov              r11, 8
                        add              rsp, 16;                             jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:   mov              r11, 9
                        mov              r10, 2;                              jmp   n9_lit_integer_α
n8_statement_begin_β:   mov              r11, 9
                        add              rsp, 32;                             jmp   n12_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:       sub              rsp, 16
                        mov              r11, 10
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_assign_α
.Lx51_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:           mov              r11, 11
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # X
                        mov              qword ptr [r9 + 8], rdx;             jmp   n11_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    mov              r11, 12
                        mov              r10, 2
                        add              rsp, 48;                             jmp   n12_lit_integer_α
#=======================================================================================================================
#         GT(&STNO, 1)                                                :S(YES)F(NO)
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:      sub              rsp, 16
                        mov              r11, 13
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx55_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n13_call_α
.Lx55_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:             sub              rsp, 16
                        mov              r11, 14
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd57:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd57]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx56_240
                        add              rsp, 16;                             jmp   n14_statement_begin_α
.Lx56_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n14_statement_begin_α
n13_call_β:             mov              r11, 14
                        add              rsp, 16;                             jmp   n14_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:  mov              r11, 15
                        mov              r10, 3;                              jmp   n15_keyword_snobol4_α
n14_statement_begin_β:  mov              r11, 15
                        add              rsp, 32;                             jmp   n23_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n15_keyword_snobol4_α:  sub              rsp, 16
                        mov              r11, 16
                        mov              rcx, qword ptr [rip + g_stno@GOTPCREL]
                        mov              rdx, qword ptr [rcx]
                        mov              rax, 3
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n16_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:      sub              rsp, 16
                        mov              r11, 17
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_coerce_numeric_α
n16_lit_integer_β:      mov              r11, 17
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n14_statement_begin_β
.Lx61_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n17_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 18
                        mov              eax, dword ptr [rsp + 32]            # keyword_snobol4
                        cmp              al, 5;                               je    .Lx63_1
                        cmp              al, 3;                               jne   .Lx63_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx63_0
.Lx63_1:                mov              rax, qword ptr [rsp + 32]            # keyword_snobol4
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # keyword_snobol4
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_coerce_numeric_α
.Lx63_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n18_coerce_numeric_α
n17_coerce_numeric_β:   mov              r11, 18
                        add              rsp, 16;                             jmp   n16_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n18_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 19
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx65_1
                        cmp              al, 3;                               jne   .Lx65_0
                        mov              eax, dword ptr [rsp + 48]            # keyword_snobol4
                        cmp              al, 3;                               jne   .Lx65_0
.Lx65_1:                mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n19_cmp_test_α
.Lx65_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # keyword_snobol4
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n19_cmp_test_α
n18_coerce_numeric_β:   mov              r11, 19
                        add              rsp, 16;                             jmp   n17_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n19_cmp_test_α:         sub              rsp, 16
                        mov              r11, 20
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx67_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jg    .Lx67_239
                        add              rsp, 16;                             jmp   n18_coerce_numeric_β
.Lx67_239:                                                                    jmp   n20_statement_end_α
.Lx67_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jg    .Lx67_240
                        add              rsp, 16;                             jmp   n18_coerce_numeric_β
.Lx67_240:                                                                    jmp   n20_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:    mov              r11, 21
                        mov              r10, 3
                        add              rsp, 112;                            jmp   n21_lit_integer_α
#=======================================================================================================================
# YES     OUTPUT = 'stno ok'
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:      sub              rsp, 16
                        mov              r11, 22
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n22_call_α
.Lx70_0:                .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:             sub              rsp, 16
                        mov              r11, 23
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd72:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd72]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx71_240
                        add              rsp, 16;                             jmp   n25_statement_begin_α
.Lx71_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n25_statement_begin_α
n22_call_β:             mov              r11, 23
                        add              rsp, 16;                             jmp   n25_statement_begin_α
#=======================================================================================================================
# NO      OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:      sub              rsp, 16
                        mov              r11, 24
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_call_α
.Lx73_0:                .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:             sub              rsp, 16
                        mov              r11, 25
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd75:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd75]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx74_240
                        add              rsp, 16;                             jmp   n33_statement_begin_α
.Lx74_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n33_statement_begin_α
n24_call_β:             mov              r11, 25
                        add              rsp, 16;                             jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_begin_α:  mov              r11, 26
                        mov              r10, 4;                              jmp   n26_lit_string_α
n25_statement_begin_β:  mov              r11, 26
                        add              rsp, 32;                             jmp   n29_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:       sub              rsp, 16
                        mov              r11, 27
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx78_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n27_assign_α
.Lx78_0:                .quad            .Lx78_0_s
.Lx78_0_s:              .string          "stno ok"
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:           mov              r11, 28
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx79_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n28_statement_end_α
.Lx79_0:                .quad            .Lx79_0_s
.Lx79_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    mov              r11, 29
                        mov              r10, 4
                        add              rsp, 48;                             jmp   n29_lit_integer_α
#=======================================================================================================================
#         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:      sub              rsp, 16
                        mov              r11, 30
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_call_α
.Lx82_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n30_call_α:             sub              rsp, 16
                        mov              r11, 31
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd84:             .string          "SNO$STMT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd84]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx83_240
                        add              rsp, 16;                             jmp   n31_statement_begin_α
.Lx83_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n31_statement_begin_α
n30_call_β:             mov              r11, 31
                        add              rsp, 16;                             jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:  mov              r11, 32
                        mov              r10, 5;                              jmp   n32_statement_end_α
n31_statement_begin_β:  mov              r11, 32
                        add              rsp, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    mov              r11, 33
                        mov              r10, 5
                        add              rsp, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:  mov              r11, 34
                        mov              r10, 6;                              jmp   n34_lit_string_α
n33_statement_begin_β:  mov              r11, 34
                        add              rsp, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:       sub              rsp, 16
                        mov              r11, 35
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n35_assign_α
.Lx91_0:                .quad            .Lx91_0_s
.Lx91_0_s:              .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_α:           mov              r11, 36
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx92_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n36_statement_end_α
.Lx92_0:                .quad            .Lx92_0_s
.Lx92_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    mov              r11, 37
                        mov              r10, 6
                        add              rsp, 48;                             jmp   main_γ
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

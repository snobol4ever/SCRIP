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
#=======================================================================================================================
#         LT(3, 5)                                                    :S(A)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 1;                              jmp   n1_lit_integer_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:       sub              rsp, 16
                        mov              r11, 2
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n2_lit_integer_α
.Lx35_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:       sub              rsp, 16
                        mov              r11, 3
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n3_coerce_numeric_α
n2_lit_integer_β:       mov              r11, 3
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n0_statement_begin_β
.Lx36_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 4
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx38_1
                        cmp              al, 3;                               jne   .Lx38_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx38_0
.Lx38_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_coerce_numeric_α
.Lx38_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 147
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n4_coerce_numeric_α
n3_coerce_numeric_β:    mov              r11, 4
                        add              rsp, 16;                             jmp   n2_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n4_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 5
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx40_1
                        cmp              al, 3;                               jne   .Lx40_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx40_0
.Lx40_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_cmp_test_α
.Lx40_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 148
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n5_cmp_test_α
n4_coerce_numeric_β:    mov              r11, 5
                        add              rsp, 16;                             jmp   n3_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n5_cmp_test_α:          sub              rsp, 16
                        mov              r11, 6
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx42_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jl    .Lx42_239
                        add              rsp, 16;                             jmp   n4_coerce_numeric_β
.Lx42_239:                                                                    jmp   n6_statement_end_α
.Lx42_0:                lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            js    .Lx42_240
                        add              rsp, 16;                             jmp   n4_coerce_numeric_β
.Lx42_240:                                                                    jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     mov              r11, 7
                        mov              r10, 1
                        add              rsp, 80;                             jmp   n7_statement_begin_α
#=======================================================================================================================
# A       OUTPUT = '3 < 5'
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:   mov              r11, 8
                        mov              r10, 2;                              jmp   n8_lit_string_α
n7_statement_begin_β:   mov              r11, 8;                              jmp   n11_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:        sub              rsp, 16
                        mov              r11, 9
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx47_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_assign_α
.Lx47_0:                .quad            .Lx47_0_s
.Lx47_0_s:              .string          "3 < 5"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:            mov              r11, 10
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx48_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n10_statement_end_α
.Lx48_0:                .quad            .Lx48_0_s
.Lx48_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:    mov              r11, 11
                        mov              r10, 2
                        add              rsp, 16;                             jmp   n11_statement_begin_α
#=======================================================================================================================
#         LE(5, 5)                                                    :S(B)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:  mov              r11, 12
                        mov              r10, 3;                              jmp   n12_lit_integer_α
n11_statement_begin_β:  mov              r11, 12;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:      sub              rsp, 16
                        mov              r11, 13
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n13_lit_integer_α
.Lx53_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:      sub              rsp, 16
                        mov              r11, 14
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n14_coerce_numeric_α
n13_lit_integer_β:      mov              r11, 14
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n11_statement_begin_β
.Lx54_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n14_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 15
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx56_1
                        cmp              al, 3;                               jne   .Lx56_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx56_0
.Lx56_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n15_coerce_numeric_α
.Lx56_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 118
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n15_coerce_numeric_α
n14_coerce_numeric_β:   mov              r11, 15
                        add              rsp, 16;                             jmp   n13_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n15_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx58_1
                        cmp              al, 3;                               jne   .Lx58_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx58_0
.Lx58_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_cmp_test_α
.Lx58_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 119
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n16_cmp_test_α
n15_coerce_numeric_β:   mov              r11, 16
                        add              rsp, 16;                             jmp   n14_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n16_cmp_test_α:         sub              rsp, 16
                        mov              r11, 17
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx60_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jle   .Lx60_239
                        add              rsp, 16;                             jmp   n15_coerce_numeric_β
.Lx60_239:                                                                    jmp   n17_statement_end_α
.Lx60_0:                lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jle   .Lx60_240
                        add              rsp, 16;                             jmp   n15_coerce_numeric_β
.Lx60_240:                                                                    jmp   n17_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    mov              r11, 18
                        mov              r10, 3
                        add              rsp, 80;                             jmp   n18_statement_begin_α
#=======================================================================================================================
# B       OUTPUT = '5 <= 5'
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:  mov              r11, 19
                        mov              r10, 4;                              jmp   n19_lit_string_α
n18_statement_begin_β:  mov              r11, 19;                             jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:       sub              rsp, 16
                        mov              r11, 20
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n20_assign_α
.Lx65_0:                .quad            .Lx65_0_s
.Lx65_0_s:              .string          "5 <= 5"
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:           mov              r11, 21
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx66_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n21_statement_end_α
.Lx66_0:                .quad            .Lx66_0_s
.Lx66_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    mov              r11, 22
                        mov              r10, 4
                        add              rsp, 16;                             jmp   n22_statement_begin_α
#=======================================================================================================================
#         GE(7, 5)                                                    :S(C)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:  mov              r11, 23
                        mov              r10, 5;                              jmp   n23_lit_integer_α
n22_statement_begin_β:  mov              r11, 23;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:      sub              rsp, 16
                        mov              r11, 24
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_lit_integer_α
.Lx71_0:                .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:      sub              rsp, 16
                        mov              r11, 25
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx72_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n25_coerce_numeric_α
n24_lit_integer_β:      mov              r11, 25
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n22_statement_begin_β
.Lx72_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n25_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 26
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx74_1
                        cmp              al, 3;                               jne   .Lx74_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx74_0
.Lx74_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n26_coerce_numeric_α
.Lx74_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n26_coerce_numeric_α
n25_coerce_numeric_β:   mov              r11, 26
                        add              rsp, 16;                             jmp   n24_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n26_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 27
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx76_1
                        cmp              al, 3;                               jne   .Lx76_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx76_0
.Lx76_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n27_cmp_test_α
.Lx76_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n27_cmp_test_α
n26_coerce_numeric_β:   mov              r11, 27
                        add              rsp, 16;                             jmp   n25_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n27_cmp_test_α:         sub              rsp, 16
                        mov              r11, 28
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx78_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx78_239
                        add              rsp, 16;                             jmp   n26_coerce_numeric_β
.Lx78_239:                                                                    jmp   n28_statement_end_α
.Lx78_0:                lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jns   .Lx78_240
                        add              rsp, 16;                             jmp   n26_coerce_numeric_β
.Lx78_240:                                                                    jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    mov              r11, 29
                        mov              r10, 5
                        add              rsp, 80;                             jmp   n29_statement_begin_α
#=======================================================================================================================
# C       OUTPUT = '7 >= 5'
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:  mov              r11, 30
                        mov              r10, 6;                              jmp   n30_lit_string_α
n29_statement_begin_β:  mov              r11, 30;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_string_α:       sub              rsp, 16
                        mov              r11, 31
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx83_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_assign_α
.Lx83_0:                .quad            .Lx83_0_s
.Lx83_0_s:              .string          "7 >= 5"
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:           mov              r11, 32
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n32_statement_end_α
.Lx84_0:                .quad            .Lx84_0_s
.Lx84_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    mov              r11, 33
                        mov              r10, 6
                        add              rsp, 16;                             jmp   main_γ
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

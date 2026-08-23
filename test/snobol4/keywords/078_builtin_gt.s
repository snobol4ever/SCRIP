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
#         GT(5, 3)                                                    :S(YES)F(NO)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 1;                              jmp   n1_lit_integer_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:       sub              rsp, 16
                        mov              r11, 2
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n2_lit_integer_α
.Lx36_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:       sub              rsp, 16
                        mov              r11, 3
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx37_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n3_coerce_numeric_α
n2_lit_integer_β:       mov              r11, 3
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n0_statement_begin_β
.Lx37_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 4
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx39_1
                        cmp              al, 3;                               jne   .Lx39_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx39_0
.Lx39_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_coerce_numeric_α
.Lx39_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n4_coerce_numeric_α
n3_coerce_numeric_β:    mov              r11, 4
                        add              rsp, 16;                             jmp   n2_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n4_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 5
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx41_1
                        cmp              al, 3;                               jne   .Lx41_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx41_0
.Lx41_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_cmp_test_α
.Lx41_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n5_cmp_test_α
n4_coerce_numeric_β:    mov              r11, 5
                        add              rsp, 16;                             jmp   n3_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n5_cmp_test_α:          sub              rsp, 16
                        mov              r11, 6
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx43_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jg    .Lx43_239
                        add              rsp, 16;                             jmp   n4_coerce_numeric_β
.Lx43_239:                                                                    jmp   n6_statement_end_α
.Lx43_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jg    .Lx43_240
                        add              rsp, 16;                             jmp   n4_coerce_numeric_β
.Lx43_240:                                                                    jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     mov              r11, 7
                        mov              r10, 1
                        add              rsp, 80;                             jmp   n7_statement_begin_α
#=======================================================================================================================
# YES     OUTPUT = '5 > 3'
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:   mov              r11, 8
                        mov              r10, 2;                              jmp   n8_lit_string_α
n7_statement_begin_β:   mov              r11, 8;                              jmp   n11_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:        sub              rsp, 16
                        mov              r11, 9
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx48_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_assign_α
.Lx48_0:                .quad            .Lx48_0_s
.Lx48_0_s:              .string          "5 > 3"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:            mov              r11, 10
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n10_statement_end_α
.Lx49_0:                .quad            .Lx49_0_s
.Lx49_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:    mov              r11, 11
                        mov              r10, 2
                        add              rsp, 16;                             jmp   n11_statement_begin_α
#=======================================================================================================================
#         :(NEXT)
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:  mov              r11, 12
                        mov              r10, 3;                              jmp   n12_statement_end_α
n11_statement_begin_β:  mov              r11, 12;                             jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:    mov              r11, 13
                        mov              r10, 3;                              jmp   n17_statement_begin_α
#=======================================================================================================================
# NO      OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:  mov              r11, 14
                        mov              r10, 4;                              jmp   n14_lit_string_α
n13_statement_begin_β:  mov              r11, 14;                             jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:       sub              rsp, 16
                        mov              r11, 15
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx58_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n15_assign_α
.Lx58_0:                .quad            .Lx58_0_s
.Lx58_0_s:              .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_α:           mov              r11, 16
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx59_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n16_statement_end_α
.Lx59_0:                .quad            .Lx59_0_s
.Lx59_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:    mov              r11, 17
                        mov              r10, 4
                        add              rsp, 16;                             jmp   n17_statement_begin_α
#=======================================================================================================================
# NEXT    GT(3, 5)                                                    :S(YES2)F(NO2)
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:  mov              r11, 18
                        mov              r10, 5;                              jmp   n18_lit_integer_α
n17_statement_begin_β:  mov              r11, 18;                             jmp   n30_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_integer_α:      sub              rsp, 16
                        mov              r11, 19
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n19_lit_integer_α
.Lx64_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:      sub              rsp, 16
                        mov              r11, 20
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n20_coerce_numeric_α
n19_lit_integer_β:      mov              r11, 20
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n17_statement_begin_β
.Lx65_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n20_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 21
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx67_1
                        cmp              al, 3;                               jne   .Lx67_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx67_0
.Lx67_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n21_coerce_numeric_α
.Lx67_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n21_coerce_numeric_α
n20_coerce_numeric_β:   mov              r11, 21
                        add              rsp, 16;                             jmp   n19_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n21_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 22
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx69_1
                        cmp              al, 3;                               jne   .Lx69_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx69_0
.Lx69_1:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n22_cmp_test_α
.Lx69_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n22_cmp_test_α
n21_coerce_numeric_β:   mov              r11, 22
                        add              rsp, 16;                             jmp   n20_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n22_cmp_test_α:         sub              rsp, 16
                        mov              r11, 23
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx71_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jg    .Lx71_239
                        add              rsp, 16;                             jmp   n21_coerce_numeric_β
.Lx71_239:                                                                    jmp   n23_statement_end_α
.Lx71_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jg    .Lx71_240
                        add              rsp, 16;                             jmp   n21_coerce_numeric_β
.Lx71_240:                                                                    jmp   n23_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:    mov              r11, 24
                        mov              r10, 5
                        add              rsp, 80;                             jmp   n24_statement_begin_α
#=======================================================================================================================
# YES2    OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:  mov              r11, 25
                        mov              r10, 6;                              jmp   n25_lit_string_α
n24_statement_begin_β:  mov              r11, 25;                             jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:       sub              rsp, 16
                        mov              r11, 26
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n26_assign_α
.Lx76_0:                .quad            .Lx76_0_s
.Lx76_0_s:              .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:           mov              r11, 27
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx77_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n27_statement_end_α
.Lx77_0:                .quad            .Lx77_0_s
.Lx77_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:    mov              r11, 28
                        mov              r10, 6
                        add              rsp, 16;                             jmp   n28_statement_begin_α
#=======================================================================================================================
#         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:  mov              r11, 29
                        mov              r10, 7;                              jmp   n29_statement_end_α
n28_statement_begin_β:  mov              r11, 29;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:    mov              r11, 30
                        mov              r10, 7;                              jmp   main_γ
#=======================================================================================================================
# NO2     OUTPUT = '3 not > 5'
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:  mov              r11, 31
                        mov              r10, 8;                              jmp   n31_lit_string_α
n30_statement_begin_β:  mov              r11, 31;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:       sub              rsp, 16
                        mov              r11, 32
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 9
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n32_assign_α
.Lx86_0:                .quad            .Lx86_0_s
.Lx86_0_s:              .string          "3 not > 5"
#-----------------------------------------------------------------------------------------------------------------------
n32_assign_α:           mov              r11, 33
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n33_statement_end_α
.Lx87_0:                .quad            .Lx87_0_s
.Lx87_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_end_α:    mov              r11, 34
                        mov              r10, 8
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

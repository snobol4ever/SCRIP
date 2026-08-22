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
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 0;                              jmp   n1_statement_end_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:     mov              r11, 2
                        mov              r10, 1;                              jmp   n2_statement_begin_α
#=======================================================================================================================
#         LT(5, 4)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_lit_integer_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              r11, 4
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx226_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_lit_integer_α
.Lx226_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:       sub              rsp, 16
                        mov              r11, 5
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx227_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_coerce_numeric_α
n4_lit_integer_β:       mov              r11, 5
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx227_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n5_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 6
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx229_1
                        cmp              al, 3;                               jne   .Lx229_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx229_0
.Lx229_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n6_coerce_numeric_α
.Lx229_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 147
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n6_coerce_numeric_α
n5_coerce_numeric_β:    mov              r11, 6
                        add              rsp, 16;                             jmp   n4_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n6_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 7
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx231_1
                        cmp              al, 3;                               jne   .Lx231_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx231_0
.Lx231_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n7_cmp_test_α
.Lx231_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 148
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n7_cmp_test_α
n6_coerce_numeric_β:    mov              r11, 7
                        add              rsp, 16;                             jmp   n5_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n7_cmp_test_α:          sub              rsp, 16
                        mov              r11, 8
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx233_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jl    .Lx233_239
                        add              rsp, 16;                             jmp   n6_coerce_numeric_β
.Lx233_239:                                                                   jmp   n8_statement_end_α
.Lx233_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            js    .Lx233_240
                        add              rsp, 16;                             jmp   n6_coerce_numeric_β
.Lx233_240:                                                                   jmp   n8_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:     mov              r11, 9
                        mov              r10, 2
                        add              rsp, 80;                             jmp   n9_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/001: lt(5,4) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:   mov              r11, 10
                        mov              r10, 3;                              jmp   n10_lit_string_α
n9_statement_begin_β:   mov              r11, 10;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:       sub              rsp, 16
                        mov              r11, 11
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx238_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n11_assign_α
.Lx238_0:               .quad            .Lx238_0_s
.Lx238_0_s:             .string          "FAIL 912/001: lt(5,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:           mov              r11, 12
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx239_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n12_statement_end_α
.Lx239_0:               .quad            .Lx239_0_s
.Lx239_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:    mov              r11, 13
                        mov              r10, 3
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001    LT(4, 4)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:  mov              r11, 14
                        mov              r10, 4;                              jmp   n14_lit_integer_α
n13_statement_begin_β:  mov              r11, 14;                             jmp   n24_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:      sub              rsp, 16
                        mov              r11, 15
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx244_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n15_lit_integer_α
.Lx244_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:      sub              rsp, 16
                        mov              r11, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx245_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_coerce_numeric_α
n15_lit_integer_β:      mov              r11, 16
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n13_statement_begin_β
.Lx245_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n16_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 17
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx247_1
                        cmp              al, 3;                               jne   .Lx247_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx247_0
.Lx247_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_coerce_numeric_α
.Lx247_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 147
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n17_coerce_numeric_α
n16_coerce_numeric_β:   mov              r11, 17
                        add              rsp, 16;                             jmp   n15_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n17_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 18
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx249_1
                        cmp              al, 3;                               jne   .Lx249_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx249_0
.Lx249_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_cmp_test_α
.Lx249_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 148
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n18_cmp_test_α
n17_coerce_numeric_β:   mov              r11, 18
                        add              rsp, 16;                             jmp   n16_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n18_cmp_test_α:         sub              rsp, 16
                        mov              r11, 19
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx251_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jl    .Lx251_239
                        add              rsp, 16;                             jmp   n17_coerce_numeric_β
.Lx251_239:                                                                   jmp   n19_statement_end_α
.Lx251_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            js    .Lx251_240
                        add              rsp, 16;                             jmp   n17_coerce_numeric_β
.Lx251_240:                                                                   jmp   n19_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:    mov              r11, 20
                        mov              r10, 4
                        add              rsp, 80;                             jmp   n20_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/002: lt(4,4) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:  mov              r11, 21
                        mov              r10, 5;                              jmp   n21_lit_string_α
n20_statement_begin_β:  mov              r11, 21;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:       sub              rsp, 16
                        mov              r11, 22
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx256_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n22_assign_α
.Lx256_0:               .quad            .Lx256_0_s
.Lx256_0_s:             .string          "FAIL 912/002: lt(4,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:           mov              r11, 23
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n23_statement_end_α
.Lx257_0:               .quad            .Lx257_0_s
.Lx257_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:    mov              r11, 24
                        mov              r10, 5
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002    LT(4, 5)                   :s(e003)
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:  mov              r11, 25
                        mov              r10, 6;                              jmp   n25_lit_integer_α
n24_statement_begin_β:  mov              r11, 25;                             jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:      sub              rsp, 16
                        mov              r11, 26
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx262_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n26_lit_integer_α
.Lx262_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:      sub              rsp, 16
                        mov              r11, 27
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx263_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n27_coerce_numeric_α
n26_lit_integer_β:      mov              r11, 27
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n24_statement_begin_β
.Lx263_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n27_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 28
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx265_1
                        cmp              al, 3;                               jne   .Lx265_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx265_0
.Lx265_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n28_coerce_numeric_α
.Lx265_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 147
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n28_coerce_numeric_α
n27_coerce_numeric_β:   mov              r11, 28
                        add              rsp, 16;                             jmp   n26_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n28_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 29
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx267_1
                        cmp              al, 3;                               jne   .Lx267_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx267_0
.Lx267_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n29_cmp_test_α
.Lx267_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 148
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n29_cmp_test_α
n28_coerce_numeric_β:   mov              r11, 29
                        add              rsp, 16;                             jmp   n27_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n29_cmp_test_α:         sub              rsp, 16
                        mov              r11, 30
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx269_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jl    .Lx269_239
                        add              rsp, 16;                             jmp   n28_coerce_numeric_β
.Lx269_239:                                                                   jmp   n30_statement_end_α
.Lx269_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            js    .Lx269_240
                        add              rsp, 16;                             jmp   n28_coerce_numeric_β
.Lx269_240:                                                                   jmp   n30_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    mov              r11, 31
                        mov              r10, 6
                        add              rsp, 80;                             jmp   n35_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/003: lt(4,5) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:  mov              r11, 32
                        mov              r10, 7;                              jmp   n32_lit_string_α
n31_statement_begin_β:  mov              r11, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:       sub              rsp, 16
                        mov              r11, 33
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n33_assign_α
.Lx274_0:               .quad            .Lx274_0_s
.Lx274_0_s:             .string          "FAIL 912/003: lt(4,5) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:           mov              r11, 34
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx275_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n34_statement_end_α
.Lx275_0:               .quad            .Lx275_0_s
.Lx275_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    mov              r11, 35
                        mov              r10, 7
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 8, line 14: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:  mov              r11, 36
                        mov              r10, 8;                              jmp   n36_statement_end_α
n35_statement_begin_β:  mov              r11, 36;                             jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    mov              r11, 37
                        mov              r10, 8;                              jmp   n37_statement_begin_α
#=======================================================================================================================
#         LE(5, 2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:  mov              r11, 38
                        mov              r10, 9;                              jmp   n38_lit_integer_α
n37_statement_begin_β:  mov              r11, 38;                             jmp   n48_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_integer_α:      sub              rsp, 16
                        mov              r11, 39
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx284_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_lit_integer_α
.Lx284_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:      sub              rsp, 16
                        mov              r11, 40
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx285_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n40_coerce_numeric_α
n39_lit_integer_β:      mov              r11, 40
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n37_statement_begin_β
.Lx285_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n40_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 41
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx287_1
                        cmp              al, 3;                               jne   .Lx287_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx287_0
.Lx287_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n41_coerce_numeric_α
.Lx287_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 118
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n41_coerce_numeric_α
n40_coerce_numeric_β:   mov              r11, 41
                        add              rsp, 16;                             jmp   n39_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n41_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 42
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx289_1
                        cmp              al, 3;                               jne   .Lx289_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx289_0
.Lx289_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n42_cmp_test_α
.Lx289_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 119
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n42_cmp_test_α
n41_coerce_numeric_β:   mov              r11, 42
                        add              rsp, 16;                             jmp   n40_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n42_cmp_test_α:         sub              rsp, 16
                        mov              r11, 43
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx291_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jle   .Lx291_239
                        add              rsp, 16;                             jmp   n41_coerce_numeric_β
.Lx291_239:                                                                   jmp   n43_statement_end_α
.Lx291_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jle   .Lx291_240
                        add              rsp, 16;                             jmp   n41_coerce_numeric_β
.Lx291_240:                                                                   jmp   n43_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:    mov              r11, 44
                        mov              r10, 9
                        add              rsp, 80;                             jmp   n44_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/004: le(5,2) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:  mov              r11, 45
                        mov              r10, 10;                             jmp   n45_lit_string_α
n44_statement_begin_β:  mov              r11, 45;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_string_α:       sub              rsp, 16
                        mov              r11, 46
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx296_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n46_assign_α
.Lx296_0:               .quad            .Lx296_0_s
.Lx296_0_s:             .string          "FAIL 912/004: le(5,2) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n46_assign_α:           mov              r11, 47
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n47_statement_end_α
.Lx297_0:               .quad            .Lx297_0_s
.Lx297_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:    mov              r11, 48
                        mov              r10, 10
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004    LE(4, 4)                   :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:  mov              r11, 49
                        mov              r10, 11;                             jmp   n49_lit_integer_α
n48_statement_begin_β:  mov              r11, 49;                             jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_integer_α:      sub              rsp, 16
                        mov              r11, 50
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx302_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n50_lit_integer_α
.Lx302_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:      sub              rsp, 16
                        mov              r11, 51
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx303_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n51_coerce_numeric_α
n50_lit_integer_β:      mov              r11, 51
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n48_statement_begin_β
.Lx303_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n51_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 52
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx305_1
                        cmp              al, 3;                               jne   .Lx305_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx305_0
.Lx305_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n52_coerce_numeric_α
.Lx305_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 118
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n52_coerce_numeric_α
n51_coerce_numeric_β:   mov              r11, 52
                        add              rsp, 16;                             jmp   n50_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n52_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 53
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx307_1
                        cmp              al, 3;                               jne   .Lx307_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx307_0
.Lx307_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n53_cmp_test_α
.Lx307_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 119
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n53_cmp_test_α
n52_coerce_numeric_β:   mov              r11, 53
                        add              rsp, 16;                             jmp   n51_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n53_cmp_test_α:         sub              rsp, 16
                        mov              r11, 54
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx309_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jle   .Lx309_239
                        add              rsp, 16;                             jmp   n52_coerce_numeric_β
.Lx309_239:                                                                   jmp   n54_statement_end_α
.Lx309_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jle   .Lx309_240
                        add              rsp, 16;                             jmp   n52_coerce_numeric_β
.Lx309_240:                                                                   jmp   n54_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:    mov              r11, 55
                        mov              r10, 11
                        add              rsp, 80;                             jmp   n59_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/005: le(4,4) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:  mov              r11, 56
                        mov              r10, 12;                             jmp   n56_lit_string_α
n55_statement_begin_β:  mov              r11, 56;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_string_α:       sub              rsp, 16
                        mov              r11, 57
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx314_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n57_assign_α
.Lx314_0:               .quad            .Lx314_0_s
.Lx314_0_s:             .string          "FAIL 912/005: le(4,4) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_α:           mov              r11, 58
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx315_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n58_statement_end_α
.Lx315_0:               .quad            .Lx315_0_s
.Lx315_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:    mov              r11, 59
                        mov              r10, 12
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e005    LE(4, 10)                  :s(e006)
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:  mov              r11, 60
                        mov              r10, 13;                             jmp   n60_lit_integer_α
n59_statement_begin_β:  mov              r11, 60;                             jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_integer_α:      sub              rsp, 16
                        mov              r11, 61
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx320_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n61_lit_integer_α
.Lx320_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:      sub              rsp, 16
                        mov              r11, 62
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx321_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n62_coerce_numeric_α
n61_lit_integer_β:      mov              r11, 62
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n59_statement_begin_β
.Lx321_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n62_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 63
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx323_1
                        cmp              al, 3;                               jne   .Lx323_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx323_0
.Lx323_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n63_coerce_numeric_α
.Lx323_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 118
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n63_coerce_numeric_α
n62_coerce_numeric_β:   mov              r11, 63
                        add              rsp, 16;                             jmp   n61_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n63_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 64
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx325_1
                        cmp              al, 3;                               jne   .Lx325_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx325_0
.Lx325_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n64_cmp_test_α
.Lx325_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 119
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n64_cmp_test_α
n63_coerce_numeric_β:   mov              r11, 64
                        add              rsp, 16;                             jmp   n62_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n64_cmp_test_α:         sub              rsp, 16
                        mov              r11, 65
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx327_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jle   .Lx327_239
                        add              rsp, 16;                             jmp   n63_coerce_numeric_β
.Lx327_239:                                                                   jmp   n65_statement_end_α
.Lx327_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jle   .Lx327_240
                        add              rsp, 16;                             jmp   n63_coerce_numeric_β
.Lx327_240:                                                                   jmp   n65_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:    mov              r11, 66
                        mov              r10, 13
                        add              rsp, 80;                             jmp   n70_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/006: le(4,10) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:  mov              r11, 67
                        mov              r10, 14;                             jmp   n67_lit_string_α
n66_statement_begin_β:  mov              r11, 67;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:       sub              rsp, 16
                        mov              r11, 68
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx332_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n68_assign_α
.Lx332_0:               .quad            .Lx332_0_s
.Lx332_0_s:             .string          "FAIL 912/006: le(4,10) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n68_assign_α:           mov              r11, 69
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx333_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n69_statement_end_α
.Lx333_0:               .quad            .Lx333_0_s
.Lx333_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:    mov              r11, 70
                        mov              r10, 14
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e006  <stmt 15, line 23: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:  mov              r11, 71
                        mov              r10, 15;                             jmp   n71_statement_end_α
n70_statement_begin_β:  mov              r11, 71;                             jmp   n72_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:    mov              r11, 72
                        mov              r10, 15;                             jmp   n72_statement_begin_α
#=======================================================================================================================
#         EQ(4, 5)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:  mov              r11, 73
                        mov              r10, 16;                             jmp   n73_lit_integer_α
n72_statement_begin_β:  mov              r11, 73;                             jmp   n83_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_integer_α:      sub              rsp, 16
                        mov              r11, 74
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx342_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n74_lit_integer_α
.Lx342_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:      sub              rsp, 16
                        mov              r11, 75
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx343_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n75_coerce_numeric_α
n74_lit_integer_β:      mov              r11, 75
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n72_statement_begin_β
.Lx343_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n75_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 76
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx345_1
                        cmp              al, 3;                               jne   .Lx345_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx345_0
.Lx345_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n76_coerce_numeric_α
.Lx345_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 101
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n76_coerce_numeric_α
n75_coerce_numeric_β:   mov              r11, 76
                        add              rsp, 16;                             jmp   n74_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n76_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 77
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx347_1
                        cmp              al, 3;                               jne   .Lx347_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx347_0
.Lx347_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n77_cmp_test_α
.Lx347_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 102
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n77_cmp_test_α
n76_coerce_numeric_β:   mov              r11, 77
                        add              rsp, 16;                             jmp   n75_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n77_cmp_test_α:         sub              rsp, 16
                        mov              r11, 78
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx349_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            je    .Lx349_239
                        add              rsp, 16;                             jmp   n76_coerce_numeric_β
.Lx349_239:                                                                   jmp   n78_statement_end_α
.Lx349_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            je    .Lx349_240
                        add              rsp, 16;                             jmp   n76_coerce_numeric_β
.Lx349_240:                                                                   jmp   n78_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:    mov              r11, 79
                        mov              r10, 16
                        add              rsp, 80;                             jmp   n79_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/007: eq(4,5) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:  mov              r11, 80
                        mov              r10, 17;                             jmp   n80_lit_string_α
n79_statement_begin_β:  mov              r11, 80;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:       sub              rsp, 16
                        mov              r11, 81
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx354_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n81_assign_α
.Lx354_0:               .quad            .Lx354_0_s
.Lx354_0_s:             .string          "FAIL 912/007: eq(4,5) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n81_assign_α:           mov              r11, 82
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx355_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n82_statement_end_α
.Lx355_0:               .quad            .Lx355_0_s
.Lx355_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_end_α:    mov              r11, 83
                        mov              r10, 17
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e007    EQ(5, 5)                   :s(e008)
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_begin_α:  mov              r11, 84
                        mov              r10, 18;                             jmp   n84_lit_integer_α
n83_statement_begin_β:  mov              r11, 84;                             jmp   n90_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_integer_α:      sub              rsp, 16
                        mov              r11, 85
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx360_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n85_lit_integer_α
.Lx360_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_integer_α:      sub              rsp, 16
                        mov              r11, 86
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx361_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n86_coerce_numeric_α
n85_lit_integer_β:      mov              r11, 86
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n83_statement_begin_β
.Lx361_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n86_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 87
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx363_1
                        cmp              al, 3;                               jne   .Lx363_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx363_0
.Lx363_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n87_coerce_numeric_α
.Lx363_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 101
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n87_coerce_numeric_α
n86_coerce_numeric_β:   mov              r11, 87
                        add              rsp, 16;                             jmp   n85_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n87_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 88
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx365_1
                        cmp              al, 3;                               jne   .Lx365_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx365_0
.Lx365_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n88_cmp_test_α
.Lx365_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 102
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n88_cmp_test_α
n87_coerce_numeric_β:   mov              r11, 88
                        add              rsp, 16;                             jmp   n86_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n88_cmp_test_α:         sub              rsp, 16
                        mov              r11, 89
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx367_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            je    .Lx367_239
                        add              rsp, 16;                             jmp   n87_coerce_numeric_β
.Lx367_239:                                                                   jmp   n89_statement_end_α
.Lx367_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            je    .Lx367_240
                        add              rsp, 16;                             jmp   n87_coerce_numeric_β
.Lx367_240:                                                                   jmp   n89_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:    mov              r11, 90
                        mov              r10, 18
                        add              rsp, 80;                             jmp   n94_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/008: eq(5,5) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_begin_α:  mov              r11, 91
                        mov              r10, 19;                             jmp   n91_lit_string_α
n90_statement_begin_β:  mov              r11, 91;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_string_α:       sub              rsp, 16
                        mov              r11, 92
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx372_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n92_assign_α
.Lx372_0:               .quad            .Lx372_0_s
.Lx372_0_s:             .string          "FAIL 912/008: eq(5,5) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n92_assign_α:           mov              r11, 93
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx373_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n93_statement_end_α
.Lx373_0:               .quad            .Lx373_0_s
.Lx373_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_end_α:    mov              r11, 94
                        mov              r10, 19
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e008  <stmt 20, line 30: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_begin_α:  mov              r11, 95
                        mov              r10, 20;                             jmp   n95_statement_end_α
n94_statement_begin_β:  mov              r11, 95;                             jmp   n96_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_end_α:    mov              r11, 96
                        mov              r10, 20;                             jmp   n96_statement_begin_α
#=======================================================================================================================
#         NE(4, 4)                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_begin_α:  mov              r11, 97
                        mov              r10, 21;                             jmp   n97_lit_integer_α
n96_statement_begin_β:  mov              r11, 97;                             jmp   n107_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:      sub              rsp, 16
                        mov              r11, 98
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx382_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n98_lit_integer_α
.Lx382_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n98_lit_integer_α:      sub              rsp, 16
                        mov              r11, 99
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx383_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n99_coerce_numeric_α
n98_lit_integer_β:      mov              r11, 99
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n96_statement_begin_β
.Lx383_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n99_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 100
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx385_1
                        cmp              al, 3;                               jne   .Lx385_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx385_0
.Lx385_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n100_coerce_numeric_α
.Lx385_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n100_coerce_numeric_α
n99_coerce_numeric_β:   mov              r11, 100
                        add              rsp, 16;                             jmp   n98_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n100_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 101
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx387_1
                        cmp              al, 3;                               jne   .Lx387_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx387_0
.Lx387_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n101_cmp_test_α
.Lx387_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n101_cmp_test_α
n100_coerce_numeric_β:  mov              r11, 101
                        add              rsp, 16;                             jmp   n99_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n101_cmp_test_α:        sub              rsp, 16
                        mov              r11, 102
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx389_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx389_239
                        add              rsp, 16;                             jmp   n100_coerce_numeric_β
.Lx389_239:                                                                   jmp   n102_statement_end_α
.Lx389_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jne   .Lx389_240
                        add              rsp, 16;                             jmp   n100_coerce_numeric_β
.Lx389_240:                                                                   jmp   n102_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   mov              r11, 103
                        mov              r10, 21
                        add              rsp, 80;                             jmp   n103_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/009: ne(4,4) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α: mov              r11, 104
                        mov              r10, 22;                             jmp   n104_lit_string_α
n103_statement_begin_β: mov              r11, 104;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_string_α:      sub              rsp, 16
                        mov              r11, 105
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx394_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n105_assign_α
.Lx394_0:               .quad            .Lx394_0_s
.Lx394_0_s:             .string          "FAIL 912/009: ne(4,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n105_assign_α:          mov              r11, 106
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx395_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n106_statement_end_α
.Lx395_0:               .quad            .Lx395_0_s
.Lx395_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:   mov              r11, 107
                        mov              r10, 22
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e009    NE(4, 6)                   :s(e010)
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_begin_α: mov              r11, 108
                        mov              r10, 23;                             jmp   n108_lit_integer_α
n107_statement_begin_β: mov              r11, 108;                            jmp   n114_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n108_lit_integer_α:     sub              rsp, 16
                        mov              r11, 109
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx400_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n109_lit_integer_α
.Lx400_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n109_lit_integer_α:     sub              rsp, 16
                        mov              r11, 110
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx401_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n110_coerce_numeric_α
n109_lit_integer_β:     mov              r11, 110
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n107_statement_begin_β
.Lx401_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n110_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 111
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx403_1
                        cmp              al, 3;                               jne   .Lx403_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx403_0
.Lx403_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n111_coerce_numeric_α
.Lx403_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n111_coerce_numeric_α
n110_coerce_numeric_β:  mov              r11, 111
                        add              rsp, 16;                             jmp   n109_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n111_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 112
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx405_1
                        cmp              al, 3;                               jne   .Lx405_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx405_0
.Lx405_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n112_cmp_test_α
.Lx405_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n112_cmp_test_α
n111_coerce_numeric_β:  mov              r11, 112
                        add              rsp, 16;                             jmp   n110_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n112_cmp_test_α:        sub              rsp, 16
                        mov              r11, 113
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx407_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx407_239
                        add              rsp, 16;                             jmp   n111_coerce_numeric_β
.Lx407_239:                                                                   jmp   n113_statement_end_α
.Lx407_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jne   .Lx407_240
                        add              rsp, 16;                             jmp   n111_coerce_numeric_β
.Lx407_240:                                                                   jmp   n113_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_end_α:   mov              r11, 114
                        mov              r10, 23
                        add              rsp, 80;                             jmp   n118_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/010: ne(4,6) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_begin_α: mov              r11, 115
                        mov              r10, 24;                             jmp   n115_lit_string_α
n114_statement_begin_β: mov              r11, 115;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n115_lit_string_α:      sub              rsp, 16
                        mov              r11, 116
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx412_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n116_assign_α
.Lx412_0:               .quad            .Lx412_0_s
.Lx412_0_s:             .string          "FAIL 912/010: ne(4,6) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n116_assign_α:          mov              r11, 117
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx413_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n117_statement_end_α
.Lx413_0:               .quad            .Lx413_0_s
.Lx413_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_end_α:   mov              r11, 118
                        mov              r10, 24
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e010  <stmt 25, line 37: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n118_statement_begin_α: mov              r11, 119
                        mov              r10, 25;                             jmp   n119_statement_end_α
n118_statement_begin_β: mov              r11, 119;                            jmp   n120_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_end_α:   mov              r11, 120
                        mov              r10, 25;                             jmp   n120_statement_begin_α
#=======================================================================================================================
#         GT(4, 6)                   :f(e011)
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_begin_α: mov              r11, 121
                        mov              r10, 26;                             jmp   n121_lit_integer_α
n120_statement_begin_β: mov              r11, 121;                            jmp   n131_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_integer_α:     sub              rsp, 16
                        mov              r11, 122
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx422_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n122_lit_integer_α
.Lx422_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n122_lit_integer_α:     sub              rsp, 16
                        mov              r11, 123
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx423_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n123_coerce_numeric_α
n122_lit_integer_β:     mov              r11, 123
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n120_statement_begin_β
.Lx423_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n123_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 124
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx425_1
                        cmp              al, 3;                               jne   .Lx425_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx425_0
.Lx425_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n124_coerce_numeric_α
.Lx425_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n124_coerce_numeric_α
n123_coerce_numeric_β:  mov              r11, 124
                        add              rsp, 16;                             jmp   n122_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n124_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 125
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx427_1
                        cmp              al, 3;                               jne   .Lx427_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx427_0
.Lx427_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n125_cmp_test_α
.Lx427_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n125_cmp_test_α
n124_coerce_numeric_β:  mov              r11, 125
                        add              rsp, 16;                             jmp   n123_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n125_cmp_test_α:        sub              rsp, 16
                        mov              r11, 126
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx429_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jg    .Lx429_239
                        add              rsp, 16;                             jmp   n124_coerce_numeric_β
.Lx429_239:                                                                   jmp   n126_statement_end_α
.Lx429_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jg    .Lx429_240
                        add              rsp, 16;                             jmp   n124_coerce_numeric_β
.Lx429_240:                                                                   jmp   n126_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_end_α:   mov              r11, 127
                        mov              r10, 26
                        add              rsp, 80;                             jmp   n127_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/011: gt(4,6) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_begin_α: mov              r11, 128
                        mov              r10, 27;                             jmp   n128_lit_string_α
n127_statement_begin_β: mov              r11, 128;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n128_lit_string_α:      sub              rsp, 16
                        mov              r11, 129
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx434_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n129_assign_α
.Lx434_0:               .quad            .Lx434_0_s
.Lx434_0_s:             .string          "FAIL 912/011: gt(4,6) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n129_assign_α:          mov              r11, 130
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx435_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n130_statement_end_α
.Lx435_0:               .quad            .Lx435_0_s
.Lx435_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_end_α:   mov              r11, 131
                        mov              r10, 27
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e011    GT(4, 4)                   :f(e012)
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_begin_α: mov              r11, 132
                        mov              r10, 28;                             jmp   n132_lit_integer_α
n131_statement_begin_β: mov              r11, 132;                            jmp   n142_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_integer_α:     sub              rsp, 16
                        mov              r11, 133
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx440_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n133_lit_integer_α
.Lx440_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_integer_α:     sub              rsp, 16
                        mov              r11, 134
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx441_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n134_coerce_numeric_α
n133_lit_integer_β:     mov              r11, 134
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n131_statement_begin_β
.Lx441_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n134_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 135
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx443_1
                        cmp              al, 3;                               jne   .Lx443_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx443_0
.Lx443_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n135_coerce_numeric_α
.Lx443_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n135_coerce_numeric_α
n134_coerce_numeric_β:  mov              r11, 135
                        add              rsp, 16;                             jmp   n133_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n135_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 136
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx445_1
                        cmp              al, 3;                               jne   .Lx445_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx445_0
.Lx445_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n136_cmp_test_α
.Lx445_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n136_cmp_test_α
n135_coerce_numeric_β:  mov              r11, 136
                        add              rsp, 16;                             jmp   n134_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n136_cmp_test_α:        sub              rsp, 16
                        mov              r11, 137
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx447_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jg    .Lx447_239
                        add              rsp, 16;                             jmp   n135_coerce_numeric_β
.Lx447_239:                                                                   jmp   n137_statement_end_α
.Lx447_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jg    .Lx447_240
                        add              rsp, 16;                             jmp   n135_coerce_numeric_β
.Lx447_240:                                                                   jmp   n137_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_end_α:   mov              r11, 138
                        mov              r10, 28
                        add              rsp, 80;                             jmp   n138_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/012: gt(4,4) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_begin_α: mov              r11, 139
                        mov              r10, 29;                             jmp   n139_lit_string_α
n138_statement_begin_β: mov              r11, 139;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n139_lit_string_α:      sub              rsp, 16
                        mov              r11, 140
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx452_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n140_assign_α
.Lx452_0:               .quad            .Lx452_0_s
.Lx452_0_s:             .string          "FAIL 912/012: gt(4,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n140_assign_α:          mov              r11, 141
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx453_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n141_statement_end_α
.Lx453_0:               .quad            .Lx453_0_s
.Lx453_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_end_α:   mov              r11, 142
                        mov              r10, 29
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e012    GT(5, 2)                   :s(e013)
#-----------------------------------------------------------------------------------------------------------------------
n142_statement_begin_α: mov              r11, 143
                        mov              r10, 30;                             jmp   n143_lit_integer_α
n142_statement_begin_β: mov              r11, 143;                            jmp   n149_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n143_lit_integer_α:     sub              rsp, 16
                        mov              r11, 144
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx458_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n144_lit_integer_α
.Lx458_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n144_lit_integer_α:     sub              rsp, 16
                        mov              r11, 145
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx459_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n145_coerce_numeric_α
n144_lit_integer_β:     mov              r11, 145
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n142_statement_begin_β
.Lx459_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n145_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 146
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx461_1
                        cmp              al, 3;                               jne   .Lx461_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx461_0
.Lx461_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n146_coerce_numeric_α
.Lx461_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n146_coerce_numeric_α
n145_coerce_numeric_β:  mov              r11, 146
                        add              rsp, 16;                             jmp   n144_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n146_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 147
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx463_1
                        cmp              al, 3;                               jne   .Lx463_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx463_0
.Lx463_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n147_cmp_test_α
.Lx463_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n147_cmp_test_α
n146_coerce_numeric_β:  mov              r11, 147
                        add              rsp, 16;                             jmp   n145_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n147_cmp_test_α:        sub              rsp, 16
                        mov              r11, 148
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx465_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jg    .Lx465_239
                        add              rsp, 16;                             jmp   n146_coerce_numeric_β
.Lx465_239:                                                                   jmp   n148_statement_end_α
.Lx465_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jg    .Lx465_240
                        add              rsp, 16;                             jmp   n146_coerce_numeric_β
.Lx465_240:                                                                   jmp   n148_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:   mov              r11, 149
                        mov              r10, 30
                        add              rsp, 80;                             jmp   n153_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/013: gt(5,2) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_begin_α: mov              r11, 150
                        mov              r10, 31;                             jmp   n150_lit_string_α
n149_statement_begin_β: mov              r11, 150;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_string_α:      sub              rsp, 16
                        mov              r11, 151
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx470_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n151_assign_α
.Lx470_0:               .quad            .Lx470_0_s
.Lx470_0_s:             .string          "FAIL 912/013: gt(5,2) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n151_assign_α:          mov              r11, 152
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx471_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n152_statement_end_α
.Lx471_0:               .quad            .Lx471_0_s
.Lx471_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_end_α:   mov              r11, 153
                        mov              r10, 31
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e013  <stmt 32, line 46: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α: mov              r11, 154
                        mov              r10, 32;                             jmp   n154_statement_end_α
n153_statement_begin_β: mov              r11, 154;                            jmp   n155_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_end_α:   mov              r11, 155
                        mov              r10, 32;                             jmp   n155_statement_begin_α
#=======================================================================================================================
#         GE(5, 7)                   :f(e014)
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_begin_α: mov              r11, 156
                        mov              r10, 33;                             jmp   n156_lit_integer_α
n155_statement_begin_β: mov              r11, 156;                            jmp   n166_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n156_lit_integer_α:     sub              rsp, 16
                        mov              r11, 157
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx480_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n157_lit_integer_α
.Lx480_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n157_lit_integer_α:     sub              rsp, 16
                        mov              r11, 158
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx481_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n158_coerce_numeric_α
n157_lit_integer_β:     mov              r11, 158
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n155_statement_begin_β
.Lx481_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n158_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 159
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx483_1
                        cmp              al, 3;                               jne   .Lx483_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx483_0
.Lx483_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n159_coerce_numeric_α
.Lx483_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n159_coerce_numeric_α
n158_coerce_numeric_β:  mov              r11, 159
                        add              rsp, 16;                             jmp   n157_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n159_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 160
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx485_1
                        cmp              al, 3;                               jne   .Lx485_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx485_0
.Lx485_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n160_cmp_test_α
.Lx485_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n160_cmp_test_α
n159_coerce_numeric_β:  mov              r11, 160
                        add              rsp, 16;                             jmp   n158_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n160_cmp_test_α:        sub              rsp, 16
                        mov              r11, 161
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx487_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx487_239
                        add              rsp, 16;                             jmp   n159_coerce_numeric_β
.Lx487_239:                                                                   jmp   n161_statement_end_α
.Lx487_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jns   .Lx487_240
                        add              rsp, 16;                             jmp   n159_coerce_numeric_β
.Lx487_240:                                                                   jmp   n161_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_end_α:   mov              r11, 162
                        mov              r10, 33
                        add              rsp, 80;                             jmp   n162_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/014: ge(5,7) should fail'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_begin_α: mov              r11, 163
                        mov              r10, 34;                             jmp   n163_lit_string_α
n162_statement_begin_β: mov              r11, 163;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n163_lit_string_α:      sub              rsp, 16
                        mov              r11, 164
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx492_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n164_assign_α
.Lx492_0:               .quad            .Lx492_0_s
.Lx492_0_s:             .string          "FAIL 912/014: ge(5,7) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n164_assign_α:          mov              r11, 165
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx493_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n165_statement_end_α
.Lx493_0:               .quad            .Lx493_0_s
.Lx493_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n165_statement_end_α:   mov              r11, 166
                        mov              r10, 34
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e014    GE(4, 4)                   :s(e015)
#-----------------------------------------------------------------------------------------------------------------------
n166_statement_begin_α: mov              r11, 167
                        mov              r10, 35;                             jmp   n167_lit_integer_α
n166_statement_begin_β: mov              r11, 167;                            jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n167_lit_integer_α:     sub              rsp, 16
                        mov              r11, 168
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx498_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n168_lit_integer_α
.Lx498_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n168_lit_integer_α:     sub              rsp, 16
                        mov              r11, 169
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx499_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n169_coerce_numeric_α
n168_lit_integer_β:     mov              r11, 169
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n166_statement_begin_β
.Lx499_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n169_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 170
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx501_1
                        cmp              al, 3;                               jne   .Lx501_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx501_0
.Lx501_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n170_coerce_numeric_α
.Lx501_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n170_coerce_numeric_α
n169_coerce_numeric_β:  mov              r11, 170
                        add              rsp, 16;                             jmp   n168_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n170_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 171
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx503_1
                        cmp              al, 3;                               jne   .Lx503_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx503_0
.Lx503_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n171_cmp_test_α
.Lx503_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n171_cmp_test_α
n170_coerce_numeric_β:  mov              r11, 171
                        add              rsp, 16;                             jmp   n169_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n171_cmp_test_α:        sub              rsp, 16
                        mov              r11, 172
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx505_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx505_239
                        add              rsp, 16;                             jmp   n170_coerce_numeric_β
.Lx505_239:                                                                   jmp   n172_statement_end_α
.Lx505_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jns   .Lx505_240
                        add              rsp, 16;                             jmp   n170_coerce_numeric_β
.Lx505_240:                                                                   jmp   n172_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:   mov              r11, 173
                        mov              r10, 35
                        add              rsp, 80;                             jmp   n177_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/015: ge(4,4) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_begin_α: mov              r11, 174
                        mov              r10, 36;                             jmp   n174_lit_string_α
n173_statement_begin_β: mov              r11, 174;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n174_lit_string_α:      sub              rsp, 16
                        mov              r11, 175
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx510_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n175_assign_α
.Lx510_0:               .quad            .Lx510_0_s
.Lx510_0_s:             .string          "FAIL 912/015: ge(4,4) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n175_assign_α:          mov              r11, 176
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx511_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n176_statement_end_α
.Lx511_0:               .quad            .Lx511_0_s
.Lx511_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_end_α:   mov              r11, 177
                        mov              r10, 36
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e015    GE(7, 5)                   :s(e016)
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_begin_α: mov              r11, 178
                        mov              r10, 37;                             jmp   n178_lit_integer_α
n177_statement_begin_β: mov              r11, 178;                            jmp   n184_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n178_lit_integer_α:     sub              rsp, 16
                        mov              r11, 179
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx516_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n179_lit_integer_α
.Lx516_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n179_lit_integer_α:     sub              rsp, 16
                        mov              r11, 180
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx517_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n180_coerce_numeric_α
n179_lit_integer_β:     mov              r11, 180
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n177_statement_begin_β
.Lx517_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n180_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 181
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx519_1
                        cmp              al, 3;                               jne   .Lx519_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx519_0
.Lx519_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n181_coerce_numeric_α
.Lx519_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n181_coerce_numeric_α
n180_coerce_numeric_β:  mov              r11, 181
                        add              rsp, 16;                             jmp   n179_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n181_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 182
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx521_1
                        cmp              al, 3;                               jne   .Lx521_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx521_0
.Lx521_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n182_cmp_test_α
.Lx521_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n182_cmp_test_α
n181_coerce_numeric_β:  mov              r11, 182
                        add              rsp, 16;                             jmp   n180_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n182_cmp_test_α:        sub              rsp, 16
                        mov              r11, 183
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx523_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx523_239
                        add              rsp, 16;                             jmp   n181_coerce_numeric_β
.Lx523_239:                                                                   jmp   n183_statement_end_α
.Lx523_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jns   .Lx523_240
                        add              rsp, 16;                             jmp   n181_coerce_numeric_β
.Lx523_240:                                                                   jmp   n183_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_end_α:   mov              r11, 184
                        mov              r10, 37
                        add              rsp, 80;                             jmp   n188_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/016: ge(7,5) should succeed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_begin_α: mov              r11, 185
                        mov              r10, 38;                             jmp   n185_lit_string_α
n184_statement_begin_β: mov              r11, 185;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n185_lit_string_α:      sub              rsp, 16
                        mov              r11, 186
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx528_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n186_assign_α
.Lx528_0:               .quad            .Lx528_0_s
.Lx528_0_s:             .string          "FAIL 912/016: ge(7,5) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n186_assign_α:          mov              r11, 187
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx529_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n187_statement_end_α
.Lx529_0:               .quad            .Lx529_0_s
.Lx529_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_end_α:   mov              r11, 188
                        mov              r10, 38
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e016  <stmt 39, line 55: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_begin_α: mov              r11, 189
                        mov              r10, 39;                             jmp   n189_statement_end_α
n188_statement_begin_β: mov              r11, 189;                            jmp   n190_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n189_statement_end_α:   mov              r11, 190
                        mov              r10, 39;                             jmp   n190_statement_begin_α
#=======================================================================================================================
#         NE(4, 5 - 1)               :f(e017)
#-----------------------------------------------------------------------------------------------------------------------
n190_statement_begin_α: mov              r11, 191
                        mov              r10, 40;                             jmp   n191_lit_integer_α
n190_statement_begin_β: mov              r11, 191;                            jmp   n201_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n191_lit_integer_α:     sub              rsp, 16
                        mov              r11, 192
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx538_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n192_lit_integer_α
.Lx538_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n192_lit_integer_α:     sub              rsp, 16
                        mov              r11, 193
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx539_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n193_coerce_numeric_α
n192_lit_integer_β:     mov              r11, 193
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n190_statement_begin_β
.Lx539_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n193_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 194
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx541_1
                        cmp              al, 3;                               jne   .Lx541_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              al, 3;                               jne   .Lx541_0
.Lx541_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n194_coerce_numeric_α
.Lx541_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n194_coerce_numeric_α
n193_coerce_numeric_β:  mov              r11, 194
                        add              rsp, 16;                             jmp   n192_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n194_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 195
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx543_1
                        cmp              al, 3;                               jne   .Lx543_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              al, 3;                               jne   .Lx543_0
.Lx543_1:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n195_cmp_test_α
.Lx543_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n195_cmp_test_α
n194_coerce_numeric_β:  mov              r11, 195
                        add              rsp, 16;                             jmp   n193_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n195_cmp_test_α:        sub              rsp, 16
                        mov              r11, 196
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx545_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx545_239
                        add              rsp, 16;                             jmp   n194_coerce_numeric_β
.Lx545_239:                                                                   jmp   n196_statement_end_α
.Lx545_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jne   .Lx545_240
                        add              rsp, 16;                             jmp   n194_coerce_numeric_β
.Lx545_240:                                                                   jmp   n196_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n196_statement_end_α:   mov              r11, 197
                        mov              r10, 40
                        add              rsp, 80;                             jmp   n197_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/017: ne(4,5-1) should fail (both=4)' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n197_statement_begin_α: mov              r11, 198
                        mov              r10, 41;                             jmp   n198_lit_string_α
n197_statement_begin_β: mov              r11, 198;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n198_lit_string_α:      sub              rsp, 16
                        mov              r11, 199
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 44
                        mov              rax, qword ptr [rip + .Lx550_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n199_assign_α
.Lx550_0:               .quad            .Lx550_0_s
.Lx550_0_s:             .string          "FAIL 912/017: ne(4,5-1) should fail (both=4)"
#-----------------------------------------------------------------------------------------------------------------------
n199_assign_α:          mov              r11, 200
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx551_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n200_statement_end_α
.Lx551_0:               .quad            .Lx551_0_s
.Lx551_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n200_statement_end_α:   mov              r11, 201
                        mov              r10, 41
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e017  <stmt 42, line 60: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n201_statement_begin_α: mov              r11, 202
                        mov              r10, 42;                             jmp   n202_statement_end_α
n201_statement_begin_β: mov              r11, 202;                            jmp   n203_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_end_α:   mov              r11, 203
                        mov              r10, 42;                             jmp   n203_statement_begin_α
#=======================================================================================================================
#         NE('12', 12)                   :f(e018)
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_begin_α: mov              r11, 204
                        mov              r10, 43;                             jmp   n204_lit_string_α
n203_statement_begin_β: mov              r11, 204;                            jmp   n214_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n204_lit_string_α:      sub              rsp, 16
                        mov              r11, 205
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx560_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n205_lit_integer_α
.Lx560_0:               .quad            .Lx560_0_s
.Lx560_0_s:             .string          "12"
#-----------------------------------------------------------------------------------------------------------------------
n205_lit_integer_α:     sub              rsp, 16
                        mov              r11, 206
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx561_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n206_coerce_numeric_α
n205_lit_integer_β:     mov              r11, 206
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n203_statement_begin_β
.Lx561_0:               .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n206_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 207
                        mov              eax, dword ptr [rsp + 32]            # lit_string
                        cmp              al, 5;                               je    .Lx563_1
                        cmp              al, 3;                               jne   .Lx563_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx563_0
.Lx563_1:               mov              rax, qword ptr [rsp + 32]            # lit_string
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_string
                        mov              qword ptr [rsp + 8], rax;            jmp   n207_coerce_numeric_α
.Lx563_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n207_coerce_numeric_α
n206_coerce_numeric_β:  mov              r11, 207
                        add              rsp, 16;                             jmp   n205_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n207_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 208
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx565_1
                        cmp              al, 3;                               jne   .Lx565_0
                        mov              eax, dword ptr [rsp + 48]            # lit_string
                        cmp              al, 3;                               jne   .Lx565_0
.Lx565_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n208_cmp_test_α
.Lx565_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # lit_string
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n208_cmp_test_α
n207_coerce_numeric_β:  mov              r11, 208
                        add              rsp, 16;                             jmp   n206_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n208_cmp_test_α:        sub              rsp, 16
                        mov              r11, 209
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx567_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx567_239
                        add              rsp, 16;                             jmp   n207_coerce_numeric_β
.Lx567_239:                                                                   jmp   n209_statement_end_α
.Lx567_0:               lea              rdi, [rsp + 32]                      # a
                        lea              rsi, [rsp + 16]                      # b
                        call             rt_cmp_d@PLT
                        test             eax, eax;                            jne   .Lx567_240
                        add              rsp, 16;                             jmp   n207_coerce_numeric_β
.Lx567_240:                                                                   jmp   n209_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n209_statement_end_α:   mov              r11, 210
                        mov              r10, 43
                        add              rsp, 80;                             jmp   n210_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 912/018: ne(string,int) type mismatch' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_begin_α: mov              r11, 211
                        mov              r10, 44;                             jmp   n211_lit_string_α
n210_statement_begin_β: mov              r11, 211;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n211_lit_string_α:      sub              rsp, 16
                        mov              r11, 212
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 42
                        mov              rax, qword ptr [rip + .Lx572_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n212_assign_α
.Lx572_0:               .quad            .Lx572_0_s
.Lx572_0_s:             .string          "FAIL 912/018: ne(string,int) type mismatch"
#-----------------------------------------------------------------------------------------------------------------------
n212_assign_α:          mov              r11, 213
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx573_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n213_statement_end_α
.Lx573_0:               .quad            .Lx573_0_s
.Lx573_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n213_statement_end_α:   mov              r11, 214
                        mov              r10, 44
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e018  <stmt 45, line 65: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_begin_α: mov              r11, 215
                        mov              r10, 45;                             jmp   n215_statement_end_α
n214_statement_begin_β: mov              r11, 215;                            jmp   n216_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_end_α:   mov              r11, 216
                        mov              r10, 45;                             jmp   n216_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 912_num_pred (18/18)'
#-----------------------------------------------------------------------------------------------------------------------
n216_statement_begin_α: mov              r11, 217
                        mov              r10, 46;                             jmp   n217_lit_string_α
n216_statement_begin_β: mov              r11, 217;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n217_lit_string_α:      sub              rsp, 16
                        mov              r11, 218
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx582_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n218_assign_α
.Lx582_0:               .quad            .Lx582_0_s
.Lx582_0_s:             .string          "PASS 912_num_pred (18/18)"
#-----------------------------------------------------------------------------------------------------------------------
n218_assign_α:          mov              r11, 219
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx583_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n219_statement_end_α
.Lx583_0:               .quad            .Lx583_0_s
.Lx583_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_end_α:   mov              r11, 220
                        mov              r10, 46
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

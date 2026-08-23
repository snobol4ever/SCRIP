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
#         DIFFER(1 + 2.0, 3.0)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_lit_real_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n11_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_real_α:          sub              rsp, 16
                        mov              r11, 4
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_lit_real_α
.Lx34_0:                .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_real_α:          sub              rsp, 16
                        mov              r11, 5
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_differ_α
.Lx35_0:                .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n5_differ_α:            sub              rsp, 16
                        mov              r11, 6
                        mov              rdi, qword ptr [rsp + 32]            # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx37_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n2_statement_begin_β
.Lx37_240:                                                                    jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     mov              r11, 7
                        mov              r10, 2
                        add              rsp, 48;                             jmp   n7_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 413/001: int+real promotes to real' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:   mov              r11, 8
                        mov              r10, 3;                              jmp   n8_lit_string_α
n7_statement_begin_β:   mov              r11, 8;                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:        sub              rsp, 16
                        mov              r11, 9
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_assign_α
.Lx42_0:                .quad            .Lx42_0_s
.Lx42_0_s:              .string          "FAIL 413/001: int+real promotes to real"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:            mov              r11, 10
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n10_statement_end_α
.Lx43_0:                .quad            .Lx43_0_s
.Lx43_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:    mov              r11, 11
                        mov              r10, 3
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 4, line 7: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:  mov              r11, 12
                        mov              r10, 4;                              jmp   n12_statement_end_α
n11_statement_begin_β:  mov              r11, 12;                             jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:    mov              r11, 13
                        mov              r10, 4;                              jmp   n13_statement_begin_α
#=======================================================================================================================
#         DIFFER(3.0 / 2, 1.5)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:  mov              r11, 14
                        mov              r10, 5;                              jmp   n14_lit_real_α
n13_statement_begin_β:  mov              r11, 14;                             jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_real_α:         sub              rsp, 16
                        mov              r11, 15
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx52_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n15_lit_real_α
.Lx52_0:                .quad            4609434218613702656
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_real_α:         sub              rsp, 16
                        mov              r11, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_differ_α
.Lx53_0:                .quad            4609434218613702656
#-----------------------------------------------------------------------------------------------------------------------
n16_differ_α:           sub              rsp, 16
                        mov              r11, 17
                        mov              rdi, qword ptr [rsp + 32]            # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx55_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n13_statement_begin_β
.Lx55_240:                                                                    jmp   n17_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    mov              r11, 18
                        mov              r10, 5
                        add              rsp, 48;                             jmp   n18_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 413/002: real/int promotes to real' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:  mov              r11, 19
                        mov              r10, 6;                              jmp   n19_lit_string_α
n18_statement_begin_β:  mov              r11, 19;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:       sub              rsp, 16
                        mov              r11, 20
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n20_assign_α
.Lx60_0:                .quad            .Lx60_0_s
.Lx60_0_s:              .string          "FAIL 413/002: real/int promotes to real"
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:           mov              r11, 21
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n21_statement_end_α
.Lx61_0:                .quad            .Lx61_0_s
.Lx61_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    mov              r11, 22
                        mov              r10, 6
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 7, line 11: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:  mov              r11, 23
                        mov              r10, 7;                              jmp   n23_statement_end_α
n22_statement_begin_β:  mov              r11, 23;                             jmp   n24_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:    mov              r11, 24
                        mov              r10, 7;                              jmp   n24_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 413_arith_mixed (2/2)'
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:  mov              r11, 25
                        mov              r10, 8;                              jmp   n25_lit_string_α
n24_statement_begin_β:  mov              r11, 25;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:       sub              rsp, 16
                        mov              r11, 26
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n26_assign_α
.Lx70_0:                .quad            .Lx70_0_s
.Lx70_0_s:              .string          "PASS 413_arith_mixed (2/2)"
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:           mov              r11, 27
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n27_statement_end_α
.Lx71_0:                .quad            .Lx71_0_s
.Lx71_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:    mov              r11, 28
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

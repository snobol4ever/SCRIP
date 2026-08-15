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
#         <stmt 1, line 1: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         DIFFER('a' 'b', 'ab')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_lit_string_α
n2_statement_begin_β:                                                         jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx55_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_lit_string_α
.Lx55_0:                .quad            .Lx55_0_s
.Lx55_0_s:              .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx56_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_binop_α
n4_lit_string_β:        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx56_0:                .quad            .Lx56_0_s
.Lx56_0_s:              .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n5_binop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n6_lit_string_α
n5_binop_β:             add              rsp, 16;                             jmp   n4_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx58_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n7_call_α
n6_lit_string_β:        add              rsp, 16;                             jmp   n5_binop_β
.Lx58_0:                .quad            .Lx58_0_s
.Lx58_0_s:              .string          "ab"
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
.Lrkfnzd60:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd60]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx59_240
                        add              rsp, 16;                             jmp   n6_lit_string_β
.Lx59_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_statement_end_α
n7_call_β:              add              rsp, 16;                             jmp   n6_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:     add              rsp, 80;                             jmp   n9_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 310/001: two-string concat'      :(END)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:                                                         jmp   n10_lit_string_α
n9_statement_begin_β:                                                         jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n11_assign_α
.Lx65_0:                .quad            .Lx65_0_s
.Lx65_0_s:              .string          "FAIL 310/001: two-string concat"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx66_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n12_statement_end_α
.Lx66_0:                .quad            .Lx66_0_s
.Lx66_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 4, line 7: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:                                                        jmp   n14_statement_end_α
n13_statement_begin_β:                                                        jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:                                                          jmp   n15_statement_begin_α
#=======================================================================================================================
#         DIFFER('a' 'b' 'c', 'abc')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                                        jmp   n16_lit_string_α
n15_statement_begin_β:                                                        jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_lit_string_α
.Lx75_0:                .quad            .Lx75_0_s
.Lx75_0_s:              .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_binop_α
n17_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n15_statement_begin_β
.Lx76_0:                .quad            .Lx76_0_s
.Lx76_0_s:              .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n19_lit_string_α
n18_binop_β:            add              rsp, 16;                             jmp   n17_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx78_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n20_binop_α
n19_lit_string_β:       add              rsp, 16;                             jmp   n18_binop_β
.Lx78_0:                .quad            .Lx78_0_s
.Lx78_0_s:              .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n20_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n21_lit_string_α
n20_binop_β:            add              rsp, 16;                             jmp   n19_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n22_call_α
n21_lit_string_β:       add              rsp, 16;                             jmp   n20_binop_β
.Lx80_0:                .quad            .Lx80_0_s
.Lx80_0_s:              .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:             sub              rsp, 16
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
.Lrkfnzd82:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd82]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx81_240
                        add              rsp, 16;                             jmp   n21_lit_string_β
.Lx81_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n23_statement_end_α
n22_call_β:             add              rsp, 16;                             jmp   n21_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:    add              rsp, 112;                            jmp   n24_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 310/002: three-string concat'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:                                                        jmp   n25_lit_string_α
n24_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n26_assign_α
.Lx87_0:                .quad            .Lx87_0_s
.Lx87_0_s:              .string          "FAIL 310/002: three-string concat"
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n27_statement_end_α
.Lx88_0:                .quad            .Lx88_0_s
.Lx88_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 7, line 11: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                                        jmp   n29_statement_end_α
n28_statement_begin_β:                                                        jmp   n30_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:                                                          jmp   n30_statement_begin_α
#=======================================================================================================================
#         DIFFER(('hello' ' ') 'world', 'hello world')                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:                                                        jmp   n31_lit_string_α
n30_statement_begin_β:                                                        jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n32_lit_string_α
.Lx97_0:                .quad            .Lx97_0_s
.Lx97_0_s:              .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n33_binop_α
n32_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n30_statement_begin_β
.Lx98_0:                .quad            .Lx98_0_s
.Lx98_0_s:              .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n33_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n34_lit_string_α
n33_binop_β:            add              rsp, 16;                             jmp   n32_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx100_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n35_binop_α
n34_lit_string_β:       add              rsp, 16;                             jmp   n33_binop_β
.Lx100_0:               .quad            .Lx100_0_s
.Lx100_0_s:             .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n35_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n36_lit_string_α
n35_binop_β:            add              rsp, 16;                             jmp   n34_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx102_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n37_call_α
n36_lit_string_β:       add              rsp, 16;                             jmp   n35_binop_β
.Lx102_0:               .quad            .Lx102_0_s
.Lx102_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n37_call_α:             sub              rsp, 16
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
.Lrkfnzd104:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd104]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx103_240
                        add              rsp, 16;                             jmp   n36_lit_string_β
.Lx103_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n38_statement_end_α
n37_call_β:             add              rsp, 16;                             jmp   n36_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:    add              rsp, 112;                            jmp   n39_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 310/003: left-associative concat' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:                                                        jmp   n40_lit_string_α
n39_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n41_assign_α
.Lx109_0:               .quad            .Lx109_0_s
.Lx109_0_s:             .string          "FAIL 310/003: left-associative concat"
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n42_statement_end_α
.Lx110_0:               .quad            .Lx110_0_s
.Lx110_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 10, line 16: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:                                                        jmp   n44_statement_end_α
n43_statement_begin_β:                                                        jmp   n45_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:                                                          jmp   n45_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 310_concat_strings (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:                                                        jmp   n46_lit_string_α
n45_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 29
                        mov              rax, qword ptr [rip + .Lx119_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n47_assign_α
.Lx119_0:               .quad            .Lx119_0_s
.Lx119_0_s:             .string          "PASS 310_concat_strings (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n48_statement_end_α
.Lx120_0:               .quad            .Lx120_0_s
.Lx120_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    add              rsp, 16;                             jmp   main_γ
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

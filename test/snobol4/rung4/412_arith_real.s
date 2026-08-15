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
#         DIFFER(2.0 + 3.0, 5.0)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_lit_real_α
n2_statement_begin_β:                                                         jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_real_α:          sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_lit_real_α
.Lx91_0:                .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_real_α:          sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx92_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_binop_α
.Lx92_0:                .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n5_binop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx93_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n2_statement_begin_β
.Lx93_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n6_lit_real_α
n5_binop_β:             add              rsp, 16
                        add              rsp, 32;                             jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_real_α:          sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx94_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n7_call_α
.Lx94_0:                .quad            4617315517961601024
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
.Lrkfnzd96:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd96]
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
                        cmp              eax, 104;                            jne   .Lx95_240
                        add              rsp, 32;                             jmp   n5_binop_β
.Lx95_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_statement_end_α
n7_call_β:              add              rsp, 32;                             jmp   n5_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:     add              rsp, 80;                             jmp   n9_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/001: 2.0+3.0'              :(END)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:                                                         jmp   n10_lit_string_α
n9_statement_begin_β:                                                         jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n11_assign_α
.Lx101_0:               .quad            .Lx101_0_s
.Lx101_0_s:             .string          "FAIL 412/001: 2.0+3.0"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx102_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n12_statement_end_α
.Lx102_0:               .quad            .Lx102_0_s
.Lx102_0_s:             .string          "OUTPUT"
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
#         DIFFER(3.0 - 1.0, 2.0)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                                        jmp   n16_lit_real_α
n15_statement_begin_β:                                                        jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx111_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_lit_real_α
.Lx111_0:               .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx112_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_binop_α
.Lx112_0:               .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx113_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n15_statement_begin_β
.Lx113_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n19_lit_real_α
n18_binop_β:            add              rsp, 16
                        add              rsp, 32;                             jmp   n15_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx114_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n20_call_α
.Lx114_0:               .quad            4611686018427387904
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
.Lrkfnzd116:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd116]
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
                        cmp              eax, 104;                            jne   .Lx115_240
                        add              rsp, 32;                             jmp   n18_binop_β
.Lx115_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n21_statement_end_α
n20_call_β:             add              rsp, 32;                             jmp   n18_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    add              rsp, 80;                             jmp   n22_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/002: 3.0-1.0'              :(END)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                                        jmp   n23_lit_string_α
n22_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_assign_α
.Lx121_0:               .quad            .Lx121_0_s
.Lx121_0_s:             .string          "FAIL 412/002: 3.0-1.0"
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx122_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n25_statement_end_α
.Lx122_0:               .quad            .Lx122_0_s
.Lx122_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 7, line 10: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                                        jmp   n27_statement_end_α
n26_statement_begin_β:                                                        jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:                                                          jmp   n28_statement_begin_α
#=======================================================================================================================
#         DIFFER(3.0 * 2.0, 6.0)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                                        jmp   n29_lit_real_α
n28_statement_begin_β:                                                        jmp   n39_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_lit_real_α
.Lx131_0:               .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx132_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_binop_α
.Lx132_0:               .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n31_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                            jne   .Lx133_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n28_statement_begin_β
.Lx133_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n32_lit_real_α
n31_binop_β:            add              rsp, 16
                        add              rsp, 32;                             jmp   n28_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n33_call_α
.Lx134_0:               .quad            4618441417868443648
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
.Lrkfnzd136:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd136]
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
                        cmp              eax, 104;                            jne   .Lx135_240
                        add              rsp, 32;                             jmp   n31_binop_β
.Lx135_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n34_statement_end_α
n33_call_β:             add              rsp, 32;                             jmp   n31_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    add              rsp, 80;                             jmp   n35_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/003: 3.0*2.0'              :(END)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:                                                        jmp   n36_lit_string_α
n35_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n37_assign_α
.Lx141_0:               .quad            .Lx141_0_s
.Lx141_0_s:             .string          "FAIL 412/003: 3.0*2.0"
#-----------------------------------------------------------------------------------------------------------------------
n37_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n38_statement_end_α
.Lx142_0:               .quad            .Lx142_0_s
.Lx142_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 10, line 13: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:                                                        jmp   n40_statement_end_α
n39_statement_begin_β:                                                        jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:                                                          jmp   n41_statement_begin_α
#=======================================================================================================================
#         DIFFER(3.0 / 2.0, 1.5)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:                                                        jmp   n42_lit_real_α
n41_statement_begin_β:                                                        jmp   n52_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx151_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_lit_real_α
.Lx151_0:               .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx152_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n44_binop_α
.Lx152_0:               .quad            4611686018427387904
#-----------------------------------------------------------------------------------------------------------------------
n44_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_div@PLT
                        cmp              eax, 104;                            jne   .Lx153_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n41_statement_begin_β
.Lx153_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n45_lit_real_α
n44_binop_β:            add              rsp, 16
                        add              rsp, 32;                             jmp   n41_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx154_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n46_call_α
.Lx154_0:               .quad            4609434218613702656
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
.Lrkfnzd156:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd156]
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
                        cmp              eax, 104;                            jne   .Lx155_240
                        add              rsp, 32;                             jmp   n44_binop_β
.Lx155_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n47_statement_end_α
n46_call_β:             add              rsp, 32;                             jmp   n44_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:    add              rsp, 80;                             jmp   n48_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/004: 3.0/2.0'              :(END)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:                                                        jmp   n49_lit_string_α
n48_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx161_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n50_assign_α
.Lx161_0:               .quad            .Lx161_0_s
.Lx161_0_s:             .string          "FAIL 412/004: 3.0/2.0"
#-----------------------------------------------------------------------------------------------------------------------
n50_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx162_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n51_statement_end_α
.Lx162_0:               .quad            .Lx162_0_s
.Lx162_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 13, line 16: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:                                                        jmp   n53_statement_end_α
n52_statement_begin_β:                                                        jmp   n54_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:                                                          jmp   n54_statement_begin_α
#=======================================================================================================================
#         DIFFER(3.0 ** 3, 27.0)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:                                                        jmp   n55_lit_real_α
n54_statement_begin_β:                                                        jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx171_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n56_lit_integer_α
.Lx171_0:               .quad            4613937818241073152
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n57_binop_α
n56_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n54_statement_begin_β
.Lx172_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n57_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_pow@PLT
                        cmp              eax, 104;                            jne   .Lx173_240
                        add              rsp, 16;                             jmp   n56_lit_integer_β
.Lx173_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n58_lit_real_α
n57_binop_β:            add              rsp, 16;                             jmp   n56_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx174_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n59_call_α
.Lx174_0:               .quad            4628293042053316608
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
.Lrkfnzd176:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd176]
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
                        cmp              eax, 104;                            jne   .Lx175_240
                        add              rsp, 32;                             jmp   n57_binop_β
.Lx175_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n60_statement_end_α
n59_call_β:             add              rsp, 32;                             jmp   n57_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 80;                             jmp   n61_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/005: 3.0**3'               :(END)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:                                                        jmp   n62_lit_string_α
n61_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx181_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n63_assign_α
.Lx181_0:               .quad            .Lx181_0_s
.Lx181_0_s:             .string          "FAIL 412/005: 3.0**3"
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx182_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n64_statement_end_α
.Lx182_0:               .quad            .Lx182_0_s
.Lx182_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e005  <stmt 16, line 21: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:                                                        jmp   n66_statement_end_α
n65_statement_begin_β:                                                        jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:                                                          jmp   n67_statement_begin_α
#=======================================================================================================================
#         DIFFER(-1.0, 0.0 - 1.0)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:                                                        jmp   n68_lit_real_α
n67_statement_begin_β:                                                        jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n69_unop_α
.Lx191_0:               .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n69_unop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # lit_real
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_num_neg@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n70_lit_real_α
n69_unop_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n67_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx193_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n71_lit_real_α
.Lx193_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n72_binop_α
.Lx194_0:               .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n72_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_real
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx195_240
                        add              rsp, 48;                             jmp   n69_unop_β
.Lx195_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n73_call_α
n72_binop_β:            add              rsp, 48;                             jmp   n69_unop_β
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
.Lrkfnzd197:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd197]
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
                        cmp              eax, 104;                            jne   .Lx196_240
                        add              rsp, 16;                             jmp   n72_binop_β
.Lx196_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n74_statement_end_α
n73_call_β:             add              rsp, 16;                             jmp   n72_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_end_α:    add              rsp, 96;                             jmp   n75_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 412/006: unary minus on real'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_begin_α:                                                        jmp   n76_lit_string_α
n75_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n77_assign_α
.Lx202_0:               .quad            .Lx202_0_s
.Lx202_0_s:             .string          "FAIL 412/006: unary minus on real"
#-----------------------------------------------------------------------------------------------------------------------
n77_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n78_statement_end_α
.Lx203_0:               .quad            .Lx203_0_s
.Lx203_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e006  <stmt 19, line 26: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:                                                        jmp   n80_statement_end_α
n79_statement_begin_β:                                                        jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:                                                          jmp   n81_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 412_arith_real (6/6)'
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:                                                        jmp   n82_lit_string_α
n81_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx212_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n83_assign_α
.Lx212_0:               .quad            .Lx212_0_s
.Lx212_0_s:             .string          "PASS 412_arith_real (6/6)"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n84_statement_end_α
.Lx213_0:               .quad            .Lx213_0_s
.Lx213_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_end_α:    add              rsp, 16;                             jmp   main_γ
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

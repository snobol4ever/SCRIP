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
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         DIFFER(3 + 2, 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_lit_integer_α
n2_statement_begin_β:                                                         jmp   n11_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_lit_integer_α
.Lx113_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx114_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_call_α
n4_lit_integer_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx114_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n5_call_α:              sub              rsp, 16
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
                        add              rsp, 16;                             jmp   n4_lit_integer_β
.Lx115_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n6_statement_end_α
n5_call_β:              add              rsp, 16;                             jmp   n4_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 48;                             jmp   n7_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/001: 3+2'                   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                                         jmp   n8_lit_string_α
n7_statement_begin_β:                                                         jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_assign_α
.Lx121_0:               .quad            .Lx121_0_s
.Lx121_0_s:             .string          "FAIL 410/001: 3+2"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:            mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx122_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n10_statement_end_α
.Lx122_0:               .quad            .Lx122_0_s
.Lx122_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 4, line 7: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:                                                        jmp   n12_statement_end_α
n11_statement_begin_β:                                                        jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:                                                          jmp   n13_statement_begin_α
#=======================================================================================================================
#         DIFFER(3 - 2, 1)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:                                                        jmp   n14_lit_integer_α
n13_statement_begin_β:                                                        jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n15_lit_integer_α
.Lx131_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx132_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_call_α
n15_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n13_statement_begin_β
.Lx132_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n16_call_α:             sub              rsp, 16
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
.Lrkfnzd134:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd134]
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
                        cmp              eax, 104;                            jne   .Lx133_240
                        add              rsp, 16;                             jmp   n15_lit_integer_β
.Lx133_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n17_statement_end_α
n16_call_β:             add              rsp, 16;                             jmp   n15_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    add              rsp, 48;                             jmp   n18_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/002: 3-2'                   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:                                                        jmp   n19_lit_string_α
n18_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n20_assign_α
.Lx139_0:               .quad            .Lx139_0_s
.Lx139_0_s:             .string          "FAIL 410/002: 3-2"
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n21_statement_end_α
.Lx140_0:               .quad            .Lx140_0_s
.Lx140_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 7, line 10: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                                        jmp   n23_statement_end_α
n22_statement_begin_β:                                                        jmp   n24_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:                                                          jmp   n24_statement_begin_α
#=======================================================================================================================
#         DIFFER(3 * 2, 6)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:                                                        jmp   n25_lit_integer_α
n24_statement_begin_β:                                                        jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n26_lit_integer_α
.Lx149_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n27_call_α
n26_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n24_statement_begin_β
.Lx150_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n27_call_α:             sub              rsp, 16
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
.Lrkfnzd152:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd152]
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
                        cmp              eax, 104;                            jne   .Lx151_240
                        add              rsp, 16;                             jmp   n26_lit_integer_β
.Lx151_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n28_statement_end_α
n27_call_β:             add              rsp, 16;                             jmp   n26_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    add              rsp, 48;                             jmp   n29_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/003: 3*2'                   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                                        jmp   n30_lit_string_α
n29_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_assign_α
.Lx157_0:               .quad            .Lx157_0_s
.Lx157_0_s:             .string          "FAIL 410/003: 3*2"
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx158_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n32_statement_end_α
.Lx158_0:               .quad            .Lx158_0_s
.Lx158_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 10, line 13: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                                        jmp   n34_statement_end_α
n33_statement_begin_β:                                                        jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:                                                          jmp   n35_statement_begin_α
#=======================================================================================================================
#         DIFFER(5 / 2, 2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:                                                        jmp   n36_lit_integer_α
n35_statement_begin_β:                                                        jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n37_lit_integer_α
.Lx167_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx168_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n38_call_α
n37_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n35_statement_begin_β
.Lx168_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n38_call_α:             sub              rsp, 16
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
.Lrkfnzd170:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd170]
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
                        cmp              eax, 104;                            jne   .Lx169_240
                        add              rsp, 16;                             jmp   n37_lit_integer_β
.Lx169_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n39_statement_end_α
n38_call_β:             add              rsp, 16;                             jmp   n37_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_end_α:    add              rsp, 48;                             jmp   n40_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/004: 5/2 integer division'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_begin_α:                                                        jmp   n41_lit_string_α
n40_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx175_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n42_assign_α
.Lx175_0:               .quad            .Lx175_0_s
.Lx175_0_s:             .string          "FAIL 410/004: 5/2 integer division"
#-----------------------------------------------------------------------------------------------------------------------
n42_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n43_statement_end_α
.Lx176_0:               .quad            .Lx176_0_s
.Lx176_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 13, line 16: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:                                                        jmp   n45_statement_end_α
n44_statement_begin_β:                                                        jmp   n46_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:                                                          jmp   n46_statement_begin_α
#=======================================================================================================================
#         DIFFER(2 ** 3, 8)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:                                                        jmp   n47_lit_integer_α
n46_statement_begin_β:                                                        jmp   n57_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n48_lit_integer_α
.Lx185_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx186_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n49_binop_α
n48_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n46_statement_begin_β
.Lx186_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n49_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_pow@PLT
                        cmp              eax, 104;                            jne   .Lx187_240
                        add              rsp, 16;                             jmp   n48_lit_integer_β
.Lx187_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n50_lit_integer_α
n49_binop_β:            add              rsp, 16;                             jmp   n48_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx188_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n51_call_α
n50_lit_integer_β:      add              rsp, 16;                             jmp   n49_binop_β
.Lx188_0:               .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n51_call_α:             sub              rsp, 16
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
.Lrkfnzd190:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd190]
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
                        cmp              eax, 104;                            jne   .Lx189_240
                        add              rsp, 16;                             jmp   n50_lit_integer_β
.Lx189_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n52_statement_end_α
n51_call_β:             add              rsp, 16;                             jmp   n50_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    add              rsp, 80;                             jmp   n53_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/005: 2**3'                  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:                                                        jmp   n54_lit_string_α
n53_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 18
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n55_assign_α
.Lx195_0:               .quad            .Lx195_0_s
.Lx195_0_s:             .string          "FAIL 410/005: 2**3"
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx196_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n56_statement_end_α
.Lx196_0:               .quad            .Lx196_0_s
.Lx196_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e005  <stmt 16, line 19: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                                        jmp   n58_statement_end_α
n57_statement_begin_β:                                                        jmp   n59_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:                                                          jmp   n59_statement_begin_α
#=======================================================================================================================
#         DIFFER('3' + 2, 5)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:                                                        jmp   n60_lit_integer_α
n59_statement_begin_β:                                                        jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n61_lit_integer_α
.Lx205_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx206_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n62_call_α
n61_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n59_statement_begin_β
.Lx206_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:             sub              rsp, 16
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
.Lrkfnzd208:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd208]
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
                        cmp              eax, 104;                            jne   .Lx207_240
                        add              rsp, 16;                             jmp   n61_lit_integer_β
.Lx207_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n63_statement_end_α
n62_call_β:             add              rsp, 16;                             jmp   n61_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:    add              rsp, 48;                             jmp   n64_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/006: string+int coerce'     :(END)
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:                                                        jmp   n65_lit_string_α
n64_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n66_assign_α
.Lx213_0:               .quad            .Lx213_0_s
.Lx213_0_s:             .string          "FAIL 410/006: string+int coerce"
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx214_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n67_statement_end_α
.Lx214_0:               .quad            .Lx214_0_s
.Lx214_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e006  <stmt 19, line 24: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:                                                        jmp   n69_statement_end_α
n68_statement_begin_β:                                                        jmp   n70_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:                                                          jmp   n70_statement_begin_α
#=======================================================================================================================
#         DIFFER(3 + '-2', 1)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:                                                        jmp   n71_lit_integer_α
n70_statement_begin_β:                                                        jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx223_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n72_lit_integer_α
.Lx223_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx224_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n73_call_α
n72_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n70_statement_begin_β
.Lx224_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:             sub              rsp, 16
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
.Lrkfnzd226:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd226]
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
                        cmp              eax, 104;                            jne   .Lx225_240
                        add              rsp, 16;                             jmp   n72_lit_integer_β
.Lx225_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n74_statement_end_α
n73_call_β:             add              rsp, 16;                             jmp   n72_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_end_α:    add              rsp, 48;                             jmp   n75_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/007: int+neg-string'        :(END)
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_begin_α:                                                        jmp   n76_lit_string_α
n75_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx231_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n77_assign_α
.Lx231_0:               .quad            .Lx231_0_s
.Lx231_0_s:             .string          "FAIL 410/007: int+neg-string"
#-----------------------------------------------------------------------------------------------------------------------
n77_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n78_statement_end_α
.Lx232_0:               .quad            .Lx232_0_s
.Lx232_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e007  <stmt 22, line 27: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:                                                        jmp   n80_statement_end_α
n79_statement_begin_β:                                                        jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:                                                          jmp   n81_statement_begin_α
#=======================================================================================================================
#         DIFFER('1' + '0', 1)                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:                                                        jmp   n82_lit_integer_α
n81_statement_begin_β:                                                        jmp   n90_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx241_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n83_lit_integer_α
.Lx241_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx242_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n84_call_α
n83_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n81_statement_begin_β
.Lx242_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n84_call_α:             sub              rsp, 16
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
.Lrkfnzd244:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd244]
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
                        cmp              eax, 104;                            jne   .Lx243_240
                        add              rsp, 16;                             jmp   n83_lit_integer_β
.Lx243_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n85_statement_end_α
n84_call_β:             add              rsp, 16;                             jmp   n83_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:    add              rsp, 48;                             jmp   n86_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/008: string+string coerce'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:                                                        jmp   n87_lit_string_α
n86_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx249_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n88_assign_α
.Lx249_0:               .quad            .Lx249_0_s
.Lx249_0_s:             .string          "FAIL 410/008: string+string coerce"
#-----------------------------------------------------------------------------------------------------------------------
n88_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx250_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n89_statement_end_α
.Lx250_0:               .quad            .Lx250_0_s
.Lx250_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e008  <stmt 25, line 30: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_begin_α:                                                        jmp   n91_statement_end_α
n90_statement_begin_β:                                                        jmp   n92_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:                                                          jmp   n92_statement_begin_α
#=======================================================================================================================
#         DIFFER(5 + '', 5)                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:                                                        jmp   n93_lit_integer_α
n92_statement_begin_β:                                                        jmp   n101_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n94_lit_integer_α
.Lx259_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n95_call_α
n94_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n92_statement_begin_β
.Lx260_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n95_call_α:             sub              rsp, 16
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
.Lrkfnzd262:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd262]
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
                        cmp              eax, 104;                            jne   .Lx261_240
                        add              rsp, 16;                             jmp   n94_lit_integer_β
.Lx261_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n96_statement_end_α
n95_call_β:             add              rsp, 16;                             jmp   n94_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_end_α:    add              rsp, 48;                             jmp   n97_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/009: null addend is zero'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_begin_α:                                                        jmp   n98_lit_string_α
n97_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n98_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n99_assign_α
.Lx267_0:               .quad            .Lx267_0_s
.Lx267_0_s:             .string          "FAIL 410/009: null addend is zero"
#-----------------------------------------------------------------------------------------------------------------------
n99_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx268_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n100_statement_end_α
.Lx268_0:               .quad            .Lx268_0_s
.Lx268_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e009  <stmt 28, line 35: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_begin_α:                                                       jmp   n102_statement_end_α
n101_statement_begin_β:                                                       jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:                                                         jmp   n103_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 410_arith_int (9/9)'
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                                       jmp   n104_lit_string_α
n103_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 24
                        mov              rax, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n105_assign_α
.Lx277_0:               .quad            .Lx277_0_s
.Lx277_0_s:             .string          "PASS 410_arith_int (9/9)"
#-----------------------------------------------------------------------------------------------------------------------
n105_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx278_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n106_statement_end_α
.Lx278_0:               .quad            .Lx278_0_s
.Lx278_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:   add              rsp, 16;                             jmp   main_γ
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

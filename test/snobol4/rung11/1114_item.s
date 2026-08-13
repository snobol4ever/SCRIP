                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "aaa"
.Lgvan1:                .string          "ama"
.Lgvan2:                .string          "tt"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_statement_end_α
n0_statement_begin_β:                                               jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                 jmp   n2_statement_begin_α
#=======================================================================================================================
#         aaa = ARRAY(10)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_integer_α
n2_statement_begin_β:                                               jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_call_α
.Lx187_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd189:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd189]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx188_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx188_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n5_assign_α
n4_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:            mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # aaa
                        mov              qword ptr [r9 + 8], rdx;   jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 32;                   jmp   n7_statement_begin_α
#=======================================================================================================================
#         aaa = ARRAY(10)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                               jmp   n8_statement_end_α
n7_statement_begin_β:                                               jmp   n9_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:                                                 jmp   n9_statement_begin_α
#=======================================================================================================================
#         ITEM(aaa, 1) = 5
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:                                               jmp   n10_var_α
n9_statement_begin_β:                                               jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # aaa
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n12_subscript_α
n11_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n9_statement_begin_β
.Lx200_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n12_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx201_240
                        add              rsp, 16;                   jmp   n11_lit_integer_β
.Lx201_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n13_lit_integer_α
n12_subscript_β:        add              rsp, 16;                   jmp   n11_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n14_assign_var_α
n13_lit_integer_β:      add              rsp, 16;                   jmp   n12_subscript_β
.Lx202_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx203_240
                        add              rsp, 16;                   jmp   n13_lit_integer_β
.Lx203_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n15_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    add              rsp, 80;                   jmp   n16_statement_begin_α
#=======================================================================================================================
#         DIFFER(ITEM(aaa, 1), 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:                                              jmp   n17_var_α
n16_statement_begin_β:                                              jmp   n27_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # aaa
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n18_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx209_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n19_call_α
n18_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n16_statement_begin_β
.Lx209_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:             sub              rsp, 16
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
.Lrkfnzd211:            .string          "ITEM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd211]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx210_240
                        add              rsp, 16;                   jmp   n18_lit_integer_β
.Lx210_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n20_lit_integer_α
n19_call_β:             add              rsp, 16;                   jmp   n18_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx212_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n21_call_α
n20_lit_integer_β:      add              rsp, 32;                   jmp   n18_lit_integer_β
.Lx212_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n21_call_α:             sub              rsp, 16
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
.Lrkfnzd214:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd214]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx213_240
                        add              rsp, 16;                   jmp   n20_lit_integer_β
.Lx213_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n22_statement_end_α
n21_call_β:             add              rsp, 16;                   jmp   n20_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:    add              rsp, 80;                   jmp   n23_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1114/001: item 1D assign/read'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                              jmp   n24_lit_string_α
n23_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx219_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n25_assign_α
.Lx219_0:               .quad            .Lx219_0_s
.Lx219_0_s:             .string          "FAIL 1114/001: item 1D assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx220_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n26_statement_end_α
.Lx220_0:               .quad            .Lx220_0_s
.Lx220_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_begin_α:                                              jmp   n28_statement_end_α
n27_statement_begin_β:                                              jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:                                                jmp   n29_statement_begin_α
#=======================================================================================================================
#         DIFFER(aaa<1>, 5)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                              jmp   n30_var_α
n29_statement_begin_β:                                              jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # aaa
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n31_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx230_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n32_subscript_α
n31_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n29_statement_begin_β
.Lx230_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n32_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx231_240
                        add              rsp, 16;                   jmp   n31_lit_integer_β
.Lx231_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n33_deref_α
n32_subscript_β:        add              rsp, 16;                   jmp   n31_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n33_deref_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_deref@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx232_240
                        add              rsp, 16;                   jmp   n32_subscript_β
.Lx232_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n34_lit_integer_α
n33_deref_β:            add              rsp, 16;                   jmp   n32_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n35_call_α
n34_lit_integer_β:      add              rsp, 16;                   jmp   n33_deref_β
.Lx233_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:             sub              rsp, 16
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
.Lrkfnzd235:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd235]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx234_240
                        add              rsp, 16;                   jmp   n34_lit_integer_β
.Lx234_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n36_statement_end_α
n35_call_β:             add              rsp, 16;                   jmp   n34_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    add              rsp, 96;                   jmp   n37_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1114/002: item == bracket read' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:                                              jmp   n38_lit_string_α
n37_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 35
                        mov              rax, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n39_assign_α
.Lx240_0:               .quad            .Lx240_0_s
.Lx240_0_s:             .string          "FAIL 1114/002: item == bracket read"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx241_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n40_statement_end_α
.Lx241_0:               .quad            .Lx241_0_s
.Lx241_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:                                              jmp   n42_statement_end_α
n41_statement_begin_β:                                              jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:                                                jmp   n43_statement_begin_α
#=======================================================================================================================
#         aaa<2> = 22
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:                                              jmp   n44_var_α
n43_statement_begin_β:                                              jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # aaa
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx251_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n46_subscript_α
n45_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n43_statement_begin_β
.Lx251_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n46_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx252_240
                        add              rsp, 16;                   jmp   n45_lit_integer_β
.Lx252_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n47_lit_integer_α
n46_subscript_β:        add              rsp, 16;                   jmp   n45_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n48_assign_var_α
n47_lit_integer_β:      add              rsp, 16;                   jmp   n46_subscript_β
.Lx253_0:               .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n48_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx254_240
                        add              rsp, 16;                   jmp   n47_lit_integer_β
.Lx254_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n49_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_end_α:    add              rsp, 80;                   jmp   n50_statement_begin_α
#=======================================================================================================================
#         DIFFER(ITEM(aaa, 2), 22)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_begin_α:                                              jmp   n51_var_α
n50_statement_begin_β:                                              jmp   n61_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # aaa
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n53_call_α
n52_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n50_statement_begin_β
.Lx260_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:             sub              rsp, 16
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
.Lrkfnzd262:            .string          "ITEM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd262]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx261_240
                        add              rsp, 16;                   jmp   n52_lit_integer_β
.Lx261_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n54_lit_integer_α
n53_call_β:             add              rsp, 16;                   jmp   n52_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx263_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n55_call_α
n54_lit_integer_β:      add              rsp, 32;                   jmp   n52_lit_integer_β
.Lx263_0:               .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n55_call_α:             sub              rsp, 16
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
.Lrkfnzd265:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd265]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx264_240
                        add              rsp, 16;                   jmp   n54_lit_integer_β
.Lx264_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n56_statement_end_α
n55_call_β:             add              rsp, 16;                   jmp   n54_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:    add              rsp, 80;                   jmp   n57_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1114/003: bracket assign, item read' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                              jmp   n58_lit_string_α
n57_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 40
                        mov              rax, qword ptr [rip + .Lx270_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n59_assign_α
.Lx270_0:               .quad            .Lx270_0_s
.Lx270_0_s:             .string          "FAIL 1114/003: bracket assign, item read"
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx271_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n60_statement_end_α
.Lx271_0:               .quad            .Lx271_0_s
.Lx271_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:                                              jmp   n62_statement_end_α
n61_statement_begin_β:                                              jmp   n63_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_end_α:                                                jmp   n63_statement_begin_α
#=======================================================================================================================
#         ama = ARRAY('2,2,2,2')
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_begin_α:                                              jmp   n64_lit_string_α
n63_statement_begin_β:                                              jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx280_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n65_call_α
.Lx280_0:               .quad            .Lx280_0_s
.Lx280_0_s:             .string          "2,2,2,2"
#-----------------------------------------------------------------------------------------------------------------------
n65_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd282:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd282]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx281_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n63_statement_begin_β
.Lx281_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n66_assign_α
n65_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n63_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # ama
                        mov              qword ptr [r9 + 24], rdx;  jmp   n67_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:    add              rsp, 32;                   jmp   n68_statement_begin_α
#=======================================================================================================================
#         ITEM(ama, 1,2,1,2) = 1212
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:                                              jmp   n69_var_α
n68_statement_begin_β:                                              jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # ama
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n70_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n71_subscript_α
n70_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n68_statement_begin_β
.Lx289_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n71_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx290_240
                        add              rsp, 16;                   jmp   n70_lit_integer_β
.Lx290_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n72_lit_integer_α
n71_subscript_β:        add              rsp, 16;                   jmp   n70_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx291_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n73_subscript_α
n72_lit_integer_β:      add              rsp, 16;                   jmp   n71_subscript_β
.Lx291_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n73_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx292_240
                        add              rsp, 16;                   jmp   n72_lit_integer_β
.Lx292_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n74_lit_integer_α
n73_subscript_β:        add              rsp, 16;                   jmp   n72_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx293_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n75_subscript_α
n74_lit_integer_β:      add              rsp, 16;                   jmp   n73_subscript_β
.Lx293_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n75_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx294_240
                        add              rsp, 16;                   jmp   n74_lit_integer_β
.Lx294_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n76_lit_integer_α
n75_subscript_β:        add              rsp, 16;                   jmp   n74_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx295_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n77_subscript_α
n76_lit_integer_β:      add              rsp, 16;                   jmp   n75_subscript_β
.Lx295_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n77_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx296_240
                        add              rsp, 16;                   jmp   n76_lit_integer_β
.Lx296_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n78_lit_integer_α
n77_subscript_β:        add              rsp, 16;                   jmp   n76_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n79_assign_var_α
n78_lit_integer_β:      add              rsp, 16;                   jmp   n77_subscript_β
.Lx297_0:               .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx298_240
                        add              rsp, 16;                   jmp   n78_lit_integer_β
.Lx298_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n80_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:    add              rsp, 176;                  jmp   n81_statement_begin_α
#=======================================================================================================================
#         DIFFER(ITEM(ama, 1,2,1,2), 1212)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:                                              jmp   n82_var_α
n81_statement_begin_β:                                              jmp   n95_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n82_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # ama
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n83_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx304_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n84_lit_integer_α
n83_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n81_statement_begin_β
.Lx304_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx305_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n85_lit_integer_α
n84_lit_integer_β:      add              rsp, 16;                   jmp   n83_lit_integer_β
.Lx305_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx306_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n86_lit_integer_α
n85_lit_integer_β:      add              rsp, 16;                   jmp   n84_lit_integer_β
.Lx306_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n87_call_α
n86_lit_integer_β:      add              rsp, 16;                   jmp   n85_lit_integer_β
.Lx307_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n87_call_α:             sub              rsp, 16
                        sub              rsp, 80
                        mov              r8, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 40], r8
                        mov              r8, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 48], r8
                        mov              r8, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 56], r8
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 64], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], r8
                        .section         .rodata
.Lrkfnzd309:            .string          "ITEM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd309]
                        lea              rsi, [rsp + 0]
                        mov              edx, 5
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 80
                        cmp              eax, 104;                  jne   .Lx308_240
                        add              rsp, 16;                   jmp   n86_lit_integer_β
.Lx308_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n88_lit_integer_α
n87_call_β:             add              rsp, 16;                   jmp   n86_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx310_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n89_call_α
n88_lit_integer_β:      add              rsp, 32;                   jmp   n86_lit_integer_β
.Lx310_0:               .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n89_call_α:             sub              rsp, 16
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
.Lrkfnzd312:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd312]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx311_240
                        add              rsp, 16;                   jmp   n88_lit_integer_β
.Lx311_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n90_statement_end_α
n89_call_β:             add              rsp, 16;                   jmp   n88_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:    add              rsp, 128;                  jmp   n91_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1114/004: item 4D assign/read'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:                                              jmp   n92_lit_string_α
n91_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx317_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n93_assign_α
.Lx317_0:               .quad            .Lx317_0_s
.Lx317_0_s:             .string          "FAIL 1114/004: item 4D assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx318_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n94_statement_end_α
.Lx318_0:               .quad            .Lx318_0_s
.Lx318_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_begin_α:                                              jmp   n96_statement_end_α
n95_statement_begin_β:                                              jmp   n97_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_end_α:                                                jmp   n97_statement_begin_α
#=======================================================================================================================
#         DIFFER(ama<1,2,1,2>, 1212)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_begin_α:                                              jmp   n98_var_α
n97_statement_begin_β:                                              jmp   n115_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n98_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # ama
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n99_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n99_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx328_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n100_subscript_α
n99_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n97_statement_begin_β
.Lx328_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n100_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx329_240
                        add              rsp, 16;                   jmp   n99_lit_integer_β
.Lx329_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n101_lit_integer_α
n100_subscript_β:       add              rsp, 16;                   jmp   n99_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx330_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n102_subscript_α
n101_lit_integer_β:     add              rsp, 16;                   jmp   n100_subscript_β
.Lx330_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n102_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx331_240
                        add              rsp, 16;                   jmp   n101_lit_integer_β
.Lx331_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n103_lit_integer_α
n102_subscript_β:       add              rsp, 16;                   jmp   n101_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n103_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx332_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n104_subscript_α
n103_lit_integer_β:     add              rsp, 16;                   jmp   n102_subscript_β
.Lx332_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n104_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx333_240
                        add              rsp, 16;                   jmp   n103_lit_integer_β
.Lx333_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n105_lit_integer_α
n104_subscript_β:       add              rsp, 16;                   jmp   n103_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n105_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx334_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n106_subscript_α
n105_lit_integer_β:     add              rsp, 16;                   jmp   n104_subscript_β
.Lx334_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n106_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx335_240
                        add              rsp, 16;                   jmp   n105_lit_integer_β
.Lx335_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n107_deref_α
n106_subscript_β:       add              rsp, 16;                   jmp   n105_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n107_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_deref@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx336_240
                        add              rsp, 16;                   jmp   n106_subscript_β
.Lx336_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n108_lit_integer_α
n107_deref_β:           add              rsp, 16;                   jmp   n106_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n108_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx337_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n109_call_α
n108_lit_integer_β:     add              rsp, 16;                   jmp   n107_deref_β
.Lx337_0:               .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n109_call_α:            sub              rsp, 16
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
.Lrkfnzd339:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd339]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx338_240
                        add              rsp, 16;                   jmp   n108_lit_integer_β
.Lx338_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n110_statement_end_α
n109_call_β:            add              rsp, 16;                   jmp   n108_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_end_α:   add              rsp, 192;                  jmp   n111_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1114/005: item 4D == bracket'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_begin_α:                                             jmp   n112_lit_string_α
n111_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n112_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx344_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n113_assign_α
.Lx344_0:               .quad            .Lx344_0_s
.Lx344_0_s:             .string          "FAIL 1114/005: item 4D == bracket"
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx345_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n114_statement_end_α
.Lx345_0:               .quad            .Lx345_0_s
.Lx345_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_begin_α:                                             jmp   n116_statement_end_α
n115_statement_begin_β:                                             jmp   n117_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_end_α:                                               jmp   n117_statement_begin_α
#=======================================================================================================================
#         ama<2,1,2,1> = 2121
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_begin_α:                                             jmp   n118_var_α
n117_statement_begin_β:                                             jmp   n130_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n118_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # ama
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n119_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx355_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n120_subscript_α
n119_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n117_statement_begin_β
.Lx355_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n120_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx356_240
                        add              rsp, 16;                   jmp   n119_lit_integer_β
.Lx356_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n121_lit_integer_α
n120_subscript_β:       add              rsp, 16;                   jmp   n119_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx357_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n122_subscript_α
n121_lit_integer_β:     add              rsp, 16;                   jmp   n120_subscript_β
.Lx357_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n122_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx358_240
                        add              rsp, 16;                   jmp   n121_lit_integer_β
.Lx358_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n123_lit_integer_α
n122_subscript_β:       add              rsp, 16;                   jmp   n121_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n123_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx359_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n124_subscript_α
n123_lit_integer_β:     add              rsp, 16;                   jmp   n122_subscript_β
.Lx359_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n124_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx360_240
                        add              rsp, 16;                   jmp   n123_lit_integer_β
.Lx360_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n125_lit_integer_α
n124_subscript_β:       add              rsp, 16;                   jmp   n123_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n125_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx361_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n126_subscript_α
n125_lit_integer_β:     add              rsp, 16;                   jmp   n124_subscript_β
.Lx361_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n126_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx362_240
                        add              rsp, 16;                   jmp   n125_lit_integer_β
.Lx362_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n127_lit_integer_α
n126_subscript_β:       add              rsp, 16;                   jmp   n125_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx363_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n128_assign_var_α
n127_lit_integer_β:     add              rsp, 16;                   jmp   n126_subscript_β
.Lx363_0:               .quad            2121
#-----------------------------------------------------------------------------------------------------------------------
n128_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx364_240
                        add              rsp, 16;                   jmp   n127_lit_integer_β
.Lx364_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n129_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n129_statement_end_α:   add              rsp, 176;                  jmp   n130_statement_begin_α
#=======================================================================================================================
#         DIFFER(ITEM(ama, 2,1,2,1), 2121)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_begin_α:                                             jmp   n131_var_α
n130_statement_begin_β:                                             jmp   n144_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n131_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # ama
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n132_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx370_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n133_lit_integer_α
n132_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n130_statement_begin_β
.Lx370_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx371_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n134_lit_integer_α
n133_lit_integer_β:     add              rsp, 16;                   jmp   n132_lit_integer_β
.Lx371_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n134_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx372_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n135_lit_integer_α
n134_lit_integer_β:     add              rsp, 16;                   jmp   n133_lit_integer_β
.Lx372_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n135_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx373_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n136_call_α
n135_lit_integer_β:     add              rsp, 16;                   jmp   n134_lit_integer_β
.Lx373_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n136_call_α:            sub              rsp, 16
                        sub              rsp, 80
                        mov              r8, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 40], r8
                        mov              r8, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 48], r8
                        mov              r8, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 56], r8
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 64], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], r8
                        .section         .rodata
.Lrkfnzd375:            .string          "ITEM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd375]
                        lea              rsi, [rsp + 0]
                        mov              edx, 5
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 80
                        cmp              eax, 104;                  jne   .Lx374_240
                        add              rsp, 16;                   jmp   n135_lit_integer_β
.Lx374_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n137_lit_integer_α
n136_call_β:            add              rsp, 16;                   jmp   n135_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n137_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx376_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n138_call_α
n137_lit_integer_β:     add              rsp, 32;                   jmp   n135_lit_integer_β
.Lx376_0:               .quad            2121
#-----------------------------------------------------------------------------------------------------------------------
n138_call_α:            sub              rsp, 16
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
.Lrkfnzd378:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd378]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx377_240
                        add              rsp, 16;                   jmp   n137_lit_integer_β
.Lx377_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n139_statement_end_α
n138_call_β:            add              rsp, 16;                   jmp   n137_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_end_α:   add              rsp, 128;                  jmp   n140_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1114/006: bracket 4D assign, item read' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_begin_α:                                             jmp   n141_lit_string_α
n140_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n141_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx383_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n142_assign_α
.Lx383_0:               .quad            .Lx383_0_s
.Lx383_0_s:             .string          "FAIL 1114/006: bracket 4D assign, item read"
#-----------------------------------------------------------------------------------------------------------------------
n142_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx384_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n143_statement_end_α
.Lx384_0:               .quad            .Lx384_0_s
.Lx384_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_begin_α:                                             jmp   n145_statement_end_α
n144_statement_begin_β:                                             jmp   n146_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_end_α:                                               jmp   n146_statement_begin_α
#=======================================================================================================================
#         tt = TABLE()
#-----------------------------------------------------------------------------------------------------------------------
n146_statement_begin_α:                                             jmp   n147_call_α
n146_statement_begin_β:                                             jmp   n150_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n147_call_α:            sub              rsp, 16
                        .section         .rodata
.Lrkfnzd394:            .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd394]
                        xor              esi, esi
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx393_240
                        add              rsp, 16;                   jmp   n146_statement_begin_β
.Lx393_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n148_assign_α
n147_call_β:            add              rsp, 16;                   jmp   n146_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n148_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # tt
                        mov              qword ptr [r9 + 40], rdx;  jmp   n149_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_end_α:   add              rsp, 16;                   jmp   n150_statement_begin_α
#=======================================================================================================================
#         ITEM(tt, 'key') = 'val'
#-----------------------------------------------------------------------------------------------------------------------
n150_statement_begin_α:                                             jmp   n151_var_α
n150_statement_begin_β:                                             jmp   n157_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n151_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # tt
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n152_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n152_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx401_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n153_subscript_α
n152_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n150_statement_begin_β
.Lx401_0:               .quad            .Lx401_0_s
.Lx401_0_s:             .string          "key"
#-----------------------------------------------------------------------------------------------------------------------
n153_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_subscript_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx402_240
                        add              rsp, 16;                   jmp   n152_lit_string_β
.Lx402_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n154_lit_string_α
n153_subscript_β:       add              rsp, 16;                   jmp   n152_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n154_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx403_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n155_assign_var_α
n154_lit_string_β:      add              rsp, 16;                   jmp   n153_subscript_β
.Lx403_0:               .quad            .Lx403_0_s
.Lx403_0_s:             .string          "val"
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx404_240
                        add              rsp, 16;                   jmp   n154_lit_string_β
.Lx404_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n156_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:   add              rsp, 80;                   jmp   n157_statement_begin_α
#=======================================================================================================================
#         DIFFER(ITEM(tt, 'key'), 'val')                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_begin_α:                                             jmp   n158_var_α
n157_statement_begin_β:                                             jmp   n168_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # tt
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n159_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n159_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx410_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n160_call_α
n159_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n157_statement_begin_β
.Lx410_0:               .quad            .Lx410_0_s
.Lx410_0_s:             .string          "key"
#-----------------------------------------------------------------------------------------------------------------------
n160_call_α:            sub              rsp, 16
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
.Lrkfnzd412:            .string          "ITEM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd412]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx411_240
                        add              rsp, 16;                   jmp   n159_lit_string_β
.Lx411_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n161_lit_string_α
n160_call_β:            add              rsp, 16;                   jmp   n159_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n161_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx413_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n162_call_α
n161_lit_string_β:      add              rsp, 32;                   jmp   n159_lit_string_β
.Lx413_0:               .quad            .Lx413_0_s
.Lx413_0_s:             .string          "val"
#-----------------------------------------------------------------------------------------------------------------------
n162_call_α:            sub              rsp, 16
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
.Lrkfnzd415:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd415]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx414_240
                        add              rsp, 16;                   jmp   n161_lit_string_β
.Lx414_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n163_statement_end_α
n162_call_β:            add              rsp, 16;                   jmp   n161_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n163_statement_end_α:   add              rsp, 80;                   jmp   n164_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1114/007: item on table'        :(END)
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_begin_α:                                             jmp   n165_lit_string_α
n164_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n165_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx420_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n166_assign_α
.Lx420_0:               .quad            .Lx420_0_s
.Lx420_0_s:             .string          "FAIL 1114/007: item on table"
#-----------------------------------------------------------------------------------------------------------------------
n166_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx421_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n167_statement_end_α
.Lx421_0:               .quad            .Lx421_0_s
.Lx421_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n167_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n168_statement_begin_α:                                             jmp   n169_statement_end_α
n168_statement_begin_β:                                             jmp   n170_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n169_statement_end_α:                                               jmp   n170_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1114_item (7/7)'
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_begin_α:                                             jmp   n171_lit_string_α
n170_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n171_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx430_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n172_assign_α
.Lx430_0:               .quad            .Lx430_0_s
.Lx430_0_s:             .string          "PASS 1114_item (7/7)"
#-----------------------------------------------------------------------------------------------------------------------
n172_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx431_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n173_statement_end_α
.Lx431_0:               .quad            .Lx431_0_s
.Lx431_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n174_goto_α:                                                        jmp   n27_statement_begin_α
n174_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n175_goto_α:                                                        jmp   n41_statement_begin_α
n175_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n176_goto_α:                                                        jmp   n61_statement_begin_α
n176_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n177_goto_α:                                                        jmp   n95_statement_begin_α
n177_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n178_goto_α:                                                        jmp   n115_statement_begin_α
n178_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n179_goto_α:                                                        jmp   n144_statement_begin_α
n179_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n180_goto_α:                                                        jmp   n168_statement_begin_α
n180_goto_β:                                                        jmp   main_ω
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

                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "a"
.Lgvan1:                .string          "b"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
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
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_statement_end_α
n0_statement_begin_β:                                               jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                 jmp   n2_statement_begin_α
#=======================================================================================================================
#         a = ARRAY(3)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_integer_α
n2_statement_begin_β:                                               jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_call_α
.Lx178_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd180:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd180]
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
                        cmp              eax, 104;                  jne   .Lx179_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx179_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n5_assign_α
n4_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:            mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # a
                        mov              qword ptr [r9 + 8], rdx;   jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 32;                   jmp   n7_statement_begin_α
#=======================================================================================================================
#         a = ARRAY(3)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                               jmp   n8_statement_end_α
n7_statement_begin_β:                                               jmp   n9_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:                                                 jmp   n9_statement_begin_α
#=======================================================================================================================
#         DIFFER(a<1>)               :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:                                               jmp   n10_var_α
n9_statement_begin_β:                                               jmp   n20_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n12_subscript_α
n11_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n9_statement_begin_β
.Lx191_0:               .quad            1
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
                        cmp              eax, 104;                  jne   .Lx192_240
                        add              rsp, 16;                   jmp   n11_lit_integer_β
.Lx192_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n13_deref_α
n12_subscript_β:        add              rsp, 16;                   jmp   n11_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n13_deref_α:            sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx193_240
                        add              rsp, 16;                   jmp   n12_subscript_β
.Lx193_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n14_call_α
n13_deref_β:            add              rsp, 16;                   jmp   n12_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd195:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd195]
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
                        cmp              eax, 104;                  jne   .Lx194_240
                        add              rsp, 16;                   jmp   n13_deref_β
.Lx194_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n15_statement_end_α
n14_call_β:             add              rsp, 16;                   jmp   n13_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    add              rsp, 80;                   jmp   n16_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/001: array element init null' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:                                              jmp   n17_lit_string_α
n16_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n18_assign_α
.Lx200_0:               .quad            .Lx200_0_s
.Lx200_0_s:             .string          "FAIL 1110/001: array element init null"
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx201_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n19_statement_end_α
.Lx201_0:               .quad            .Lx201_0_s
.Lx201_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:                                              jmp   n21_statement_end_α
n20_statement_begin_β:                                              jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:                                                jmp   n22_statement_begin_α
#=======================================================================================================================
#         a<2> = 4.5
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                              jmp   n23_var_α
n22_statement_begin_β:                                              jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n24_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx211_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n25_subscript_α
n24_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n22_statement_begin_β
.Lx211_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n25_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx212_240
                        add              rsp, 16;                   jmp   n24_lit_integer_β
.Lx212_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n26_lit_real_α
n25_subscript_β:        add              rsp, 16;                   jmp   n24_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n27_assign_var_α
.Lx213_0:               .quad            4616752568008179712
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_real
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
                        cmp              eax, 104;                  jne   .Lx214_240
                        add              rsp, 32;                   jmp   n25_subscript_β
.Lx214_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    add              rsp, 80;                   jmp   n29_statement_begin_α
#=======================================================================================================================
#         DIFFER(a<2>, 4.5)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                              jmp   n30_var_α
n29_statement_begin_β:                                              jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n31_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n32_subscript_α
n31_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n29_statement_begin_β
.Lx220_0:               .quad            2
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
                        cmp              eax, 104;                  jne   .Lx221_240
                        add              rsp, 16;                   jmp   n31_lit_integer_β
.Lx221_240:             mov              qword ptr [rsp + 0], rax                       # result
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
                        cmp              eax, 104;                  jne   .Lx222_240
                        add              rsp, 16;                   jmp   n32_subscript_β
.Lx222_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n34_lit_real_α
n33_deref_β:            add              rsp, 16;                   jmp   n32_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx223_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n35_call_α
.Lx223_0:               .quad            4616752568008179712
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
.Lrkfnzd225:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd225]
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
                        cmp              eax, 104;                  jne   .Lx224_240
                        add              rsp, 32;                   jmp   n33_deref_β
.Lx224_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n36_statement_end_α
n35_call_β:             add              rsp, 32;                   jmp   n33_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    add              rsp, 96;                   jmp   n37_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/002: array assign/read'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:                                              jmp   n38_lit_string_α
n37_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx230_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n39_assign_α
.Lx230_0:               .quad            .Lx230_0_s
.Lx230_0_s:             .string          "FAIL 1110/002: array assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx231_0]
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
.Lx231_0:               .quad            .Lx231_0_s
.Lx231_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:                                              jmp   n42_statement_end_α
n41_statement_begin_β:                                              jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:                                                jmp   n43_statement_begin_α
#=======================================================================================================================
#         a<4>                       :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:                                              jmp   n44_var_α
n43_statement_begin_β:                                              jmp   n53_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx241_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n46_subscript_α
n45_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n43_statement_begin_β
.Lx241_0:               .quad            4
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
                        cmp              eax, 104;                  jne   .Lx242_240
                        add              rsp, 16;                   jmp   n45_lit_integer_β
.Lx242_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n47_deref_α
n46_subscript_β:        add              rsp, 16;                   jmp   n45_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n47_deref_α:            sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx243_240
                        add              rsp, 16;                   jmp   n46_subscript_β
.Lx243_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n48_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    add              rsp, 64;                   jmp   n49_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/003: OOB high should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:                                              jmp   n50_lit_string_α
n49_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 35
                        mov              rax, qword ptr [rip + .Lx248_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n51_assign_α
.Lx248_0:               .quad            .Lx248_0_s
.Lx248_0_s:             .string          "FAIL 1110/003: OOB high should fail"
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx249_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n52_statement_end_α
.Lx249_0:               .quad            .Lx249_0_s
.Lx249_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:                                              jmp   n54_statement_end_α
n53_statement_begin_β:                                              jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:                                                jmp   n55_statement_begin_α
#=======================================================================================================================
#         a<0>                       :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:                                              jmp   n56_var_α
n55_statement_begin_β:                                              jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n58_subscript_α
n57_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n55_statement_begin_β
.Lx259_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n58_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx260_240
                        add              rsp, 16;                   jmp   n57_lit_integer_β
.Lx260_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n59_deref_α
n58_subscript_β:        add              rsp, 16;                   jmp   n57_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n59_deref_α:            sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx261_240
                        add              rsp, 16;                   jmp   n58_subscript_β
.Lx261_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n60_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 64;                   jmp   n61_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/004: OOB zero should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:                                              jmp   n62_lit_string_α
n61_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 35
                        mov              rax, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n63_assign_α
.Lx266_0:               .quad            .Lx266_0_s
.Lx266_0_s:             .string          "FAIL 1110/004: OOB zero should fail"
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx267_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n64_statement_end_α
.Lx267_0:               .quad            .Lx267_0_s
.Lx267_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:                                              jmp   n66_statement_end_α
n65_statement_begin_β:                                              jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:                                                jmp   n67_statement_begin_α
#=======================================================================================================================
#         DIFFER(PROTOTYPE(a), 3)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:                                              jmp   n68_var_α
n67_statement_begin_β:                                              jmp   n77_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n69_call_α
#-----------------------------------------------------------------------------------------------------------------------
n69_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd278:            .string          "PROTOTYPE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd278]
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
                        cmp              eax, 104;                  jne   .Lx277_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n67_statement_begin_β
.Lx277_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n70_lit_integer_α
n69_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n67_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx279_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n71_call_α
n70_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n67_statement_begin_β
.Lx279_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n71_call_α:             sub              rsp, 16
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
.Lrkfnzd281:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd281]
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
                        cmp              eax, 104;                  jne   .Lx280_240
                        add              rsp, 16;                   jmp   n70_lit_integer_β
.Lx280_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n72_statement_end_α
n71_call_β:             add              rsp, 16;                   jmp   n70_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_end_α:    add              rsp, 64;                   jmp   n73_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/005: prototype(array(3)) is integer 3' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_begin_α:                                              jmp   n74_lit_string_α
n73_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 47
                        mov              rax, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n75_assign_α
.Lx286_0:               .quad            .Lx286_0_s
.Lx286_0_s:             .string          "FAIL 1110/005: prototype(array(3)) is integer 3"
#-----------------------------------------------------------------------------------------------------------------------
n75_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx287_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n76_statement_end_α
.Lx287_0:               .quad            .Lx287_0_s
.Lx287_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_begin_α:                                              jmp   n78_statement_end_α
n77_statement_begin_β:                                              jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:                                                jmp   n79_statement_begin_α
#=======================================================================================================================
#         b = ARRAY('3')
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:                                              jmp   n80_lit_string_α
n79_statement_begin_β:                                              jmp   n84_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx296_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n81_call_α
.Lx296_0:               .quad            .Lx296_0_s
.Lx296_0_s:             .string          "3"
#-----------------------------------------------------------------------------------------------------------------------
n81_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd298:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd298]
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
                        cmp              eax, 104;                  jne   .Lx297_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n79_statement_begin_β
.Lx297_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n82_assign_α
n81_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n79_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n82_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # b
                        mov              qword ptr [r9 + 24], rdx;  jmp   n83_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_end_α:    add              rsp, 32;                   jmp   n84_statement_begin_α
#=======================================================================================================================
#         b<2> = 'x'
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_begin_α:                                              jmp   n85_var_α
n84_statement_begin_β:                                              jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n85_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n86_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx305_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n87_subscript_α
n86_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n84_statement_begin_β
.Lx305_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n87_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx306_240
                        add              rsp, 16;                   jmp   n86_lit_integer_β
.Lx306_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n88_lit_string_α
n87_subscript_β:        add              rsp, 16;                   jmp   n86_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n89_assign_var_α
n88_lit_string_β:       add              rsp, 16;                   jmp   n87_subscript_β
.Lx307_0:               .quad            .Lx307_0_s
.Lx307_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n89_assign_var_α:       sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx308_240
                        add              rsp, 16;                   jmp   n88_lit_string_β
.Lx308_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n90_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:    add              rsp, 80;                   jmp   n91_statement_begin_α
#=======================================================================================================================
#         DIFFER(b<2>, 'x')                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:                                              jmp   n92_var_α
n91_statement_begin_β:                                              jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n93_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx314_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n94_subscript_α
n93_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n91_statement_begin_β
.Lx314_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n94_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx315_240
                        add              rsp, 16;                   jmp   n93_lit_integer_β
.Lx315_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n95_deref_α
n94_subscript_β:        add              rsp, 16;                   jmp   n93_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n95_deref_α:            sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx316_240
                        add              rsp, 16;                   jmp   n94_subscript_β
.Lx316_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n96_lit_string_α
n95_deref_β:            add              rsp, 16;                   jmp   n94_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx317_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n97_call_α
n96_lit_string_β:       add              rsp, 16;                   jmp   n95_deref_β
.Lx317_0:               .quad            .Lx317_0_s
.Lx317_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n97_call_α:             sub              rsp, 16
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
.Lrkfnzd319:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd319]
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
                        cmp              eax, 104;                  jne   .Lx318_240
                        add              rsp, 16;                   jmp   n96_lit_string_β
.Lx318_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n98_statement_end_α
n97_call_β:             add              rsp, 16;                   jmp   n96_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_end_α:    add              rsp, 96;                   jmp   n99_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/006: array from string dim' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_begin_α:                                              jmp   n100_lit_string_α
n99_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx324_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n101_assign_α
.Lx324_0:               .quad            .Lx324_0_s
.Lx324_0_s:             .string          "FAIL 1110/006: array from string dim"
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx325_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n102_statement_end_α
.Lx325_0:               .quad            .Lx325_0_s
.Lx325_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                             jmp   n104_statement_end_α
n103_statement_begin_β:                                             jmp   n105_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:                                               jmp   n105_statement_begin_α
#=======================================================================================================================
#         DIFFER(PROTOTYPE(b), 3)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_begin_α:                                             jmp   n106_var_α
n105_statement_begin_β:                                             jmp   n115_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n106_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n107_call_α
#-----------------------------------------------------------------------------------------------------------------------
n107_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd336:            .string          "PROTOTYPE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd336]
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
                        cmp              eax, 104;                  jne   .Lx335_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n105_statement_begin_β
.Lx335_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n108_lit_integer_α
n107_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n105_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n108_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx337_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n109_call_α
n108_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                   jmp   n105_statement_begin_β
.Lx337_0:               .quad            3
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
n110_statement_end_α:   add              rsp, 64;                   jmp   n111_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/007: prototype string-dim is integer' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_begin_α:                                             jmp   n112_lit_string_α
n111_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n112_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 46
                        mov              rax, qword ptr [rip + .Lx344_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n113_assign_α
.Lx344_0:               .quad            .Lx344_0_s
.Lx344_0_s:             .string          "FAIL 1110/007: prototype string-dim is integer"
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
#         a<1> = 3.14
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_begin_α:                                             jmp   n118_var_α
n117_statement_begin_β:                                             jmp   n124_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n118_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n119_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx355_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n120_subscript_α
n119_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n117_statement_begin_β
.Lx355_0:               .quad            1
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n121_lit_real_α
n120_subscript_β:       add              rsp, 16;                   jmp   n119_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx357_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n122_assign_var_α
.Lx357_0:               .quad            4614253070214989087
#-----------------------------------------------------------------------------------------------------------------------
n122_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_real
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
                        cmp              eax, 104;                  jne   .Lx358_240
                        add              rsp, 32;                   jmp   n120_subscript_β
.Lx358_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n123_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_end_α:   add              rsp, 80;                   jmp   n124_statement_begin_α
#=======================================================================================================================
#         DIFFER(a<1>, 3.14)                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_begin_α:                                             jmp   n125_var_α
n124_statement_begin_β:                                             jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n125_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n126_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n126_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx364_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n127_subscript_α
n126_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n124_statement_begin_β
.Lx364_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n127_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx365_240
                        add              rsp, 16;                   jmp   n126_lit_integer_β
.Lx365_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n128_deref_α
n127_subscript_β:       add              rsp, 16;                   jmp   n126_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n128_deref_α:           sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx366_240
                        add              rsp, 16;                   jmp   n127_subscript_β
.Lx366_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n129_lit_real_α
n128_deref_β:           add              rsp, 16;                   jmp   n127_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n129_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx367_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n130_call_α
.Lx367_0:               .quad            4614253070214989087
#-----------------------------------------------------------------------------------------------------------------------
n130_call_α:            sub              rsp, 16
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
.Lrkfnzd369:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd369]
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
                        cmp              eax, 104;                  jne   .Lx368_240
                        add              rsp, 32;                   jmp   n128_deref_β
.Lx368_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n131_statement_end_α
n130_call_β:            add              rsp, 32;                   jmp   n128_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_end_α:   add              rsp, 96;                   jmp   n132_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/008: array stores real'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_begin_α:                                             jmp   n133_lit_string_α
n132_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx374_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n134_assign_α
.Lx374_0:               .quad            .Lx374_0_s
.Lx374_0_s:             .string          "FAIL 1110/008: array stores real"
#-----------------------------------------------------------------------------------------------------------------------
n134_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx375_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n135_statement_end_α
.Lx375_0:               .quad            .Lx375_0_s
.Lx375_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_begin_α:                                             jmp   n137_statement_end_α
n136_statement_begin_β:                                             jmp   n138_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_end_α:                                               jmp   n138_statement_begin_α
#=======================================================================================================================
#         a<3> = 'z'
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_begin_α:                                             jmp   n139_var_α
n138_statement_begin_β:                                             jmp   n145_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n139_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n140_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n140_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx385_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n141_subscript_α
n140_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n138_statement_begin_β
.Lx385_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n141_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx386_240
                        add              rsp, 16;                   jmp   n140_lit_integer_β
.Lx386_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n142_lit_string_α
n141_subscript_β:       add              rsp, 16;                   jmp   n140_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx387_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n143_assign_var_α
n142_lit_string_β:      add              rsp, 16;                   jmp   n141_subscript_β
.Lx387_0:               .quad            .Lx387_0_s
.Lx387_0_s:             .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n143_assign_var_α:      sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx388_240
                        add              rsp, 16;                   jmp   n142_lit_string_β
.Lx388_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n144_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_end_α:   add              rsp, 80;                   jmp   n145_statement_begin_α
#=======================================================================================================================
#         DIFFER(a<3>, 'z')                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_begin_α:                                             jmp   n146_var_α
n145_statement_begin_β:                                             jmp   n157_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n146_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n147_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n147_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx394_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n148_subscript_α
n147_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n145_statement_begin_β
.Lx394_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n148_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx395_240
                        add              rsp, 16;                   jmp   n147_lit_integer_β
.Lx395_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n149_deref_α
n148_subscript_β:       add              rsp, 16;                   jmp   n147_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n149_deref_α:           sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx396_240
                        add              rsp, 16;                   jmp   n148_subscript_β
.Lx396_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n150_lit_string_α
n149_deref_β:           add              rsp, 16;                   jmp   n148_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx397_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n151_call_α
n150_lit_string_β:      add              rsp, 16;                   jmp   n149_deref_β
.Lx397_0:               .quad            .Lx397_0_s
.Lx397_0_s:             .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n151_call_α:            sub              rsp, 16
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
.Lrkfnzd399:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd399]
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
                        cmp              eax, 104;                  jne   .Lx398_240
                        add              rsp, 16;                   jmp   n150_lit_string_β
.Lx398_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n152_statement_end_α
n151_call_β:            add              rsp, 16;                   jmp   n150_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_end_α:   add              rsp, 96;                   jmp   n153_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/009: array stores string'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α:                                             jmp   n154_lit_string_α
n153_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n154_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx404_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n155_assign_α
.Lx404_0:               .quad            .Lx404_0_s
.Lx404_0_s:             .string          "FAIL 1110/009: array stores string"
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx405_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n156_statement_end_α
.Lx405_0:               .quad            .Lx405_0_s
.Lx405_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_begin_α:                                             jmp   n158_statement_end_α
n157_statement_begin_β:                                             jmp   n159_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_statement_end_α:                                               jmp   n159_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1110_array_1d (9/9)'
#-----------------------------------------------------------------------------------------------------------------------
n159_statement_begin_α:                                             jmp   n160_lit_string_α
n159_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n160_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 24
                        mov              rax, qword ptr [rip + .Lx414_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n161_assign_α
.Lx414_0:               .quad            .Lx414_0_s
.Lx414_0_s:             .string          "PASS 1110_array_1d (9/9)"
#-----------------------------------------------------------------------------------------------------------------------
n161_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx415_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n162_statement_end_α
.Lx415_0:               .quad            .Lx415_0_s
.Lx415_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n163_goto_α:                                                        jmp   n20_statement_begin_α
n163_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n164_goto_α:                                                        jmp   n41_statement_begin_α
n164_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n165_goto_α:                                                        jmp   n53_statement_begin_α
n165_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n166_goto_α:                                                        jmp   n65_statement_begin_α
n166_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n167_goto_α:                                                        jmp   n77_statement_begin_α
n167_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n168_goto_α:                                                        jmp   n103_statement_begin_α
n168_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n169_goto_α:                                                        jmp   n115_statement_begin_α
n169_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n170_goto_α:                                                        jmp   n136_statement_begin_α
n170_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n171_goto_α:                                                        jmp   n157_statement_begin_α
n171_goto_β:                                                        jmp   main_ω
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

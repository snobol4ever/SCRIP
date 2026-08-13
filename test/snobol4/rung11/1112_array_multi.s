                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "c"
.Lgvan1:                .string          "d"
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
#         c = ARRAY('2,2')
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_string_α
n2_statement_begin_β:                                               jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_call_α
.Lx120_0:               .quad            .Lx120_0_s
.Lx120_0_s:             .string          "2,2"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd122:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd122]
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
                        cmp              eax, 104;                  jne   .Lx121_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx121_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n5_assign_α
n4_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:            mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # c
                        mov              qword ptr [r9 + 8], rdx;   jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 32;                   jmp   n7_statement_begin_α
#=======================================================================================================================
#         c<1,2> = '*'
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                               jmp   n8_var_α
n7_statement_begin_β:                                               jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # c
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n10_subscript_α
n9_lit_integer_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n7_statement_begin_β
.Lx129_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n10_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx130_240
                        add              rsp, 16;                   jmp   n9_lit_integer_β
.Lx130_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n11_lit_integer_α
n10_subscript_β:        add              rsp, 16;                   jmp   n9_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n12_subscript_α
n11_lit_integer_β:      add              rsp, 16;                   jmp   n10_subscript_β
.Lx131_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n12_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx132_240
                        add              rsp, 16;                   jmp   n11_lit_integer_β
.Lx132_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n13_lit_string_α
n12_subscript_β:        add              rsp, 16;                   jmp   n11_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx133_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n14_assign_var_α
n13_lit_string_β:       add              rsp, 16;                   jmp   n12_subscript_β
.Lx133_0:               .quad            .Lx133_0_s
.Lx133_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_var_α:       sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx134_240
                        add              rsp, 16;                   jmp   n13_lit_string_β
.Lx134_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n15_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    add              rsp, 112;                  jmp   n16_statement_begin_α
#=======================================================================================================================
#         DIFFER(c<1,2>, '*')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:                                              jmp   n17_var_α
n16_statement_begin_β:                                              jmp   n30_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # c
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n18_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n19_subscript_α
n18_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n16_statement_begin_β
.Lx140_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n19_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx141_240
                        add              rsp, 16;                   jmp   n18_lit_integer_β
.Lx141_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n20_lit_integer_α
n19_subscript_β:        add              rsp, 16;                   jmp   n18_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n21_subscript_α
n20_lit_integer_β:      add              rsp, 16;                   jmp   n19_subscript_β
.Lx142_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n21_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx143_240
                        add              rsp, 16;                   jmp   n20_lit_integer_β
.Lx143_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n22_deref_α
n21_subscript_β:        add              rsp, 16;                   jmp   n20_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n22_deref_α:            sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx144_240
                        add              rsp, 16;                   jmp   n21_subscript_β
.Lx144_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n23_lit_string_α
n22_deref_β:            add              rsp, 16;                   jmp   n21_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx145_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n24_call_α
n23_lit_string_β:       add              rsp, 16;                   jmp   n22_deref_β
.Lx145_0:               .quad            .Lx145_0_s
.Lx145_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:             sub              rsp, 16
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
.Lrkfnzd147:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd147]
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
                        cmp              eax, 104;                  jne   .Lx146_240
                        add              rsp, 16;                   jmp   n23_lit_string_β
.Lx146_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n25_statement_end_α
n24_call_β:             add              rsp, 16;                   jmp   n23_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    add              rsp, 128;                  jmp   n26_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1112/001: 2D array assign/read' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                              jmp   n27_lit_string_α
n26_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 35
                        mov              rax, qword ptr [rip + .Lx152_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n28_assign_α
.Lx152_0:               .quad            .Lx152_0_s
.Lx152_0_s:             .string          "FAIL 1112/001: 2D array assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx153_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n29_statement_end_α
.Lx153_0:               .quad            .Lx153_0_s
.Lx153_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:                                              jmp   n31_statement_end_α
n30_statement_begin_β:                                              jmp   n32_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:                                                jmp   n32_statement_begin_α
#=======================================================================================================================
#         DIFFER(PROTOTYPE(c), '2,2')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:                                              jmp   n33_var_α
n32_statement_begin_β:                                              jmp   n42_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # c
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n34_call_α
#-----------------------------------------------------------------------------------------------------------------------
n34_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd164:            .string          "PROTOTYPE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd164]
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
                        cmp              eax, 104;                  jne   .Lx163_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n32_statement_begin_β
.Lx163_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n35_lit_string_α
n34_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n32_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx165_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n36_call_α
n35_lit_string_β:       add              rsp, 16
                        add              rsp, 32;                   jmp   n32_statement_begin_β
.Lx165_0:               .quad            .Lx165_0_s
.Lx165_0_s:             .string          "2,2"
#-----------------------------------------------------------------------------------------------------------------------
n36_call_α:             sub              rsp, 16
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
.Lrkfnzd167:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd167]
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
                        cmp              eax, 104;                  jne   .Lx166_240
                        add              rsp, 16;                   jmp   n35_lit_string_β
.Lx166_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n37_statement_end_α
n36_call_β:             add              rsp, 16;                   jmp   n35_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:    add              rsp, 64;                   jmp   n38_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1112/002: prototype of 2D array' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_begin_α:                                              jmp   n39_lit_string_α
n38_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n40_assign_α
.Lx172_0:               .quad            .Lx172_0_s
.Lx172_0_s:             .string          "FAIL 1112/002: prototype of 2D array"
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx173_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n41_statement_end_α
.Lx173_0:               .quad            .Lx173_0_s
.Lx173_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:                                              jmp   n43_statement_end_α
n42_statement_begin_β:                                              jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:                                                jmp   n44_statement_begin_α
#=======================================================================================================================
#         d = ARRAY('-1:1,2')
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:                                              jmp   n45_lit_string_α
n44_statement_begin_β:                                              jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx182_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n46_call_α
.Lx182_0:               .quad            .Lx182_0_s
.Lx182_0_s:             .string          "-1:1,2"
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd184:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd184]
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
                        cmp              eax, 104;                  jne   .Lx183_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n44_statement_begin_β
.Lx183_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n47_assign_α
n46_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n44_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # d
                        mov              qword ptr [r9 + 24], rdx;  jmp   n48_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    add              rsp, 32;                   jmp   n49_statement_begin_α
#=======================================================================================================================
#         d<-1,1> = 0
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:                                              jmp   n50_var_α
n49_statement_begin_β:                                              jmp   n59_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # d
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n51_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n52_unop_α
n51_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n49_statement_begin_β
.Lx191_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n52_unop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n53_subscript_α
n52_unop_β:             add              rsp, 16;                   jmp   n51_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n53_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # unop
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
                        cmp              eax, 104;                  jne   .Lx193_240
                        add              rsp, 16;                   jmp   n52_unop_β
.Lx193_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n54_lit_integer_α
n53_subscript_β:        add              rsp, 16;                   jmp   n52_unop_β
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n55_subscript_α
n54_lit_integer_β:      add              rsp, 16;                   jmp   n53_subscript_β
.Lx194_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n55_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx195_240
                        add              rsp, 16;                   jmp   n54_lit_integer_β
.Lx195_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n56_lit_integer_α
n55_subscript_β:        add              rsp, 16;                   jmp   n54_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx196_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n57_assign_var_α
n56_lit_integer_β:      add              rsp, 16;                   jmp   n55_subscript_β
.Lx196_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_var_α:       sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx197_240
                        add              rsp, 16;                   jmp   n56_lit_integer_β
.Lx197_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n58_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:    add              rsp, 128;                  jmp   n59_statement_begin_α
#=======================================================================================================================
#         DIFFER(d<-1,1>, 0)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:                                              jmp   n60_var_α
n59_statement_begin_β:                                              jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # d
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n61_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n62_unop_α
n61_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n59_statement_begin_β
.Lx203_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n62_unop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n63_subscript_α
n62_unop_β:             add              rsp, 16;                   jmp   n61_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n63_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # unop
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
                        cmp              eax, 104;                  jne   .Lx205_240
                        add              rsp, 16;                   jmp   n62_unop_β
.Lx205_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n64_lit_integer_α
n63_subscript_β:        add              rsp, 16;                   jmp   n62_unop_β
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx206_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n65_subscript_α
n64_lit_integer_β:      add              rsp, 16;                   jmp   n63_subscript_β
.Lx206_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n65_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx207_240
                        add              rsp, 16;                   jmp   n64_lit_integer_β
.Lx207_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n66_deref_α
n65_subscript_β:        add              rsp, 16;                   jmp   n64_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n66_deref_α:            sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx208_240
                        add              rsp, 16;                   jmp   n65_subscript_β
.Lx208_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n67_lit_integer_α
n66_deref_β:            add              rsp, 16;                   jmp   n65_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx209_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n68_call_α
n67_lit_integer_β:      add              rsp, 16;                   jmp   n66_deref_β
.Lx209_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:             sub              rsp, 16
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
.Lrkfnzd211:            .string          "DIFFER"
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
                        add              rsp, 16;                   jmp   n67_lit_integer_β
.Lx210_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n69_statement_end_α
n68_call_β:             add              rsp, 16;                   jmp   n67_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:    add              rsp, 144;                  jmp   n70_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1112/003: custom lower bound assign/read' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:                                              jmp   n71_lit_string_α
n70_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx216_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n72_assign_α
.Lx216_0:               .quad            .Lx216_0_s
.Lx216_0_s:             .string          "FAIL 1112/003: custom lower bound assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx217_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n73_statement_end_α
.Lx217_0:               .quad            .Lx217_0_s
.Lx217_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:                                              jmp   n75_statement_end_α
n74_statement_begin_β:                                              jmp   n76_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:                                                jmp   n76_statement_begin_α
#=======================================================================================================================
#         d<-2,1>                    :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_begin_α:                                              jmp   n77_var_α
n76_statement_begin_β:                                              jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n77_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # d
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n78_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx227_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n79_unop_α
n78_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n76_statement_begin_β
.Lx227_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n79_unop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n80_subscript_α
n79_unop_β:             add              rsp, 16;                   jmp   n78_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n80_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # unop
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
                        cmp              eax, 104;                  jne   .Lx229_240
                        add              rsp, 16;                   jmp   n79_unop_β
.Lx229_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n81_lit_integer_α
n80_subscript_β:        add              rsp, 16;                   jmp   n79_unop_β
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx230_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n82_subscript_α
n81_lit_integer_β:      add              rsp, 16;                   jmp   n80_subscript_β
.Lx230_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n82_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx231_240
                        add              rsp, 16;                   jmp   n81_lit_integer_β
.Lx231_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n83_deref_α
n82_subscript_β:        add              rsp, 16;                   jmp   n81_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n83_deref_α:            sub              rsp, 16
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
                        add              rsp, 16;                   jmp   n82_subscript_β
.Lx232_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n84_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_end_α:    add              rsp, 112;                  jmp   n85_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1112/004: below lower bound fails' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_begin_α:                                              jmp   n86_lit_string_α
n85_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx237_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n87_assign_α
.Lx237_0:               .quad            .Lx237_0_s
.Lx237_0_s:             .string          "FAIL 1112/004: below lower bound fails"
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx238_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n88_statement_end_α
.Lx238_0:               .quad            .Lx238_0_s
.Lx238_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:                                              jmp   n90_statement_end_α
n89_statement_begin_β:                                              jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:                                                jmp   n91_statement_begin_α
#=======================================================================================================================
#         d<2,1>                     :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:                                              jmp   n92_var_α
n91_statement_begin_β:                                              jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # d
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n93_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx248_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n94_subscript_α
n93_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n91_statement_begin_β
.Lx248_0:               .quad            2
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
                        cmp              eax, 104;                  jne   .Lx249_240
                        add              rsp, 16;                   jmp   n93_lit_integer_β
.Lx249_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n95_lit_integer_α
n94_subscript_β:        add              rsp, 16;                   jmp   n93_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx250_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n96_subscript_α
n95_lit_integer_β:      add              rsp, 16;                   jmp   n94_subscript_β
.Lx250_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n96_subscript_α:        sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx251_240
                        add              rsp, 16;                   jmp   n95_lit_integer_β
.Lx251_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n97_deref_α
n96_subscript_β:        add              rsp, 16;                   jmp   n95_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n97_deref_α:            sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx252_240
                        add              rsp, 16;                   jmp   n96_subscript_β
.Lx252_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n98_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_end_α:    add              rsp, 96;                   jmp   n99_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1112/005: above upper bound fails' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_begin_α:                                              jmp   n100_lit_string_α
n99_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n101_assign_α
.Lx257_0:               .quad            .Lx257_0_s
.Lx257_0_s:             .string          "FAIL 1112/005: above upper bound fails"
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx258_0]
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
.Lx258_0:               .quad            .Lx258_0_s
.Lx258_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                             jmp   n104_statement_end_α
n103_statement_begin_β:                                             jmp   n105_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:                                               jmp   n105_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1112_array_multi (5/5)'
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_begin_α:                                             jmp   n106_lit_string_α
n105_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n107_assign_α
.Lx267_0:               .quad            .Lx267_0_s
.Lx267_0_s:             .string          "PASS 1112_array_multi (5/5)"
#-----------------------------------------------------------------------------------------------------------------------
n107_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx268_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n108_statement_end_α
.Lx268_0:               .quad            .Lx268_0_s
.Lx268_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n109_goto_α:                                                        jmp   n30_statement_begin_α
n109_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n110_goto_α:                                                        jmp   n42_statement_begin_α
n110_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n111_goto_α:                                                        jmp   n74_statement_begin_α
n111_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n112_goto_α:                                                        jmp   n89_statement_begin_α
n112_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n113_goto_α:                                                        jmp   n103_statement_begin_α
n113_goto_β:                                                        jmp   main_ω
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

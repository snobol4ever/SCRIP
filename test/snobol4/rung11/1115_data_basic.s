                        .intel_syntax    noprefix
                        .text
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lclassspec0:           .string          "NODE(VAL,LSON,RSON)"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lclassspec0]
                        call             record_register@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "A"
.Lgvan1:                .string          "B"
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
                        call             proc_startup
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
#         DATA('NODE(VAL,LSON,RSON)')
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_string_α
n2_statement_begin_β:                                               jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 19
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_call_α
.Lx125_0:               .quad            .Lx125_0_s
.Lx125_0_s:             .string          "NODE(VAL,LSON,RSON)"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd127:            .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd127]
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
                        cmp              eax, 104;                  jne   .Lx126_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx126_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n5_statement_end_α
n4_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     add              rsp, 32;                   jmp   n6_statement_begin_α
#=======================================================================================================================
#         DATA('NODE(VAL,LSON,RSON)')
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:                                               jmp   n7_statement_end_α
n6_statement_begin_β:                                               jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:                                                 jmp   n8_statement_begin_α
#=======================================================================================================================
#         A = NODE('x', 'y', 'z')
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:                                               jmp   n9_lit_string_α
n8_statement_begin_β:                                               jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n10_lit_string_α
.Lx136_0:               .quad            .Lx136_0_s
.Lx136_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n11_lit_string_α
n10_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n8_statement_begin_β
.Lx137_0:               .quad            .Lx137_0_s
.Lx137_0_s:             .string          "y"
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n12_call_α
n11_lit_string_β:       add              rsp, 16;                   jmp   n10_lit_string_β
.Lx138_0:               .quad            .Lx138_0_s
.Lx138_0_s:             .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n12_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd140:            .string          "NODE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd140]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
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
                        add              rsp, 48
                        cmp              eax, 104;                  jne   .Lx139_240
                        add              rsp, 16;                   jmp   n11_lit_string_β
.Lx139_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n13_assign_α
n12_call_β:             add              rsp, 16;                   jmp   n11_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # A
                        mov              qword ptr [r9 + 8], rdx;   jmp   n14_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:    add              rsp, 64;                   jmp   n15_statement_begin_α
#=======================================================================================================================
#         A = NODE('x', 'y', 'z')
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                              jmp   n16_statement_end_α
n15_statement_begin_β:                                              jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:                                                jmp   n17_statement_begin_α
#=======================================================================================================================
#         DIFFER(REPLACE(DATATYPE(A),&LCASE,&UCASE), 'NODE')   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:                                              jmp   n18_var_α
n17_statement_begin_β:                                              jmp   n30_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # A
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n19_call_α
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd152:            .string          "DATATYPE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd152]
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
                        cmp              eax, 104;                  jne   .Lx151_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n17_statement_begin_β
.Lx151_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n20_keyword_snobol4_α
n19_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n17_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n20_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx153_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n21_keyword_snobol4_α
.Lx153_0:               .quad            .Lx153_0_s
.Lx153_0_s:             .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n21_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx154_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n22_call_α
.Lx154_0:               .quad            .Lx154_0_s
.Lx154_0_s:             .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd156:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd156]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
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
                        add              rsp, 48
                        cmp              eax, 104;                  jne   .Lx155_240
                        add              rsp, 16
                        add              rsp, 64;                   jmp   n17_statement_begin_β
.Lx155_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n23_lit_string_α
n22_call_β:             add              rsp, 16
                        add              rsp, 64;                   jmp   n17_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n24_call_α
n23_lit_string_β:       add              rsp, 16
                        add              rsp, 80;                   jmp   n17_statement_begin_β
.Lx157_0:               .quad            .Lx157_0_s
.Lx157_0_s:             .string          "NODE"
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
.Lrkfnzd159:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd159]
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
                        cmp              eax, 104;                  jne   .Lx158_240
                        add              rsp, 16;                   jmp   n23_lit_string_β
.Lx158_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n25_statement_end_α
n24_call_β:             add              rsp, 16;                   jmp   n23_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    add              rsp, 112;                  jmp   n26_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1115/001: datatype of node'            :(END)
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                              jmp   n27_lit_string_α
n26_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx164_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n28_assign_α
.Lx164_0:               .quad            .Lx164_0_s
.Lx164_0_s:             .string          "FAIL 1115/001: datatype of node"
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx165_0]
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
.Lx165_0:               .quad            .Lx165_0_s
.Lx165_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:                                              jmp   n31_statement_end_α
n30_statement_begin_β:                                              jmp   n32_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:                                                jmp   n32_statement_begin_α
#=======================================================================================================================
#         DIFFER(VAL(A), 'x')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:                                              jmp   n33_var_α
n32_statement_begin_β:                                              jmp   n42_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # A
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
.Lrkfnzd176:            .string          "VAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd176]
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
                        cmp              eax, 104;                  jne   .Lx175_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n32_statement_begin_β
.Lx175_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n35_lit_string_α
n34_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n32_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx177_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n36_call_α
n35_lit_string_β:       add              rsp, 16
                        add              rsp, 32;                   jmp   n32_statement_begin_β
.Lx177_0:               .quad            .Lx177_0_s
.Lx177_0_s:             .string          "x"
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
.Lrkfnzd179:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd179]
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
                        cmp              eax, 104;                  jne   .Lx178_240
                        add              rsp, 16;                   jmp   n35_lit_string_β
.Lx178_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n37_statement_end_α
n36_call_β:             add              rsp, 16;                   jmp   n35_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:    add              rsp, 64;                   jmp   n38_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1115/002: field accessor val'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_begin_α:                                              jmp   n39_lit_string_α
n38_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n40_assign_α
.Lx184_0:               .quad            .Lx184_0_s
.Lx184_0_s:             .string          "FAIL 1115/002: field accessor val"
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx185_0]
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
.Lx185_0:               .quad            .Lx185_0_s
.Lx185_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:                                              jmp   n43_statement_end_α
n42_statement_begin_β:                                              jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:                                                jmp   n44_statement_begin_α
#=======================================================================================================================
#         B = NODE()
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:                                              jmp   n45_call_α
n44_statement_begin_β:                                              jmp   n48_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_call_α:             sub              rsp, 16
                        .section         .rodata
.Lrkfnzd195:            .string          "NODE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd195]
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
                        cmp              eax, 104;                  jne   .Lx194_240
                        add              rsp, 16;                   jmp   n44_statement_begin_β
.Lx194_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n46_assign_α
n45_call_β:             add              rsp, 16;                   jmp   n44_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n46_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # B
                        mov              qword ptr [r9 + 24], rdx;  jmp   n47_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:    add              rsp, 16;                   jmp   n48_statement_begin_α
#=======================================================================================================================
#         DIFFER(RSON(B))            :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:                                              jmp   n49_var_α
n48_statement_begin_β:                                              jmp   n57_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # B
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n50_call_α
#-----------------------------------------------------------------------------------------------------------------------
n50_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd203:            .string          "RSON"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd203]
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
                        cmp              eax, 104;                  jne   .Lx202_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n48_statement_begin_β
.Lx202_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n51_call_α
n50_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n48_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n51_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd205:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd205]
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
                        cmp              eax, 104;                  jne   .Lx204_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n48_statement_begin_β
.Lx204_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n52_statement_end_α
n51_call_β:             add              rsp, 16
                        add              rsp, 32;                   jmp   n48_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    add              rsp, 48;                   jmp   n53_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1115/003: unset field is null'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:                                              jmp   n54_lit_string_α
n53_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx210_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n55_assign_α
.Lx210_0:               .quad            .Lx210_0_s
.Lx210_0_s:             .string          "FAIL 1115/003: unset field is null"
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx211_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n56_statement_end_α
.Lx211_0:               .quad            .Lx211_0_s
.Lx211_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                              jmp   n58_statement_end_α
n57_statement_begin_β:                                              jmp   n59_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:                                                jmp   n59_statement_begin_α
#=======================================================================================================================
#         LSON(B) = A
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:                                              jmp   n60_var_α
n59_statement_begin_β:                                              jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # B
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n61_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n61_field_var_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx221_0]
                        mov              rsi, qword ptr [rsp + 16]                      # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_field_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx221_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n59_statement_begin_β
.Lx221_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n62_var_α
n61_field_var_β:        add              rsp, 16
                        add              rsp, 16;                   jmp   n59_statement_begin_β
.Lx221_0:               .quad            .Lx221_0_s
.Lx221_0_s:             .string          "LSON"
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # A
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n63_assign_var_α
n62_var_β:              add              rsp, 16;                   jmp   n61_field_var_β
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # field_var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
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
                        cmp              eax, 104;                  jne   .Lx223_240
                        add              rsp, 16;                   jmp   n62_var_β
.Lx223_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n64_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    add              rsp, 64;                   jmp   n65_statement_begin_α
#=======================================================================================================================
#         DIFFER(RSON(LSON(B)), 'z')                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:                                              jmp   n66_var_α
n65_statement_begin_β:                                              jmp   n76_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # B
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n67_call_α
#-----------------------------------------------------------------------------------------------------------------------
n67_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd230:            .string          "LSON"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd230]
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
                        cmp              eax, 104;                  jne   .Lx229_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n65_statement_begin_β
.Lx229_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n68_call_α
n67_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n65_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd232:            .string          "RSON"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd232]
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
                        cmp              eax, 104;                  jne   .Lx231_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n65_statement_begin_β
.Lx231_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n69_lit_string_α
n68_call_β:             add              rsp, 16
                        add              rsp, 32;                   jmp   n65_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n70_call_α
n69_lit_string_β:       add              rsp, 16
                        add              rsp, 48;                   jmp   n65_statement_begin_β
.Lx233_0:               .quad            .Lx233_0_s
.Lx233_0_s:             .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n70_call_α:             sub              rsp, 16
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
                        add              rsp, 16;                   jmp   n69_lit_string_β
.Lx234_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n71_statement_end_α
n70_call_β:             add              rsp, 16;                   jmp   n69_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:    add              rsp, 80;                   jmp   n72_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1115/004: nested accessor after mutate' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:                                              jmp   n73_lit_string_α
n72_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n74_assign_α
.Lx240_0:               .quad            .Lx240_0_s
.Lx240_0_s:             .string          "FAIL 1115/004: nested accessor after mutate"
#-----------------------------------------------------------------------------------------------------------------------
n74_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n75_statement_end_α
.Lx241_0:               .quad            .Lx241_0_s
.Lx241_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_begin_α:                                              jmp   n77_statement_end_α
n76_statement_begin_β:                                              jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:                                                jmp   n78_statement_begin_α
#=======================================================================================================================
#         DIFFER($'B', B)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:                                              jmp   n79_lit_string_α
n78_statement_begin_β:                                              jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx250_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n80_call_α
.Lx250_0:               .quad            .Lx250_0_s
.Lx250_0_s:             .string          "B"
#-----------------------------------------------------------------------------------------------------------------------
n80_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd252:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd252]
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
                        cmp              eax, 104;                  jne   .Lx251_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n78_statement_begin_β
.Lx251_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n81_deref_α
n80_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n78_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n81_deref_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # call
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
                        cmp              eax, 104;                  jne   .Lx253_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n78_statement_begin_β
.Lx253_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n82_var_α
n81_deref_β:            add              rsp, 16
                        add              rsp, 32;                   jmp   n78_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n82_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # B
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n83_call_α
n82_var_β:              add              rsp, 16;                   jmp   n81_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n83_call_α:             sub              rsp, 16
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
.Lrkfnzd256:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd256]
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
                        cmp              eax, 104;                  jne   .Lx255_240
                        add              rsp, 16;                   jmp   n82_var_β
.Lx255_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n84_statement_end_α
n83_call_β:             add              rsp, 16;                   jmp   n82_var_β
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_end_α:    add              rsp, 80;                   jmp   n85_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1115/005: value() by variable name' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_begin_α:                                              jmp   n86_lit_string_α
n85_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx261_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n87_assign_α
.Lx261_0:               .quad            .Lx261_0_s
.Lx261_0_s:             .string          "FAIL 1115/005: value() by variable name"
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx262_0]
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
.Lx262_0:               .quad            .Lx262_0_s
.Lx262_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:                                              jmp   n90_statement_end_α
n89_statement_begin_β:                                              jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:                                                jmp   n91_statement_begin_α
#=======================================================================================================================
#         VAL(A) = 'new'
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:                                              jmp   n92_var_α
n91_statement_begin_β:                                              jmp   n97_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # A
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n93_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n93_field_var_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx272_0]
                        mov              rsi, qword ptr [rsp + 16]                      # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_field_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx272_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n91_statement_begin_β
.Lx272_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n94_lit_string_α
n93_field_var_β:        add              rsp, 16
                        add              rsp, 16;                   jmp   n91_statement_begin_β
.Lx272_0:               .quad            .Lx272_0_s
.Lx272_0_s:             .string          "VAL"
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx273_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n95_assign_var_α
n94_lit_string_β:       add              rsp, 16;                   jmp   n93_field_var_β
.Lx273_0:               .quad            .Lx273_0_s
.Lx273_0_s:             .string          "new"
#-----------------------------------------------------------------------------------------------------------------------
n95_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # field_var
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
                        cmp              eax, 104;                  jne   .Lx274_240
                        add              rsp, 16;                   jmp   n94_lit_string_β
.Lx274_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n96_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_end_α:    add              rsp, 64;                   jmp   n97_statement_begin_α
#=======================================================================================================================
#         DIFFER(VAL(A), 'new')                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_begin_α:                                              jmp   n98_var_α
n97_statement_begin_β:                                              jmp   n107_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n98_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # A
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n99_call_α
#-----------------------------------------------------------------------------------------------------------------------
n99_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd281:            .string          "VAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd281]
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
                        cmp              eax, 104;                  jne   .Lx280_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n97_statement_begin_β
.Lx280_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n100_lit_string_α
n99_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n97_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx282_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n101_call_α
n100_lit_string_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n97_statement_begin_β
.Lx282_0:               .quad            .Lx282_0_s
.Lx282_0_s:             .string          "new"
#-----------------------------------------------------------------------------------------------------------------------
n101_call_α:            sub              rsp, 16
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
.Lrkfnzd284:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd284]
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
                        cmp              eax, 104;                  jne   .Lx283_240
                        add              rsp, 16;                   jmp   n100_lit_string_β
.Lx283_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n102_statement_end_α
n101_call_β:            add              rsp, 16;                   jmp   n100_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   add              rsp, 64;                   jmp   n103_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1115/006: mutate field and read back' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                             jmp   n104_lit_string_α
n103_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 41
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n105_assign_α
.Lx289_0:               .quad            .Lx289_0_s
.Lx289_0_s:             .string          "FAIL 1115/006: mutate field and read back"
#-----------------------------------------------------------------------------------------------------------------------
n105_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx290_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n106_statement_end_α
.Lx290_0:               .quad            .Lx290_0_s
.Lx290_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_begin_α:                                             jmp   n108_statement_end_α
n107_statement_begin_β:                                             jmp   n109_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:                                               jmp   n109_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1115_data_basic (6/6)'
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_begin_α:                                             jmp   n110_lit_string_α
n109_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx299_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n111_assign_α
.Lx299_0:               .quad            .Lx299_0_s
.Lx299_0_s:             .string          "PASS 1115_data_basic (6/6)"
#-----------------------------------------------------------------------------------------------------------------------
n111_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx300_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n112_statement_end_α
.Lx300_0:               .quad            .Lx300_0_s
.Lx300_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_end_α:   add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n113_goto_α:                                                        jmp   n30_statement_begin_α
n113_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n114_goto_α:                                                        jmp   n42_statement_begin_α
n114_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n115_goto_α:                                                        jmp   n57_statement_begin_α
n115_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n116_goto_α:                                                        jmp   n76_statement_begin_α
n116_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n117_goto_α:                                                        jmp   n89_statement_begin_α
n117_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n118_goto_α:                                                        jmp   n107_statement_begin_α
n118_goto_β:                                                        jmp   main_ω
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

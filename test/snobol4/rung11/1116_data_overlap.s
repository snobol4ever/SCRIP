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
                        .section         .rodata
.Lclassspec1:           .string          "CLUNK(VALUE,LSON)"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lclassspec1]
                        call             record_register@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "A"
.Lgvan1:                .string          "B"
.Lgvan2:                .string          "C"
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
                        call             proc_startup
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
#         DATA('NODE(VAL,LSON,RSON)')
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                               jmp   n3_lit_string_α
n2_statement_begin_β:                                               jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 19
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_call_α
.Lx86_0:                .quad            .Lx86_0_s
.Lx86_0_s:              .string          "NODE(VAL,LSON,RSON)"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd88:             .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd88]
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
                        cmp              eax, 104;                  jne   .Lx87_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
.Lx87_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n5_statement_end_α
n4_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     add              rsp, 32;                   jmp   n6_statement_begin_α
#=======================================================================================================================
#         DATA('CLUNK(VALUE,LSON)')
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:                                               jmp   n7_lit_string_α
n6_statement_begin_β:                                               jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx93_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n8_call_α
.Lx93_0:                .quad            .Lx93_0_s
.Lx93_0_s:              .string          "CLUNK(VALUE,LSON)"
#-----------------------------------------------------------------------------------------------------------------------
n8_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd95:             .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd95]
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
                        cmp              eax, 104;                  jne   .Lx94_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n6_statement_begin_β
.Lx94_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n9_statement_end_α
n8_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n6_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     add              rsp, 32;                   jmp   n10_statement_begin_α
#=======================================================================================================================
#         DATA('CLUNK(VALUE,LSON)')
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:                                              jmp   n11_statement_end_α
n10_statement_begin_β:                                              jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:                                                jmp   n12_statement_begin_α
#=======================================================================================================================
#         A = NODE('x', 'y', 'z')
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:                                              jmp   n13_lit_string_α
n12_statement_begin_β:                                              jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx104_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n14_lit_string_α
.Lx104_0:               .quad            .Lx104_0_s
.Lx104_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx105_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n15_lit_string_α
n14_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n12_statement_begin_β
.Lx105_0:               .quad            .Lx105_0_s
.Lx105_0_s:             .string          "y"
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx106_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n16_call_α
n15_lit_string_β:       add              rsp, 16;                   jmp   n14_lit_string_β
.Lx106_0:               .quad            .Lx106_0_s
.Lx106_0_s:             .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n16_call_α:             sub              rsp, 16
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
.Lrkfnzd108:            .string          "NODE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd108]
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
                        cmp              eax, 104;                  jne   .Lx107_240
                        add              rsp, 16;                   jmp   n15_lit_string_β
.Lx107_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n17_assign_α
n16_call_β:             add              rsp, 16;                   jmp   n15_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # A
                        mov              qword ptr [r9 + 8], rdx;   jmp   n18_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:    add              rsp, 64;                   jmp   n19_statement_begin_α
#=======================================================================================================================
#         B = NODE()
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_begin_α:                                              jmp   n20_call_α
n19_statement_begin_β:                                              jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:             sub              rsp, 16
                        .section         .rodata
.Lrkfnzd115:            .string          "NODE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd115]
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
                        cmp              eax, 104;                  jne   .Lx114_240
                        add              rsp, 16;                   jmp   n19_statement_begin_β
.Lx114_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n21_assign_α
n20_call_β:             add              rsp, 16;                   jmp   n19_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # B
                        mov              qword ptr [r9 + 24], rdx;  jmp   n22_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:    add              rsp, 16;                   jmp   n23_statement_begin_α
#=======================================================================================================================
#         LSON(B) = A
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                              jmp   n24_var_α
n23_statement_begin_β:                                              jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # B
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n25_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n25_field_var_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx122_0]
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
                        cmp              eax, 104;                  jne   .Lx122_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n23_statement_begin_β
.Lx122_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n26_var_α
n25_field_var_β:        add              rsp, 16
                        add              rsp, 16;                   jmp   n23_statement_begin_β
.Lx122_0:               .quad            .Lx122_0_s
.Lx122_0_s:             .string          "LSON"
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # A
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n27_assign_var_α
n26_var_β:              add              rsp, 16;                   jmp   n25_field_var_β
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_var_α:       sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx124_240
                        add              rsp, 16;                   jmp   n26_var_β
.Lx124_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    add              rsp, 64;                   jmp   n29_statement_begin_α
#=======================================================================================================================
#         DIFFER(RSON(LSON(B)), 'z')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                              jmp   n30_var_α
n29_statement_begin_β:                                              jmp   n40_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # B
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n31_call_α
#-----------------------------------------------------------------------------------------------------------------------
n31_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd131:            .string          "LSON"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd131]
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
                        cmp              eax, 104;                  jne   .Lx130_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n29_statement_begin_β
.Lx130_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n32_call_α
n31_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n29_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n32_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd133:            .string          "RSON"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd133]
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
                        cmp              eax, 104;                  jne   .Lx132_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n29_statement_begin_β
.Lx132_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n33_lit_string_α
n32_call_β:             add              rsp, 16
                        add              rsp, 32;                   jmp   n29_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n34_call_α
n33_lit_string_β:       add              rsp, 16
                        add              rsp, 48;                   jmp   n29_statement_begin_β
.Lx134_0:               .quad            .Lx134_0_s
.Lx134_0_s:             .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n34_call_α:             sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx135_240
                        add              rsp, 16;                   jmp   n33_lit_string_β
.Lx135_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n35_statement_end_α
n34_call_β:             add              rsp, 16;                   jmp   n33_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:    add              rsp, 80;                   jmp   n36_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1116/001: node.rson after clunk data def' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:                                              jmp   n37_lit_string_α
n36_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n38_assign_α
.Lx141_0:               .quad            .Lx141_0_s
.Lx141_0_s:             .string          "FAIL 1116/001: node.rson after clunk data def"
#-----------------------------------------------------------------------------------------------------------------------
n38_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx142_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n39_statement_end_α
.Lx142_0:               .quad            .Lx142_0_s
.Lx142_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_begin_α:                                              jmp   n41_statement_end_α
n40_statement_begin_β:                                              jmp   n42_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_end_α:                                                jmp   n42_statement_begin_α
#=======================================================================================================================
#         DIFFER($'B', B)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:                                              jmp   n43_lit_string_α
n42_statement_begin_β:                                              jmp   n53_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx151_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n44_call_α
.Lx151_0:               .quad            .Lx151_0_s
.Lx151_0_s:             .string          "B"
#-----------------------------------------------------------------------------------------------------------------------
n44_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd153:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd153]
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
                        cmp              eax, 104;                  jne   .Lx152_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n42_statement_begin_β
.Lx152_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n45_deref_α
n44_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n42_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n45_deref_α:            sub              rsp, 16
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
                        cmp              eax, 104;                  jne   .Lx154_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n42_statement_begin_β
.Lx154_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n46_var_α
n45_deref_β:            add              rsp, 16
                        add              rsp, 32;                   jmp   n42_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n46_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # B
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n47_call_α
n46_var_β:              add              rsp, 16;                   jmp   n45_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:             sub              rsp, 16
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
.Lrkfnzd157:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd157]
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
                        cmp              eax, 104;                  jne   .Lx156_240
                        add              rsp, 16;                   jmp   n46_var_β
.Lx156_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n48_statement_end_α
n47_call_β:             add              rsp, 16;                   jmp   n46_var_β
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    add              rsp, 80;                   jmp   n49_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1116/002: value() still works after clunk' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:                                              jmp   n50_lit_string_α
n49_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 46
                        mov              rax, qword ptr [rip + .Lx162_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n51_assign_α
.Lx162_0:               .quad            .Lx162_0_s
.Lx162_0_s:             .string          "FAIL 1116/002: value() still works after clunk"
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx163_0]
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
.Lx163_0:               .quad            .Lx163_0_s
.Lx163_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:                                              jmp   n54_statement_end_α
n53_statement_begin_β:                                              jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:                                                jmp   n55_statement_begin_α
#=======================================================================================================================
#         C = CLUNK('alpha', 'beta')
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:                                              jmp   n56_lit_string_α
n55_statement_begin_β:                                              jmp   n61_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n57_lit_string_α
.Lx172_0:               .quad            .Lx172_0_s
.Lx172_0_s:             .string          "alpha"
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx173_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n58_call_α
n57_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n55_statement_begin_β
.Lx173_0:               .quad            .Lx173_0_s
.Lx173_0_s:             .string          "beta"
#-----------------------------------------------------------------------------------------------------------------------
n58_call_α:             sub              rsp, 16
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
.Lrkfnzd175:            .string          "CLUNK"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd175]
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
                        cmp              eax, 104;                  jne   .Lx174_240
                        add              rsp, 16;                   jmp   n57_lit_string_β
.Lx174_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n59_assign_α
n58_call_β:             add              rsp, 16;                   jmp   n57_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # C
                        mov              qword ptr [r9 + 40], rdx;  jmp   n60_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 48;                   jmp   n61_statement_begin_α
#=======================================================================================================================
#         DIFFER(LSON(C), 'beta')                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:                                              jmp   n62_var_α
n61_statement_begin_β:                                              jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # C
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n63_call_α
#-----------------------------------------------------------------------------------------------------------------------
n63_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd183:            .string          "LSON"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd183]
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
                        cmp              eax, 104;                  jne   .Lx182_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n61_statement_begin_β
.Lx182_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n64_lit_string_α
n63_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n61_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n65_call_α
n64_lit_string_β:       add              rsp, 16
                        add              rsp, 32;                   jmp   n61_statement_begin_β
.Lx184_0:               .quad            .Lx184_0_s
.Lx184_0_s:             .string          "beta"
#-----------------------------------------------------------------------------------------------------------------------
n65_call_α:             sub              rsp, 16
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
.Lrkfnzd186:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd186]
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
                        cmp              eax, 104;                  jne   .Lx185_240
                        add              rsp, 16;                   jmp   n64_lit_string_β
.Lx185_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n66_statement_end_α
n65_call_β:             add              rsp, 16;                   jmp   n64_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:    add              rsp, 64;                   jmp   n67_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1116/003: clunk.lson accessor'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:                                              jmp   n68_lit_string_α
n67_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n69_assign_α
.Lx191_0:               .quad            .Lx191_0_s
.Lx191_0_s:             .string          "FAIL 1116/003: clunk.lson accessor"
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx192_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n70_statement_end_α
.Lx192_0:               .quad            .Lx192_0_s
.Lx192_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:                                              jmp   n72_statement_end_α
n71_statement_begin_β:                                              jmp   n73_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_end_α:                                                jmp   n73_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1116_data_overlap (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_begin_α:                                              jmp   n74_lit_string_α
n73_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n75_assign_α
.Lx201_0:               .quad            .Lx201_0_s
.Lx201_0_s:             .string          "PASS 1116_data_overlap (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n75_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx202_0]
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
.Lx202_0:               .quad            .Lx202_0_s
.Lx202_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n77_goto_α:                                                         jmp   n40_statement_begin_α
n77_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n78_goto_α:                                                         jmp   n53_statement_begin_α
n78_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n79_goto_α:                                                         jmp   n71_statement_begin_α
n79_goto_β:                                                         jmp   main_ω
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

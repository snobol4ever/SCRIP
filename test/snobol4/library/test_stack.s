                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_α:
proc_LBL__stack_init_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_lit_string_α
n0_statement_begin_β:
                                                                                        jmp   n4_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_assign_α
.Lx7_0:
                        .quad            .Lx7_0_s
.Lx7_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n3_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:
                                                                                        jmp   n4_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n4_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_β:
                                                                                        jmp   proc_LBL__stack_init_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_α:
proc_LBL__stack_push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:
                                                                                        jmp   n14_var_α
n13_statement_begin_β:
                                                                                        jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n15_var_α
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n16_call_α
n15_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n13_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n16_call_α:
                        sub              rsp, 16
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
.Lrkfnzd33:             .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd33]
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
                        cmp              eax, 104
                                                                                        jne   .Lx32_240
                        add              rsp, 16
                                                                                        jmp   n15_var_β
.Lx32_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n17_assign_α
n16_call_β:
                        add              rsp, 16
                                                                                        jmp   n15_var_β
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n18_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n19_statement_begin_α
#=======================================================================================================================
#         stack_push(w1)
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_begin_α:
                                                                                        jmp   n20_var_α
n19_statement_begin_β:
                                                                                        jmp   n24_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_field_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx40_0]
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
                        cmp              eax, 104
                                                                                        jne   .Lx40_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n19_statement_begin_β
.Lx40_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n22_assign_α
.Lx40_0:
                        .quad            .Lx40_0_s
.Lx40_0_s:
                        .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              qword ptr [r9 + 24], rdx
                                                                                        jmp   n23_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:
                                                                                        jmp   n24_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n25_call_α
.Lx44_0:
                        .quad            .Lx44_0_s
.Lx44_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd46:             .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd46]
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
                        cmp              eax, 104
                                                                                        jne   .Lx45_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n27_save_restore_α
.Lx45_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n26_save_restore_α
n25_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n27_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n26_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_β:
                                                                                        jmp   proc_LBL__stack_push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_α:
proc_LBL__stack_pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:
                                                                                        jmp   n52_var_α
n51_statement_begin_β:
                                                                                        jmp   n89_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_call_α
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd94:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd94]
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
                        cmp              eax, 104
                                                                                        jne   .Lx93_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n51_statement_begin_β
.Lx93_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n54_statement_end_α
n53_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n51_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:
                                                                                        jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:
                                                                                        jmp   n56_var_α
n55_statement_begin_β:
                        add              rsp, 32
                                                                                        jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n57_call_α
n56_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n55_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n57_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd101:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd101]
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
                        cmp              eax, 104
                                                                                        jne   .Lx100_240
                        add              rsp, 16
                                                                                        jmp   n56_var_β
.Lx100_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n58_statement_end_α
n57_call_β:
                        add              rsp, 16
                                                                                        jmp   n56_var_β
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:
                                                                                        jmp   n59_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:
                                                                                        jmp   n60_var_α
n59_statement_begin_β:
                                                                                        jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n61_call_α
n60_var_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n59_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n61_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd108:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd108]
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
                        cmp              eax, 104
                                                                                        jne   .Lx107_240
                        add              rsp, 16
                                                                                        jmp   n60_var_β
.Lx107_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n62_assign_α
n61_call_β:
                        add              rsp, 16
                                                                                        jmp   n60_var_β
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n63_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:
                                                                                        jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:
                                                                                        jmp   n65_var_α
n64_statement_begin_β:
                                                                                        jmp   n88_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n66_call_α
n65_var_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n64_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n66_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd116:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd116]
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
                        cmp              eax, 104
                                                                                        jne   .Lx115_240
                        add              rsp, 16
                                                                                        jmp   n65_var_β
.Lx115_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_assign_α
n66_call_β:
                        add              rsp, 16
                                                                                        jmp   n65_var_β
#-----------------------------------------------------------------------------------------------------------------------
n67_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n68_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:
                                                                                        jmp   n88_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_begin_α:
                                                                                        jmp   n70_var_α
n69_statement_begin_β:
                                                                                        jmp   n76_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n70_var_α:
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 672], rax                     # result
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n71_call_α
#-----------------------------------------------------------------------------------------------------------------------
n71_call_α:
                        mov              rax, qword ptr [rsp + 672]
                        mov              qword ptr [rsp + 640], rax
                        mov              rax, qword ptr [rsp + 680]
                        mov              qword ptr [rsp + 648], rax
                        .section         .rodata
.Lrkfn124:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn124]
                        lea              rsi, [rsp + 640]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        cmp              eax, 104
                                                                                        je    n69_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n72_var_α
n71_call_β:
                                                                                        jmp   n69_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n72_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 736], rax                     # result
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n73_call_α
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:
                        mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 744]
                        mov              qword ptr [rsp + 712], rax
                        .section         .rodata
.Lrkfn127:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn127]
                        lea              rsi, [rsp + 704]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                        cmp              eax, 104
                                                                                        je    n69_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n74_assign_var_α
n73_call_β:
                                                                                        jmp   n69_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n74_assign_var_α:
                        mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
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
                        cmp              eax, 104
                                                                                        je    n69_statement_begin_β
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n75_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:
                                                                                        jmp   n76_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_begin_α:
                                                                                        jmp   n77_var_α
n76_statement_begin_β:
                                                                                        jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n77_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 816], rax                     # result
                        mov              qword ptr [rsp + 824], rdx
                                                                                        jmp   n78_call_α
#-----------------------------------------------------------------------------------------------------------------------
n78_call_α:
                        mov              rax, qword ptr [rsp + 816]
                        mov              qword ptr [rsp + 784], rax
                        mov              rax, qword ptr [rsp + 824]
                        mov              qword ptr [rsp + 792], rax
                        .section         .rodata
.Lrkfn135:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn135]
                        lea              rsi, [rsp + 784]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 104
                                                                                        je    n76_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n79_assign_α
n78_call_β:
                                                                                        jmp   n76_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              rdx, qword ptr [rsp + 776]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n80_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:
                                                                                        jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:
                                                                                        jmp   n82_lit_string_α
n81_statement_begin_β:
                                                                                        jmp   n86_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:
                        mov              qword ptr [rsp + 880], 2                       # result
                        mov              dword ptr [rsp + 884], 5
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 888], rax
                                                                                        jmp   n83_call_α
.Lx141_0:
                        .quad            .Lx141_0_s
.Lx141_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n83_call_α:
                        mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 848], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 856], rax
                        .section         .rodata
.Lrkfn143:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn143]
                        lea              rsi, [rsp + 848]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                        cmp              eax, 104
                                                                                        je    n81_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n84_assign_α
n83_call_β:
                                                                                        jmp   n81_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              rdx, qword ptr [rsp + 840]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n85_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:
                                                                                        jmp   n86_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:
                        mov              qword ptr [rsp + 32], 2                        # result
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx147_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n87_call_α
.Lx147_0:
                        .quad            .Lx147_0_s
.Lx147_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n87_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn149:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn149]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 104
                                                                                        je    n89_save_restore_α
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n88_save_restore_α
n87_call_β:
                                                                                        jmp   n89_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n88_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
n89_save_restore_α:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_β:
                                                                                        jmp   proc_LBL__stack_pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_α:
proc_LBL__stack_peek_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_begin_α:
                                                                                        jmp   n155_var_α
n154_statement_begin_β:
                                                                                        jmp   n164_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n155_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n156_call_α
#-----------------------------------------------------------------------------------------------------------------------
n156_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd169:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd169]
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
                        cmp              eax, 104
                                                                                        jne   .Lx168_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n154_statement_begin_β
.Lx168_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n157_statement_end_α
n156_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n154_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_end_α:
                                                                                        jmp   n158_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_statement_begin_α:
                                                                                        jmp   n159_var_α
n158_statement_begin_β:
                                                                                        jmp   n163_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n159_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n160_call_α
n159_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n158_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n160_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd176:            .string          "sval"
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
                        cmp              eax, 104
                                                                                        jne   .Lx175_240
                        add              rsp, 16
                                                                                        jmp   n159_var_β
.Lx175_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n161_assign_α
n160_call_β:
                        add              rsp, 16
                                                                                        jmp   n159_var_β
#-----------------------------------------------------------------------------------------------------------------------
n161_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax                       # stack_peek
                        mov              qword ptr [r9 + 88], rdx
                                                                                        jmp   n162_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_end_α:
                                                                                        jmp   n163_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n163_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
n164_save_restore_α:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_β:
                                                                                        jmp   proc_LBL__stack_peek_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_α:
proc_LBL__stack_top_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_begin_α:
                                                                                        jmp   n185_var_α
n184_statement_begin_β:
                                                                                        jmp   n196_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n185_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n186_call_α
#-----------------------------------------------------------------------------------------------------------------------
n186_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd201:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd201]
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
                        cmp              eax, 104
                                                                                        jne   .Lx200_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n184_statement_begin_β
.Lx200_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n187_statement_end_α
n186_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n184_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_end_α:
                                                                                        jmp   n188_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_begin_α:
                                                                                        jmp   n189_var_α
n188_statement_begin_β:
                                                                                        jmp   n193_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n189_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n190_field_var_α
n189_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n188_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n190_field_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx207_0]
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
                        cmp              eax, 104
                                                                                        jne   .Lx207_240
                        add              rsp, 16
                                                                                        jmp   n189_var_β
.Lx207_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n191_assign_α
.Lx207_0:
                        .quad            .Lx207_0_s
.Lx207_0_s:
                        .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n191_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # stack_top
                        mov              qword ptr [r9 + 104], rdx
                                                                                        jmp   n192_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n192_statement_end_α:
                                                                                        jmp   n193_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n193_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx211_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n194_call_α
.Lx211_0:
                        .quad            .Lx211_0_s
.Lx211_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n194_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd213:            .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd213]
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
                        cmp              eax, 104
                                                                                        jne   .Lx212_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n196_save_restore_α
.Lx212_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n195_save_restore_α
n194_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n196_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n195_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
n196_save_restore_α:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_β:
                                                                                        jmp   proc_LBL__stack_top_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_α:
proc_LBL__stack_depth_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n218_statement_begin_α:
                                                                                        jmp   n219_lit_integer_α
n218_statement_begin_β:
                                                                                        jmp   n222_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n219_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx244_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n220_assign_α
.Lx244_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n220_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              qword ptr [r9 + 120], rdx
                                                                                        jmp   n221_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n221_statement_end_α:
                                                                                        jmp   n222_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n222_statement_begin_α:
                                                                                        jmp   n223_var_α
n222_statement_begin_β:
                                                                                        jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n223_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n224_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n224_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n225_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n225_statement_end_α:
                                                                                        jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_begin_α:
                                                                                        jmp   n227_var_α
n226_statement_begin_β:
                        add              rsp, 32
                                                                                        jmp   n241_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n227_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n228_call_α
n227_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n226_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n228_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd258:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd258]
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
                        cmp              eax, 104
                                                                                        jne   .Lx257_240
                        add              rsp, 16
                                                                                        jmp   n227_var_β
.Lx257_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n229_statement_end_α
n228_call_β:
                        add              rsp, 16
                                                                                        jmp   n227_var_β
#-----------------------------------------------------------------------------------------------------------------------
n229_statement_end_α:
                                                                                        jmp   n230_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_begin_α:
                                                                                        jmp   n231_var_α
n230_statement_begin_β:
                                                                                        jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n231_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n232_lit_integer_α
n231_var_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n230_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n232_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n233_binop_α
n232_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n231_var_β
.Lx264_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n233_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx265_240
                        add              rsp, 16
                                                                                        jmp   n232_lit_integer_β
.Lx265_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n234_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n234_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              qword ptr [r9 + 120], rdx
                                                                                        jmp   n235_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_end_α:
                                                                                        jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_begin_α:
                                                                                        jmp   n237_var_α
n236_statement_begin_β:
                                                                                        jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n237_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n238_call_α
n237_var_β:
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n236_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n238_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd273:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd273]
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
                        cmp              eax, 104
                                                                                        jne   .Lx272_240
                        add              rsp, 16
                                                                                        jmp   n237_var_β
.Lx272_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n239_assign_α
n238_call_β:
                        add              rsp, 16
                                                                                        jmp   n237_var_β
#-----------------------------------------------------------------------------------------------------------------------
n239_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n240_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_end_α:
                        add              rsp, 144
                                                                                        jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n241_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_β:
                                                                                        jmp   proc_LBL__stack_depth_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_init_α
proc_stack_init_α:
proc_stack_init_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n279_save_restore_α:
                                                                                        jmp   n280_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n280_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__stack_init_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_stack_init_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_β:
                                                                                        jmp   proc_stack_init_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_push_α
proc_stack_push_α:
proc_stack_push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n285_save_restore_α:
                                                                                        jmp   n286_goto_deferred_α
stack_push_shim:
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
                                                                                        jmp   n286_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n286_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__stack_push_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_stack_push_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_β:
                                                                                        jmp   proc_stack_push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_pop_α
proc_stack_pop_α:
proc_stack_pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n291_save_restore_α:
                                                                                        jmp   n292_goto_deferred_α
stack_pop_shim:
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
                                                                                        jmp   n292_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n292_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__stack_pop_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_stack_pop_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_β:
                                                                                        jmp   proc_stack_pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_peek_α
proc_stack_peek_α:
proc_stack_peek_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n297_save_restore_α:
                                                                                        jmp   n298_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n298_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__stack_peek_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_stack_peek_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_β:
                                                                                        jmp   proc_stack_peek_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_top_α
proc_stack_top_α:
proc_stack_top_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n303_save_restore_α:
                                                                                        jmp   n304_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n304_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__stack_top_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_stack_top_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_β:
                                                                                        jmp   proc_stack_top_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_ω:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_depth_α
proc_stack_depth_α:
proc_stack_depth_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n309_save_restore_α:
                                                                                        jmp   n310_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n310_goto_deferred_α:
                        lea              rax, [rip + proc_LBL__stack_depth_α]
                                                                                        jmp   rax
                                                                                        jmp   proc_stack_depth_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_β:
                                                                                        jmp   proc_stack_depth_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_γ:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_ω:
                                                                                        jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lclassspec0:           .string          "slink(snext,sval)"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lclassspec0]
                        call             record_register@PLT
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__stack_init"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__stack_init_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__stack_push"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__stack_push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__stack_pop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__stack_pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__stack_peek"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__stack_peek_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "LBL__stack_top"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_LBL__stack_top_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "LBL__stack_depth"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_LBL__stack_depth_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "stack_init"
.Lgvan1:                .string          "stack_push"
.Lgvan2:                .string          "x"
.Lgvan3:                .string          "stack_pop"
.Lgvan4:                .string          "var"
.Lgvan5:                .string          "stack_peek"
.Lgvan6:                .string          "stack_top"
.Lgvan7:                .string          "stack_depth"
.Lgvan8:                .string          "sd"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "myvar"
.Lgvan12:               .string          "subject"
.Lgvan13:               .string          "w1"
.Lgvan14:               .string          "w2"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .quad            .Lgvan7
                        .quad            .Lgvan8
                        .quad            .Lgvan9
                        .quad            .Lgvan10
                        .quad            .Lgvan11
                        .quad            .Lgvan12
                        .quad            .Lgvan13
                        .quad            .Lgvan14
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 15
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 15
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n315_statement_begin_α:
                                                                                        jmp   n316_statement_end_α
n315_statement_begin_β:
                                                                                        jmp   n317_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_end_α:
                                                                                        jmp   n317_statement_begin_α
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_begin_α:
                                                                                        jmp   n318_lit_string_α
n317_statement_begin_β:
                                                                                        jmp   n321_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n318_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx554_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n319_call_α
.Lx554_0:
                        .quad            .Lx554_0_s
.Lx554_0_s:
                        .string          "slink(snext,sval)"
#-----------------------------------------------------------------------------------------------------------------------
n319_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd556:            .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd556]
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
                        cmp              eax, 104
                                                                                        jne   .Lx555_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n317_statement_begin_β
.Lx555_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n320_statement_end_α
n319_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n317_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n321_statement_begin_α
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n321_statement_begin_α:
                                                                                        jmp   n322_statement_end_α
n321_statement_begin_β:
                                                                                        jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:
                                                                                        jmp   n323_statement_begin_α
#=======================================================================================================================
#         OUTPUT = myvar                  ;* 99
#-----------------------------------------------------------------------------------------------------------------------
n323_statement_begin_α:
                                                                                        jmp   n324_func_activate_α
n323_statement_begin_β:
                                                                                        jmp   n326_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx566_0]
                        mov              rsi, qword ptr [rip + .Lx566_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_init_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n325_statement_end_α
n324_func_activate_β:
                                                                                        jmp   n323_statement_begin_β
.Lx566_0:
                        .quad            .Lx566_0_s
.Lx566_0_s:
                        .string          "stack_init"
.Lx566_1:
                        .quad            .Lx566_1_s
.Lx566_1_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n325_statement_end_α:
                                                                                        jmp   n326_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n326_statement_begin_α:
                                                                                        jmp   n327_statement_end_α
n326_statement_begin_β:
                                                                                        jmp   n328_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n327_statement_end_α:
                                                                                        jmp   n328_statement_begin_α
#=======================================================================================================================
#         subject = 'hello world'
#-----------------------------------------------------------------------------------------------------------------------
n328_statement_begin_α:
                                                                                        jmp   n329_func_activate_α
n328_statement_begin_β:
                                                                                        jmp   n331_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n329_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx576_0]
                        mov              rsi, qword ptr [rip + .Lx576_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_push_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n330_statement_end_α
n329_func_activate_β:
                                                                                        jmp   n328_statement_begin_β
.Lx576_0:
                        .quad            .Lx576_0_s
.Lx576_0_s:
                        .string          "stack_push"
.Lx576_1:
                        .quad            .Lx576_1_s
.Lx576_1_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n330_statement_end_α:
                                                                                        jmp   n331_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n331_statement_begin_α:
                                                                                        jmp   n332_statement_end_α
n331_statement_begin_β:
                                                                                        jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n332_statement_end_α:
                                                                                        jmp   n333_statement_begin_α
#=======================================================================================================================
#         OUTPUT = stack_pop()            ;* hello
#-----------------------------------------------------------------------------------------------------------------------
n333_statement_begin_α:
                                                                                        jmp   n334_func_activate_α
n333_statement_begin_β:
                                                                                        jmp   n336_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n334_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx586_0]
                        mov              rsi, qword ptr [rip + .Lx586_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_pop_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n335_statement_end_α
n334_func_activate_β:
                                                                                        jmp   n333_statement_begin_β
.Lx586_0:
                        .quad            .Lx586_0_s
.Lx586_0_s:
                        .string          "stack_pop"
.Lx586_1:
                        .quad            .Lx586_1_s
.Lx586_1_s:
                        .string          "var"
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_end_α:
                                                                                        jmp   n336_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_statement_begin_α:
                                                                                        jmp   n337_statement_end_α
n336_statement_begin_β:
                                                                                        jmp   n338_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n337_statement_end_α:
                                                                                        jmp   n338_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n338_statement_begin_α:
                                                                                        jmp   n339_func_activate_α
n338_statement_begin_β:
                                                                                        jmp   n341_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n339_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx596_0]
                        mov              rsi, qword ptr [rip + .Lx596_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_peek_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n340_statement_end_α
n339_func_activate_β:
                                                                                        jmp   n338_statement_begin_β
.Lx596_0:
                        .quad            .Lx596_0_s
.Lx596_0_s:
                        .string          "stack_peek"
.Lx596_1:
                        .quad            .Lx596_1_s
.Lx596_1_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n340_statement_end_α:
                                                                                        jmp   n341_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n341_statement_begin_α:
                                                                                        jmp   n342_statement_end_α
n341_statement_begin_β:
                                                                                        jmp   n343_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n342_statement_end_α:
                                                                                        jmp   n343_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n343_statement_begin_α:
                                                                                        jmp   n344_func_activate_α
n343_statement_begin_β:
                                                                                        jmp   n346_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n344_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx606_0]
                        mov              rsi, qword ptr [rip + .Lx606_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_top_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n345_statement_end_α
n344_func_activate_β:
                                                                                        jmp   n343_statement_begin_β
.Lx606_0:
                        .quad            .Lx606_0_s
.Lx606_0_s:
                        .string          "stack_top"
.Lx606_1:
                        .quad            .Lx606_1_s
.Lx606_1_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n345_statement_end_α:
                                                                                        jmp   n346_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n346_statement_begin_α:
                                                                                        jmp   n347_statement_end_α
n346_statement_begin_β:
                                                                                        jmp   n348_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_end_α:
                                                                                        jmp   n348_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n348_statement_begin_α:
                                                                                        jmp   n349_func_activate_α
n348_statement_begin_β:
                                                                                        jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n349_func_activate_α:
                        mov              rdi, qword ptr [rip + .Lx616_0]
                        mov              rsi, qword ptr [rip + .Lx616_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_depth_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n350_statement_end_α
n349_func_activate_β:
                                                                                        jmp   n348_statement_begin_β
.Lx616_0:
                        .quad            .Lx616_0_s
.Lx616_0_s:
                        .string          "stack_depth"
.Lx616_1:
                        .quad            .Lx616_1_s
.Lx616_1_s:
                        .string          "sd"
#-----------------------------------------------------------------------------------------------------------------------
n350_statement_end_α:
                                                                                        jmp   n351_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n351_statement_begin_α:
                                                                                        jmp   n352_statement_end_α
n351_statement_begin_β:
                                                                                        jmp   n353_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n352_statement_end_α:
                                                                                        jmp   n353_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n353_statement_begin_α:
                                                                                        jmp   n354_statement_end_α
n353_statement_begin_β:
                                                                                        jmp   n355_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n354_statement_end_α:
                                                                                        jmp   n355_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n355_statement_begin_α:
                                                                                        jmp   n356_lit_string_α
n355_statement_begin_β:
                                                                                        jmp   n360_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n356_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx629_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n357_lit_integer_α
.Lx629_0:
                        .quad            .Lx629_0_s
.Lx629_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n357_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx630_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n358_call_α
n357_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n355_statement_begin_β
.Lx630_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n358_call_α:
                        sub              rsp, 16
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
.Lrkfnzd632:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd632]
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
                        cmp              eax, 104
                                                                                        jne   .Lx631_240
                        add              rsp, 16
                                                                                        jmp   n357_lit_integer_β
.Lx631_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n359_statement_end_α
n358_call_β:
                        add              rsp, 16
                                                                                        jmp   n357_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n359_statement_end_α:
                                                                                        jmp   n360_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n360_statement_begin_α:
                                                                                        jmp   n361_statement_end_α
n360_statement_begin_β:
                                                                                        jmp   n362_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n361_statement_end_α:
                                                                                        jmp   n362_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_begin_α:
                                                                                        jmp   n363_call_α
n362_statement_begin_β:
                                                                                        jmp   n365_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n363_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx642_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx642_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx642_6]
                        lea              r11, [rip + .Lx642_7]
                                                                                        jmp   rax
.Lx642_6:
                        mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx642_2
.Lx642_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx642_2
.Lx642_5:
                        add              rsp, 16
.Lx642_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx642_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n362_statement_begin_β
.Lx642_240:
                                                                                        jmp   n364_statement_end_α
n363_call_β:
                                                                                        jmp   n362_statement_begin_β
.Lx642_0:
                        .quad            .Lx642_0_s
.Lx642_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n364_statement_end_α:
                                                                                        jmp   n365_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n365_statement_begin_α:
                                                                                        jmp   n366_lit_string_α
n365_statement_begin_β:
                                                                                        jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n366_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx647_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n367_call_α
n366_lit_string_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n365_statement_begin_β
.Lx647_0:
                        .quad            .Lx647_0_s
.Lx647_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n367_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx649_6]
                        lea              r11, [rip + .Lx649_7]
                                                                                        jmp   stack_push_shim
.Lx649_6:
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              rdx, qword ptr [r9 + 24]
                                                                                        jmp   .Lx649_2
.Lx649_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx649_2
.Lx649_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx649_240
                        add              rsp, 16
                                                                                        jmp   n366_lit_string_β
.Lx649_240:
                                                                                        jmp   n368_statement_end_α
n367_call_β:
                                                                                        jmp   n366_lit_string_β
.Lx649_0:
                        .quad            .Lx649_0_s
.Lx649_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n368_statement_end_α:
                                                                                        jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n369_statement_begin_α:
                                                                                        jmp   n370_lit_string_α
n369_statement_begin_β:
                                                                                        jmp   n373_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n370_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx654_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n371_call_α
n370_lit_string_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n369_statement_begin_β
.Lx654_0:
                        .quad            .Lx654_0_s
.Lx654_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n371_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx656_6]
                        lea              r11, [rip + .Lx656_7]
                                                                                        jmp   stack_push_shim
.Lx656_6:
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              rdx, qword ptr [r9 + 24]
                                                                                        jmp   .Lx656_2
.Lx656_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx656_2
.Lx656_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx656_240
                        add              rsp, 16
                                                                                        jmp   n370_lit_string_β
.Lx656_240:
                                                                                        jmp   n372_statement_end_α
n371_call_β:
                                                                                        jmp   n370_lit_string_β
.Lx656_0:
                        .quad            .Lx656_0_s
.Lx656_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n372_statement_end_α:
                                                                                        jmp   n373_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n373_statement_begin_α:
                                                                                        jmp   n374_lit_string_α
n373_statement_begin_β:
                                                                                        jmp   n377_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n374_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx661_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n375_call_α
n374_lit_string_β:
                        add              rsp, 16
                        add              rsp, 128
                                                                                        jmp   n373_statement_begin_β
.Lx661_0:
                        .quad            .Lx661_0_s
.Lx661_0_s:
                        .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n375_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx663_6]
                        lea              r11, [rip + .Lx663_7]
                                                                                        jmp   stack_push_shim
.Lx663_6:
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              rdx, qword ptr [r9 + 24]
                                                                                        jmp   .Lx663_2
.Lx663_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx663_2
.Lx663_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx663_240
                        add              rsp, 16
                                                                                        jmp   n374_lit_string_β
.Lx663_240:
                                                                                        jmp   n376_statement_end_α
n375_call_β:
                                                                                        jmp   n374_lit_string_β
.Lx663_0:
                        .quad            .Lx663_0_s
.Lx663_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n376_statement_end_α:
                                                                                        jmp   n377_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n377_statement_begin_α:
                                                                                        jmp   n378_call_α
n377_statement_begin_β:
                                                                                        jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n378_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx669_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx669_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx669_6]
                        lea              r11, [rip + .Lx669_7]
                                                                                        jmp   rax
.Lx669_6:
                        mov              rdi, qword ptr [r9 + 112]
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx669_2
.Lx669_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx669_2
.Lx669_5:
                        add              rsp, 32
.Lx669_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx669_240
                        add              rsp, 16
                        add              rsp, 160
                                                                                        jmp   n377_statement_begin_β
.Lx669_240:
                                                                                        jmp   n379_assign_α
n378_call_β:
                                                                                        jmp   n377_statement_begin_β
.Lx669_0:
                        .quad            .Lx669_0_s
.Lx669_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n379_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx670_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n380_statement_end_α
.Lx670_0:
                        .quad            .Lx670_0_s
.Lx670_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n380_statement_end_α:
                                                                                        jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n381_statement_begin_α:
                                                                                        jmp   n382_call_α
n381_statement_begin_β:
                                                                                        jmp   n385_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n382_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx676_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx676_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx676_6]
                        lea              r11, [rip + .Lx676_7]
                                                                                        jmp   rax
.Lx676_6:
                        mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx676_2
.Lx676_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx676_2
.Lx676_5:
                        add              rsp, 32
.Lx676_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx676_240
                        add              rsp, 16
                        add              rsp, 176
                                                                                        jmp   n381_statement_begin_β
.Lx676_240:
                                                                                        jmp   n383_assign_α
n382_call_β:
                                                                                        jmp   n381_statement_begin_β
.Lx676_0:
                        .quad            .Lx676_0_s
.Lx676_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n383_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx677_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n384_statement_end_α
.Lx677_0:
                        .quad            .Lx677_0_s
.Lx677_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n384_statement_end_α:
                                                                                        jmp   n385_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n385_statement_begin_α:
                                                                                        jmp   n386_call_α
n385_statement_begin_β:
                                                                                        jmp   n389_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n386_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx683_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx683_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx683_6]
                        lea              r11, [rip + .Lx683_7]
                                                                                        jmp   rax
.Lx683_6:
                        mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx683_2
.Lx683_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx683_2
.Lx683_5:
                        add              rsp, 32
.Lx683_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx683_240
                        add              rsp, 16
                        add              rsp, 192
                                                                                        jmp   n385_statement_begin_β
.Lx683_240:
                                                                                        jmp   n387_assign_α
n386_call_β:
                                                                                        jmp   n385_statement_begin_β
.Lx683_0:
                        .quad            .Lx683_0_s
.Lx683_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n387_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx684_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n388_statement_end_α
.Lx684_0:
                        .quad            .Lx684_0_s
.Lx684_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n388_statement_end_α:
                                                                                        jmp   n389_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n389_statement_begin_α:
                                                                                        jmp   n390_call_α
n389_statement_begin_β:
                                                                                        jmp   n393_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n390_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx690_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx690_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx690_6]
                        lea              r11, [rip + .Lx690_7]
                                                                                        jmp   rax
.Lx690_6:
                        mov              rdi, qword ptr [r9 + 112]
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx690_2
.Lx690_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx690_2
.Lx690_5:
                        add              rsp, 32
.Lx690_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx690_240
                        add              rsp, 16
                        add              rsp, 208
                                                                                        jmp   n389_statement_begin_β
.Lx690_240:
                                                                                        jmp   n391_assign_α
n390_call_β:
                                                                                        jmp   n389_statement_begin_β
.Lx690_0:
                        .quad            .Lx690_0_s
.Lx690_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n391_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx691_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n392_statement_end_α
.Lx691_0:
                        .quad            .Lx691_0_s
.Lx691_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n392_statement_end_α:
                                                                                        jmp   n393_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n393_statement_begin_α:
                                                                                        jmp   n394_call_α
n393_statement_begin_β:
                                                                                        jmp   n397_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n394_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx697_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx697_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx697_6]
                        lea              r11, [rip + .Lx697_7]
                                                                                        jmp   rax
.Lx697_6:
                        mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx697_2
.Lx697_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx697_2
.Lx697_5:
                        add              rsp, 32
.Lx697_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx697_240
                        add              rsp, 16
                        add              rsp, 224
                                                                                        jmp   n393_statement_begin_β
.Lx697_240:
                                                                                        jmp   n395_assign_α
n394_call_β:
                                                                                        jmp   n393_statement_begin_β
.Lx697_0:
                        .quad            .Lx697_0_s
.Lx697_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n395_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx698_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n396_statement_end_α
.Lx698_0:
                        .quad            .Lx698_0_s
.Lx698_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n396_statement_end_α:
                                                                                        jmp   n397_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n397_statement_begin_α:
                                                                                        jmp   n398_call_α
n397_statement_begin_β:
                                                                                        jmp   n401_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n398_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx704_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx704_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx704_6]
                        lea              r11, [rip + .Lx704_7]
                                                                                        jmp   rax
.Lx704_6:
                        mov              rdi, qword ptr [r9 + 112]
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx704_2
.Lx704_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx704_2
.Lx704_5:
                        add              rsp, 32
.Lx704_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx704_240
                        add              rsp, 16
                        add              rsp, 240
                                                                                        jmp   n397_statement_begin_β
.Lx704_240:
                                                                                        jmp   n399_assign_α
n398_call_β:
                                                                                        jmp   n397_statement_begin_β
.Lx704_0:
                        .quad            .Lx704_0_s
.Lx704_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n399_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx705_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n400_statement_end_α
.Lx705_0:
                        .quad            .Lx705_0_s
.Lx705_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n400_statement_end_α:
                                                                                        jmp   n401_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n401_statement_begin_α:
                                                                                        jmp   n402_statement_end_α
n401_statement_begin_β:
                                                                                        jmp   n403_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n402_statement_end_α:
                                                                                        jmp   n403_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n403_statement_begin_α:
                                                                                        jmp   n404_call_α
n403_statement_begin_β:
                        add              rsp, 256
                                                                                        jmp   n410_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n404_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx715_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx715_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx715_6]
                        lea              r11, [rip + .Lx715_7]
                                                                                        jmp   rax
.Lx715_6:
                        mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx715_2
.Lx715_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx715_2
.Lx715_5:
                        add              rsp, 32
.Lx715_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx715_240
                        add              rsp, 16
                        add              rsp, 256
                                                                                        jmp   n403_statement_begin_β
.Lx715_240:
                                                                                        jmp   n405_statement_end_α
n404_call_β:
                                                                                        jmp   n403_statement_begin_β
.Lx715_0:
                        .quad            .Lx715_0_s
.Lx715_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n405_statement_end_α:
                                                                                        jmp   n406_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n406_statement_begin_α:
                                                                                        jmp   n407_lit_string_α
n406_statement_begin_β:
                                                                                        jmp   n414_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n407_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx720_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n408_assign_α
.Lx720_0:
                        .quad            .Lx720_0_s
.Lx720_0_s:
                        .string          "FAIL: empty pop should FRETURN"
#-----------------------------------------------------------------------------------------------------------------------
n408_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx721_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n409_statement_end_α
.Lx721_0:
                        .quad            .Lx721_0_s
.Lx721_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n409_statement_end_α:
                                                                                        jmp   n414_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n410_statement_begin_α:
                                                                                        jmp   n411_lit_string_α
n410_statement_begin_β:
                                                                                        jmp   n414_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n411_lit_string_α:
                        mov              qword ptr [rsp + 1920], 2                      # result
                        mov              dword ptr [rsp + 1924], 8
                        mov              rax, qword ptr [rip + .Lx726_0]
                        mov              qword ptr [rsp + 1928], rax
                                                                                        jmp   n412_assign_α
.Lx726_0:
                        .quad            .Lx726_0_s
.Lx726_0_s:
                        .string          "empty ok"
#-----------------------------------------------------------------------------------------------------------------------
n412_assign_α:
                        mov              rsi, qword ptr [rsp + 1920]
                        mov              rdx, qword ptr [rsp + 1928]
                        mov              rdi, qword ptr [rip + .Lx727_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n413_statement_end_α
.Lx727_0:
                        .quad            .Lx727_0_s
.Lx727_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n413_statement_end_α:
                                                                                        jmp   n414_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n414_statement_begin_α:
                                                                                        jmp   n415_statement_end_α
n414_statement_begin_β:
                                                                                        jmp   n416_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n415_statement_end_α:
                                                                                        jmp   n416_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n416_statement_begin_α:
                                                                                        jmp   n417_call_α
n416_statement_begin_β:
                                                                                        jmp   n419_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n417_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx737_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx737_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx737_6]
                        lea              r11, [rip + .Lx737_7]
                                                                                        jmp   rax
.Lx737_6:
                        mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx737_2
.Lx737_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx737_2
.Lx737_5:
                        add              rsp, 16
.Lx737_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx737_240
                        add              rsp, 16
                        add              rsp, 288
                                                                                        jmp   n416_statement_begin_β
.Lx737_240:
                                                                                        jmp   n418_statement_end_α
n417_call_β:
                                                                                        jmp   n416_statement_begin_β
.Lx737_0:
                        .quad            .Lx737_0_s
.Lx737_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n418_statement_end_α:
                                                                                        jmp   n419_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n419_statement_begin_α:
                                                                                        jmp   n420_lit_string_α
n419_statement_begin_β:
                                                                                        jmp   n423_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n420_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx742_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n421_call_α
n420_lit_string_β:
                        add              rsp, 16
                        add              rsp, 304
                                                                                        jmp   n419_statement_begin_β
.Lx742_0:
                        .quad            .Lx742_0_s
.Lx742_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n421_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx744_6]
                        lea              r11, [rip + .Lx744_7]
                                                                                        jmp   stack_push_shim
.Lx744_6:
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              rdx, qword ptr [r9 + 24]
                                                                                        jmp   .Lx744_2
.Lx744_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx744_2
.Lx744_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx744_240
                        add              rsp, 16
                                                                                        jmp   n420_lit_string_β
.Lx744_240:
                                                                                        jmp   n422_statement_end_α
n421_call_β:
                                                                                        jmp   n420_lit_string_β
.Lx744_0:
                        .quad            .Lx744_0_s
.Lx744_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n422_statement_end_α:
                                                                                        jmp   n423_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n423_statement_begin_α:
                                                                                        jmp   n424_call_α
n423_statement_begin_β:
                                                                                        jmp   n427_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n424_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]                       # stack_peek
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx750_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx750_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx750_6]
                        lea              r11, [rip + .Lx750_7]
                                                                                        jmp   rax
.Lx750_6:
                        mov              rdi, qword ptr [r9 + 80]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx750_2
.Lx750_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx750_2
.Lx750_5:
                        add              rsp, 16
.Lx750_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx750_240
                        add              rsp, 16
                        add              rsp, 336
                                                                                        jmp   n423_statement_begin_β
.Lx750_240:
                                                                                        jmp   n425_assign_α
n424_call_β:
                                                                                        jmp   n423_statement_begin_β
.Lx750_0:
                        .quad            .Lx750_0_s
.Lx750_0_s:
                        .string          "stack_peek"
#-----------------------------------------------------------------------------------------------------------------------
n425_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx751_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n426_statement_end_α
.Lx751_0:
                        .quad            .Lx751_0_s
.Lx751_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n426_statement_end_α:
                                                                                        jmp   n427_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n427_statement_begin_α:
                                                                                        jmp   n428_call_α
n427_statement_begin_β:
                                                                                        jmp   n431_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n428_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx757_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx757_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx757_6]
                        lea              r11, [rip + .Lx757_7]
                                                                                        jmp   rax
.Lx757_6:
                        mov              rdi, qword ptr [r9 + 112]
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx757_2
.Lx757_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx757_2
.Lx757_5:
                        add              rsp, 32
.Lx757_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx757_240
                        add              rsp, 16
                        add              rsp, 352
                                                                                        jmp   n427_statement_begin_β
.Lx757_240:
                                                                                        jmp   n429_assign_α
n428_call_β:
                                                                                        jmp   n427_statement_begin_β
.Lx757_0:
                        .quad            .Lx757_0_s
.Lx757_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n429_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx758_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n430_statement_end_α
.Lx758_0:
                        .quad            .Lx758_0_s
.Lx758_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n430_statement_end_α:
                                                                                        jmp   n431_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n431_statement_begin_α:
                                                                                        jmp   n432_call_α
n431_statement_begin_β:
                                                                                        jmp   n435_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n432_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx764_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx764_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx764_6]
                        lea              r11, [rip + .Lx764_7]
                                                                                        jmp   rax
.Lx764_6:
                        mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx764_2
.Lx764_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx764_2
.Lx764_5:
                        add              rsp, 32
.Lx764_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx764_240
                        add              rsp, 16
                        add              rsp, 368
                                                                                        jmp   n431_statement_begin_β
.Lx764_240:
                                                                                        jmp   n433_assign_α
n432_call_β:
                                                                                        jmp   n431_statement_begin_β
.Lx764_0:
                        .quad            .Lx764_0_s
.Lx764_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n433_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx765_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n434_statement_end_α
.Lx765_0:
                        .quad            .Lx765_0_s
.Lx765_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n434_statement_end_α:
                                                                                        jmp   n435_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n435_statement_begin_α:
                                                                                        jmp   n436_statement_end_α
n435_statement_begin_β:
                                                                                        jmp   n437_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n436_statement_end_α:
                                                                                        jmp   n437_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n437_statement_begin_α:
                                                                                        jmp   n438_call_α
n437_statement_begin_β:
                                                                                        jmp   n440_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n438_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx775_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx775_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx775_6]
                        lea              r11, [rip + .Lx775_7]
                                                                                        jmp   rax
.Lx775_6:
                        mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx775_2
.Lx775_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx775_2
.Lx775_5:
                        add              rsp, 16
.Lx775_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx775_240
                        add              rsp, 16
                        add              rsp, 384
                                                                                        jmp   n437_statement_begin_β
.Lx775_240:
                                                                                        jmp   n439_statement_end_α
n438_call_β:
                                                                                        jmp   n437_statement_begin_β
.Lx775_0:
                        .quad            .Lx775_0_s
.Lx775_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n439_statement_end_α:
                                                                                        jmp   n440_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n440_statement_begin_α:
                                                                                        jmp   n441_lit_integer_α
n440_statement_begin_β:
                                                                                        jmp   n444_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n441_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx780_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n442_call_α
n441_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 400
                                                                                        jmp   n440_statement_begin_β
.Lx780_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n442_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx782_6]
                        lea              r11, [rip + .Lx782_7]
                                                                                        jmp   stack_push_shim
.Lx782_6:
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              rdx, qword ptr [r9 + 24]
                                                                                        jmp   .Lx782_2
.Lx782_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx782_2
.Lx782_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx782_240
                        add              rsp, 16
                                                                                        jmp   n441_lit_integer_β
.Lx782_240:
                                                                                        jmp   n443_statement_end_α
n442_call_β:
                                                                                        jmp   n441_lit_integer_β
.Lx782_0:
                        .quad            .Lx782_0_s
.Lx782_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n443_statement_end_α:
                                                                                        jmp   n444_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n444_statement_begin_α:
                                                                                        jmp   n445_lit_integer_α
n444_statement_begin_β:
                                                                                        jmp   n448_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n445_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx787_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n446_call_α
n445_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 432
                                                                                        jmp   n444_statement_begin_β
.Lx787_0:
                        .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n446_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx789_6]
                        lea              r11, [rip + .Lx789_7]
                                                                                        jmp   stack_push_shim
.Lx789_6:
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              rdx, qword ptr [r9 + 24]
                                                                                        jmp   .Lx789_2
.Lx789_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx789_2
.Lx789_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx789_240
                        add              rsp, 16
                                                                                        jmp   n445_lit_integer_β
.Lx789_240:
                                                                                        jmp   n447_statement_end_α
n446_call_β:
                                                                                        jmp   n445_lit_integer_β
.Lx789_0:
                        .quad            .Lx789_0_s
.Lx789_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n447_statement_end_α:
                                                                                        jmp   n448_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n448_statement_begin_α:
                                                                                        jmp   n449_lit_string_α
n448_statement_begin_β:
                                                                                        jmp   n452_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n449_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx794_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n450_call_α
n449_lit_string_β:
                        add              rsp, 16
                        add              rsp, 464
                                                                                        jmp   n448_statement_begin_β
.Lx794_0:
                        .quad            .Lx794_0_s
.Lx794_0_s:
                        .string          "myvar"
#-----------------------------------------------------------------------------------------------------------------------
n450_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx796_6]
                        lea              r11, [rip + .Lx796_7]
                                                                                        jmp   stack_pop_shim
.Lx796_6:
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              rdx, qword ptr [r9 + 56]
                                                                                        jmp   .Lx796_2
.Lx796_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx796_2
.Lx796_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx796_240
                        add              rsp, 16
                                                                                        jmp   n449_lit_string_β
.Lx796_240:
                                                                                        jmp   n451_statement_end_α
n450_call_β:
                                                                                        jmp   n449_lit_string_β
.Lx796_0:
                        .quad            .Lx796_0_s
.Lx796_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n451_statement_end_α:
                                                                                        jmp   n452_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n452_statement_begin_α:
                                                                                        jmp   n453_var_α
n452_statement_begin_β:
                                                                                        jmp   n456_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n453_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]                      # myvar
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n454_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n454_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx802_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n455_statement_end_α
.Lx802_0:
                        .quad            .Lx802_0_s
.Lx802_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n455_statement_end_α:
                                                                                        jmp   n456_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n456_statement_begin_α:
                                                                                        jmp   n457_statement_end_α
n456_statement_begin_β:
                                                                                        jmp   n458_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n457_statement_end_α:
                                                                                        jmp   n458_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n458_statement_begin_α:
                                                                                        jmp   n459_call_α
n458_statement_begin_β:
                                                                                        jmp   n461_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n459_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx812_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx812_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx812_6]
                        lea              r11, [rip + .Lx812_7]
                                                                                        jmp   rax
.Lx812_6:
                        mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx812_2
.Lx812_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx812_2
.Lx812_5:
                        add              rsp, 16
.Lx812_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx812_240
                        add              rsp, 16
                        add              rsp, 512
                                                                                        jmp   n458_statement_begin_β
.Lx812_240:
                                                                                        jmp   n460_statement_end_α
n459_call_β:
                                                                                        jmp   n458_statement_begin_β
.Lx812_0:
                        .quad            .Lx812_0_s
.Lx812_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n460_statement_end_α:
                                                                                        jmp   n461_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n461_statement_begin_α:
                                                                                        jmp   n462_lit_string_α
n461_statement_begin_β:
                                                                                        jmp   n465_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n462_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx817_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n463_assign_α
.Lx817_0:
                        .quad            .Lx817_0_s
.Lx817_0_s:
                        .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n463_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax                      # subject
                        mov              qword ptr [r9 + 200], rdx
                                                                                        jmp   n464_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n464_statement_end_α:
                                                                                        jmp   n465_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n465_statement_begin_α:
                                                                                        jmp   n466_var_α
n465_statement_begin_β:
                        add              rsp, 544
                                                                                        jmp   n497_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n466_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]                      # subject
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n467_match_begin_α
n466_var_β:
                        add              rsp, 16
                        add              rsp, 544
                                                                                        jmp   n497_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n467_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 2656], rdi
                        mov              qword ptr [rsp + 2664], rsi
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx825_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n468_match_assign_save_α
n467_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx825_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx825_1
                                                                                        jmp   .Lx825_0
.Lx825_1:
n467_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n466_var_β
#-----------------------------------------------------------------------------------------------------------------------
n468_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n469_match_break_α
n468_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n467_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n469_match_break_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx829_0:
                        cmp              ecx, r15d
                                                                                        jl    .Lx829_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n467_match_begin_β
.Lx829_240:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx829_1
                        add              ecx, 1
                                                                                        jmp   .Lx829_0
.Lx829_1:
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx
                                                                                        jmp   n470_match_assign_cond_α
n469_match_break_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n467_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n470_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n471_match_lit_α
n470_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n469_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n471_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n470_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n470_match_assign_cond_β
                        add              r14d, 1
                                                                                        jmp   n472_match_assign_save_α
n471_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n470_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n472_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n473_match_rem_α
n472_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n471_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n473_match_rem_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d
                                                                                        jmp   n474_match_assign_cond_α
n473_match_rem_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n471_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n474_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n475_match_end_α
n474_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n473_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n475_match_end_α:
                        mov              r8, r12
.Lx840_9:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx840_9
                        mov              eax, dword ptr [rsp + 64]
                        mov              dword ptr [rsp + 2544], eax
                        mov              qword ptr [rsp + 2568], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx840_5:
                        sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax
                                                                                        jne   .Lx840_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx840_1:
                        test             rax, rax
                                                                                        je    .Lx840_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx840_3]
                        lea              rdx, [rip + .Lx840_4]
                                                                                        jmp   rax
.Lx840_3:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx840_1
.Lx840_4:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx840_1
.Lx840_2:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx840_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx840_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n476_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n476_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx841_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n477_match_replace_α
.Lx841_0:
                        .quad            .Lx841_0_s
.Lx841_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n477_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx843_0]
                        mov              rsi, qword ptr [rsp + 2672]
                        mov              rdx, qword ptr [rsp + 2680]
                        mov              ecx, dword ptr [rsp + 2464]
                        mov              r8, qword ptr [rsp + 2488]
                        lea              r9, [rsp + 0]                                  # lit_string
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx843_1
.Lx843_0:
                        .quad            .Lx843_0_s
.Lx843_0_s:
                        .string          "subject"
.Lx843_1:
                                                                                        jmp   n478_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n478_statement_end_α:
                                                                                        jmp   n479_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n479_statement_begin_α:
                                                                                        jmp   n480_var_α
n479_statement_begin_β:
                                                                                        jmp   n483_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n480_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]                      # w1
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n481_call_α
n480_var_β:
                        add              rsp, 16
                        add              rsp, 640
                                                                                        jmp   n479_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n481_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx850_6]
                        lea              r11, [rip + .Lx850_7]
                                                                                        jmp   stack_push_shim
.Lx850_6:
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              rdx, qword ptr [r9 + 24]
                                                                                        jmp   .Lx850_2
.Lx850_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx850_2
.Lx850_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx850_240
                        add              rsp, 16
                                                                                        jmp   n480_var_β
.Lx850_240:
                                                                                        jmp   n482_statement_end_α
n481_call_β:
                                                                                        jmp   n480_var_β
.Lx850_0:
                        .quad            .Lx850_0_s
.Lx850_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n482_statement_end_α:
                                                                                        jmp   n483_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n483_statement_begin_α:
                                                                                        jmp   n484_var_α
n483_statement_begin_β:
                                                                                        jmp   n487_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n484_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # w2
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n485_call_α
n484_var_β:
                        add              rsp, 16
                        add              rsp, 672
                                                                                        jmp   n483_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n485_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx
                        lea              r10, [rip + .Lx857_6]
                        lea              r11, [rip + .Lx857_7]
                                                                                        jmp   stack_push_shim
.Lx857_6:
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              rdx, qword ptr [r9 + 24]
                                                                                        jmp   .Lx857_2
.Lx857_7:
                        mov              eax, 104
                        xor              edx, edx
                                                                                        jmp   .Lx857_2
.Lx857_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx857_240
                        add              rsp, 16
                                                                                        jmp   n484_var_β
.Lx857_240:
                                                                                        jmp   n486_statement_end_α
n485_call_β:
                                                                                        jmp   n484_var_β
.Lx857_0:
                        .quad            .Lx857_0_s
.Lx857_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n486_statement_end_α:
                                                                                        jmp   n487_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n487_statement_begin_α:
                                                                                        jmp   n488_call_α
n487_statement_begin_β:
                                                                                        jmp   n491_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n488_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx863_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx863_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx863_6]
                        lea              r11, [rip + .Lx863_7]
                                                                                        jmp   rax
.Lx863_6:
                        mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx863_2
.Lx863_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx863_2
.Lx863_5:
                        add              rsp, 32
.Lx863_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx863_240
                        add              rsp, 16
                        add              rsp, 704
                                                                                        jmp   n487_statement_begin_β
.Lx863_240:
                                                                                        jmp   n489_assign_α
n488_call_β:
                                                                                        jmp   n487_statement_begin_β
.Lx863_0:
                        .quad            .Lx863_0_s
.Lx863_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n489_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx864_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n490_statement_end_α
.Lx864_0:
                        .quad            .Lx864_0_s
.Lx864_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n490_statement_end_α:
                                                                                        jmp   n491_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n491_statement_begin_α:
                                                                                        jmp   n492_call_α
n491_statement_begin_β:
                                                                                        jmp   n495_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n492_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx870_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx870_5
                        mov              r10, rax
                        mov              rax, r10
                        lea              r10, [rip + .Lx870_6]
                        lea              r11, [rip + .Lx870_7]
                                                                                        jmp   rax
.Lx870_6:
                        mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx870_2
.Lx870_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx870_2
.Lx870_5:
                        add              rsp, 32
.Lx870_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx870_240
                        add              rsp, 16
                        add              rsp, 720
                                                                                        jmp   n491_statement_begin_β
.Lx870_240:
                                                                                        jmp   n493_assign_α
n492_call_β:
                                                                                        jmp   n491_statement_begin_β
.Lx870_0:
                        .quad            .Lx870_0_s
.Lx870_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n493_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx871_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n494_statement_end_α
.Lx871_0:
                        .quad            .Lx871_0_s
.Lx871_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n494_statement_end_α:
                                                                                        jmp   n495_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n495_statement_begin_α:
                                                                                        jmp   n496_statement_end_α
n495_statement_begin_β:
                        add              rsp, 736
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n496_statement_end_α:
                        add              rsp, 560
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n497_statement_begin_α:
                                                                                        jmp   n498_lit_string_α
n497_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n498_lit_string_α:
                        mov              qword ptr [rsp + 2864], 2                      # result
                        mov              dword ptr [rsp + 2868], 26
                        mov              rax, qword ptr [rip + .Lx880_0]
                        mov              qword ptr [rsp + 2872], rax
                                                                                        jmp   n499_assign_α
.Lx880_0:
                        .quad            .Lx880_0_s
.Lx880_0_s:
                        .string          "FAIL: pattern match failed"
#-----------------------------------------------------------------------------------------------------------------------
n499_assign_α:
                        mov              rsi, qword ptr [rsp + 2864]
                        mov              rdx, qword ptr [rsp + 2872]
                        mov              rdi, qword ptr [rip + .Lx881_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n500_statement_end_α
.Lx881_0:
                        .quad            .Lx881_0_s
.Lx881_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n500_statement_end_α:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n501_goto_α:
                                                                                        jmp   n326_statement_begin_α
n501_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n502_goto_α:
                                                                                        jmp   n331_statement_begin_α
n502_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n503_goto_α:
                                                                                        jmp   n504_statement_begin_α
n503_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n504_statement_begin_α:
                                                                                        jmp   n505_var_α
n504_statement_begin_β:
                                                                                        jmp   n511_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n505_var_α:
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 672], rax                     # result
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n506_call_α
#-----------------------------------------------------------------------------------------------------------------------
n506_call_α:
                        mov              rax, qword ptr [rsp + 672]
                        mov              qword ptr [rsp + 640], rax
                        mov              rax, qword ptr [rsp + 680]
                        mov              qword ptr [rsp + 648], rax
                        .section         .rodata
.Lrkfn891:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn891]
                        lea              rsi, [rsp + 640]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        cmp              eax, 104
                                                                                        je    n504_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n507_var_α
n506_call_β:
                                                                                        jmp   n504_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n507_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 736], rax                     # result
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n508_call_α
#-----------------------------------------------------------------------------------------------------------------------
n508_call_α:
                        mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 744]
                        mov              qword ptr [rsp + 712], rax
                        .section         .rodata
.Lrkfn894:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn894]
                        lea              rsi, [rsp + 704]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                        cmp              eax, 104
                                                                                        je    n504_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n509_assign_var_α
n508_call_β:
                                                                                        jmp   n504_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n509_assign_var_α:
                        mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
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
                        cmp              eax, 104
                                                                                        je    n504_statement_begin_β
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n510_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n510_statement_end_α:
                                                                                        jmp   n511_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n511_statement_begin_α:
                                                                                        jmp   n512_var_α
n511_statement_begin_β:
                                                                                        jmp   n516_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n512_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 816], rax                     # result
                        mov              qword ptr [rsp + 824], rdx
                                                                                        jmp   n513_call_α
#-----------------------------------------------------------------------------------------------------------------------
n513_call_α:
                        mov              rax, qword ptr [rsp + 816]
                        mov              qword ptr [rsp + 784], rax
                        mov              rax, qword ptr [rsp + 824]
                        mov              qword ptr [rsp + 792], rax
                        .section         .rodata
.Lrkfn902:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn902]
                        lea              rsi, [rsp + 784]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 104
                                                                                        je    n511_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n514_assign_α
n513_call_β:
                                                                                        jmp   n511_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n514_assign_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              rdx, qword ptr [rsp + 776]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n515_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n515_statement_end_α:
                                                                                        jmp   n516_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n516_statement_begin_α:
                                                                                        jmp   n517_lit_string_α
n516_statement_begin_β:
                                                                                        jmp   n521_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n517_lit_string_α:
                        mov              qword ptr [rsp + 880], 2                       # result
                        mov              dword ptr [rsp + 884], 5
                        mov              rax, qword ptr [rip + .Lx908_0]
                        mov              qword ptr [rsp + 888], rax
                                                                                        jmp   n518_call_α
.Lx908_0:
                        .quad            .Lx908_0_s
.Lx908_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n518_call_α:
                        mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 848], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 856], rax
                        .section         .rodata
.Lrkfn910:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn910]
                        lea              rsi, [rsp + 848]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                        cmp              eax, 104
                                                                                        je    n516_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n519_assign_α
n518_call_β:
                                                                                        jmp   n516_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n519_assign_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              rdx, qword ptr [rsp + 840]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n520_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n520_statement_end_α:
                                                                                        jmp   n521_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n521_lit_string_α:
                        mov              qword ptr [rsp + 32], 2                        # result
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx914_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n522_call_α
.Lx914_0:
                        .quad            .Lx914_0_s
.Lx914_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n522_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn916:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn916]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 104
                                                                                        je    n524_save_restore_α
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n523_save_restore_α
n522_call_β:
                                                                                        jmp   n524_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n523_save_restore_α:
                                                                                        jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
n524_save_restore_α:
                                                                                        jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
n525_goto_α:
                                                                                        jmp   n336_statement_begin_α
n525_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n526_goto_α:
                                                                                        jmp   n341_statement_begin_α
n526_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n527_goto_α:
                                                                                        jmp   n346_statement_begin_α
n527_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n528_goto_α:
                                                                                        jmp   n529_statement_begin_α
n528_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n529_statement_begin_α:
                                                                                        jmp   n530_var_α
n529_statement_begin_β:
                                                                                        jmp   n523_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n530_var_α:
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 1248], rax                    # result
                        mov              qword ptr [rsp + 1256], rdx
                                                                                        jmp   n531_call_α
#-----------------------------------------------------------------------------------------------------------------------
n531_call_α:
                        mov              rax, qword ptr [rsp + 1248]
                        mov              qword ptr [rsp + 1216], rax
                        mov              rax, qword ptr [rsp + 1256]
                        mov              qword ptr [rsp + 1224], rax
                        .section         .rodata
.Lrkfn929:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn929]
                        lea              rsi, [rsp + 1216]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1200], rax
                        mov              qword ptr [rsp + 1208], rdx
                        cmp              eax, 104
                                                                                        je    n529_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n532_statement_end_α
n531_call_β:
                                                                                        jmp   n529_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n532_statement_end_α:
                                                                                        jmp   n533_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n533_statement_begin_α:
                                                                                        jmp   n534_var_α
n533_statement_begin_β:
                                                                                        jmp   n539_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n534_var_α:
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 1280], rax                    # result
                        mov              qword ptr [rsp + 1288], rdx
                                                                                        jmp   n535_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n535_lit_integer_α:
                        mov              qword ptr [rsp + 1296], 3                      # result
                        mov              rax, qword ptr [rip + .Lx935_0]
                        mov              qword ptr [rsp + 1304], rax
                                                                                        jmp   n536_binop_α
.Lx935_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n536_binop_α:
                        mov              eax, dword ptr [rsp + 1280]
                        cmp              eax, 3
                                                                                        jne   .Lx936_0
                        mov              rax, qword ptr [rsp + 1288]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rsp + 1264], 3
                        mov              qword ptr [rsp + 1272], rax
                                                                                        jmp   n537_assign_α
.Lx936_0:
                        mov              rdi, qword ptr [rsp + 1280]
                        mov              rsi, qword ptr [rsp + 1288]
                        mov              rdx, qword ptr [rsp + 1296]
                        mov              rcx, qword ptr [rsp + 1304]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx936_240
                        add              rsp, 32
                                                                                        jmp   n533_statement_begin_β
.Lx936_240:
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n537_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n537_assign_α:
                        mov              rax, qword ptr [rsp + 1264]
                        mov              rdx, qword ptr [rsp + 1272]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              qword ptr [r9 + 120], rdx
                                                                                        jmp   n538_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n538_statement_end_α:
                                                                                        jmp   n539_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n539_statement_begin_α:
                                                                                        jmp   n540_var_α
n539_statement_begin_β:
                                                                                        jmp   n529_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n540_var_α:
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 1360], rax                    # result
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n541_call_α
#-----------------------------------------------------------------------------------------------------------------------
n541_call_α:
                        mov              rax, qword ptr [rsp + 1360]
                        mov              qword ptr [rsp + 1328], rax
                        mov              rax, qword ptr [rsp + 1368]
                        mov              qword ptr [rsp + 1336], rax
                        .section         .rodata
.Lrkfn944:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn944]
                        lea              rsi, [rsp + 1328]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1312], rax
                        mov              qword ptr [rsp + 1320], rdx
                        cmp              eax, 104
                                                                                        je    n539_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n542_assign_α
n541_call_β:
                                                                                        jmp   n539_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n542_assign_α:
                        mov              rax, qword ptr [rsp + 1312]
                        mov              rdx, qword ptr [rsp + 1320]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n543_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n543_statement_end_α:
                                                                                        jmp   n529_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n544_goto_α:
                                                                                        jmp   n351_statement_begin_α
n544_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n545_goto_α:
                                                                                        jmp   n406_statement_begin_α
n545_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n546_goto_α:
                                                                                        jmp   n414_statement_begin_α
n546_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n547_goto_α:
                                                                                        jmp   n497_statement_begin_α
n547_goto_β:
                                                                                        jmp   main_ω
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
                        .section         .rodata
.S0:                    .string          "w1"
.S1:                    .string          "w2"
                        .text
                        .section         .note.GNU-stack,"",@progbits

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
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx12_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx12_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_β:
                                                                                        jmp   proc_LBL__stack_init_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_γ:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        .section         .rodata
.Lrkfnzd33:             .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd33]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd46:             .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd46]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx48_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx48_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx50_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx50_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_β:
                                                                                        jmp   proc_LBL__stack_push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_γ:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd94:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd94]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd101:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd101]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd108:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd108]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd116:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd116]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              qword ptr [rsp + 624], rax                     # result
                        mov              qword ptr [rsp + 632], rdx
                                                                                        jmp   n71_call_α
#-----------------------------------------------------------------------------------------------------------------------
n71_call_α:
                        mov              rax, qword ptr [rsp + 624]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 632]
                        mov              qword ptr [rsp + 600], rax
                        .section         .rodata
.Lrkfn124:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn124]
                        lea              rsi, [rsp + 592]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
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
                        mov              qword ptr [rsp + 688], rax                     # result
                        mov              qword ptr [rsp + 696], rdx
                                                                                        jmp   n73_call_α
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:
                        mov              rax, qword ptr [rsp + 688]
                        mov              qword ptr [rsp + 656], rax
                        mov              rax, qword ptr [rsp + 696]
                        mov              qword ptr [rsp + 664], rax
                        .section         .rodata
.Lrkfn127:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn127]
                        lea              rsi, [rsp + 656]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
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
                        mov              rdi, qword ptr [rsp + 576]
                        mov              rsi, qword ptr [rsp + 584]
                        mov              rdx, qword ptr [rsp + 640]
                        mov              rcx, qword ptr [rsp + 648]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
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
                        mov              qword ptr [rsp + 768], rax                     # result
                        mov              qword ptr [rsp + 776], rdx
                                                                                        jmp   n78_call_α
#-----------------------------------------------------------------------------------------------------------------------
n78_call_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 744], rax
                        .section         .rodata
.Lrkfn135:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn135]
                        lea              rsi, [rsp + 736]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
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
                        mov              rax, qword ptr [rsp + 720]
                        mov              rdx, qword ptr [rsp + 728]
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
                        mov              qword ptr [rsp + 832], 2                       # result
                        mov              dword ptr [rsp + 836], 5
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n83_call_α
.Lx141_0:
                        .quad            .Lx141_0_s
.Lx141_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n83_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 800], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 808], rax
                        .section         .rodata
.Lrkfn143:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn143]
                        lea              rsi, [rsp + 800]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
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
                        mov              rax, qword ptr [rsp + 784]
                        mov              rdx, qword ptr [rsp + 792]
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx151_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx151_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n89_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx153_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx153_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_β:
                                                                                        jmp   proc_LBL__stack_pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_γ:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd169:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd169]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd176:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd176]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx181_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx181_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n164_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx183_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx183_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_β:
                                                                                        jmp   proc_LBL__stack_peek_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_γ:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd201:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd201]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd213:            .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd213]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx215_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx215_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n196_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx217_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx217_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_β:
                                                                                        jmp   proc_LBL__stack_top_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_γ:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd258:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd258]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd273:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd273]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx278_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx278_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_β:
                                                                                        jmp   proc_LBL__stack_depth_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_γ:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_init_α
proc_stack_init_α:
proc_stack_init_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n279_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rbp
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n280_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n280_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx284_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx284_1
.Lx284_0:
                        .quad            .Lx284_0_s
.Lx284_0_s:
                        .string          "stack_init"
.Lx284_1:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_push_α
proc_stack_push_α:
proc_stack_push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n285_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rbp
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n286_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n286_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx290_1
.Lx290_0:
                        .quad            .Lx290_0_s
.Lx290_0_s:
                        .string          "stack_push"
.Lx290_1:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_pop_α
proc_stack_pop_α:
proc_stack_pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n291_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rbp
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n292_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n292_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx296_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx296_1
.Lx296_0:
                        .quad            .Lx296_0_s
.Lx296_0_s:
                        .string          "stack_pop"
.Lx296_1:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_peek_α
proc_stack_peek_α:
proc_stack_peek_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n297_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rbp
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n298_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n298_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx302_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx302_1
.Lx302_0:
                        .quad            .Lx302_0_s
.Lx302_0_s:
                        .string          "stack_peek"
.Lx302_1:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_top_α
proc_stack_top_α:
proc_stack_top_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n303_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rbp
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n304_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n304_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx308_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx308_1
.Lx308_0:
                        .quad            .Lx308_0_s
.Lx308_0_s:
                        .string          "stack_top"
.Lx308_1:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_depth_α
proc_stack_depth_α:
proc_stack_depth_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n309_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rbp
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n310_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n310_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx314_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx314_1
.Lx314_0:
                        .quad            .Lx314_0_s
.Lx314_0_s:
                        .string          "stack_depth"
.Lx314_1:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_ω:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              esi, 2784
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
                        mov              esi, 2784
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
                        mov              esi, 2784
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
                        mov              esi, 2784
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
                        mov              esi, 2784
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
                        mov              esi, 2784
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "stack_init"
                        .align           8
.Lstartup_pnames6:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + .Lstartup_pnames6]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + proc_stack_init_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname7:       .string          "stack_push"
.Lstartup_pp7_0:        .string          "x"
                        .align           8
.Lstartup_pnames7:
                        .quad            .Lstartup_pp7_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + .Lstartup_pnames7]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + proc_stack_push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "stack_pop"
.Lstartup_pp8_0:        .string          "var"
                        .align           8
.Lstartup_pnames8:
                        .quad            .Lstartup_pp8_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + .Lstartup_pnames8]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + proc_stack_pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "stack_peek"
                        .align           8
.Lstartup_pnames9:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + .Lstartup_pnames9]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + proc_stack_peek_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "stack_top"
                        .align           8
.Lstartup_pnames10:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + .Lstartup_pnames10]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + proc_stack_top_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "stack_depth"
.Lstartup_pp11_0:       .string          "sd"
                        .align           8
.Lstartup_pnames11:
                        .quad            .Lstartup_pp11_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + .Lstartup_pnames11]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + proc_stack_depth_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
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
                        mov              rax, qword ptr [rip + .Lx613_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n319_call_α
.Lx613_0:
                        .quad            .Lx613_0_s
.Lx613_0_s:
                        .string          "slink(snext,sval)"
#-----------------------------------------------------------------------------------------------------------------------
n319_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd615:            .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd615]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        jne   .Lx614_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n317_statement_begin_β
.Lx614_240:
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
                                                                                        jmp   n324_statement_end_α
n323_statement_begin_β:
                                                                                        jmp   n325_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_statement_end_α:
                                                                                        jmp   n325_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n325_statement_begin_α:
                                                                                        jmp   n326_statement_end_α
n325_statement_begin_β:
                                                                                        jmp   n327_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n326_statement_end_α:
                                                                                        jmp   n327_statement_begin_α
#=======================================================================================================================
#         subject = 'hello world'
#-----------------------------------------------------------------------------------------------------------------------
n327_statement_begin_α:
                                                                                        jmp   n328_statement_end_α
n327_statement_begin_β:
                                                                                        jmp   n329_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n328_statement_end_α:
                                                                                        jmp   n329_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n329_statement_begin_α:
                                                                                        jmp   n330_statement_end_α
n329_statement_begin_β:
                                                                                        jmp   n331_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n330_statement_end_α:
                                                                                        jmp   n331_statement_begin_α
#=======================================================================================================================
#         OUTPUT = stack_pop()            ;* hello
#-----------------------------------------------------------------------------------------------------------------------
n331_statement_begin_α:
                                                                                        jmp   n332_statement_end_α
n331_statement_begin_β:
                                                                                        jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n332_statement_end_α:
                                                                                        jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n333_statement_begin_α:
                                                                                        jmp   n334_statement_end_α
n333_statement_begin_β:
                                                                                        jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:
                                                                                        jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α:
                                                                                        jmp   n336_statement_end_α
n335_statement_begin_β:
                                                                                        jmp   n337_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_statement_end_α:
                                                                                        jmp   n337_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n337_statement_begin_α:
                                                                                        jmp   n338_statement_end_α
n337_statement_begin_β:
                                                                                        jmp   n339_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n338_statement_end_α:
                                                                                        jmp   n339_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n339_statement_begin_α:
                                                                                        jmp   n340_statement_end_α
n339_statement_begin_β:
                                                                                        jmp   n341_statement_begin_α
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
                                                                                        jmp   n344_statement_end_α
n343_statement_begin_β:
                                                                                        jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n344_statement_end_α:
                                                                                        jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n345_statement_begin_α:
                                                                                        jmp   n346_statement_end_α
n345_statement_begin_β:
                                                                                        jmp   n347_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n346_statement_end_α:
                                                                                        jmp   n347_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_begin_α:
                                                                                        jmp   n348_statement_end_α
n347_statement_begin_β:
                                                                                        jmp   n349_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n348_statement_end_α:
                                                                                        jmp   n349_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n349_statement_begin_α:
                                                                                        jmp   n350_lit_string_α
n349_statement_begin_β:
                                                                                        jmp   n354_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n350_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx676_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n351_lit_integer_α
.Lx676_0:
                        .quad            .Lx676_0_s
.Lx676_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n351_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx677_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n352_call_α
n351_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n349_statement_begin_β
.Lx677_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n352_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        .section         .rodata
.Lrkfnzd679:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd679]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        jne   .Lx678_240
                        add              rsp, 16
                                                                                        jmp   n351_lit_integer_β
.Lx678_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n353_statement_end_α
n352_call_β:
                        add              rsp, 16
                                                                                        jmp   n351_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n353_statement_end_α:
                                                                                        jmp   n354_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n354_statement_begin_α:
                                                                                        jmp   n355_statement_end_α
n354_statement_begin_β:
                                                                                        jmp   n356_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n355_statement_end_α:
                                                                                        jmp   n356_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n356_statement_begin_α:
                                                                                        jmp   n357_call_α
n356_statement_begin_β:
                                                                                        jmp   n359_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n357_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx689_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx689_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx689_6]
                        lea              rdx, [rip + .Lx689_7]
                                                                                        jmp   rax
.Lx689_6:
                        mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx689_2
.Lx689_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx689_2
.Lx689_5:
                        add              rsp, 16
.Lx689_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx689_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n356_statement_begin_β
.Lx689_240:
                                                                                        jmp   n358_statement_end_α
n357_call_β:
                                                                                        jmp   n356_statement_begin_β
.Lx689_0:
                        .quad            .Lx689_0_s
.Lx689_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n358_statement_end_α:
                                                                                        jmp   n359_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n359_statement_begin_α:
                                                                                        jmp   n360_lit_string_α
n359_statement_begin_β:
                                                                                        jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n360_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx694_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n361_call_α
n360_lit_string_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n359_statement_begin_β
.Lx694_0:
                        .quad            .Lx694_0_s
.Lx694_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n361_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx696_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx696_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx696_6]
                        lea              rdx, [rip + .Lx696_7]
                                                                                        jmp   rax
.Lx696_6:
                        mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx696_2
.Lx696_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx696_2
.Lx696_5:
                        add              rsp, 32
.Lx696_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx696_240
                        add              rsp, 16
                                                                                        jmp   n360_lit_string_β
.Lx696_240:
                                                                                        jmp   n362_statement_end_α
n361_call_β:
                                                                                        jmp   n360_lit_string_β
.Lx696_0:
                        .quad            .Lx696_0_s
.Lx696_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_end_α:
                                                                                        jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n363_statement_begin_α:
                                                                                        jmp   n364_lit_string_α
n363_statement_begin_β:
                                                                                        jmp   n367_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx701_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n365_call_α
n364_lit_string_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n363_statement_begin_β
.Lx701_0:
                        .quad            .Lx701_0_s
.Lx701_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n365_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx703_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx703_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx703_6]
                        lea              rdx, [rip + .Lx703_7]
                                                                                        jmp   rax
.Lx703_6:
                        mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx703_2
.Lx703_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx703_2
.Lx703_5:
                        add              rsp, 32
.Lx703_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx703_240
                        add              rsp, 16
                                                                                        jmp   n364_lit_string_β
.Lx703_240:
                                                                                        jmp   n366_statement_end_α
n365_call_β:
                                                                                        jmp   n364_lit_string_β
.Lx703_0:
                        .quad            .Lx703_0_s
.Lx703_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n366_statement_end_α:
                                                                                        jmp   n367_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n367_statement_begin_α:
                                                                                        jmp   n368_lit_string_α
n367_statement_begin_β:
                                                                                        jmp   n371_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n368_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx708_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n369_call_α
n368_lit_string_β:
                        add              rsp, 16
                        add              rsp, 128
                                                                                        jmp   n367_statement_begin_β
.Lx708_0:
                        .quad            .Lx708_0_s
.Lx708_0_s:
                        .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n369_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx710_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx710_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx710_6]
                        lea              rdx, [rip + .Lx710_7]
                                                                                        jmp   rax
.Lx710_6:
                        mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx710_2
.Lx710_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx710_2
.Lx710_5:
                        add              rsp, 32
.Lx710_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx710_240
                        add              rsp, 16
                                                                                        jmp   n368_lit_string_β
.Lx710_240:
                                                                                        jmp   n370_statement_end_α
n369_call_β:
                                                                                        jmp   n368_lit_string_β
.Lx710_0:
                        .quad            .Lx710_0_s
.Lx710_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n370_statement_end_α:
                                                                                        jmp   n371_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n371_statement_begin_α:
                                                                                        jmp   n372_call_α
n371_statement_begin_β:
                                                                                        jmp   n375_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n372_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx716_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx716_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx716_6]
                        lea              rdx, [rip + .Lx716_7]
                                                                                        jmp   rax
.Lx716_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx716_2
.Lx716_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx716_2
.Lx716_5:
                        add              rsp, 32
.Lx716_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx716_240
                        add              rsp, 16
                        add              rsp, 160
                                                                                        jmp   n371_statement_begin_β
.Lx716_240:
                                                                                        jmp   n373_assign_α
n372_call_β:
                                                                                        jmp   n371_statement_begin_β
.Lx716_0:
                        .quad            .Lx716_0_s
.Lx716_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n373_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx717_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n374_statement_end_α
.Lx717_0:
                        .quad            .Lx717_0_s
.Lx717_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n374_statement_end_α:
                                                                                        jmp   n375_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n375_statement_begin_α:
                                                                                        jmp   n376_call_α
n375_statement_begin_β:
                                                                                        jmp   n379_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n376_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx723_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx723_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx723_6]
                        lea              rdx, [rip + .Lx723_7]
                                                                                        jmp   rax
.Lx723_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx723_2
.Lx723_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx723_2
.Lx723_5:
                        add              rsp, 32
.Lx723_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx723_240
                        add              rsp, 16
                        add              rsp, 176
                                                                                        jmp   n375_statement_begin_β
.Lx723_240:
                                                                                        jmp   n377_assign_α
n376_call_β:
                                                                                        jmp   n375_statement_begin_β
.Lx723_0:
                        .quad            .Lx723_0_s
.Lx723_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n377_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx724_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n378_statement_end_α
.Lx724_0:
                        .quad            .Lx724_0_s
.Lx724_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n378_statement_end_α:
                                                                                        jmp   n379_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n379_statement_begin_α:
                                                                                        jmp   n380_call_α
n379_statement_begin_β:
                                                                                        jmp   n383_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n380_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx730_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx730_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx730_6]
                        lea              rdx, [rip + .Lx730_7]
                                                                                        jmp   rax
.Lx730_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx730_2
.Lx730_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx730_2
.Lx730_5:
                        add              rsp, 32
.Lx730_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx730_240
                        add              rsp, 16
                        add              rsp, 192
                                                                                        jmp   n379_statement_begin_β
.Lx730_240:
                                                                                        jmp   n381_assign_α
n380_call_β:
                                                                                        jmp   n379_statement_begin_β
.Lx730_0:
                        .quad            .Lx730_0_s
.Lx730_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n381_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx731_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n382_statement_end_α
.Lx731_0:
                        .quad            .Lx731_0_s
.Lx731_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n382_statement_end_α:
                                                                                        jmp   n383_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n383_statement_begin_α:
                                                                                        jmp   n384_call_α
n383_statement_begin_β:
                                                                                        jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n384_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx737_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx737_6]
                        lea              rdx, [rip + .Lx737_7]
                                                                                        jmp   rax
.Lx737_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        add              rsp, 32
.Lx737_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx737_240
                        add              rsp, 16
                        add              rsp, 208
                                                                                        jmp   n383_statement_begin_β
.Lx737_240:
                                                                                        jmp   n385_assign_α
n384_call_β:
                                                                                        jmp   n383_statement_begin_β
.Lx737_0:
                        .quad            .Lx737_0_s
.Lx737_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n385_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx738_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n386_statement_end_α
.Lx738_0:
                        .quad            .Lx738_0_s
.Lx738_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n386_statement_end_α:
                                                                                        jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n387_statement_begin_α:
                                                                                        jmp   n388_call_α
n387_statement_begin_β:
                                                                                        jmp   n391_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n388_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx744_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx744_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx744_6]
                        lea              rdx, [rip + .Lx744_7]
                                                                                        jmp   rax
.Lx744_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx744_2
.Lx744_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx744_2
.Lx744_5:
                        add              rsp, 32
.Lx744_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx744_240
                        add              rsp, 16
                        add              rsp, 224
                                                                                        jmp   n387_statement_begin_β
.Lx744_240:
                                                                                        jmp   n389_assign_α
n388_call_β:
                                                                                        jmp   n387_statement_begin_β
.Lx744_0:
                        .quad            .Lx744_0_s
.Lx744_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n389_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx745_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n390_statement_end_α
.Lx745_0:
                        .quad            .Lx745_0_s
.Lx745_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n390_statement_end_α:
                                                                                        jmp   n391_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n391_statement_begin_α:
                                                                                        jmp   n392_call_α
n391_statement_begin_β:
                                                                                        jmp   n395_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n392_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx751_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx751_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx751_6]
                        lea              rdx, [rip + .Lx751_7]
                                                                                        jmp   rax
.Lx751_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx751_2
.Lx751_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx751_2
.Lx751_5:
                        add              rsp, 32
.Lx751_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx751_240
                        add              rsp, 16
                        add              rsp, 240
                                                                                        jmp   n391_statement_begin_β
.Lx751_240:
                                                                                        jmp   n393_assign_α
n392_call_β:
                                                                                        jmp   n391_statement_begin_β
.Lx751_0:
                        .quad            .Lx751_0_s
.Lx751_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n393_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx752_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n394_statement_end_α
.Lx752_0:
                        .quad            .Lx752_0_s
.Lx752_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n394_statement_end_α:
                                                                                        jmp   n395_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n395_statement_begin_α:
                                                                                        jmp   n396_statement_end_α
n395_statement_begin_β:
                                                                                        jmp   n397_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n396_statement_end_α:
                                                                                        jmp   n397_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n397_statement_begin_α:
                                                                                        jmp   n398_call_α
n397_statement_begin_β:
                        add              rsp, 256
                                                                                        jmp   n404_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n398_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx762_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx762_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx762_6]
                        lea              rdx, [rip + .Lx762_7]
                                                                                        jmp   rax
.Lx762_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx762_2
.Lx762_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx762_2
.Lx762_5:
                        add              rsp, 32
.Lx762_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx762_240
                        add              rsp, 16
                        add              rsp, 256
                                                                                        jmp   n397_statement_begin_β
.Lx762_240:
                                                                                        jmp   n399_statement_end_α
n398_call_β:
                                                                                        jmp   n397_statement_begin_β
.Lx762_0:
                        .quad            .Lx762_0_s
.Lx762_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n399_statement_end_α:
                                                                                        jmp   n400_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n400_statement_begin_α:
                                                                                        jmp   n401_lit_string_α
n400_statement_begin_β:
                                                                                        jmp   n408_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n401_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx767_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n402_assign_α
.Lx767_0:
                        .quad            .Lx767_0_s
.Lx767_0_s:
                        .string          "FAIL: empty pop should FRETURN"
#-----------------------------------------------------------------------------------------------------------------------
n402_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx768_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n403_statement_end_α
.Lx768_0:
                        .quad            .Lx768_0_s
.Lx768_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n403_statement_end_α:
                                                                                        jmp   n408_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n404_statement_begin_α:
                                                                                        jmp   n405_lit_string_α
n404_statement_begin_β:
                                                                                        jmp   n408_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n405_lit_string_α:
                        mov              qword ptr [rsp + 1824], 2                      # result
                        mov              dword ptr [rsp + 1828], 8
                        mov              rax, qword ptr [rip + .Lx773_0]
                        mov              qword ptr [rsp + 1832], rax
                                                                                        jmp   n406_assign_α
.Lx773_0:
                        .quad            .Lx773_0_s
.Lx773_0_s:
                        .string          "empty ok"
#-----------------------------------------------------------------------------------------------------------------------
n406_assign_α:
                        mov              rsi, qword ptr [rsp + 1824]
                        mov              rdx, qword ptr [rsp + 1832]
                        mov              rdi, qword ptr [rip + .Lx774_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n407_statement_end_α
.Lx774_0:
                        .quad            .Lx774_0_s
.Lx774_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n407_statement_end_α:
                                                                                        jmp   n408_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n408_statement_begin_α:
                                                                                        jmp   n409_statement_end_α
n408_statement_begin_β:
                                                                                        jmp   n410_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n409_statement_end_α:
                                                                                        jmp   n410_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n410_statement_begin_α:
                                                                                        jmp   n411_call_α
n410_statement_begin_β:
                                                                                        jmp   n413_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n411_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx784_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx784_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx784_6]
                        lea              rdx, [rip + .Lx784_7]
                                                                                        jmp   rax
.Lx784_6:
                        mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx784_2
.Lx784_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx784_2
.Lx784_5:
                        add              rsp, 16
.Lx784_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx784_240
                        add              rsp, 16
                        add              rsp, 288
                                                                                        jmp   n410_statement_begin_β
.Lx784_240:
                                                                                        jmp   n412_statement_end_α
n411_call_β:
                                                                                        jmp   n410_statement_begin_β
.Lx784_0:
                        .quad            .Lx784_0_s
.Lx784_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n412_statement_end_α:
                                                                                        jmp   n413_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n413_statement_begin_α:
                                                                                        jmp   n414_lit_string_α
n413_statement_begin_β:
                                                                                        jmp   n417_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n414_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx789_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n415_call_α
n414_lit_string_β:
                        add              rsp, 16
                        add              rsp, 304
                                                                                        jmp   n413_statement_begin_β
.Lx789_0:
                        .quad            .Lx789_0_s
.Lx789_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n415_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx791_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx791_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx791_6]
                        lea              rdx, [rip + .Lx791_7]
                                                                                        jmp   rax
.Lx791_6:
                        mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx791_2
.Lx791_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx791_2
.Lx791_5:
                        add              rsp, 32
.Lx791_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx791_240
                        add              rsp, 16
                                                                                        jmp   n414_lit_string_β
.Lx791_240:
                                                                                        jmp   n416_statement_end_α
n415_call_β:
                                                                                        jmp   n414_lit_string_β
.Lx791_0:
                        .quad            .Lx791_0_s
.Lx791_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n416_statement_end_α:
                                                                                        jmp   n417_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n417_statement_begin_α:
                                                                                        jmp   n418_call_α
n417_statement_begin_β:
                                                                                        jmp   n421_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n418_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]                       # stack_peek
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx797_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx797_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx797_6]
                        lea              rdx, [rip + .Lx797_7]
                                                                                        jmp   rax
.Lx797_6:
                        mov              rdi, qword ptr [r9 + 80]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx797_2
.Lx797_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx797_2
.Lx797_5:
                        add              rsp, 16
.Lx797_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx797_240
                        add              rsp, 16
                        add              rsp, 336
                                                                                        jmp   n417_statement_begin_β
.Lx797_240:
                                                                                        jmp   n419_assign_α
n418_call_β:
                                                                                        jmp   n417_statement_begin_β
.Lx797_0:
                        .quad            .Lx797_0_s
.Lx797_0_s:
                        .string          "stack_peek"
#-----------------------------------------------------------------------------------------------------------------------
n419_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx798_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n420_statement_end_α
.Lx798_0:
                        .quad            .Lx798_0_s
.Lx798_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n420_statement_end_α:
                                                                                        jmp   n421_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n421_statement_begin_α:
                                                                                        jmp   n422_call_α
n421_statement_begin_β:
                                                                                        jmp   n425_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n422_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx804_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx804_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx804_6]
                        lea              rdx, [rip + .Lx804_7]
                                                                                        jmp   rax
.Lx804_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx804_2
.Lx804_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 136], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx804_2
.Lx804_5:
                        add              rsp, 32
.Lx804_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx804_240
                        add              rsp, 16
                        add              rsp, 352
                                                                                        jmp   n421_statement_begin_β
.Lx804_240:
                                                                                        jmp   n423_assign_α
n422_call_β:
                                                                                        jmp   n421_statement_begin_β
.Lx804_0:
                        .quad            .Lx804_0_s
.Lx804_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n423_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx805_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n424_statement_end_α
.Lx805_0:
                        .quad            .Lx805_0_s
.Lx805_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n424_statement_end_α:
                                                                                        jmp   n425_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n425_statement_begin_α:
                                                                                        jmp   n426_call_α
n425_statement_begin_β:
                                                                                        jmp   n429_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n426_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx811_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx811_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx811_6]
                        lea              rdx, [rip + .Lx811_7]
                                                                                        jmp   rax
.Lx811_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx811_2
.Lx811_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx811_2
.Lx811_5:
                        add              rsp, 32
.Lx811_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx811_240
                        add              rsp, 16
                        add              rsp, 368
                                                                                        jmp   n425_statement_begin_β
.Lx811_240:
                                                                                        jmp   n427_assign_α
n426_call_β:
                                                                                        jmp   n425_statement_begin_β
.Lx811_0:
                        .quad            .Lx811_0_s
.Lx811_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n427_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx812_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n428_statement_end_α
.Lx812_0:
                        .quad            .Lx812_0_s
.Lx812_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n428_statement_end_α:
                                                                                        jmp   n429_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n429_statement_begin_α:
                                                                                        jmp   n430_statement_end_α
n429_statement_begin_β:
                                                                                        jmp   n431_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n430_statement_end_α:
                                                                                        jmp   n431_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n431_statement_begin_α:
                                                                                        jmp   n432_call_α
n431_statement_begin_β:
                                                                                        jmp   n434_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n432_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx822_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx822_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx822_6]
                        lea              rdx, [rip + .Lx822_7]
                                                                                        jmp   rax
.Lx822_6:
                        mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx822_2
.Lx822_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx822_2
.Lx822_5:
                        add              rsp, 16
.Lx822_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx822_240
                        add              rsp, 16
                        add              rsp, 384
                                                                                        jmp   n431_statement_begin_β
.Lx822_240:
                                                                                        jmp   n433_statement_end_α
n432_call_β:
                                                                                        jmp   n431_statement_begin_β
.Lx822_0:
                        .quad            .Lx822_0_s
.Lx822_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n433_statement_end_α:
                                                                                        jmp   n434_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n434_statement_begin_α:
                                                                                        jmp   n435_lit_integer_α
n434_statement_begin_β:
                                                                                        jmp   n438_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n435_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx827_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n436_call_α
n435_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 400
                                                                                        jmp   n434_statement_begin_β
.Lx827_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n436_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx829_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx829_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx829_6]
                        lea              rdx, [rip + .Lx829_7]
                                                                                        jmp   rax
.Lx829_6:
                        mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx829_2
.Lx829_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx829_2
.Lx829_5:
                        add              rsp, 32
.Lx829_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx829_240
                        add              rsp, 16
                                                                                        jmp   n435_lit_integer_β
.Lx829_240:
                                                                                        jmp   n437_statement_end_α
n436_call_β:
                                                                                        jmp   n435_lit_integer_β
.Lx829_0:
                        .quad            .Lx829_0_s
.Lx829_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n437_statement_end_α:
                                                                                        jmp   n438_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n438_statement_begin_α:
                                                                                        jmp   n439_lit_integer_α
n438_statement_begin_β:
                                                                                        jmp   n442_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n439_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx834_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n440_call_α
n439_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 432
                                                                                        jmp   n438_statement_begin_β
.Lx834_0:
                        .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n440_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx836_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx836_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx836_6]
                        lea              rdx, [rip + .Lx836_7]
                                                                                        jmp   rax
.Lx836_6:
                        mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx836_2
.Lx836_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx836_2
.Lx836_5:
                        add              rsp, 32
.Lx836_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx836_240
                        add              rsp, 16
                                                                                        jmp   n439_lit_integer_β
.Lx836_240:
                                                                                        jmp   n441_statement_end_α
n440_call_β:
                                                                                        jmp   n439_lit_integer_β
.Lx836_0:
                        .quad            .Lx836_0_s
.Lx836_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n441_statement_end_α:
                                                                                        jmp   n442_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n442_statement_begin_α:
                                                                                        jmp   n443_lit_string_α
n442_statement_begin_β:
                                                                                        jmp   n446_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n443_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx841_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n444_call_α
n443_lit_string_β:
                        add              rsp, 16
                        add              rsp, 464
                                                                                        jmp   n442_statement_begin_β
.Lx841_0:
                        .quad            .Lx841_0_s
.Lx841_0_s:
                        .string          "myvar"
#-----------------------------------------------------------------------------------------------------------------------
n444_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx843_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx843_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx843_6]
                        lea              rdx, [rip + .Lx843_7]
                                                                                        jmp   rax
.Lx843_6:
                        mov              rdi, qword ptr [r9 + 48]                       # stack_pop
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx843_2
.Lx843_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx843_2
.Lx843_5:
                        add              rsp, 32
.Lx843_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx843_240
                        add              rsp, 16
                                                                                        jmp   n443_lit_string_β
.Lx843_240:
                                                                                        jmp   n445_statement_end_α
n444_call_β:
                                                                                        jmp   n443_lit_string_β
.Lx843_0:
                        .quad            .Lx843_0_s
.Lx843_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n445_statement_end_α:
                                                                                        jmp   n446_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n446_statement_begin_α:
                                                                                        jmp   n447_var_α
n446_statement_begin_β:
                                                                                        jmp   n450_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n447_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]                      # myvar
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n448_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n448_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx849_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n449_statement_end_α
.Lx849_0:
                        .quad            .Lx849_0_s
.Lx849_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n449_statement_end_α:
                                                                                        jmp   n450_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n450_statement_begin_α:
                                                                                        jmp   n451_statement_end_α
n450_statement_begin_β:
                                                                                        jmp   n452_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n451_statement_end_α:
                                                                                        jmp   n452_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n452_statement_begin_α:
                                                                                        jmp   n453_call_α
n452_statement_begin_β:
                                                                                        jmp   n455_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n453_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx859_0]
                        mov              esi, 0
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx859_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx859_6]
                        lea              rdx, [rip + .Lx859_7]
                                                                                        jmp   rax
.Lx859_6:
                        mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx859_2
.Lx859_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        add              rsp, 16
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx859_2
.Lx859_5:
                        add              rsp, 16
.Lx859_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx859_240
                        add              rsp, 16
                        add              rsp, 512
                                                                                        jmp   n452_statement_begin_β
.Lx859_240:
                                                                                        jmp   n454_statement_end_α
n453_call_β:
                                                                                        jmp   n452_statement_begin_β
.Lx859_0:
                        .quad            .Lx859_0_s
.Lx859_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n454_statement_end_α:
                                                                                        jmp   n455_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n455_statement_begin_α:
                                                                                        jmp   n456_lit_string_α
n455_statement_begin_β:
                                                                                        jmp   n459_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n456_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx864_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n457_assign_α
.Lx864_0:
                        .quad            .Lx864_0_s
.Lx864_0_s:
                        .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n457_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax                      # subject
                        mov              qword ptr [r9 + 200], rdx
                                                                                        jmp   n458_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n458_statement_end_α:
                                                                                        jmp   n459_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n459_statement_begin_α:
                                                                                        jmp   n460_var_α
n459_statement_begin_β:
                        add              rsp, 544
                                                                                        jmp   n491_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n460_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]                      # subject
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n461_match_begin_α
n460_var_β:
                        add              rsp, 16
                        add              rsp, 544
                                                                                        jmp   n491_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n461_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 2560], rdi
                        mov              qword ptr [rsp + 2568], rsi
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
.Lx872_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n462_match_assign_save_α
n461_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx872_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx872_1
                                                                                        jmp   .Lx872_0
.Lx872_1:
n461_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n460_var_β
#-----------------------------------------------------------------------------------------------------------------------
n462_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n463_match_break_α
n462_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n461_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n463_match_break_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx876_0:
                        cmp              ecx, r15d
                                                                                        jl    .Lx876_237
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n461_match_begin_β
.Lx876_237:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx876_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx876_238
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n461_match_begin_β
.Lx876_238:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx876_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx876_239
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n461_match_begin_β
.Lx876_239:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx876_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx876_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n461_match_begin_β
.Lx876_240:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx876_1
                        add              ecx, 1
                                                                                        jmp   .Lx876_0
.Lx876_1:
                        mov              dword ptr [rsp + 16], r14d
                        mov              r14d, ecx
                                                                                        jmp   n464_match_assign_cond_α
n463_match_break_β:
                        mov              r14d, dword ptr [rsp + 16]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n461_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n464_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n465_match_lit_α
n464_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n463_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n465_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n464_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n464_match_assign_cond_β
                        add              r14d, 1
                                                                                        jmp   n466_match_assign_save_α
n465_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n464_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n466_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n467_match_rem_α
n466_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n465_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n467_match_rem_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d
                                                                                        jmp   n468_match_assign_cond_α
n467_match_rem_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n465_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n468_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n469_match_end_α
n468_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n467_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n469_match_end_α:
                        mov              r10, r12
.Lx887_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx887_9
                        mov              eax, dword ptr [rsp + 64]
                        mov              dword ptr [rsp + 2448], eax
                        mov              qword ptr [rsp + 2472], r14
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx887_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx887_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx887_1:
                        test             rax, rax
                                                                                        je    .Lx887_2
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx887_3]
                        lea              rdx, [rip + .Lx887_4]
                                                                                        jmp   rax
.Lx887_3:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx887_1
.Lx887_4:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx887_1
.Lx887_2:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
.Lx887_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx887_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n470_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n470_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx888_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n471_match_replace_α
.Lx888_0:
                        .quad            .Lx888_0_s
.Lx888_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n471_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx890_0]
                        mov              rsi, qword ptr [rsp + 2576]
                        mov              rdx, qword ptr [rsp + 2584]
                        mov              ecx, dword ptr [rsp + 2304]
                        mov              r8, qword ptr [rsp + 2328]
                        lea              r9, [rsp + 0]                                  # lit_string
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx890_1
.Lx890_0:
                        .quad            .Lx890_0_s
.Lx890_0_s:
                        .string          "subject"
.Lx890_1:
                                                                                        jmp   n472_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n472_statement_end_α:
                                                                                        jmp   n473_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n473_statement_begin_α:
                                                                                        jmp   n474_var_α
n473_statement_begin_β:
                                                                                        jmp   n477_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n474_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]                      # w1
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n475_call_α
n474_var_β:
                        add              rsp, 16
                        add              rsp, 640
                                                                                        jmp   n473_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n475_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx897_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx897_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx897_6]
                        lea              rdx, [rip + .Lx897_7]
                                                                                        jmp   rax
.Lx897_6:
                        mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx897_2
.Lx897_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx897_2
.Lx897_5:
                        add              rsp, 32
.Lx897_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx897_240
                        add              rsp, 16
                                                                                        jmp   n474_var_β
.Lx897_240:
                                                                                        jmp   n476_statement_end_α
n475_call_β:
                                                                                        jmp   n474_var_β
.Lx897_0:
                        .quad            .Lx897_0_s
.Lx897_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n476_statement_end_α:
                                                                                        jmp   n477_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n477_statement_begin_α:
                                                                                        jmp   n478_var_α
n477_statement_begin_β:
                                                                                        jmp   n481_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n478_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # w2
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n479_call_α
n478_var_β:
                        add              rsp, 16
                        add              rsp, 672
                                                                                        jmp   n477_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n479_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]                      # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 32
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx904_0]
                        mov              esi, 1
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx904_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx904_6]
                        lea              rdx, [rip + .Lx904_7]
                                                                                        jmp   rax
.Lx904_6:
                        mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx904_2
.Lx904_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx904_2
.Lx904_5:
                        add              rsp, 32
.Lx904_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx904_240
                        add              rsp, 16
                                                                                        jmp   n478_var_β
.Lx904_240:
                                                                                        jmp   n480_statement_end_α
n479_call_β:
                                                                                        jmp   n478_var_β
.Lx904_0:
                        .quad            .Lx904_0_s
.Lx904_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n480_statement_end_α:
                                                                                        jmp   n481_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n481_statement_begin_α:
                                                                                        jmp   n482_call_α
n481_statement_begin_β:
                                                                                        jmp   n485_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n482_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx910_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx910_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx910_6]
                        lea              rdx, [rip + .Lx910_7]
                                                                                        jmp   rax
.Lx910_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx910_2
.Lx910_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx910_2
.Lx910_5:
                        add              rsp, 32
.Lx910_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx910_240
                        add              rsp, 16
                        add              rsp, 704
                                                                                        jmp   n481_statement_begin_β
.Lx910_240:
                                                                                        jmp   n483_assign_α
n482_call_β:
                                                                                        jmp   n481_statement_begin_β
.Lx910_0:
                        .quad            .Lx910_0_s
.Lx910_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n483_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx911_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n484_statement_end_α
.Lx911_0:
                        .quad            .Lx911_0_s
.Lx911_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n484_statement_end_α:
                                                                                        jmp   n485_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n485_statement_begin_α:
                                                                                        jmp   n486_call_α
n485_statement_begin_β:
                                                                                        jmp   n489_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n486_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx917_0]
                        mov              esi, 1
                        mov              edx, 0
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    .Lx917_5
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx917_6]
                        lea              rdx, [rip + .Lx917_7]
                                                                                        jmp   rax
.Lx917_6:
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
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx917_2
.Lx917_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        add              rsp, 32
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx917_2
.Lx917_5:
                        add              rsp, 32
.Lx917_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx917_240
                        add              rsp, 16
                        add              rsp, 720
                                                                                        jmp   n485_statement_begin_β
.Lx917_240:
                                                                                        jmp   n487_assign_α
n486_call_β:
                                                                                        jmp   n485_statement_begin_β
.Lx917_0:
                        .quad            .Lx917_0_s
.Lx917_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n487_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx918_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n488_statement_end_α
.Lx918_0:
                        .quad            .Lx918_0_s
.Lx918_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n488_statement_end_α:
                                                                                        jmp   n489_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n489_statement_begin_α:
                                                                                        jmp   n490_statement_end_α
n489_statement_begin_β:
                        add              rsp, 736
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n490_statement_end_α:
                        add              rsp, 560
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n491_statement_begin_α:
                                                                                        jmp   n492_lit_string_α
n491_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n492_lit_string_α:
                        mov              qword ptr [rsp + 2768], 2                      # result
                        mov              dword ptr [rsp + 2772], 26
                        mov              rax, qword ptr [rip + .Lx927_0]
                        mov              qword ptr [rsp + 2776], rax
                                                                                        jmp   n493_assign_α
.Lx927_0:
                        .quad            .Lx927_0_s
.Lx927_0_s:
                        .string          "FAIL: pattern match failed"
#-----------------------------------------------------------------------------------------------------------------------
n493_assign_α:
                        mov              rsi, qword ptr [rsp + 2768]
                        mov              rdx, qword ptr [rsp + 2776]
                        mov              rdi, qword ptr [rip + .Lx928_0]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
.Lx928_0:
                        .quad            .Lx928_0_s
.Lx928_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n494_statement_end_α:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n495_goto_α:
                                                                                        jmp   n496_statement_begin_α
n495_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n496_statement_begin_α:
                                                                                        jmp   n497_lit_string_α
n496_statement_begin_β:
                                                                                        jmp   n500_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n497_lit_string_α:
                        mov              qword ptr [rsp + 176], 2                       # result
                        mov              dword ptr [rsp + 180], 0
                        mov              rax, qword ptr [rip + .Lx934_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n498_assign_α
.Lx934_0:
                        .quad            .Lx934_0_s
.Lx934_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n498_assign_α:
                        mov              rax, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n499_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n499_statement_end_α:
                                                                                        jmp   n500_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n500_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx939_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx939_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n501_goto_α:
                                                                                        jmp   n325_statement_begin_α
n501_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n502_goto_α:
                                                                                        jmp   n503_statement_begin_α
n502_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n503_statement_begin_α:
                                                                                        jmp   n504_var_α
n503_statement_begin_β:
                                                                                        jmp   n509_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n504_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 256], rax                     # result
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n505_var_α
#-----------------------------------------------------------------------------------------------------------------------
n505_var_α:
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 272], rax                     # result
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n506_call_α
#-----------------------------------------------------------------------------------------------------------------------
n506_call_α:
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 224], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 232], rax
                        mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 264]
                        mov              qword ptr [rsp + 216], rax
                        .section         .rodata
.Lrkfn947:              .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn947]
                        lea              rsi, [rsp + 208]
                        mov              edx, 2
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                        cmp              eax, 104
                                                                                        je    n503_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n507_assign_α
n506_call_β:
                                                                                        jmp   n503_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n507_assign_α:
                        mov              rax, qword ptr [rsp + 192]
                        mov              rdx, qword ptr [rsp + 200]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n508_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n508_statement_end_α:
                                                                                        jmp   n509_statement_begin_α
#=======================================================================================================================
#         stack_push(w1)
#-----------------------------------------------------------------------------------------------------------------------
n509_statement_begin_α:
                                                                                        jmp   n510_var_α
n509_statement_begin_β:
                                                                                        jmp   n514_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n510_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n511_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n511_field_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx954_0]
                        mov              rsi, qword ptr [rsp + 16]                      # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        jne   .Lx954_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n509_statement_begin_β
.Lx954_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n512_assign_α
.Lx954_0:
                        .quad            .Lx954_0_s
.Lx954_0_s:
                        .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n512_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              qword ptr [r9 + 24], rdx
                                                                                        jmp   n513_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n513_statement_end_α:
                                                                                        jmp   n514_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n514_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx958_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n515_call_α
.Lx958_0:
                        .quad            .Lx958_0_s
.Lx958_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n515_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd960:            .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd960]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        jne   .Lx959_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n516_save_restore_α
.Lx959_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n500_save_restore_α
n515_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n516_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n516_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx962_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx962_0:
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n517_goto_α:
                                                                                        jmp   n329_statement_begin_α
n517_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n518_goto_α:
                                                                                        jmp   n519_statement_begin_α
n518_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n519_statement_begin_α:
                                                                                        jmp   n520_var_α
n519_statement_begin_β:
                                                                                        jmp   n516_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n520_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 368], rax                     # result
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n521_call_α
#-----------------------------------------------------------------------------------------------------------------------
n521_call_α:
                        mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 376]
                        mov              qword ptr [rsp + 344], rax
                        .section         .rodata
.Lrkfn969:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn969]
                        lea              rsi, [rsp + 336]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                        cmp              eax, 104
                                                                                        je    n519_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n522_statement_end_α
n521_call_β:
                                                                                        jmp   n519_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n522_statement_end_α:
                                                                                        jmp   n523_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n523_statement_begin_α:
                                                                                        jmp   n524_var_α
n523_statement_begin_β:
                                                                                        jmp   n537_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n524_var_α:
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 432], rax                     # result
                        mov              qword ptr [rsp + 440], rdx
                                                                                        jmp   n525_call_α
#-----------------------------------------------------------------------------------------------------------------------
n525_call_α:
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 400], rax
                        mov              rax, qword ptr [rsp + 440]
                        mov              qword ptr [rsp + 408], rax
                        .section         .rodata
.Lrkfn976:              .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn976]
                        lea              rsi, [rsp + 400]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        cmp              eax, 104
                                                                                        je    n523_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n526_statement_end_α
n525_call_β:
                                                                                        jmp   n523_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n526_statement_end_α:
                                                                                        jmp   n527_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n527_statement_begin_α:
                                                                                        jmp   n528_var_α
n527_statement_begin_β:
                                                                                        jmp   n532_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n528_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 496], rax                     # result
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n529_call_α
#-----------------------------------------------------------------------------------------------------------------------
n529_call_α:
                        mov              rax, qword ptr [rsp + 496]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 504]
                        mov              qword ptr [rsp + 472], rax
                        .section         .rodata
.Lrkfn983:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn983]
                        lea              rsi, [rsp + 464]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                        cmp              eax, 104
                                                                                        je    n527_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n530_assign_α
n529_call_β:
                                                                                        jmp   n527_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n530_assign_α:
                        mov              rax, qword ptr [rsp + 448]
                        mov              rdx, qword ptr [rsp + 456]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n531_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n531_statement_end_α:
                                                                                        jmp   n532_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n532_statement_begin_α:
                                                                                        jmp   n533_var_α
n532_statement_begin_β:
                                                                                        jmp   n500_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n533_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 560], rax                     # result
                        mov              qword ptr [rsp + 568], rdx
                                                                                        jmp   n534_call_α
#-----------------------------------------------------------------------------------------------------------------------
n534_call_α:
                        mov              rax, qword ptr [rsp + 560]
                        mov              qword ptr [rsp + 528], rax
                        mov              rax, qword ptr [rsp + 568]
                        mov              qword ptr [rsp + 536], rax
                        .section         .rodata
.Lrkfn991:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn991]
                        lea              rsi, [rsp + 528]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                        cmp              eax, 104
                                                                                        je    n532_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n535_assign_α
n534_call_β:
                                                                                        jmp   n532_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n535_assign_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              rdx, qword ptr [rsp + 520]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n536_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n536_statement_end_α:
                                                                                        jmp   n500_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n537_statement_begin_α:
                                                                                        jmp   n538_var_α
n537_statement_begin_β:
                                                                                        jmp   n544_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n538_var_α:
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 624], rax                     # result
                        mov              qword ptr [rsp + 632], rdx
                                                                                        jmp   n539_call_α
#-----------------------------------------------------------------------------------------------------------------------
n539_call_α:
                        mov              rax, qword ptr [rsp + 624]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 632]
                        mov              qword ptr [rsp + 600], rax
                        .section         .rodata
.Lrkfn999:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn999]
                        lea              rsi, [rsp + 592]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        cmp              eax, 104
                                                                                        je    n537_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n540_var_α
n539_call_β:
                                                                                        jmp   n537_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n540_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 688], rax                     # result
                        mov              qword ptr [rsp + 696], rdx
                                                                                        jmp   n541_call_α
#-----------------------------------------------------------------------------------------------------------------------
n541_call_α:
                        mov              rax, qword ptr [rsp + 688]
                        mov              qword ptr [rsp + 656], rax
                        mov              rax, qword ptr [rsp + 696]
                        mov              qword ptr [rsp + 664], rax
                        .section         .rodata
.Lrkfn1002:             .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1002]
                        lea              rsi, [rsp + 656]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                        cmp              eax, 104
                                                                                        je    n537_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n542_assign_var_α
n541_call_β:
                                                                                        jmp   n537_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n542_assign_var_α:
                        mov              rdi, qword ptr [rsp + 576]
                        mov              rsi, qword ptr [rsp + 584]
                        mov              rdx, qword ptr [rsp + 640]
                        mov              rcx, qword ptr [rsp + 648]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    n537_statement_begin_β
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                                                                                        jmp   n543_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n543_statement_end_α:
                                                                                        jmp   n544_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n544_statement_begin_α:
                                                                                        jmp   n545_var_α
n544_statement_begin_β:
                                                                                        jmp   n549_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n545_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 768], rax                     # result
                        mov              qword ptr [rsp + 776], rdx
                                                                                        jmp   n546_call_α
#-----------------------------------------------------------------------------------------------------------------------
n546_call_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 744], rax
                        .section         .rodata
.Lrkfn1010:             .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1010]
                        lea              rsi, [rsp + 736]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                        cmp              eax, 104
                                                                                        je    n544_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n547_assign_α
n546_call_β:
                                                                                        jmp   n544_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n547_assign_α:
                        mov              rax, qword ptr [rsp + 720]
                        mov              rdx, qword ptr [rsp + 728]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx
                                                                                        jmp   n548_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n548_statement_end_α:
                                                                                        jmp   n549_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n549_statement_begin_α:
                                                                                        jmp   n550_lit_string_α
n549_statement_begin_β:
                                                                                        jmp   n514_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n550_lit_string_α:
                        mov              qword ptr [rsp + 832], 2                       # result
                        mov              dword ptr [rsp + 836], 5
                        mov              rax, qword ptr [rip + .Lx1016_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n551_call_α
.Lx1016_0:
                        .quad            .Lx1016_0_s
.Lx1016_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n551_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 800], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 808], rax
                        .section         .rodata
.Lrkfn1018:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1018]
                        lea              rsi, [rsp + 800]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                        cmp              eax, 104
                                                                                        je    n549_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n552_assign_α
n551_call_β:
                                                                                        jmp   n549_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n552_assign_α:
                        mov              rax, qword ptr [rsp + 784]
                        mov              rdx, qword ptr [rsp + 792]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              qword ptr [r9 + 56], rdx
                                                                                        jmp   n553_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n553_statement_end_α:
                                                                                        jmp   n514_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n554_goto_α:
                                                                                        jmp   n537_statement_begin_α
n554_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n555_goto_α:
                                                                                        jmp   n333_statement_begin_α
n555_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n556_goto_α:
                                                                                        jmp   n557_statement_begin_α
n556_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n557_statement_begin_α:
                                                                                        jmp   n558_var_α
n557_statement_begin_β:
                                                                                        jmp   n516_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n558_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 896], rax                     # result
                        mov              qword ptr [rsp + 904], rdx
                                                                                        jmp   n559_call_α
#-----------------------------------------------------------------------------------------------------------------------
n559_call_α:
                        mov              rax, qword ptr [rsp + 896]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 904]
                        mov              qword ptr [rsp + 872], rax
                        .section         .rodata
.Lrkfn1029:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1029]
                        lea              rsi, [rsp + 864]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 104
                                                                                        je    n557_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n560_statement_end_α
n559_call_β:
                                                                                        jmp   n557_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n560_statement_end_α:
                                                                                        jmp   n561_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n561_statement_begin_α:
                                                                                        jmp   n562_var_α
n561_statement_begin_β:
                                                                                        jmp   n500_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n562_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 960], rax                     # result
                        mov              qword ptr [rsp + 968], rdx
                                                                                        jmp   n563_call_α
#-----------------------------------------------------------------------------------------------------------------------
n563_call_α:
                        mov              rax, qword ptr [rsp + 960]
                        mov              qword ptr [rsp + 928], rax
                        mov              rax, qword ptr [rsp + 968]
                        mov              qword ptr [rsp + 936], rax
                        .section         .rodata
.Lrkfn1036:             .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1036]
                        lea              rsi, [rsp + 928]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                        cmp              eax, 104
                                                                                        je    n561_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n564_assign_α
n563_call_β:
                                                                                        jmp   n561_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n564_assign_α:
                        mov              rax, qword ptr [rsp + 912]
                        mov              rdx, qword ptr [rsp + 920]
                        mov              qword ptr [r9 + 80], rax                       # stack_peek
                        mov              qword ptr [r9 + 88], rdx
                                                                                        jmp   n565_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n565_statement_end_α:
                                                                                        jmp   n500_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n566_goto_α:
                                                                                        jmp   n337_statement_begin_α
n566_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n567_goto_α:
                                                                                        jmp   n568_statement_begin_α
n567_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n568_statement_begin_α:
                                                                                        jmp   n569_var_α
n568_statement_begin_β:
                                                                                        jmp   n516_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n569_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1024], rax                    # result
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n570_call_α
#-----------------------------------------------------------------------------------------------------------------------
n570_call_α:
                        mov              rax, qword ptr [rsp + 1024]
                        mov              qword ptr [rsp + 992], rax
                        mov              rax, qword ptr [rsp + 1032]
                        mov              qword ptr [rsp + 1000], rax
                        .section         .rodata
.Lrkfn1046:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1046]
                        lea              rsi, [rsp + 992]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                        cmp              eax, 104
                                                                                        je    n568_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n571_statement_end_α
n570_call_β:
                                                                                        jmp   n568_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n571_statement_end_α:
                                                                                        jmp   n572_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n572_statement_begin_α:
                                                                                        jmp   n573_var_α
n572_statement_begin_β:
                                                                                        jmp   n514_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n573_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1040], rax                    # result
                        mov              qword ptr [rsp + 1048], rdx
                                                                                        jmp   n574_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n574_field_var_α:
                        mov              rdi, qword ptr [rip + .Lx1052_0]
                        mov              rsi, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
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
                                                                                        je    n572_statement_begin_β
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                                                                                        jmp   n575_assign_α
.Lx1052_0:
                        .quad            .Lx1052_0_s
.Lx1052_0_s:
                        .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n575_assign_α:
                        mov              rax, qword ptr [rsp + 1056]
                        mov              rdx, qword ptr [rsp + 1064]
                        mov              qword ptr [r9 + 96], rax                       # stack_top
                        mov              qword ptr [r9 + 104], rdx
                                                                                        jmp   n576_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n576_statement_end_α:
                                                                                        jmp   n514_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n577_goto_α:
                                                                                        jmp   n341_statement_begin_α
n577_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n578_goto_α:
                                                                                        jmp   n579_statement_begin_α
n578_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n579_statement_begin_α:
                                                                                        jmp   n580_lit_integer_α
n579_statement_begin_β:
                                                                                        jmp   n583_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n580_lit_integer_α:
                        mov              qword ptr [rsp + 1072], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1060_0]
                        mov              qword ptr [rsp + 1080], rax
                                                                                        jmp   n581_assign_α
.Lx1060_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n581_assign_α:
                        mov              rax, qword ptr [rsp + 1072]
                        mov              rdx, qword ptr [rsp + 1080]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              qword ptr [r9 + 120], rdx
                                                                                        jmp   n582_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n582_statement_end_α:
                                                                                        jmp   n583_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n583_statement_begin_α:
                                                                                        jmp   n584_var_α
n583_statement_begin_β:
                                                                                        jmp   n587_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n584_var_α:
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1088], rax                    # result
                        mov              qword ptr [rsp + 1096], rdx
                                                                                        jmp   n585_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n585_assign_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              rdx, qword ptr [rsp + 1096]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n586_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n586_statement_end_α:
                                                                                        jmp   n587_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n587_statement_begin_α:
                                                                                        jmp   n588_var_α
n587_statement_begin_β:
                                                                                        jmp   n500_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n588_var_α:
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 1152], rax                    # result
                        mov              qword ptr [rsp + 1160], rdx
                                                                                        jmp   n589_call_α
#-----------------------------------------------------------------------------------------------------------------------
n589_call_α:
                        mov              rax, qword ptr [rsp + 1152]
                        mov              qword ptr [rsp + 1120], rax
                        mov              rax, qword ptr [rsp + 1160]
                        mov              qword ptr [rsp + 1128], rax
                        .section         .rodata
.Lrkfn1074:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1074]
                        lea              rsi, [rsp + 1120]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        cmp              eax, 104
                                                                                        je    n587_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n590_statement_end_α
n589_call_β:
                                                                                        jmp   n587_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n590_statement_end_α:
                                                                                        jmp   n591_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n591_statement_begin_α:
                                                                                        jmp   n592_var_α
n591_statement_begin_β:
                                                                                        jmp   n597_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n592_var_α:
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 1184], rax                    # result
                        mov              qword ptr [rsp + 1192], rdx
                                                                                        jmp   n593_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n593_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 3                      # result
                        mov              rax, qword ptr [rip + .Lx1080_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n594_binop_α
.Lx1080_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n594_binop_α:
                        mov              eax, dword ptr [rsp + 1184]
                        cmp              eax, 3
                                                                                        jne   .Lx1081_0
                        mov              rax, qword ptr [rsp + 1192]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rsp + 1168], 3
                        mov              qword ptr [rsp + 1176], rax
                                                                                        jmp   n595_assign_α
.Lx1081_0:
                        mov              rdi, qword ptr [rsp + 1184]
                        mov              rsi, qword ptr [rsp + 1192]
                        mov              rdx, qword ptr [rsp + 1200]
                        mov              rcx, qword ptr [rsp + 1208]
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1081_240
                        add              rsp, 32
                                                                                        jmp   n591_statement_begin_β
.Lx1081_240:
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n595_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n595_assign_α:
                        mov              rax, qword ptr [rsp + 1168]
                        mov              rdx, qword ptr [rsp + 1176]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              qword ptr [r9 + 120], rdx
                                                                                        jmp   n596_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n596_statement_end_α:
                                                                                        jmp   n597_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n597_statement_begin_α:
                                                                                        jmp   n598_var_α
n597_statement_begin_β:
                                                                                        jmp   n587_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n598_var_α:
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 1264], rax                    # result
                        mov              qword ptr [rsp + 1272], rdx
                                                                                        jmp   n599_call_α
#-----------------------------------------------------------------------------------------------------------------------
n599_call_α:
                        mov              rax, qword ptr [rsp + 1264]
                        mov              qword ptr [rsp + 1232], rax
                        mov              rax, qword ptr [rsp + 1272]
                        mov              qword ptr [rsp + 1240], rax
                        .section         .rodata
.Lrkfn1089:             .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn1089]
                        lea              rsi, [rsp + 1232]
                        mov              edx, 1
                        mov              qword ptr [g_rtcc_block + 0], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 8],  rcx
                        mov              qword ptr [rax + 16], rdx
                        mov              qword ptr [rax + 24], rsi
                        mov              qword ptr [rax + 32], rdi
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                        cmp              eax, 104
                                                                                        je    n597_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n600_assign_α
n599_call_β:
                                                                                        jmp   n597_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n600_assign_α:
                        mov              rax, qword ptr [rsp + 1216]
                        mov              rdx, qword ptr [rsp + 1224]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              qword ptr [r9 + 136], rdx
                                                                                        jmp   n601_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n601_statement_end_α:
                                                                                        jmp   n587_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n602_goto_α:
                                                                                        jmp   n587_statement_begin_α
n602_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n603_goto_α:
                                                                                        jmp   n345_statement_begin_α
n603_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n604_goto_α:
                                                                                        jmp   n400_statement_begin_α
n604_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n605_goto_α:
                                                                                        jmp   n408_statement_begin_α
n605_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n606_goto_α:
                                                                                        jmp   n491_statement_begin_α
n606_goto_β:
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

                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_α:
proc_LBL__lwr_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_var_α
n0_statement_begin_β:
                                                                                        jmp   n7_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # lwr
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n2_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx11_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n3_keyword_snobol4_α
.Lx11_0:
                        .quad            .Lx11_0_s
.Lx11_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n3_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx12_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n4_call_α
.Lx12_0:
                        .quad            .Lx12_0_s
.Lx12_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              r10, qword ptr [rsp + 96]
                        mov              r11, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 80]
                        mov              r11, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        .section         .rodata
.Lrkfnzd14:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd14]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 48
                        cmp              eax, 104
                                                                                        jne   .Lx13_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n0_statement_begin_β
.Lx13_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n5_assign_α
n4_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # lwr
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:
                                                                                        jmp   n7_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n7_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx19_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx19_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_β:
                                                                                        jmp   proc_LBL__lwr_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lwr_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_α:
proc_LBL__upr_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:
                                                                                        jmp   n21_var_α
n20_statement_begin_β:
                                                                                        jmp   n27_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # upr
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n22_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n22_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx31_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n23_keyword_snobol4_α
.Lx31_0:
                        .quad            .Lx31_0_s
.Lx31_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n23_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx32_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n24_call_α
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              r10, qword ptr [rsp + 96]
                        mov              r11, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 80]
                        mov              r11, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        .section         .rodata
.Lrkfnzd34:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd34]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 48
                        cmp              eax, 104
                                                                                        jne   .Lx33_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n20_statement_begin_β
.Lx33_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n25_assign_α
n24_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n20_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # upr
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n26_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:
                                                                                        jmp   n27_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx39_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx39_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_β:
                                                                                        jmp   proc_LBL__upr_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__upr_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_α:
proc_LBL__cap_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_begin_α:
                                                                                        jmp   n41_var_α
n40_statement_begin_β:
                                                                                        jmp   n58_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # cap
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n43_lit_integer_α
n42_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n40_statement_begin_β
.Lx62_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx63_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n44_call_α
n43_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n42_lit_integer_β
.Lx63_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n44_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              r10, qword ptr [rsp + 96]
                        mov              r11, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 80]
                        mov              r11, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        .section         .rodata
.Lrkfnzd65:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd65]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 48
                        cmp              eax, 104
                                                                                        jne   .Lx64_240
                        add              rsp, 16
                                                                                        jmp   n43_lit_integer_β
.Lx64_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n45_keyword_snobol4_α
n44_call_β:
                        add              rsp, 16
                                                                                        jmp   n43_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n45_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx66_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n46_keyword_snobol4_α
.Lx66_0:
                        .quad            .Lx66_0_s
.Lx66_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n46_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx67_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n47_call_α
.Lx67_0:
                        .quad            .Lx67_0_s
.Lx67_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              r10, qword ptr [rsp + 96]
                        mov              r11, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 80]
                        mov              r11, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        .section         .rodata
.Lrkfnzd69:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd69]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 48
                        cmp              eax, 104
                                                                                        jne   .Lx68_240
                        add              rsp, 64
                                                                                        jmp   n43_lit_integer_β
.Lx68_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n48_var_α
n47_call_β:
                        add              rsp, 64
                                                                                        jmp   n43_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n48_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # cap
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n49_lit_integer_α
n48_var_β:
                        add              rsp, 80
                                                                                        jmp   n43_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n50_call_α
n49_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n48_var_β
.Lx71_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n50_call_α:
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
.Lrkfnzd73:             .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd73]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx72_240
                        add              rsp, 16
                                                                                        jmp   n49_lit_integer_β
.Lx72_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n51_keyword_snobol4_α
n50_call_β:
                        add              rsp, 16
                                                                                        jmp   n49_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n51_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx74_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n52_keyword_snobol4_α
.Lx74_0:
                        .quad            .Lx74_0_s
.Lx74_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n52_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx75_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_call_α
.Lx75_0:
                        .quad            .Lx75_0_s
.Lx75_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:
                        sub              rsp, 16
                        sub              rsp, 48
                        mov              r10, qword ptr [rsp + 96]
                        mov              r11, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 80]
                        mov              r11, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        .section         .rodata
.Lrkfnzd77:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd77]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 48
                        cmp              eax, 104
                                                                                        jne   .Lx76_240
                        add              rsp, 64
                                                                                        jmp   n49_lit_integer_β
.Lx76_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n54_binop_α
n53_call_β:
                        add              rsp, 64
                                                                                        jmp   n49_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n54_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # call
                        mov              rsi, qword ptr [rsp + 120]                     # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n55_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # cap
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n56_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:
                                                                                        jmp   n57_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n57_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx83_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx83_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n58_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx85_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx85_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_β:
                                                                                        jmp   proc_LBL__cap_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__cap_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_α:
proc_LBL__icase_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:
                                                                                        jmp   n87_var_α
n86_statement_begin_β:
                                                                                        jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n87_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n88_call_α
#-----------------------------------------------------------------------------------------------------------------------
n88_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd132:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd132]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx131_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n86_statement_begin_β
.Lx131_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n89_statement_end_α
n88_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n86_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:
                                                                                        jmp   n90_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n90_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx136_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx136_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:
                                                                                        jmp   n92_var_α
n91_statement_begin_β:
                                                                                        jmp   n110_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1024], rax                    # result
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n93_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n93_match_begin_α:
                        mov              qword ptr [rsp + 888], rbp                     # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rdi, qword ptr [rsp + 1024]                    # lo
                        mov              rsi, qword ptr [rsp + 1032]                    # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx141_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n94_lit_integer_α
n93_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx141_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx141_1
                                                                                        jmp   .Lx141_0
.Lx141_1:
n93_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                                                                                        jmp   n110_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_integer_α:
                        mov              qword ptr [rsp + 960], 3                       # result
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n95_match_pos_α
.Lx142_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n95_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx143_240
                        add              rsp, 16
                                                                                        jmp   n93_match_begin_β
.Lx143_240:
                                                                                        jmp   n96_match_assign_save_α
n95_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n93_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n96_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 1008], r14d
                        add              rsp, 16
                                                                                        jmp   n97_match_any_α
n96_match_assign_save_β:
                        sub              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n93_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n97_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx147_239
                        add              rsp, 16
                                                                                        jmp   n93_match_begin_β
.Lx147_239:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C0]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        jne   .Lx147_240
                        add              rsp, 16
                                                                                        jmp   n93_match_begin_β
.Lx147_240:
                        add              r14d, 1
                                                                                        jmp   n98_match_assign_cond_α
n97_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n93_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n98_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 1008]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n99_match_end_α
n98_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n97_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n99_match_end_α:
                        mov              r10, r12
.Lx151_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx151_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 896], eax
                        mov              qword ptr [rsp + 920], r14
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx151_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx151_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx151_1:
                        test             rax, rax
                                                                                        je    .Lx151_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx151_3]
                        lea              rdx, [rip + .Lx151_4]
                                                                                        jmp   rax
.Lx151_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx151_1
.Lx151_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx151_1
.Lx151_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx151_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx151_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n100_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_string_α:
                        mov              qword ptr [rsp + 944], 2                       # result
                        mov              dword ptr [rsp + 948], 0
                        mov              rax, qword ptr [rip + .Lx152_0]
                        mov              qword ptr [rsp + 952], rax
                                                                                        jmp   n101_match_replace_α
.Lx152_0:
                        .quad            .Lx152_0_s
.Lx152_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n101_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx154_0]                # name
                        mov              rsi, qword ptr [rbp + 1024]                    # sub_lo
                        mov              rdx, qword ptr [rbp + 1032]                    # sub_hi
                        mov              ecx, dword ptr [rbp + 848]                     # start
                        mov              r8, qword ptr [rbp + 872]                      # end
                        lea              r9, [rbp + 944]                                # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx154_1
.Lx154_0:
                        .quad            .Lx154_0_s
.Lx154_0_s:
                        .string          "str"
.Lx154_1:
                        mov              rbp, qword ptr [rbp + 888]                     # old_rbp
                                                                                        jmp   n102_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:
                                                                                        jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:
                                                                                        jmp   n104_var_α
n103_statement_begin_β:
                                                                                        jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_var_α:
                        mov              rax, qword ptr [1879052336]                    # icase
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1104], rax                    # result
                        mov              qword ptr [rsp + 1112], rdx
                                                                                        jmp   n105_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n105_assign_α:
                        mov              rax, qword ptr [rsp + 1104]
                        mov              rdx, qword ptr [rsp + 1112]
                        mov              qword ptr [1879052400], rax                    # PAT$0$V0
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n106_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_string_α:
                        mov              qword ptr [rsp + 1088], 2                      # result
                        mov              dword ptr [rsp + 1092], 5
                        mov              rax, qword ptr [rip + .Lx161_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n107_call_α
.Lx161_0:
                        .quad            .Lx161_0_s
.Lx161_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n107_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lrkfn163:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn163]                         # fn
                        lea              rsi, [rsp + 1056]                              # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        cmp              eax, 104
                                                                                        je    n103_statement_begin_β
                                                                                        jmp   n108_assign_α
n107_call_β:
                                                                                        jmp   n103_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n108_assign_α:
                        mov              rax, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        mov              qword ptr [1879052336], rax                    # icase
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n109_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_end_α:
                                                                                        jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_begin_α:
                                                                                        jmp   n111_var_α
n110_statement_begin_β:
                                                                                        jmp   n122_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n111_var_α:
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1312], rax                    # result
                        mov              qword ptr [rsp + 1320], rdx
                                                                                        jmp   n112_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n112_match_begin_α:
                        mov              qword ptr [rsp + 1176], rbp                    # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rdi, qword ptr [rsp + 1312]                    # lo
                        mov              rsi, qword ptr [rsp + 1320]                    # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx171_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n113_lit_integer_α
n112_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx171_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx171_1
                                                                                        jmp   .Lx171_0
.Lx171_1:
n112_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                                                                                        jmp   n122_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_integer_α:
                        mov              qword ptr [rsp + 1248], 3                      # result
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 1256], rax
                                                                                        jmp   n114_match_pos_α
.Lx172_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n114_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx173_240
                        add              rsp, 16
                                                                                        jmp   n112_match_begin_β
.Lx173_240:
                                                                                        jmp   n115_match_assign_save_α
n114_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n112_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n115_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 1296], r14d
                        add              rsp, 16
                                                                                        jmp   n116_match_len_α
n115_match_assign_save_β:
                        sub              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n112_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n116_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jle   .Lx176_240
                        add              rsp, 16
                                                                                        jmp   n112_match_begin_β
.Lx176_240:
                        add              r14d, 1
                                                                                        jmp   n117_match_assign_cond_α
n116_match_len_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n112_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n117_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 1296]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n118_match_end_α
n117_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n116_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n118_match_end_α:
                        mov              r10, r12
.Lx180_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx180_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 1184], eax
                        mov              qword ptr [rsp + 1208], r14
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx180_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx180_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx180_1:
                        test             rax, rax
                                                                                        je    .Lx180_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx180_3]
                        lea              rdx, [rip + .Lx180_4]
                                                                                        jmp   rax
.Lx180_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx180_1
.Lx180_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx180_1
.Lx180_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx180_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx180_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n119_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_string_α:
                        mov              qword ptr [rsp + 1232], 2                      # result
                        mov              dword ptr [rsp + 1236], 0
                        mov              rax, qword ptr [rip + .Lx181_0]
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n120_match_replace_α
.Lx181_0:
                        .quad            .Lx181_0_s
.Lx181_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n120_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx183_0]                # name
                        mov              rsi, qword ptr [rbp + 1312]                    # sub_lo
                        mov              rdx, qword ptr [rbp + 1320]                    # sub_hi
                        mov              ecx, dword ptr [rbp + 1136]                    # start
                        mov              r8, qword ptr [rbp + 1160]                     # end
                        lea              r9, [rbp + 1232]                               # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx183_1
.Lx183_0:
                        .quad            .Lx183_0_s
.Lx183_0_s:
                        .string          "str"
.Lx183_1:
                        mov              rbp, qword ptr [rbp + 1176]                    # old_rbp
                                                                                        jmp   n121_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n121_statement_end_α:
                                                                                        jmp   n122_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_begin_α:
                                                                                        jmp   n123_var_α
n122_statement_begin_β:
                                                                                        jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n123_var_α:
                        mov              rax, qword ptr [1879052336]                    # icase
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1344], rax                    # result
                        mov              qword ptr [rsp + 1352], rdx
                                                                                        jmp   n124_var_α
#-----------------------------------------------------------------------------------------------------------------------
n124_var_α:
                        mov              rax, qword ptr [1879052384]                    # ch
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 1360], rax                    # result
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n125_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n125_binop_α:
                        mov              rdi, qword ptr [rsp + 1344]                    # a
                        mov              rsi, qword ptr [rsp + 1352]                    # a
                        mov              rdx, qword ptr [rsp + 1360]                    # b
                        mov              rcx, qword ptr [rsp + 1368]                    # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1328], rax
                        mov              qword ptr [rsp + 1336], rdx
                                                                                        jmp   n126_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n126_assign_α:
                        mov              rax, qword ptr [rsp + 1328]
                        mov              rdx, qword ptr [rsp + 1336]
                        mov              qword ptr [1879052336], rax                    # icase
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n127_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_end_α:
                                                                                        jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_β:
                                                                                        jmp   proc_LBL__icase_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__icase_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_lwr_α
proc_lwr_α:
proc_lwr_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n194_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n195_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n195_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx199_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx199_1
.Lx199_0:
                        .quad            .Lx199_0_s
.Lx199_0_s:
                        .string          "lwr"
.Lx199_1:
                                                                                        jmp   proc_lwr_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_lwr_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_lwr_β:
                                                                                        jmp   proc_lwr_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_lwr_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_lwr_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_upr_α
proc_upr_α:
proc_upr_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n200_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n201_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n201_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx205_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx205_1
.Lx205_0:
                        .quad            .Lx205_0_s
.Lx205_0_s:
                        .string          "upr"
.Lx205_1:
                                                                                        jmp   proc_upr_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_upr_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_upr_β:
                                                                                        jmp   proc_upr_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_upr_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_upr_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_cap_α
proc_cap_α:
proc_cap_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n206_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n207_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n207_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx211_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx211_1
.Lx211_0:
                        .quad            .Lx211_0_s
.Lx211_0_s:
                        .string          "cap"
.Lx211_1:
                                                                                        jmp   proc_cap_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_cap_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_cap_β:
                                                                                        jmp   proc_cap_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_cap_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_cap_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_icase_α
proc_icase_α:
proc_icase_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n212_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n213_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n213_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx217_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx217_1
.Lx217_0:
                        .quad            .Lx217_0_s
.Lx217_0_s:
                        .string          "icase"
.Lx217_1:
                                                                                        jmp   proc_icase_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_icase_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_icase_β:
                                                                                        jmp   proc_icase_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_icase_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_icase_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        sub              rsp, 256
                        mov              qword ptr [rsp + 232], rcx
                        mov              qword ptr [rsp + 240], rdx
                        mov              qword ptr [rsp + 224], r8
                        mov              dword ptr [rsp + 216], r14d
                        mov              rax, qword ptr [rip + g_zctx@GOTPCREL]
                        mov              r10, qword ptr [rax]
                        mov              r11, qword ptr [rax + 8]
                        mov              qword ptr [rax + r10*8 + 16], r11
                        inc              qword ptr [rax]
                        mov              qword ptr [rax + 8], rsp
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + proc_PAT$0_ω]
                        mov              qword ptr [rbp + 192], rax
#-----------------------------------------------------------------------------------------------------------------------
n218_match_defer_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]                    # PAT$0$V0
                        mov              rdx, qword ptr [1879052408]
                        cmp              eax, 8
                                                                                        jne   .Lx226_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx226_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx226_10
.Lx226_9:
                        xor              eax, eax
.Lx226_10:
                        test             rax, rax
                                                                                        jz    .Lx226_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx226_4]
                        lea              rdx, [rip + .Lx226_5]
                                                                                        jmp   rax
.Lx226_4:
                                                                                        jmp   n219_match_alternate_α
.Lx226_5:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
.Lx226_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx226_2:
                        test             rax, rax
                                                                                        je    .Lx226_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx226_7]
                        lea              rdx, [rip + .Lx226_8]
                                                                                        jmp   rax
.Lx226_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx226_2
.Lx226_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx226_2
.Lx226_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        jns   .Lx226_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
.Lx226_240:
                        mov              r14d, eax
                        lea              rax, [rip + .Lx226_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n219_match_alternate_α
.Lx226_6:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
n218_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n219_match_alternate_α:
                        mov              dword ptr [rbp + 32], r14d
                        lea              rax, [rip + .Lx228_21]
                        mov              qword ptr [rbp + 48], rax
                                                                                        jmp   n223_var_α
.Lx228_21:
                        lea              rax, [rip + .Lx228_19]
                        mov              qword ptr [rbp + 48], rax
                                                                                        jmp   n220_var_α
n219_match_alternate_s0:
                        lea              rax, [rip + .Lx228_40]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n219_match_alternate_as
n219_match_alternate_s1:
                        lea              rax, [rip + .Lx228_41]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n219_match_alternate_as
.Lx228_40:
                                                                                        jmp   n225_match_value_β
.Lx228_41:
                                                                                        jmp   n222_match_value_β
n219_match_alternate_as:
                                                                                        jmp   proc_PAT$0_γ
n219_match_alternate_β:
                        mov              rax, qword ptr [rbp + 40]
                                                                                        jmp   rax
n219_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 32]
                        mov              rax, qword ptr [rbp + 48]
                                                                                        jmp   rax
.Lx228_19:
                                                                                        jmp   n218_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n220_var_α:
                        mov              rax, qword ptr [1879052368]                    # letter
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 176], rax                     # result
                        mov              qword ptr [rbp + 184], rdx
                                                                                        jmp   n221_call_α
n220_var_β:
                                                                                        jmp   n219_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n221_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # lwr
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx231_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx231_5
                        mov              rax, qword ptr [rbp + 176]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rbp + 184]
                        mov              qword ptr [1879052296], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx231_6]
                        lea              rdx, [rip + .Lx231_7]
                                                                                        jmp   rax
.Lx231_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx231_2
.Lx231_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx231_2
.Lx231_5:
                        add              rsp, 16
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx231_20
                        mov              rax, qword ptr [rbp + 176]
                        mov              rdx, qword ptr [rbp + 184]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx231_21
.Lx231_20:
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rbp + 176]                     # v
                        mov              rdx, qword ptr [rbp + 184]                     # v
                        call             rt_arg_stage@PLT
.Lx231_21:
                        mov              rdi, qword ptr [rip + .Lx231_0]                # name
                        mov              esi, 1                                         # nargs
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx231_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx231_3]
                        lea              rdx, [rip + .Lx231_4]
                                                                                        jmp   rax
.Lx231_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx231_2
.Lx231_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx231_2
.Lx231_1:
                        call             rt_faildescr@PLT
.Lx231_2:
                        mov              qword ptr [rbp + 128], rax
                        mov              qword ptr [rbp + 136], rdx
                        cmp              eax, 104
                                                                                        je    n219_match_alternate_af
                                                                                        jmp   n222_match_value_α
n221_call_β:
                                                                                        jmp   n219_match_alternate_af
.Lx231_0:
                        .quad            .Lx231_0_s
.Lx231_0_s:
                        .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n222_match_value_α:
                        lea              rdi, [rbp + 128]                               # pval
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx232_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx232_4]
                        lea              rdx, [rip + .Lx232_5]
                                                                                        jmp   rax
.Lx232_4:
                                                                                        jmp   n219_match_alternate_s1
.Lx232_5:
                                                                                        jmp   n219_match_alternate_af
.Lx232_0:
                        lea              rdi, [rbp + 128]                               # pval
                        call             rt_match_value_open@PLT
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n219_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx232_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n219_match_alternate_s1
.Lx232_6:
                        add              rsp, 16
                                                                                        jmp   n219_match_alternate_af
n222_match_value_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n223_var_α:
                        mov              rax, qword ptr [1879052368]                    # letter
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 112], rax                     # result
                        mov              qword ptr [rbp + 120], rdx
                                                                                        jmp   n224_call_α
n223_var_β:
                                                                                        jmp   n219_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n224_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # upr
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx235_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx235_5
                        mov              rax, qword ptr [rbp + 112]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx235_6]
                        lea              rdx, [rip + .Lx235_7]
                                                                                        jmp   rax
.Lx235_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx235_2
.Lx235_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx235_2
.Lx235_5:
                        add              rsp, 16
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx235_20
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx235_21
.Lx235_20:
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rbp + 112]                     # v
                        mov              rdx, qword ptr [rbp + 120]                     # v
                        call             rt_arg_stage@PLT
.Lx235_21:
                        mov              rdi, qword ptr [rip + .Lx235_0]                # name
                        mov              esi, 1                                         # nargs
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx235_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx235_3]
                        lea              rdx, [rip + .Lx235_4]
                                                                                        jmp   rax
.Lx235_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx235_2
.Lx235_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx235_2
.Lx235_1:
                        call             rt_faildescr@PLT
.Lx235_2:
                        mov              qword ptr [rbp + 64], rax
                        mov              qword ptr [rbp + 72], rdx
                        cmp              eax, 104
                                                                                        je    n219_match_alternate_af
                                                                                        jmp   n225_match_value_α
n224_call_β:
                                                                                        jmp   n219_match_alternate_af
.Lx235_0:
                        .quad            .Lx235_0_s
.Lx235_0_s:
                        .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n225_match_value_α:
                        lea              rdi, [rbp + 64]                                # pval
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx236_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx236_4]
                        lea              rdx, [rip + .Lx236_5]
                                                                                        jmp   rax
.Lx236_4:
                                                                                        jmp   n219_match_alternate_s0
.Lx236_5:
                                                                                        jmp   n219_match_alternate_af
.Lx236_0:
                        lea              rdi, [rbp + 64]                                # pval
                        call             rt_match_value_open@PLT
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n219_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx236_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n219_match_alternate_s0
.Lx236_6:
                        add              rsp, 16
                                                                                        jmp   n219_match_alternate_af
n225_match_value_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$0_scanhit:
                        mov              rdx, qword ptr [rip + g_zctx@GOTPCREL]
                        mov              rdx, qword ptr [rdx + 8]
                        cmp              qword ptr [rdx + 224], 1
                                                                                        jne   .Lx237_0
                        mov              ecx, dword ptr [rdx + 216]
                        lea              rax, [rip + g_scan_hit_start]
                        mov              dword ptr [rax + 0], ecx
.Lx237_0:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        mov              rdx, qword ptr [rip + g_zctx@GOTPCREL]
                        mov              rdx, qword ptr [rdx + 8]
                        cmp              qword ptr [rdx + 224], 1
                                                                                        jne   .Lx238_0
                        mov              eax, dword ptr [rdx + 216]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx238_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx238_0
                        mov              dword ptr [rdx + 216], eax
                        mov              r14d, eax
                        mov              rsp, rdx
                                                                                        jmp   proc_PAT$0_attempt
.Lx238_0:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        add              rsp, 8
                        pop              rax
                        mov              rdx, qword ptr [rip + g_zctx@GOTPCREL]
                        mov              r10, qword ptr [rdx]
                        mov              r11, qword ptr [rdx + 8]
                        mov              qword ptr [rdx + r10*8 + 16], r11
                        inc              qword ptr [rdx]
                        mov              qword ptr [rdx + 8], rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                                        jmp   qword ptr [rbp + 192]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        mov              rdx, qword ptr [rip + g_zctx@GOTPCREL]
                        mov              rax, qword ptr [rdx + 8]
                        push             rax
                        lea              rcx, [rip + proc_PAT$0_res]
                        push             rcx
                        mov              rcx, qword ptr [rax + 232]
                        mov              r10, qword ptr [rdx]
                        dec              r10
                        mov              qword ptr [rdx], r10
                        mov              r11, qword ptr [rdx + r10*8 + 16]
                        mov              qword ptr [rdx + 8], r11
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rdx, qword ptr [rip + g_zctx@GOTPCREL]
                        mov              rax, qword ptr [rdx + 8]
                        mov              rcx, qword ptr [rax + 240]
                        lea              rsp, [rax + 256]
                        mov              r10, qword ptr [rdx]
                        dec              r10
                        mov              qword ptr [rdx], r10
                        mov              r11, qword ptr [rdx + r10*8 + 16]
                        mov              qword ptr [rdx + 8], r11
                                                                                        jmp   rcx
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__lwr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__lwr_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2448
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__upr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__upr_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2448
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__cap"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__cap_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2448
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__icase"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__icase_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2448
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "lwr"
.Lstartup_pp4_0:        .string          "lwr"
                        .align           8
.Lstartup_pnames4:
                        .quad            .Lstartup_pp4_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + .Lstartup_pnames4]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_lwr_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "upr"
.Lstartup_pp5_0:        .string          "upr"
                        .align           8
.Lstartup_pnames5:
                        .quad            .Lstartup_pp5_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + .Lstartup_pnames5]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_upr_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "cap"
.Lstartup_pp6_0:        .string          "cap"
                        .align           8
.Lstartup_pnames6:
                        .quad            .Lstartup_pp6_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + .Lstartup_pnames6]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + proc_cap_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname7:       .string          "icase"
.Lstartup_pp7_0:        .string          "str"
.Lstartup_pp7_1:        .string          "letter"
.Lstartup_pp7_2:        .string          "ch"
                        .align           8
.Lstartup_pnames7:
                        .quad            .Lstartup_pp7_0
                        .quad            .Lstartup_pp7_1
                        .quad            .Lstartup_pp7_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + .Lstartup_pnames7]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + proc_icase_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 3
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
.Lstartup_pname8:       .string          "PAT$0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + proc_PAT$0_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 208
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "lwr"
.Lgvan1:                .string          "upr"
.Lgvan2:                .string          "cap"
.Lgvan3:                .string          "icase"
.Lgvan4:                .string          "str"
.Lgvan5:                .string          "letter"
.Lgvan6:                .string          "ch"
.Lgvan7:                .string          "PAT$0$V0"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 8
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 8
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 104
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_begin_α:
                                                                                        jmp   n240_statement_end_α
n239_statement_begin_β:
                                                                                        jmp   n241_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_end_α:
                                                                                        jmp   n241_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'no match ok'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n241_statement_begin_α:
                                                                                        jmp   n242_statement_end_α
n241_statement_begin_β:
                                                                                        jmp   n243_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n242_statement_end_α:
                                                                                        jmp   n243_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_begin_α:
                                                                                        jmp   n244_statement_end_α
n243_statement_begin_β:
                                                                                        jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_end_α:
                                                                                        jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n245_statement_begin_α:
                                                                                        jmp   n246_statement_end_α
n245_statement_begin_β:
                                                                                        jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n246_statement_end_α:
                                                                                        jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n247_statement_begin_α:
                                                                                        jmp   n248_statement_end_α
n247_statement_begin_β:
                                                                                        jmp   n249_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n248_statement_end_α:
                                                                                        jmp   n249_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n249_statement_begin_α:
                                                                                        jmp   n250_statement_end_α
n249_statement_begin_β:
                                                                                        jmp   n251_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n250_statement_end_α:
                                                                                        jmp   n251_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n251_statement_begin_α:
                                                                                        jmp   n252_statement_end_α
n251_statement_begin_β:
                                                                                        jmp   n253_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n252_statement_end_α:
                                                                                        jmp   n253_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n253_statement_begin_α:
                                                                                        jmp   n254_statement_end_α
n253_statement_begin_β:
                                                                                        jmp   n255_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_end_α:
                                                                                        jmp   n255_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n255_statement_begin_α:
                                                                                        jmp   n256_statement_end_α
n255_statement_begin_β:
                                                                                        jmp   n257_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n256_statement_end_α:
                                                                                        jmp   n257_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n257_statement_begin_α:
                                                                                        jmp   n258_statement_end_α
n257_statement_begin_β:
                                                                                        jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_end_α:
                                                                                        jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_begin_α:
                                                                                        jmp   n260_lit_string_α
n259_statement_begin_β:
                                                                                        jmp   n264_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n260_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx470_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n261_lit_integer_α
.Lx470_0:
                        .quad            .Lx470_0_s
.Lx470_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n261_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx471_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n262_call_α
n261_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n259_statement_begin_β
.Lx471_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n262_call_α:
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
.Lrkfnzd473:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd473]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx472_240
                        add              rsp, 16
                                                                                        jmp   n261_lit_integer_β
.Lx472_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n263_statement_end_α
n262_call_β:
                        add              rsp, 16
                                                                                        jmp   n261_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n263_statement_end_α:
                                                                                        jmp   n264_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n264_statement_begin_α:
                                                                                        jmp   n265_statement_end_α
n264_statement_begin_β:
                                                                                        jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:
                                                                                        jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_begin_α:
                                                                                        jmp   n267_lit_string_α
n266_statement_begin_β:
                                                                                        jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx482_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n268_call_α
n267_lit_string_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n266_statement_begin_β
.Lx482_0:
                        .quad            .Lx482_0_s
.Lx482_0_s:
                        .string          "HELLO WORLD"
#-----------------------------------------------------------------------------------------------------------------------
n268_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # lwr
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx484_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx484_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052296], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx484_6]
                        lea              rdx, [rip + .Lx484_7]
                                                                                        jmp   rax
.Lx484_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx484_2
.Lx484_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx484_2
.Lx484_5:
                        add              rsp, 16
.Lx484_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx484_240
                        add              rsp, 16
                                                                                        jmp   n267_lit_string_β
.Lx484_240:
                                                                                        jmp   n269_assign_α
n268_call_β:
                                                                                        jmp   n267_lit_string_β
.Lx484_0:
                        .quad            .Lx484_0_s
.Lx484_0_s:
                        .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n269_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx485_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n270_statement_end_α
.Lx485_0:
                        .quad            .Lx485_0_s
.Lx485_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:
                                                                                        jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_begin_α:
                                                                                        jmp   n272_lit_string_α
n271_statement_begin_β:
                                                                                        jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n272_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx490_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n273_call_α
n272_lit_string_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n271_statement_begin_β
.Lx490_0:
                        .quad            .Lx490_0_s
.Lx490_0_s:
                        .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n273_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # upr
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx492_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx492_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx492_6]
                        lea              rdx, [rip + .Lx492_7]
                                                                                        jmp   rax
.Lx492_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx492_2
.Lx492_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx492_2
.Lx492_5:
                        add              rsp, 16
.Lx492_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx492_240
                        add              rsp, 16
                                                                                        jmp   n272_lit_string_β
.Lx492_240:
                                                                                        jmp   n274_assign_α
n273_call_β:
                                                                                        jmp   n272_lit_string_β
.Lx492_0:
                        .quad            .Lx492_0_s
.Lx492_0_s:
                        .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n274_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx493_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n275_statement_end_α
.Lx493_0:
                        .quad            .Lx493_0_s
.Lx493_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_end_α:
                                                                                        jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_begin_α:
                                                                                        jmp   n277_lit_string_α
n276_statement_begin_β:
                                                                                        jmp   n281_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n277_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx498_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n278_call_α
n277_lit_string_β:
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n276_statement_begin_β
.Lx498_0:
                        .quad            .Lx498_0_s
.Lx498_0_s:
                        .string          "hELLO wORLD"
#-----------------------------------------------------------------------------------------------------------------------
n278_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # cap
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx500_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx500_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx500_6]
                        lea              rdx, [rip + .Lx500_7]
                                                                                        jmp   rax
.Lx500_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx500_2
.Lx500_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx500_2
.Lx500_5:
                        add              rsp, 16
.Lx500_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx500_240
                        add              rsp, 16
                                                                                        jmp   n277_lit_string_β
.Lx500_240:
                                                                                        jmp   n279_assign_α
n278_call_β:
                                                                                        jmp   n277_lit_string_β
.Lx500_0:
                        .quad            .Lx500_0_s
.Lx500_0_s:
                        .string          "cap"
#-----------------------------------------------------------------------------------------------------------------------
n279_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx501_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n280_statement_end_α
.Lx501_0:
                        .quad            .Lx501_0_s
.Lx501_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n280_statement_end_α:
                                                                                        jmp   n281_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n281_statement_begin_α:
                                                                                        jmp   n282_statement_end_α
n281_statement_begin_β:
                                                                                        jmp   n283_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n282_statement_end_α:
                                                                                        jmp   n283_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n283_statement_begin_α:
                                                                                        jmp   n284_lit_string_α
n283_statement_begin_β:
                                                                                        jmp   n295_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n284_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx510_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n285_match_begin_α
.Lx510_0:
                        .quad            .Lx510_0_s
.Lx510_0_s:
                        .string          "Hello"
#-----------------------------------------------------------------------------------------------------------------------
n285_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # lit_string
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rbp + 1728], r13                    # outer_Σ
                        mov              qword ptr [rbp + 1736], r14                    # outer_δ
                        mov              qword ptr [rbp + 1744], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 1752], rax                    # cap_gen
                        mov              qword ptr [rbp + 1720], rbp                    # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rbp + 1696], rsp                    # zls2_mark
                        mov              dword ptr [rbp + 1680], 0                      # start_δ
.Lx512_0:
                        mov              r14d, dword ptr [rbp + 1680]
                                                                                        jmp   n286_lit_string_α
n285_match_begin_β:
                        add              dword ptr [rbp + 1680], 1
                        mov              eax, dword ptr [rbp + 1680]
                        cmp              eax, r15d
                                                                                        jg    .Lx512_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx512_1
                                                                                        jmp   .Lx512_0
.Lx512_1:
n285_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rbp + 1696]
                        mov              r13, qword ptr [rbp + 1728]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 1736]                    # outer_δ
                        mov              r15, qword ptr [rbp + 1744]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 1752]                    # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 1720]                    # old_rbp
                                                                                        jmp   n295_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n286_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx513_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n287_call_α
.Lx513_0:
                        .quad            .Lx513_0_s
.Lx513_0_s:
                        .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n287_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 64
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052368]                    # letter
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052384]                    # ch
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052336]                    # icase
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx515_0]                # name
                        mov              esi, 3                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx515_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052360], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx515_6]
                        lea              rdx, [rip + .Lx515_7]
                                                                                        jmp   rax
.Lx515_6:
                        mov              rdi, qword ptr [1879052336]                    # icase
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052384], rax                    # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052368], rax                    # letter
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx515_2
.Lx515_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052336], rax                    # icase
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052384], rax                    # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052368], rax                    # letter
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx515_2
.Lx515_5:
                        add              rsp, 64
.Lx515_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx515_240
                        add              rsp, 16
                                                                                        jmp   n285_match_begin_α
.Lx515_240:
                                                                                        jmp   n288_match_value_α
n287_call_β:
                                                                                        jmp   n285_match_begin_α
.Lx515_0:
                        .quad            .Lx515_0_s
.Lx515_0_s:
                        .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n288_match_value_α:
                        lea              rdi, [rsp + 0]                                 # pval
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx516_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx516_4]
                        lea              rdx, [rip + .Lx516_5]
                                                                                        jmp   rax
.Lx516_4:
                                                                                        jmp   n289_match_end_α
.Lx516_5:
                                                                                        jmp   n285_match_begin_β
.Lx516_0:
                        lea              rdi, [rsp + 0]                                 # pval
                        call             rt_match_value_open@PLT
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n285_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx516_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n289_match_end_α
.Lx516_6:
                        add              rsp, 16
                                                                                        jmp   n285_match_begin_β
n288_match_value_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n289_match_end_α:
                        mov              r10, r12
.Lx518_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx518_9
                        mov              rsp, qword ptr [rbp + 1696]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx518_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx518_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx518_1:
                        test             rax, rax
                                                                                        je    .Lx518_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx518_3]
                        lea              rdx, [rip + .Lx518_4]
                                                                                        jmp   rax
.Lx518_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx518_1
.Lx518_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx518_1
.Lx518_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx518_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx518_10
                        mov              r13, qword ptr [rbp + 1728]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 1736]                    # outer_δ
                        mov              r15, qword ptr [rbp + 1744]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 1720]                    # old_rbp
                                                                                        jmp   n290_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_end_α:
                                                                                        jmp   n291_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_begin_α:
                                                                                        jmp   n292_lit_string_α
n291_statement_begin_β:
                                                                                        jmp   n295_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n292_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx523_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n293_assign_α
.Lx523_0:
                        .quad            .Lx523_0_s
.Lx523_0_s:
                        .string          "ok: icase hello"
#-----------------------------------------------------------------------------------------------------------------------
n293_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx524_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n294_statement_end_α
.Lx524_0:
                        .quad            .Lx524_0_s
.Lx524_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n294_statement_end_α:
                                                                                        jmp   n295_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n295_statement_begin_α:
                                                                                        jmp   n296_statement_end_α
n295_statement_begin_β:
                                                                                        jmp   n297_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n296_statement_end_α:
                                                                                        jmp   n297_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n297_statement_begin_α:
                                                                                        jmp   n298_lit_string_α
n297_statement_begin_β:
                                                                                        jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n298_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx533_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n299_match_begin_α
.Lx533_0:
                        .quad            .Lx533_0_s
.Lx533_0_s:
                        .string          "HELLO"
#-----------------------------------------------------------------------------------------------------------------------
n299_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # lit_string
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rbp + 1920], r13                    # outer_Σ
                        mov              qword ptr [rbp + 1928], r14                    # outer_δ
                        mov              qword ptr [rbp + 1936], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 1944], rax                    # cap_gen
                        mov              qword ptr [rbp + 1912], rbp                    # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rbp + 1888], rsp                    # zls2_mark
                        mov              dword ptr [rbp + 1872], 0                      # start_δ
.Lx535_0:
                        mov              r14d, dword ptr [rbp + 1872]
                                                                                        jmp   n300_lit_string_α
n299_match_begin_β:
                        add              dword ptr [rbp + 1872], 1
                        mov              eax, dword ptr [rbp + 1872]
                        cmp              eax, r15d
                                                                                        jg    .Lx535_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx535_1
                                                                                        jmp   .Lx535_0
.Lx535_1:
n299_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rbp + 1888]
                        mov              r13, qword ptr [rbp + 1920]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 1928]                    # outer_δ
                        mov              r15, qword ptr [rbp + 1936]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 1944]                    # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 1912]                    # old_rbp
                                                                                        jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n300_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx536_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n301_call_α
.Lx536_0:
                        .quad            .Lx536_0_s
.Lx536_0_s:
                        .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n301_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 64
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052368]                    # letter
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052384]                    # ch
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052336]                    # icase
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx538_0]                # name
                        mov              esi, 3                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx538_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052360], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx538_6]
                        lea              rdx, [rip + .Lx538_7]
                                                                                        jmp   rax
.Lx538_6:
                        mov              rdi, qword ptr [1879052336]                    # icase
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052384], rax                    # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052368], rax                    # letter
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx538_2
.Lx538_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052336], rax                    # icase
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052384], rax                    # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052368], rax                    # letter
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx538_2
.Lx538_5:
                        add              rsp, 64
.Lx538_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx538_240
                        add              rsp, 16
                                                                                        jmp   n299_match_begin_α
.Lx538_240:
                                                                                        jmp   n302_match_value_α
n301_call_β:
                                                                                        jmp   n299_match_begin_α
.Lx538_0:
                        .quad            .Lx538_0_s
.Lx538_0_s:
                        .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n302_match_value_α:
                        lea              rdi, [rsp + 0]                                 # pval
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx539_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx539_4]
                        lea              rdx, [rip + .Lx539_5]
                                                                                        jmp   rax
.Lx539_4:
                                                                                        jmp   n303_match_end_α
.Lx539_5:
                                                                                        jmp   n299_match_begin_β
.Lx539_0:
                        lea              rdi, [rsp + 0]                                 # pval
                        call             rt_match_value_open@PLT
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n299_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx539_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n303_match_end_α
.Lx539_6:
                        add              rsp, 16
                                                                                        jmp   n299_match_begin_β
n302_match_value_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n303_match_end_α:
                        mov              r10, r12
.Lx541_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx541_9
                        mov              rsp, qword ptr [rbp + 1888]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx541_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx541_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx541_1:
                        test             rax, rax
                                                                                        je    .Lx541_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx541_3]
                        lea              rdx, [rip + .Lx541_4]
                                                                                        jmp   rax
.Lx541_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx541_1
.Lx541_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx541_1
.Lx541_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx541_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx541_10
                        mov              r13, qword ptr [rbp + 1920]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 1928]                    # outer_δ
                        mov              r15, qword ptr [rbp + 1936]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 1912]                    # old_rbp
                                                                                        jmp   n304_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_end_α:
                                                                                        jmp   n305_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n305_statement_begin_α:
                                                                                        jmp   n306_lit_string_α
n305_statement_begin_β:
                                                                                        jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n306_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx546_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n307_assign_α
.Lx546_0:
                        .quad            .Lx546_0_s
.Lx546_0_s:
                        .string          "ok: icase HELLO"
#-----------------------------------------------------------------------------------------------------------------------
n307_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx547_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n308_statement_end_α
.Lx547_0:
                        .quad            .Lx547_0_s
.Lx547_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n308_statement_end_α:
                                                                                        jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n309_statement_begin_α:
                                                                                        jmp   n310_statement_end_α
n309_statement_begin_β:
                                                                                        jmp   n311_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n310_statement_end_α:
                                                                                        jmp   n311_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n311_statement_begin_α:
                                                                                        jmp   n312_lit_string_α
n311_statement_begin_β:
                                                                                        jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n312_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx556_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n313_match_begin_α
.Lx556_0:
                        .quad            .Lx556_0_s
.Lx556_0_s:
                        .string          "HeLLo"
#-----------------------------------------------------------------------------------------------------------------------
n313_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # lit_string
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rbp + 2112], r13                    # outer_Σ
                        mov              qword ptr [rbp + 2120], r14                    # outer_δ
                        mov              qword ptr [rbp + 2128], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2136], rax                    # cap_gen
                        mov              qword ptr [rbp + 2104], rbp                    # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rbp + 2080], rsp                    # zls2_mark
                        mov              dword ptr [rbp + 2064], 0                      # start_δ
.Lx558_0:
                        mov              r14d, dword ptr [rbp + 2064]
                                                                                        jmp   n314_lit_string_α
n313_match_begin_β:
                        add              dword ptr [rbp + 2064], 1
                        mov              eax, dword ptr [rbp + 2064]
                        cmp              eax, r15d
                                                                                        jg    .Lx558_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx558_1
                                                                                        jmp   .Lx558_0
.Lx558_1:
n313_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rbp + 2080]
                        mov              r13, qword ptr [rbp + 2112]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 2120]                    # outer_δ
                        mov              r15, qword ptr [rbp + 2128]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 2136]                    # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 2104]                    # old_rbp
                                                                                        jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n314_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx559_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n315_call_α
.Lx559_0:
                        .quad            .Lx559_0_s
.Lx559_0_s:
                        .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n315_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 64
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052368]                    # letter
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052384]                    # ch
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052336]                    # icase
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx561_0]                # name
                        mov              esi, 3                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx561_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052360], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx561_6]
                        lea              rdx, [rip + .Lx561_7]
                                                                                        jmp   rax
.Lx561_6:
                        mov              rdi, qword ptr [1879052336]                    # icase
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052384], rax                    # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052368], rax                    # letter
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx561_2
.Lx561_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052336], rax                    # icase
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052384], rax                    # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052368], rax                    # letter
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx561_2
.Lx561_5:
                        add              rsp, 64
.Lx561_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx561_240
                        add              rsp, 16
                                                                                        jmp   n313_match_begin_α
.Lx561_240:
                                                                                        jmp   n316_match_value_α
n315_call_β:
                                                                                        jmp   n313_match_begin_α
.Lx561_0:
                        .quad            .Lx561_0_s
.Lx561_0_s:
                        .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n316_match_value_α:
                        lea              rdi, [rsp + 0]                                 # pval
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx562_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx562_4]
                        lea              rdx, [rip + .Lx562_5]
                                                                                        jmp   rax
.Lx562_4:
                                                                                        jmp   n317_match_end_α
.Lx562_5:
                                                                                        jmp   n313_match_begin_β
.Lx562_0:
                        lea              rdi, [rsp + 0]                                 # pval
                        call             rt_match_value_open@PLT
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n313_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx562_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n317_match_end_α
.Lx562_6:
                        add              rsp, 16
                                                                                        jmp   n313_match_begin_β
n316_match_value_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n317_match_end_α:
                        mov              r10, r12
.Lx564_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx564_9
                        mov              rsp, qword ptr [rbp + 2080]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx564_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx564_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx564_1:
                        test             rax, rax
                                                                                        je    .Lx564_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx564_3]
                        lea              rdx, [rip + .Lx564_4]
                                                                                        jmp   rax
.Lx564_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx564_1
.Lx564_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx564_1
.Lx564_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx564_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx564_10
                        mov              r13, qword ptr [rbp + 2112]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 2120]                    # outer_δ
                        mov              r15, qword ptr [rbp + 2128]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 2104]                    # old_rbp
                                                                                        jmp   n318_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n318_statement_end_α:
                                                                                        jmp   n319_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n319_statement_begin_α:
                                                                                        jmp   n320_lit_string_α
n319_statement_begin_β:
                                                                                        jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n320_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx569_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n321_assign_α
.Lx569_0:
                        .quad            .Lx569_0_s
.Lx569_0_s:
                        .string          "ok: icase HeLLo"
#-----------------------------------------------------------------------------------------------------------------------
n321_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx570_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n322_statement_end_α
.Lx570_0:
                        .quad            .Lx570_0_s
.Lx570_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:
                                                                                        jmp   n323_statement_begin_α
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
                                                                                        jmp   n326_lit_string_α
n325_statement_begin_β:
                        add              rsp, 336
                                                                                        jmp   n337_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n326_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx579_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n327_match_begin_α
n326_lit_string_β:
                        add              rsp, 16
                        add              rsp, 336
                                                                                        jmp   n337_statement_begin_α
.Lx579_0:
                        .quad            .Lx579_0_s
.Lx579_0_s:
                        .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n327_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # lit_string
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rbp + 2304], r13                    # outer_Σ
                        mov              qword ptr [rbp + 2312], r14                    # outer_δ
                        mov              qword ptr [rbp + 2320], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2328], rax                    # cap_gen
                        mov              qword ptr [rbp + 2296], rbp                    # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rbp + 2272], rsp                    # zls2_mark
                        mov              dword ptr [rbp + 2256], 0                      # start_δ
.Lx581_0:
                        mov              r14d, dword ptr [rbp + 2256]
                                                                                        jmp   n328_lit_string_α
n327_match_begin_β:
                        add              dword ptr [rbp + 2256], 1
                        mov              eax, dword ptr [rbp + 2256]
                        cmp              eax, r15d
                                                                                        jg    .Lx581_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx581_1
                                                                                        jmp   .Lx581_0
.Lx581_1:
n327_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rbp + 2272]
                        mov              r13, qword ptr [rbp + 2304]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 2312]                    # outer_δ
                        mov              r15, qword ptr [rbp + 2320]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 2328]                    # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 2296]                    # old_rbp
                                                                                        jmp   n326_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n328_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx582_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n329_call_α
.Lx582_0:
                        .quad            .Lx582_0_s
.Lx582_0_s:
                        .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n329_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_string
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 64
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052368]                    # letter
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052384]                    # ch
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052336]                    # icase
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx584_0]                # name
                        mov              esi, 3                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx584_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052360], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx584_6]
                        lea              rdx, [rip + .Lx584_7]
                                                                                        jmp   rax
.Lx584_6:
                        mov              rdi, qword ptr [1879052336]                    # icase
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052384], rax                    # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052368], rax                    # letter
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx584_2
.Lx584_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052336], rax                    # icase
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052384], rax                    # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052368], rax                    # letter
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax                    # str
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx584_2
.Lx584_5:
                        add              rsp, 64
.Lx584_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx584_240
                        add              rsp, 16
                                                                                        jmp   n327_match_begin_α
.Lx584_240:
                                                                                        jmp   n330_match_value_α
n329_call_β:
                                                                                        jmp   n327_match_begin_α
.Lx584_0:
                        .quad            .Lx584_0_s
.Lx584_0_s:
                        .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n330_match_value_α:
                        lea              rdi, [rsp + 0]                                 # pval
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx585_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx585_4]
                        lea              rdx, [rip + .Lx585_5]
                                                                                        jmp   rax
.Lx585_4:
                                                                                        jmp   n331_match_end_α
.Lx585_5:
                                                                                        jmp   n327_match_begin_β
.Lx585_0:
                        lea              rdi, [rsp + 0]                                 # pval
                        call             rt_match_value_open@PLT
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n327_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx585_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n331_match_end_α
.Lx585_6:
                        add              rsp, 16
                                                                                        jmp   n327_match_begin_β
n330_match_value_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n331_match_end_α:
                        mov              r10, r12
.Lx587_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx587_9
                        mov              rsp, qword ptr [rbp + 2272]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx587_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx587_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx587_1:
                        test             rax, rax
                                                                                        je    .Lx587_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx587_3]
                        lea              rdx, [rip + .Lx587_4]
                                                                                        jmp   rax
.Lx587_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx587_1
.Lx587_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx587_1
.Lx587_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx587_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx587_10
                        mov              r13, qword ptr [rbp + 2304]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 2312]                    # outer_δ
                        mov              r15, qword ptr [rbp + 2320]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 2296]                    # old_rbp
                                                                                        jmp   n332_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n332_statement_end_α:
                                                                                        jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n333_statement_begin_α:
                                                                                        jmp   n334_lit_string_α
n333_statement_begin_β:
                        add              rsp, 384
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n334_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx592_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n335_assign_α
.Lx592_0:
                        .quad            .Lx592_0_s
.Lx592_0_s:
                        .string          "FAIL: icase matched wrong string"
#-----------------------------------------------------------------------------------------------------------------------
n335_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx593_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n336_statement_end_α
.Lx593_0:
                        .quad            .Lx593_0_s
.Lx593_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n336_statement_end_α:
                        add              rsp, 160
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n337_statement_begin_α:
                                                                                        jmp   n338_lit_string_α
n337_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n338_lit_string_α:
                        mov              qword ptr [rsp + 2416], 2                      # result
                        mov              dword ptr [rsp + 2420], 11
                        mov              rax, qword ptr [rip + .Lx598_0]
                        mov              qword ptr [rsp + 2424], rax
                                                                                        jmp   n339_assign_α
.Lx598_0:
                        .quad            .Lx598_0_s
.Lx598_0_s:
                        .string          "no match ok"
#-----------------------------------------------------------------------------------------------------------------------
n339_assign_α:
                        mov              rsi, qword ptr [rsp + 2416]                    # val
                        mov              rdx, qword ptr [rsp + 2424]                    # val
                        mov              rdi, qword ptr [rip + .Lx599_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n340_statement_end_α
.Lx599_0:
                        .quad            .Lx599_0_s
.Lx599_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n340_statement_end_α:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n341_goto_α:
                                                                                        jmp   n342_statement_begin_α
n341_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n342_statement_begin_α:
                                                                                        jmp   n343_var_α
n342_statement_begin_β:
                                                                                        jmp   n349_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n343_var_α:
                        mov              rax, qword ptr [1879052288]                    # lwr
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 128], rax                     # result
                        mov              qword ptr [rsp + 136], rdx
                                                                                        jmp   n344_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n344_keyword_snobol4_α:
                        mov              rdi, qword ptr [rip + .Lx606_0]                # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n345_keyword_snobol4_α
.Lx606_0:
                        .quad            .Lx606_0_s
.Lx606_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n345_keyword_snobol4_α:
                        mov              rdi, qword ptr [rip + .Lx607_0]                # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                                                                                        jmp   n346_call_α
.Lx607_0:
                        .quad            .Lx607_0_s
.Lx607_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n346_call_α:
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 104], rax
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 88], rax
                        mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn609:              .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn609]                         # fn
                        lea              rsi, [rsp + 64]                                # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 104
                                                                                        je    n342_statement_begin_β
                                                                                        jmp   n347_assign_α
n346_call_β:
                                                                                        jmp   n342_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n347_assign_α:
                        mov              rax, qword ptr [rsp + 48]
                        mov              rdx, qword ptr [rsp + 56]
                        mov              qword ptr [1879052288], rax                    # lwr
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n348_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n348_statement_end_α:
                                                                                        jmp   n349_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n349_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx614_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx614_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n350_goto_α:
                                                                                        jmp   n243_statement_begin_α
n350_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n351_goto_α:
                                                                                        jmp   n352_statement_begin_α
n351_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n352_statement_begin_α:
                                                                                        jmp   n353_var_α
n352_statement_begin_β:
                                                                                        jmp   n349_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n353_var_α:
                        mov              rax, qword ptr [1879052304]                    # upr
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 256], rax                     # result
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n354_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n354_keyword_snobol4_α:
                        mov              rdi, qword ptr [rip + .Lx620_0]                # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n355_keyword_snobol4_α
.Lx620_0:
                        .quad            .Lx620_0_s
.Lx620_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n355_keyword_snobol4_α:
                        mov              rdi, qword ptr [rip + .Lx621_0]                # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n356_call_α
.Lx621_0:
                        .quad            .Lx621_0_s
.Lx621_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n356_call_α:
                        mov              rax, qword ptr [rsp + 288]
                        mov              qword ptr [rsp + 224], rax
                        mov              rax, qword ptr [rsp + 296]
                        mov              qword ptr [rsp + 232], rax
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 216], rax
                        mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 264]
                        mov              qword ptr [rsp + 200], rax
                        .section         .rodata
.Lrkfn623:              .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn623]                         # fn
                        lea              rsi, [rsp + 192]                               # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                        cmp              eax, 104
                                                                                        je    n352_statement_begin_β
                                                                                        jmp   n357_assign_α
n356_call_β:
                                                                                        jmp   n352_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n357_assign_α:
                        mov              rax, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        mov              qword ptr [1879052304], rax                    # upr
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n358_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n358_statement_end_α:
                                                                                        jmp   n349_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n359_goto_α:
                                                                                        jmp   n247_statement_begin_α
n359_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n360_goto_α:
                                                                                        jmp   n361_statement_begin_α
n360_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n361_statement_begin_α:
                                                                                        jmp   n362_var_α
n361_statement_begin_β:
                                                                                        jmp   n378_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n362_var_α:
                        mov              rax, qword ptr [1879052320]                    # cap
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 480], rax                     # result
                        mov              qword ptr [rsp + 488], rdx
                                                                                        jmp   n363_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n363_lit_integer_α:
                        mov              qword ptr [rsp + 496], 3                       # result
                        mov              rax, qword ptr [rip + .Lx632_0]
                        mov              qword ptr [rsp + 504], rax
                                                                                        jmp   n364_lit_integer_α
.Lx632_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_integer_α:
                        mov              qword ptr [rsp + 512], 3                       # result
                        mov              rax, qword ptr [rip + .Lx633_0]
                        mov              qword ptr [rsp + 520], rax
                                                                                        jmp   n365_call_α
.Lx633_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n365_call_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              qword ptr [rsp + 448], rax
                        mov              rax, qword ptr [rsp + 520]
                        mov              qword ptr [rsp + 456], rax
                        mov              rax, qword ptr [rsp + 496]
                        mov              qword ptr [rsp + 432], rax
                        mov              rax, qword ptr [rsp + 504]
                        mov              qword ptr [rsp + 440], rax
                        mov              rax, qword ptr [rsp + 480]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 488]
                        mov              qword ptr [rsp + 424], rax
                        .section         .rodata
.Lrkfn635:              .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn635]                         # fn
                        lea              rsi, [rsp + 416]                               # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        cmp              eax, 104
                                                                                        je    n361_statement_begin_β
                                                                                        jmp   n366_keyword_snobol4_α
n365_call_β:
                                                                                        jmp   n361_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n366_keyword_snobol4_α:
                        mov              rdi, qword ptr [rip + .Lx636_0]                # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 528], rax
                        mov              qword ptr [rsp + 536], rdx
                                                                                        jmp   n367_keyword_snobol4_α
.Lx636_0:
                        .quad            .Lx636_0_s
.Lx636_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n367_keyword_snobol4_α:
                        mov              rdi, qword ptr [rip + .Lx637_0]                # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                                                                                        jmp   n368_call_α
.Lx637_0:
                        .quad            .Lx637_0_s
.Lx637_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n368_call_α:
                        mov              rax, qword ptr [rsp + 544]
                        mov              qword ptr [rsp + 368], rax
                        mov              rax, qword ptr [rsp + 552]
                        mov              qword ptr [rsp + 376], rax
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 352], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 360], rax
                        mov              rax, qword ptr [rsp + 400]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 408]
                        mov              qword ptr [rsp + 344], rax
                        .section         .rodata
.Lrkfn639:              .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn639]                         # fn
                        lea              rsi, [rsp + 336]                               # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                        cmp              eax, 104
                                                                                        je    n361_statement_begin_β
                                                                                        jmp   n369_var_α
n368_call_β:
                                                                                        jmp   n361_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n369_var_α:
                        mov              rax, qword ptr [1879052320]                    # cap
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 704], rax                     # result
                        mov              qword ptr [rsp + 712], rdx
                                                                                        jmp   n370_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n370_lit_integer_α:
                        mov              qword ptr [rsp + 720], 3                       # result
                        mov              rax, qword ptr [rip + .Lx641_0]
                        mov              qword ptr [rsp + 728], rax
                                                                                        jmp   n371_call_α
.Lx641_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n371_call_α:
                        mov              rax, qword ptr [rsp + 720]
                        mov              qword ptr [rsp + 672], rax
                        mov              rax, qword ptr [rsp + 728]
                        mov              qword ptr [rsp + 680], rax
                        mov              rax, qword ptr [rsp + 704]
                        mov              qword ptr [rsp + 656], rax
                        mov              rax, qword ptr [rsp + 712]
                        mov              qword ptr [rsp + 664], rax
                        .section         .rodata
.Lrkfn643:              .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn643]                         # fn
                        lea              rsi, [rsp + 656]                               # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                        cmp              eax, 104
                                                                                        je    n361_statement_begin_β
                                                                                        jmp   n372_keyword_snobol4_α
n371_call_β:
                                                                                        jmp   n361_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n372_keyword_snobol4_α:
                        mov              rdi, qword ptr [rip + .Lx644_0]                # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n373_keyword_snobol4_α
.Lx644_0:
                        .quad            .Lx644_0_s
.Lx644_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n373_keyword_snobol4_α:
                        mov              rdi, qword ptr [rip + .Lx645_0]                # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n374_call_α
.Lx645_0:
                        .quad            .Lx645_0_s
.Lx645_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n374_call_α:
                        mov              rax, qword ptr [rsp + 752]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 760]
                        mov              qword ptr [rsp + 616], rax
                        mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 744]
                        mov              qword ptr [rsp + 600], rax
                        mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 576], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 584], rax
                        .section         .rodata
.Lrkfn647:              .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn647]                         # fn
                        lea              rsi, [rsp + 576]                               # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                        cmp              eax, 104
                                                                                        je    n361_statement_begin_β
                                                                                        jmp   n375_binop_α
n374_call_β:
                                                                                        jmp   n361_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n375_binop_α:
                        mov              rdi, qword ptr [rsp + 320]                     # a
                        mov              rsi, qword ptr [rsp + 328]                     # a
                        mov              rdx, qword ptr [rsp + 560]                     # b
                        mov              rcx, qword ptr [rsp + 568]                     # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n376_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n376_assign_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              rdx, qword ptr [rsp + 312]
                        mov              qword ptr [1879052320], rax                    # cap
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n377_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n377_statement_end_α:
                                                                                        jmp   n349_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n378_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx653_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 2
                                                                                        jmp   rax
.Lx653_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n379_goto_α:
                                                                                        jmp   n251_statement_begin_α
n379_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n380_goto_α:
                                                                                        jmp   n381_statement_begin_α
n380_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n381_statement_begin_α:
                                                                                        jmp   n382_var_α
n381_statement_begin_β:
                                                                                        jmp   n385_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n382_var_α:
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 816], rax                     # result
                        mov              qword ptr [rsp + 824], rdx
                                                                                        jmp   n383_call_α
#-----------------------------------------------------------------------------------------------------------------------
n383_call_α:
                        mov              rax, qword ptr [rsp + 816]
                        mov              qword ptr [rsp + 784], rax
                        mov              rax, qword ptr [rsp + 824]
                        mov              qword ptr [rsp + 792], rax
                        .section         .rodata
.Lrkfn660:              .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn660]                         # fn
                        lea              rsi, [rsp + 784]                               # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 104
                                                                                        je    n381_statement_begin_β
                                                                                        jmp   n384_statement_end_α
n383_call_β:
                                                                                        jmp   n381_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n384_statement_end_α:
                                                                                        jmp   n349_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n385_statement_begin_α:
                                                                                        jmp   n386_var_α
n385_statement_begin_β:
                                                                                        jmp   n404_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n386_var_α:
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1024], rax                    # result
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n387_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n387_match_begin_α:
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rbp                      # old_rbp
                        mov              rdi, qword ptr [rbp + 1024]                    # lo
                        mov              rsi, qword ptr [rbp + 1032]                    # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx667_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n388_lit_integer_α
n387_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx667_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx667_1
                                                                                        jmp   .Lx667_0
.Lx667_1:
n387_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                                                                                        jmp   n404_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n388_lit_integer_α:
                        mov              qword ptr [rsp + 960], 3                       # result
                        mov              rax, qword ptr [rip + .Lx668_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n389_match_pos_α
.Lx668_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n389_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx669_240
                        add              rsp, 16
                                                                                        jmp   n387_match_begin_β
.Lx669_240:
                                                                                        jmp   n390_match_assign_save_α
n389_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n387_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n390_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + -80], r14d
                        add              rsp, 16
                                                                                        jmp   n391_match_any_α
n390_match_assign_save_β:
                        sub              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n387_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n391_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx673_239
                        add              rsp, 16
                                                                                        jmp   n387_match_begin_β
.Lx673_239:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C0]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        jne   .Lx673_240
                        add              rsp, 16
                                                                                        jmp   n387_match_begin_β
.Lx673_240:
                        add              r14d, 1
                                                                                        jmp   n392_match_assign_cond_α
n391_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n387_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n392_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -80]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n393_match_end_α
n392_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n391_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n393_match_end_α:
                        mov              r10, r12
.Lx677_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx677_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 896], eax
                        mov              qword ptr [rsp + 920], r14
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx677_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx677_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx677_1:
                        test             rax, rax
                                                                                        je    .Lx677_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx677_3]
                        lea              rdx, [rip + .Lx677_4]
                                                                                        jmp   rax
.Lx677_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx677_1
.Lx677_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx677_1
.Lx677_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx677_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx677_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n394_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n394_lit_string_α:
                        mov              qword ptr [rsp + 944], 2                       # result
                        mov              dword ptr [rsp + 948], 0
                        mov              rax, qword ptr [rip + .Lx678_0]
                        mov              qword ptr [rsp + 952], rax
                                                                                        jmp   n395_match_replace_α
.Lx678_0:
                        .quad            .Lx678_0_s
.Lx678_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n395_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx680_0]                # name
                        mov              rsi, qword ptr [rbp + 1024]                    # sub_lo
                        mov              rdx, qword ptr [rbp + 1032]                    # sub_hi
                        mov              ecx, dword ptr [rbp + 848]                     # start
                        mov              r8, qword ptr [rbp + 872]                      # end
                        lea              r9, [rbp + 944]                                # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx680_1
.Lx680_0:
                        .quad            .Lx680_0_s
.Lx680_0_s:
                        .string          "str"
.Lx680_1:
                        mov              rbp, qword ptr [rbp + 888]                     # old_rbp
                                                                                        jmp   n396_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n396_statement_end_α:
                                                                                        jmp   n397_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n397_statement_begin_α:
                                                                                        jmp   n398_var_α
n397_statement_begin_β:
                                                                                        jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n398_var_α:
                        mov              rax, qword ptr [1879052336]                    # icase
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1104], rax                    # result
                        mov              qword ptr [rsp + 1112], rdx
                                                                                        jmp   n399_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n399_assign_α:
                        mov              rax, qword ptr [rsp + 1104]
                        mov              rdx, qword ptr [rsp + 1112]
                        mov              qword ptr [1879052400], rax                    # PAT$0$V0
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n400_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n400_lit_string_α:
                        mov              qword ptr [rsp + 1088], 2                      # result
                        mov              dword ptr [rsp + 1092], 5
                        mov              rax, qword ptr [rip + .Lx687_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n401_call_α
.Lx687_0:
                        .quad            .Lx687_0_s
.Lx687_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n401_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lrkfn689:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn689]                         # fn
                        lea              rsi, [rsp + 1056]                              # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        cmp              eax, 104
                                                                                        je    n397_statement_begin_β
                                                                                        jmp   n402_assign_α
n401_call_β:
                                                                                        jmp   n397_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n402_assign_α:
                        mov              rax, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        mov              qword ptr [1879052336], rax                    # icase
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n403_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n403_statement_end_α:
                                                                                        jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n404_statement_begin_α:
                                                                                        jmp   n405_var_α
n404_statement_begin_β:
                                                                                        jmp   n416_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n405_var_α:
                        mov              rax, qword ptr [1879052352]                    # str
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1312], rax                    # result
                        mov              qword ptr [rsp + 1320], rdx
                                                                                        jmp   n406_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n406_match_begin_α:
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rbp                      # old_rbp
                        mov              rdi, qword ptr [rbp + 1312]                    # lo
                        mov              rsi, qword ptr [rbp + 1320]                    # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx697_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n407_lit_integer_α
n406_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx697_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx697_1
                                                                                        jmp   .Lx697_0
.Lx697_1:
n406_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                                                                                        jmp   n416_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n407_lit_integer_α:
                        mov              qword ptr [rsp + 1248], 3                      # result
                        mov              rax, qword ptr [rip + .Lx698_0]
                        mov              qword ptr [rsp + 1256], rax
                                                                                        jmp   n408_match_pos_α
.Lx698_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n408_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx699_240
                        add              rsp, 16
                                                                                        jmp   n406_match_begin_β
.Lx699_240:
                                                                                        jmp   n409_match_assign_save_α
n408_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n406_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n409_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + -96], r14d
                        add              rsp, 16
                                                                                        jmp   n410_match_len_α
n409_match_assign_save_β:
                        sub              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n406_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n410_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jle   .Lx702_240
                        add              rsp, 16
                                                                                        jmp   n406_match_begin_β
.Lx702_240:
                        add              r14d, 1
                                                                                        jmp   n411_match_assign_cond_α
n410_match_len_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n406_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n411_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -96]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n412_match_end_α
n411_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n410_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n412_match_end_α:
                        mov              r10, r12
.Lx706_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx706_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 1184], eax
                        mov              qword ptr [rsp + 1208], r14
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx706_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx706_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx706_1:
                        test             rax, rax
                                                                                        je    .Lx706_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx706_3]
                        lea              rdx, [rip + .Lx706_4]
                                                                                        jmp   rax
.Lx706_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx706_1
.Lx706_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx706_1
.Lx706_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx706_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx706_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n413_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n413_lit_string_α:
                        mov              qword ptr [rsp + 1232], 2                      # result
                        mov              dword ptr [rsp + 1236], 0
                        mov              rax, qword ptr [rip + .Lx707_0]
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n414_match_replace_α
.Lx707_0:
                        .quad            .Lx707_0_s
.Lx707_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n414_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx709_0]                # name
                        mov              rsi, qword ptr [rbp + 1312]                    # sub_lo
                        mov              rdx, qword ptr [rbp + 1320]                    # sub_hi
                        mov              ecx, dword ptr [rbp + 1136]                    # start
                        mov              r8, qword ptr [rbp + 1160]                     # end
                        lea              r9, [rbp + 1232]                               # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx709_1
.Lx709_0:
                        .quad            .Lx709_0_s
.Lx709_0_s:
                        .string          "str"
.Lx709_1:
                        mov              rbp, qword ptr [rbp + 1176]                    # old_rbp
                                                                                        jmp   n415_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n415_statement_end_α:
                                                                                        jmp   n416_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n416_statement_begin_α:
                                                                                        jmp   n417_var_α
n416_statement_begin_β:
                                                                                        jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n417_var_α:
                        mov              rax, qword ptr [1879052336]                    # icase
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1344], rax                    # result
                        mov              qword ptr [rsp + 1352], rdx
                                                                                        jmp   n418_var_α
#-----------------------------------------------------------------------------------------------------------------------
n418_var_α:
                        mov              rax, qword ptr [1879052384]                    # ch
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 1360], rax                    # result
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n419_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n419_binop_α:
                        mov              rdi, qword ptr [rsp + 1344]                    # a
                        mov              rsi, qword ptr [rsp + 1352]                    # a
                        mov              rdx, qword ptr [rsp + 1360]                    # b
                        mov              rcx, qword ptr [rsp + 1368]                    # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1328], rax
                        mov              qword ptr [rsp + 1336], rdx
                                                                                        jmp   n420_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n420_assign_α:
                        mov              rax, qword ptr [rsp + 1328]
                        mov              rdx, qword ptr [rsp + 1336]
                        mov              qword ptr [1879052336], rax                    # icase
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n421_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n421_statement_end_α:
                                                                                        jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n422_goto_α:
                                                                                        jmp   n404_statement_begin_α
n422_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n423_goto_α:
                                                                                        jmp   n255_statement_begin_α
n423_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n424_goto_α:
                                                                                        jmp   n295_statement_begin_α
n424_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n425_goto_α:
                                                                                        jmp   n309_statement_begin_α
n425_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n426_goto_α:
                                                                                        jmp   n323_statement_begin_α
n426_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n427_goto_α:
                                                                                        jmp   n333_statement_begin_α
n427_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
                        .section         .rodata
.S0:                    .string          "letter"
.S1:                    .string          "ch"
.S2:                    .string          "PAT$0$V0"
                        .text
                        .section         .rodata
.C0:
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                        .byte            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
                        .byte            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                        .byte            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .text
                        .section         .note.GNU-stack,"",@progbits

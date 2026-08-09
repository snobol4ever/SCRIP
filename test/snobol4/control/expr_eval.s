                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_α:
proc_LBL__Push_α_body:
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_var_α
n0_statement_begin_β:
                                                                                        jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n3_subscript_α
n2_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n0_statement_begin_β
.Lx34_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n3_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx35_240
                        add              rsp, 16
                                                                                        jmp   n2_lit_integer_β
.Lx35_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n4_var_α
n3_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n2_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n5_lit_integer_α
n4_var_β:
                        add              rsp, 16
                                                                                        jmp   n3_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx37_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n6_subscript_α
n5_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n4_var_β
.Lx37_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n6_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx38_240
                        add              rsp, 16
                                                                                        jmp   n5_lit_integer_β
.Lx38_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n7_deref_α
n6_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n5_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n7_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx39_240
                        add              rsp, 16
                                                                                        jmp   n6_subscript_β
.Lx39_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n8_lit_integer_α
n7_deref_β:
                        add              rsp, 16
                                                                                        jmp   n6_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n9_binop_α
n8_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n7_deref_β
.Lx40_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n9_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx41_240
                        add              rsp, 16
                                                                                        jmp   n8_lit_integer_β
.Lx41_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n10_assign_var_α
n9_binop_β:
                        add              rsp, 16
                                                                                        jmp   n8_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # subscript
                        mov              rsi, qword ptr [rsp + 120]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx42_240
                        add              rsp, 16
                                                                                        jmp   n9_binop_β
.Lx42_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n11_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:
                        add              rsp, 160
                                                                                        jmp   n12_statement_begin_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:
                                                                                        jmp   n13_var_α
n12_statement_begin_β:
                                                                                        jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n14_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n15_lit_integer_α
n14_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n16_subscript_α
n15_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n14_var_β
.Lx49_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n16_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx50_240
                        add              rsp, 16
                                                                                        jmp   n15_lit_integer_β
.Lx50_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n17_deref_α
n16_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n15_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n17_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx51_240
                        add              rsp, 16
                                                                                        jmp   n16_subscript_β
.Lx51_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n18_subscript_α
n17_deref_β:
                        add              rsp, 16
                                                                                        jmp   n16_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n18_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx52_240
                        add              rsp, 16
                                                                                        jmp   n17_deref_β
.Lx52_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n19_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # subscript
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # Push
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n20_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n21_statement_begin_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_begin_α:
                                                                                        jmp   n22_var_α
n21_statement_begin_β:
                                                                                        jmp   n27_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n22_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n23_call_α
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd60:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd60]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx59_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n21_statement_begin_β
.Lx59_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n24_var_α
n23_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n21_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n25_assign_var_α
n24_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n21_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx62_240
                        add              rsp, 16
                                                                                        jmp   n24_var_β
.Lx62_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n26_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:
                                                                                        jmp   n27_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_call_α
.Lx65_0:
                        .quad            .Lx65_0_s
.Lx65_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n28_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd67:             .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd67]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx66_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n30_save_restore_α
.Lx66_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n29_save_restore_α
n28_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n30_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n29_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx69_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx69_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n30_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx71_0
                        mov              cl, 2
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx71_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_α:
proc_LBL__Pop_α_body:
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:
                                                                                        jmp   n73_var_α
n72_statement_begin_β:
                                                                                        jmp   n82_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n74_var_α
#-----------------------------------------------------------------------------------------------------------------------
n74_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n75_lit_integer_α
n74_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n72_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n76_subscript_α
n75_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n74_var_β
.Lx99_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n76_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx100_240
                        add              rsp, 16
                                                                                        jmp   n75_lit_integer_β
.Lx100_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n77_deref_α
n76_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n75_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n77_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx101_240
                        add              rsp, 16
                                                                                        jmp   n76_subscript_β
.Lx101_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n78_subscript_α
n77_deref_β:
                        add              rsp, 16
                                                                                        jmp   n76_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n78_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx102_240
                        add              rsp, 16
                                                                                        jmp   n77_deref_β
.Lx102_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n79_deref_α
n78_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n77_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n79_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx103_240
                        add              rsp, 16
                                                                                        jmp   n78_subscript_β
.Lx103_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n80_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n80_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # deref
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # Pop
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n81_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_end_α:
                        add              rsp, 112
                                                                                        jmp   n82_statement_begin_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_begin_α:
                                                                                        jmp   n83_var_α
n82_statement_begin_β:
                                                                                        jmp   n94_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n83_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n84_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n85_subscript_α
n84_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n82_statement_begin_β
.Lx110_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n85_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx111_240
                        add              rsp, 16
                                                                                        jmp   n84_lit_integer_β
.Lx111_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n86_var_α
n85_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n84_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n86_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n87_lit_integer_α
n86_var_β:
                        add              rsp, 16
                                                                                        jmp   n85_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n88_subscript_α
n87_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n86_var_β
.Lx113_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n88_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx114_240
                        add              rsp, 16
                                                                                        jmp   n87_lit_integer_β
.Lx114_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n89_deref_α
n88_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n87_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n89_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx115_240
                        add              rsp, 16
                                                                                        jmp   n88_subscript_β
.Lx115_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n90_lit_integer_α
n89_deref_β:
                        add              rsp, 16
                                                                                        jmp   n88_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx116_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n91_binop_α
n90_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n89_deref_β
.Lx116_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n91_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx117_240
                        add              rsp, 16
                                                                                        jmp   n90_lit_integer_β
.Lx117_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n92_assign_var_α
n91_binop_β:
                        add              rsp, 16
                                                                                        jmp   n90_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n92_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # subscript
                        mov              rsi, qword ptr [rsp + 120]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx118_240
                        add              rsp, 16
                                                                                        jmp   n91_binop_β
.Lx118_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n93_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_end_α:
                                                                                        jmp   n94_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n94_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx122_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx122_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_α:
proc_LBL__Unary_α_body:
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_begin_α:
                                                                                        jmp   n124_call_α
n123_statement_begin_β:
                                                                                        jmp   n127_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n124_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx153_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx153_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx153_6]
                        lea              rdx, [rip + .Lx153_7]
                                                                                        jmp   rax
.Lx153_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx153_2
.Lx153_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx153_2
.Lx153_5:
                        add              rsp, 16
.Lx153_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx153_240
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_β
.Lx153_240:
                                                                                        jmp   n125_assign_α
n124_call_β:
                                                                                        jmp   n123_statement_begin_β
.Lx153_0:
                        .quad            .Lx153_0_s
.Lx153_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n125_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # arg
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n126_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n127_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_begin_α:
                                                                                        jmp   n128_call_α
n127_statement_begin_β:
                                                                                        jmp   n131_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n128_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx160_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx160_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx160_6]
                        lea              rdx, [rip + .Lx160_7]
                                                                                        jmp   rax
.Lx160_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx160_2
.Lx160_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx160_2
.Lx160_5:
                        add              rsp, 16
.Lx160_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx160_240
                        add              rsp, 16
                                                                                        jmp   n127_statement_begin_β
.Lx160_240:
                                                                                        jmp   n129_assign_α
n128_call_β:
                                                                                        jmp   n127_statement_begin_β
.Lx160_0:
                        .quad            .Lx160_0_s
.Lx160_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n129_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # op
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n130_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n131_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_begin_α:
                                                                                        jmp   n132_lit_string_α
n131_statement_begin_β:
                                                                                        jmp   n141_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n133_call_α
.Lx166_0:
                        .quad            .Lx166_0_s
.Lx166_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n133_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd168:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd168]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx167_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n131_statement_begin_β
.Lx167_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n134_call_α
n133_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n131_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n134_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx170_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx170_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx170_6]
                        lea              rdx, [rip + .Lx170_7]
                                                                                        jmp   rax
.Lx170_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx170_2
.Lx170_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # Push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx170_2
.Lx170_5:
                        add              rsp, 32
.Lx170_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx170_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n131_statement_begin_β
.Lx170_240:
                                                                                        jmp   n135_var_α
n134_call_β:
                                                                                        jmp   n131_statement_begin_β
.Lx170_0:
                        .quad            .Lx170_0_s
.Lx170_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n135_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n136_var_α
n135_var_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n131_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n136_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # arg
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n137_binop_α
n136_var_β:
                        add              rsp, 16
                                                                                        jmp   n135_var_β
#-----------------------------------------------------------------------------------------------------------------------
n137_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n138_call_α
n137_binop_β:
                        add              rsp, 16
                                                                                        jmp   n136_var_β
#-----------------------------------------------------------------------------------------------------------------------
n138_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd175:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd175]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx174_240
                        add              rsp, 16
                                                                                        jmp   n137_binop_β
.Lx174_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n139_assign_var_α
n138_call_β:
                        add              rsp, 16
                                                                                        jmp   n137_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n139_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # call
                        mov              rsi, qword ptr [rsp + 88]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx176_240
                        add              rsp, 32
                                                                                        jmp   n137_binop_β
.Lx176_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n140_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_end_α:
                        add              rsp, 128
                                                                                        jmp   n141_statement_begin_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_begin_α:
                                                                                        jmp   n142_lit_string_α
n141_statement_begin_β:
                                                                                        jmp   n146_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx181_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n143_call_α
.Lx181_0:
                        .quad            .Lx181_0_s
.Lx181_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n143_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd183:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd183]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx182_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n141_statement_begin_β
.Lx182_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n144_assign_α
n143_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n141_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n144_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # Unary
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n145_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_end_α:
                                                                                        jmp   n146_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n146_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n147_call_α
.Lx187_0:
                        .quad            .Lx187_0_s
.Lx187_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n147_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd189:            .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd189]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx188_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n149_save_restore_α
.Lx188_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n148_save_restore_α
n147_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n149_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n148_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx191_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx191_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n149_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx193_0
                        mov              cl, 2
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx193_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_α:
proc_LBL__Binary_α_body:
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_begin_α:
                                                                                        jmp   n195_call_α
n194_statement_begin_β:
                                                                                        jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n195_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx234_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx234_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx234_6]
                        lea              rdx, [rip + .Lx234_7]
                                                                                        jmp   rax
.Lx234_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx234_2
.Lx234_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx234_2
.Lx234_5:
                        add              rsp, 16
.Lx234_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx234_240
                        add              rsp, 16
                                                                                        jmp   n194_statement_begin_β
.Lx234_240:
                                                                                        jmp   n196_assign_α
n195_call_β:
                                                                                        jmp   n194_statement_begin_β
.Lx234_0:
                        .quad            .Lx234_0_s
.Lx234_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n196_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052416], rax                    # right
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n197_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n197_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n198_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_begin_α:
                                                                                        jmp   n199_call_α
n198_statement_begin_β:
                                                                                        jmp   n202_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n199_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx241_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx241_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx241_6]
                        lea              rdx, [rip + .Lx241_7]
                                                                                        jmp   rax
.Lx241_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx241_2
.Lx241_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx241_2
.Lx241_5:
                        add              rsp, 16
.Lx241_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx241_240
                        add              rsp, 16
                                                                                        jmp   n198_statement_begin_β
.Lx241_240:
                                                                                        jmp   n200_assign_α
n199_call_β:
                                                                                        jmp   n198_statement_begin_β
.Lx241_0:
                        .quad            .Lx241_0_s
.Lx241_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n200_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # op
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n201_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n201_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n202_statement_begin_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_begin_α:
                                                                                        jmp   n203_call_α
n202_statement_begin_β:
                                                                                        jmp   n206_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n203_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx248_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx248_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx248_6]
                        lea              rdx, [rip + .Lx248_7]
                                                                                        jmp   rax
.Lx248_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx248_2
.Lx248_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx248_2
.Lx248_5:
                        add              rsp, 16
.Lx248_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx248_240
                        add              rsp, 16
                                                                                        jmp   n202_statement_begin_β
.Lx248_240:
                                                                                        jmp   n204_assign_α
n203_call_β:
                                                                                        jmp   n202_statement_begin_β
.Lx248_0:
                        .quad            .Lx248_0_s
.Lx248_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n204_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052400], rax                    # left
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n205_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n205_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n206_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n206_statement_begin_α:
                                                                                        jmp   n207_lit_string_α
n206_statement_begin_β:
                                                                                        jmp   n222_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n207_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx254_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n208_call_α
.Lx254_0:
                        .quad            .Lx254_0_s
.Lx254_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n208_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd256:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd256]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx255_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n206_statement_begin_β
.Lx255_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n209_call_α
n208_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n206_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n209_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx258_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx258_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx258_6]
                        lea              rdx, [rip + .Lx258_7]
                                                                                        jmp   rax
.Lx258_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx258_2
.Lx258_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # Push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx258_2
.Lx258_5:
                        add              rsp, 32
.Lx258_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx258_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n206_statement_begin_β
.Lx258_240:
                                                                                        jmp   n210_var_α
n209_call_β:
                                                                                        jmp   n206_statement_begin_β
.Lx258_0:
                        .quad            .Lx258_0_s
.Lx258_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n210_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]                    # left
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n211_lit_string_α
n210_var_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n206_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n211_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n212_binop_α
n211_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n210_var_β
.Lx260_0:
                        .quad            .Lx260_0_s
.Lx260_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n212_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n213_var_α
n212_binop_β:
                        add              rsp, 16
                                                                                        jmp   n211_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n213_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n214_binop_α
n213_var_β:
                        add              rsp, 16
                                                                                        jmp   n212_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n214_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n215_lit_string_α
n214_binop_β:
                        add              rsp, 16
                                                                                        jmp   n213_var_β
#-----------------------------------------------------------------------------------------------------------------------
n215_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n216_binop_α
n215_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n214_binop_β
.Lx264_0:
                        .quad            .Lx264_0_s
.Lx264_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n216_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n217_var_α
n216_binop_β:
                        add              rsp, 16
                                                                                        jmp   n215_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n217_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052416]                    # right
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n218_binop_α
n217_var_β:
                        add              rsp, 16
                                                                                        jmp   n216_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n218_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n219_call_α
n218_binop_β:
                        add              rsp, 16
                                                                                        jmp   n217_var_β
#-----------------------------------------------------------------------------------------------------------------------
n219_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd269:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd269]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx268_240
                        add              rsp, 16
                                                                                        jmp   n218_binop_β
.Lx268_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n220_assign_var_α
n219_call_β:
                        add              rsp, 16
                                                                                        jmp   n218_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n220_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 176]                     # call
                        mov              rsi, qword ptr [rsp + 184]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx270_240
                        add              rsp, 32
                                                                                        jmp   n218_binop_β
.Lx270_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n221_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n221_statement_end_α:
                        add              rsp, 224
                                                                                        jmp   n222_statement_begin_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n222_statement_begin_α:
                                                                                        jmp   n223_lit_string_α
n222_statement_begin_β:
                                                                                        jmp   n227_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n223_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx275_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n224_call_α
.Lx275_0:
                        .quad            .Lx275_0_s
.Lx275_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n224_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd277:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd277]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx276_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n222_statement_begin_β
.Lx276_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n225_assign_α
n224_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n222_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n225_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052384], rax                    # Binary
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n226_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_end_α:
                                                                                        jmp   n227_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n227_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx281_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n228_call_α
.Lx281_0:
                        .quad            .Lx281_0_s
.Lx281_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n228_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd283:            .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd283]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx282_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n230_save_restore_α
.Lx282_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n229_save_restore_α
n228_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n230_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n229_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx285_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx285_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n230_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx287_0
                        mov              cl, 2
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx287_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Push_α
proc_Push_α:
proc_Push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n288_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n289_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n289_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx293_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx293_1
.Lx293_0:
                        .quad            .Lx293_0_s
.Lx293_0_s:
                        .string          "Push"
.Lx293_1:
                                                                                        jmp   proc_Push_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_β:
                                                                                        jmp   proc_Push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Pop_α
proc_Pop_α:
proc_Pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n294_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n295_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n295_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx299_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx299_1
.Lx299_0:
                        .quad            .Lx299_0_s
.Lx299_0_s:
                        .string          "Pop"
.Lx299_1:
                                                                                        jmp   proc_Pop_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_β:
                                                                                        jmp   proc_Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Unary_α
proc_Unary_α:
proc_Unary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n300_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n301_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n301_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx305_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx305_1
.Lx305_0:
                        .quad            .Lx305_0_s
.Lx305_0_s:
                        .string          "Unary"
.Lx305_1:
                                                                                        jmp   proc_Unary_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_β:
                                                                                        jmp   proc_Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Binary_α
proc_Binary_α:
proc_Binary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n306_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n307_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n307_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx311_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx311_1
.Lx311_0:
                        .quad            .Lx311_0_s
.Lx311_0_s:
                        .string          "Binary"
.Lx311_1:
                                                                                        jmp   proc_Binary_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_β:
                                                                                        jmp   proc_Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        sub              rsp, 80
                        mov              qword ptr [rsp + 56], rcx
                        mov              qword ptr [rsp + 64], rdx
                        mov              qword ptr [rsp + 72], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 48], r8
                        mov              dword ptr [rsp + 40], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + n312_match_span_β]
                        mov              qword ptr [rbp + 16], rax
#-----------------------------------------------------------------------------------------------------------------------
n312_match_span_α:
                        sub              rsp, 16
                        lea              rdi, [rip + .C0]
                        movsxd           rcx, r14d
.Lx314_0:
                        cmp              ecx, r15d
                                                                                        jge   .Lx314_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx314_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx314_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx314_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx314_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx314_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx314_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx314_1
                        add              ecx, 1
                                                                                        jmp   .Lx314_0
.Lx314_1:
                        cmp              ecx, r14d
                                                                                        jg    .Lx314_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
.Lx314_240:
                        mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx
                                                                                        jmp   proc_PAT$0_scanhit
n312_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 48], 1
                                                                                        jne   .Lx315_0
                        mov              ecx, dword ptr [rbp + 40]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx315_0:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 48], 1
                                                                                        jne   .Lx316_0
                        mov              eax, dword ptr [rbp + 40]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx316_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx316_0
                        mov              dword ptr [rbp + 40], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$0_attempt
.Lx316_0:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                                        jmp   qword ptr [rbp + 16]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 56]
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rax, qword ptr [rbp + 64]
                        lea              rsp, [rbp + 80]
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$1_α
proc_PAT$1_α:
                        sub              rsp, 160
                        mov              qword ptr [rsp + 136], rcx
                        mov              qword ptr [rsp + 144], rdx
                        mov              qword ptr [rsp + 152], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 128], r8
                        mov              dword ptr [rsp + 120], r14d
proc_PAT$1_attempt:
proc_PAT$1_α_body:
                        lea              rax, [rip + proc_PAT$1_ω]
                        mov              qword ptr [rbp + 96], rax
#-----------------------------------------------------------------------------------------------------------------------
n317_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   proc_PAT$1_scanfail
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C1]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    proc_PAT$1_scanfail
                        add              r14d, 1
                                                                                        jmp   n318_match_alternate_α
n317_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   proc_PAT$1_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n318_match_alternate_α:
                        mov              dword ptr [rbp + 32], r14d
                        lea              rax, [rip + .Lx325_21]
                        mov              qword ptr [rbp + 48], rax
                                                                                        jmp   n321_match_any_α
.Lx325_21:
                        lea              rax, [rip + .Lx325_19]
                        mov              qword ptr [rbp + 48], rax
                                                                                        jmp   n320_match_defer_α
n318_match_alternate_s0:
                        lea              rax, [rip + .Lx325_40]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n318_match_alternate_as
n318_match_alternate_s1:
                        lea              rax, [rip + .Lx325_41]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n318_match_alternate_as
.Lx325_40:
                                                                                        jmp   n321_match_any_β
.Lx325_41:
                                                                                        jmp   n320_match_defer_β
n318_match_alternate_as:
                                                                                        jmp   n319_match_defer_α
n318_match_alternate_β:
                        mov              rax, qword ptr [rbp + 40]
                                                                                        jmp   rax
n318_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 32]
                        mov              rax, qword ptr [rbp + 48]
                                                                                        jmp   rax
.Lx325_19:
                                                                                        jmp   n317_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n319_match_defer_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052672]                    # PAT$1$V1
                        mov              rdx, qword ptr [1879052680]
                        cmp              eax, 8
                                                                                        jne   .Lx326_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx326_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx326_10
.Lx326_9:
                        xor              eax, eax
.Lx326_10:
                        test             rax, rax
                                                                                        jz    .Lx326_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx326_4]
                        lea              rdx, [rip + .Lx326_5]
                                                                                        jmp   rax
.Lx326_4:
                                                                                        jmp   proc_PAT$1_scanhit
.Lx326_5:
                        add              rsp, 16
                                                                                        jmp   n318_match_alternate_β
.Lx326_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx326_2:
                        test             rax, rax
                                                                                        je    .Lx326_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx326_7]
                        lea              rdx, [rip + .Lx326_8]
                                                                                        jmp   rax
.Lx326_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx326_2
.Lx326_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx326_2
.Lx326_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        jns   .Lx326_240
                        add              rsp, 16
                                                                                        jmp   n318_match_alternate_β
.Lx326_240:
                        mov              r14d, eax
                        lea              rax, [rip + .Lx326_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   proc_PAT$1_scanhit
.Lx326_6:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n318_match_alternate_β
n319_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n320_match_defer_α:
                        mov              rax, qword ptr [1879052656]                    # PAT$1$V0
                        mov              rdx, qword ptr [1879052664]
                        cmp              eax, 8
                                                                                        jne   .Lx327_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx327_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx327_10
.Lx327_9:
                        xor              eax, eax
.Lx327_10:
                        test             rax, rax
                                                                                        jz    .Lx327_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx327_4]
                        lea              rdx, [rip + .Lx327_5]
                                                                                        jmp   rax
.Lx327_4:
                                                                                        jmp   n318_match_alternate_s1
.Lx327_5:
                                                                                        jmp   n318_match_alternate_af
.Lx327_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx327_2:
                        test             rax, rax
                                                                                        je    .Lx327_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx327_7]
                        lea              rdx, [rip + .Lx327_8]
                                                                                        jmp   rax
.Lx327_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx327_2
.Lx327_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx327_2
.Lx327_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n318_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx327_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n318_match_alternate_s1
.Lx327_6:
                        add              rsp, 16
                                                                                        jmp   n318_match_alternate_af
n320_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n321_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n318_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx329_0
                        cmp              esi, 45
                                                                                        je    .Lx329_0
                                                                                        jmp   n318_match_alternate_af
.Lx329_0:
                        add              r14d, 1
                                                                                        jmp   n318_match_alternate_s0
n321_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n318_match_alternate_af
proc_PAT$1_scanhit:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   .Lx330_0
                        mov              ecx, dword ptr [rbp + 120]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx330_0:
                                                                                        jmp   proc_PAT$1_γ
proc_PAT$1_scanfail:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   .Lx331_0
                        mov              eax, dword ptr [rbp + 120]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx331_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx331_0
                        mov              dword ptr [rbp + 120], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$1_attempt
.Lx331_0:
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_β:
                                                                                        jmp   qword ptr [rbp + 96]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$1_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 136]
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_ω:
                        mov              rax, qword ptr [rbp + 144]
                        lea              rsp, [rbp + 160]
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$2_α
proc_PAT$2_α:
                        sub              rsp, 304
                        mov              qword ptr [rsp + 280], rcx
                        mov              qword ptr [rsp + 288], rdx
                        mov              qword ptr [rsp + 296], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 272], r8
                        mov              dword ptr [rsp + 264], r14d
proc_PAT$2_attempt:
proc_PAT$2_α_body:
                        lea              rax, [rip + n332_match_alternate_β]
                        mov              qword ptr [rbp + 240], rax
#-----------------------------------------------------------------------------------------------------------------------
n332_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx346_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n337_match_defer_α
.Lx346_21:
                        lea              rax, [rip + .Lx346_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n334_match_defer_α
n332_match_alternate_s0:
                        lea              rax, [rip + .Lx346_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n332_match_alternate_as
n332_match_alternate_s1:
                        lea              rax, [rip + .Lx346_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n332_match_alternate_as
.Lx346_40:
                                                                                        jmp   n336_goto_β
.Lx346_41:
                                                                                        jmp   n333_goto_β
n332_match_alternate_as:
                                                                                        jmp   proc_PAT$2_γ
n332_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n332_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx346_19:
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n333_goto_α:
                                                                                        jmp   n332_match_alternate_af
n333_goto_β:
                                                                                        jmp   n332_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n334_match_defer_α:
                        mov              rax, qword ptr [1879052768]                    # PAT$2$V5
                        mov              rdx, qword ptr [1879052776]
                        cmp              eax, 8
                                                                                        jne   .Lx348_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx348_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx348_10
.Lx348_9:
                        xor              eax, eax
.Lx348_10:
                        test             rax, rax
                                                                                        jz    .Lx348_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx348_4]
                        lea              rdx, [rip + .Lx348_5]
                                                                                        jmp   rax
.Lx348_4:
                                                                                        jmp   n335_match_defer_α
.Lx348_5:
                                                                                        jmp   n332_match_alternate_af
.Lx348_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx348_2:
                        test             rax, rax
                                                                                        je    .Lx348_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx348_7]
                        lea              rdx, [rip + .Lx348_8]
                                                                                        jmp   rax
.Lx348_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx348_2
.Lx348_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx348_2
.Lx348_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n332_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx348_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n335_match_defer_α
.Lx348_6:
                        add              rsp, 16
                                                                                        jmp   n332_match_alternate_af
n334_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n335_match_defer_α:
                        mov              rax, qword ptr [1879052784]                    # PAT$2$V6
                        mov              rdx, qword ptr [1879052792]
                        cmp              eax, 8
                                                                                        jne   .Lx349_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx349_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx349_10
.Lx349_9:
                        xor              eax, eax
.Lx349_10:
                        test             rax, rax
                                                                                        jz    .Lx349_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx349_4]
                        lea              rdx, [rip + .Lx349_5]
                                                                                        jmp   rax
.Lx349_4:
                                                                                        jmp   n332_match_alternate_s1
.Lx349_5:
                                                                                        jmp   n334_match_defer_β
.Lx349_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx349_2:
                        test             rax, rax
                                                                                        je    .Lx349_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx349_7]
                        lea              rdx, [rip + .Lx349_8]
                                                                                        jmp   rax
.Lx349_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx349_2
.Lx349_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx349_2
.Lx349_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n334_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx349_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n332_match_alternate_s1
.Lx349_6:
                        add              rsp, 16
                                                                                        jmp   n334_match_defer_β
n335_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n336_goto_α:
                                                                                        jmp   n332_match_alternate_af
n336_goto_β:
                                                                                        jmp   n332_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n337_match_defer_α:
                        mov              rax, qword ptr [1879052688]                    # PAT$2$V0
                        mov              rdx, qword ptr [1879052696]
                        cmp              eax, 8
                                                                                        jne   .Lx351_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx351_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx351_10
.Lx351_9:
                        xor              eax, eax
.Lx351_10:
                        test             rax, rax
                                                                                        jz    .Lx351_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx351_4]
                        lea              rdx, [rip + .Lx351_5]
                                                                                        jmp   rax
.Lx351_4:
                                                                                        jmp   n338_match_lit_α
.Lx351_5:
                                                                                        jmp   n332_match_alternate_af
.Lx351_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx351_2:
                        test             rax, rax
                                                                                        je    .Lx351_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx351_7]
                        lea              rdx, [rip + .Lx351_8]
                                                                                        jmp   rax
.Lx351_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx351_2
.Lx351_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx351_2
.Lx351_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n332_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx351_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n338_match_lit_α
.Lx351_6:
                        add              rsp, 16
                                                                                        jmp   n332_match_alternate_af
n337_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n338_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n337_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 46
                                                                                        jne   n337_match_defer_β
                        add              r14d, 1
                                                                                        jmp   n339_match_alternate_α
n338_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n337_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n339_match_alternate_α:
                        mov              dword ptr [rbp + 80], r14d
                        lea              rax, [rip + .Lx355_21]
                        mov              qword ptr [rbp + 96], rax
                                                                                        jmp   n344_match_defer_α
.Lx355_21:
                        lea              rax, [rip + .Lx355_19]
                        mov              qword ptr [rbp + 96], rax
                                                                                        jmp   n343_match_defer_α
n339_match_alternate_s0:
                        lea              rax, [rip + .Lx355_40]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n339_match_alternate_as
n339_match_alternate_s1:
                        lea              rax, [rip + .Lx355_41]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n339_match_alternate_as
.Lx355_40:
                                                                                        jmp   n344_match_defer_β
.Lx355_41:
                                                                                        jmp   n343_match_defer_β
n339_match_alternate_as:
                                                                                        jmp   n340_match_alternate_α
n339_match_alternate_β:
                        mov              rax, qword ptr [rbp + 88]
                                                                                        jmp   rax
n339_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 80]
                        mov              rax, qword ptr [rbp + 96]
                                                                                        jmp   rax
.Lx355_19:
                                                                                        jmp   n338_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n340_match_alternate_α:
                        mov              dword ptr [rbp + 144], r14d
                        lea              rax, [rip + .Lx357_21]
                        mov              qword ptr [rbp + 160], rax
                                                                                        jmp   n342_match_defer_α
.Lx357_21:
                        lea              rax, [rip + .Lx357_19]
                        mov              qword ptr [rbp + 160], rax
                                                                                        jmp   n341_match_defer_α
n340_match_alternate_s0:
                        lea              rax, [rip + .Lx357_40]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n340_match_alternate_as
n340_match_alternate_s1:
                        lea              rax, [rip + .Lx357_41]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n340_match_alternate_as
.Lx357_40:
                                                                                        jmp   n342_match_defer_β
.Lx357_41:
                                                                                        jmp   n341_match_defer_β
n340_match_alternate_as:
                                                                                        jmp   n332_match_alternate_s0
n340_match_alternate_β:
                        mov              rax, qword ptr [rbp + 152]
                                                                                        jmp   rax
n340_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 144]
                        mov              rax, qword ptr [rbp + 160]
                                                                                        jmp   rax
.Lx357_19:
                                                                                        jmp   n339_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n341_match_defer_α:
                        mov              rax, qword ptr [1879052752]                    # PAT$2$V4
                        mov              rdx, qword ptr [1879052760]
                        cmp              eax, 8
                                                                                        jne   .Lx358_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx358_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx358_10
.Lx358_9:
                        xor              eax, eax
.Lx358_10:
                        test             rax, rax
                                                                                        jz    .Lx358_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx358_4]
                        lea              rdx, [rip + .Lx358_5]
                                                                                        jmp   rax
.Lx358_4:
                                                                                        jmp   n340_match_alternate_s1
.Lx358_5:
                                                                                        jmp   n340_match_alternate_af
.Lx358_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx358_2:
                        test             rax, rax
                                                                                        je    .Lx358_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx358_7]
                        lea              rdx, [rip + .Lx358_8]
                                                                                        jmp   rax
.Lx358_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx358_2
.Lx358_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx358_2
.Lx358_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n340_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx358_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n340_match_alternate_s1
.Lx358_6:
                        add              rsp, 16
                                                                                        jmp   n340_match_alternate_af
n341_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n342_match_defer_α:
                        mov              rax, qword ptr [1879052736]                    # PAT$2$V3
                        mov              rdx, qword ptr [1879052744]
                        cmp              eax, 8
                                                                                        jne   .Lx359_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx359_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx359_10
.Lx359_9:
                        xor              eax, eax
.Lx359_10:
                        test             rax, rax
                                                                                        jz    .Lx359_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx359_4]
                        lea              rdx, [rip + .Lx359_5]
                                                                                        jmp   rax
.Lx359_4:
                                                                                        jmp   n340_match_alternate_s0
.Lx359_5:
                                                                                        jmp   n340_match_alternate_af
.Lx359_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx359_2:
                        test             rax, rax
                                                                                        je    .Lx359_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx359_7]
                        lea              rdx, [rip + .Lx359_8]
                                                                                        jmp   rax
.Lx359_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx359_2
.Lx359_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx359_2
.Lx359_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n340_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx359_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n340_match_alternate_s0
.Lx359_6:
                        add              rsp, 16
                                                                                        jmp   n340_match_alternate_af
n342_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n343_match_defer_α:
                        mov              rax, qword ptr [1879052720]                    # PAT$2$V2
                        mov              rdx, qword ptr [1879052728]
                        cmp              eax, 8
                                                                                        jne   .Lx360_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx360_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx360_10
.Lx360_9:
                        xor              eax, eax
.Lx360_10:
                        test             rax, rax
                                                                                        jz    .Lx360_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx360_4]
                        lea              rdx, [rip + .Lx360_5]
                                                                                        jmp   rax
.Lx360_4:
                                                                                        jmp   n339_match_alternate_s1
.Lx360_5:
                                                                                        jmp   n339_match_alternate_af
.Lx360_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S7]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx360_2:
                        test             rax, rax
                                                                                        je    .Lx360_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx360_7]
                        lea              rdx, [rip + .Lx360_8]
                                                                                        jmp   rax
.Lx360_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx360_2
.Lx360_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx360_2
.Lx360_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n339_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx360_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n339_match_alternate_s1
.Lx360_6:
                        add              rsp, 16
                                                                                        jmp   n339_match_alternate_af
n343_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n344_match_defer_α:
                        mov              rax, qword ptr [1879052704]                    # PAT$2$V1
                        mov              rdx, qword ptr [1879052712]
                        cmp              eax, 8
                                                                                        jne   .Lx361_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx361_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx361_10
.Lx361_9:
                        xor              eax, eax
.Lx361_10:
                        test             rax, rax
                                                                                        jz    .Lx361_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx361_4]
                        lea              rdx, [rip + .Lx361_5]
                                                                                        jmp   rax
.Lx361_4:
                                                                                        jmp   n339_match_alternate_s0
.Lx361_5:
                                                                                        jmp   n339_match_alternate_af
.Lx361_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx361_2:
                        test             rax, rax
                                                                                        je    .Lx361_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx361_7]
                        lea              rdx, [rip + .Lx361_8]
                                                                                        jmp   rax
.Lx361_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx361_2
.Lx361_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx361_2
.Lx361_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n339_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx361_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n339_match_alternate_s0
.Lx361_6:
                        add              rsp, 16
                                                                                        jmp   n339_match_alternate_af
n344_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$2_scanhit:
                        cmp              qword ptr [rbp + 272], 1
                                                                                        jne   .Lx362_0
                        mov              ecx, dword ptr [rbp + 264]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx362_0:
                                                                                        jmp   proc_PAT$2_γ
proc_PAT$2_scanfail:
                        cmp              qword ptr [rbp + 272], 1
                                                                                        jne   .Lx363_0
                        mov              eax, dword ptr [rbp + 264]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx363_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx363_0
                        mov              dword ptr [rbp + 264], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$2_attempt
.Lx363_0:
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_β:
                                                                                        jmp   qword ptr [rbp + 240]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$2_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 280]
                        mov              rbp, qword ptr [rbp + 296]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_ω:
                        mov              rax, qword ptr [rbp + 288]
                        lea              rsp, [rbp + 304]
                        mov              rbp, qword ptr [rbp + 296]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$3_α
proc_PAT$3_α:
                        sub              rsp, 144
                        mov              qword ptr [rsp + 120], rcx
                        mov              qword ptr [rsp + 128], rdx
                        mov              qword ptr [rsp + 136], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 112], r8
                        mov              dword ptr [rsp + 104], r14d
proc_PAT$3_attempt:
proc_PAT$3_α_body:
                        lea              rax, [rip + n366_match_assign_cond_β]
                        mov              qword ptr [rbp + 80], rax
#-----------------------------------------------------------------------------------------------------------------------
n364_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n365_match_any_α
n364_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n365_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx370_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
.Lx370_240:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx370_0
                        cmp              esi, 45
                                                                                        je    .Lx370_0
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
.Lx370_0:
                        add              r14d, 1
                                                                                        jmp   n366_match_assign_cond_α
n365_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n366_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 48]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   proc_PAT$3_scanhit
n366_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n365_match_any_β
proc_PAT$3_scanhit:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   .Lx373_0
                        mov              ecx, dword ptr [rbp + 104]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx373_0:
                                                                                        jmp   proc_PAT$3_γ
proc_PAT$3_scanfail:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   .Lx374_0
                        mov              eax, dword ptr [rbp + 104]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx374_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx374_0
                        mov              dword ptr [rbp + 104], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$3_attempt
.Lx374_0:
                                                                                        jmp   proc_PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_β:
                                                                                        jmp   qword ptr [rbp + 80]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$3_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              rbp, qword ptr [rbp + 136]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_ω:
                        mov              rax, qword ptr [rbp + 128]
                        lea              rsp, [rbp + 144]
                        mov              rbp, qword ptr [rbp + 136]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$4_α
proc_PAT$4_α:
                        sub              rsp, 144
                        mov              qword ptr [rsp + 120], rcx
                        mov              qword ptr [rsp + 128], rdx
                        mov              qword ptr [rsp + 136], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 112], r8
                        mov              dword ptr [rsp + 104], r14d
proc_PAT$4_attempt:
proc_PAT$4_α_body:
                        lea              rax, [rip + n377_match_assign_cond_β]
                        mov              qword ptr [rbp + 80], rax
#-----------------------------------------------------------------------------------------------------------------------
n375_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n376_match_any_α
n375_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n376_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx381_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
.Lx381_240:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 42
                                                                                        je    .Lx381_0
                        cmp              esi, 47
                                                                                        je    .Lx381_0
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
.Lx381_0:
                        add              r14d, 1
                                                                                        jmp   n377_match_assign_cond_α
n376_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n377_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 48]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   proc_PAT$4_scanhit
n377_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n376_match_any_β
proc_PAT$4_scanhit:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   .Lx384_0
                        mov              ecx, dword ptr [rbp + 104]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx384_0:
                                                                                        jmp   proc_PAT$4_γ
proc_PAT$4_scanfail:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   .Lx385_0
                        mov              eax, dword ptr [rbp + 104]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx385_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx385_0
                        mov              dword ptr [rbp + 104], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$4_attempt
.Lx385_0:
                                                                                        jmp   proc_PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_β:
                                                                                        jmp   qword ptr [rbp + 80]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$4_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              rbp, qword ptr [rbp + 136]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_ω:
                        mov              rax, qword ptr [rbp + 128]
                        lea              rsp, [rbp + 144]
                        mov              rbp, qword ptr [rbp + 136]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$5_α
proc_PAT$5_α:
                        sub              rsp, 192
                        mov              qword ptr [rsp + 168], rcx
                        mov              qword ptr [rsp + 176], rdx
                        mov              qword ptr [rsp + 184], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 160], r8
                        mov              dword ptr [rsp + 152], r14d
proc_PAT$5_attempt:
proc_PAT$5_α_body:
                        lea              rax, [rip + n388_match_assign_cond_β]
                        mov              qword ptr [rbp + 128], rax
#-----------------------------------------------------------------------------------------------------------------------
n386_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n387_match_alternate_α
n386_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$5_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n387_match_alternate_α:
                        mov              dword ptr [rbp + 64], r14d
                        lea              rax, [rip + .Lx394_21]
                        mov              qword ptr [rbp + 80], rax
                                                                                        jmp   n390_match_defer_α
.Lx394_21:
                        lea              rax, [rip + .Lx394_19]
                        mov              qword ptr [rbp + 80], rax
                                                                                        jmp   n389_match_defer_α
n387_match_alternate_s0:
                        lea              rax, [rip + .Lx394_40]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n387_match_alternate_as
n387_match_alternate_s1:
                        lea              rax, [rip + .Lx394_41]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n387_match_alternate_as
.Lx394_40:
                                                                                        jmp   n390_match_defer_β
.Lx394_41:
                                                                                        jmp   n389_match_defer_β
n387_match_alternate_as:
                                                                                        jmp   n388_match_assign_cond_α
n387_match_alternate_β:
                        mov              rax, qword ptr [rbp + 72]
                                                                                        jmp   rax
n387_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 64]
                        mov              rax, qword ptr [rbp + 80]
                                                                                        jmp   rax
.Lx394_19:
                                                                                        jmp   n386_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n388_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 48]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   proc_PAT$5_scanhit
n388_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n387_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n389_match_defer_α:
                        mov              rax, qword ptr [1879052816]                    # PAT$5$V1
                        mov              rdx, qword ptr [1879052824]
                        cmp              eax, 8
                                                                                        jne   .Lx397_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx397_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx397_10
.Lx397_9:
                        xor              eax, eax
.Lx397_10:
                        test             rax, rax
                                                                                        jz    .Lx397_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx397_4]
                        lea              rdx, [rip + .Lx397_5]
                                                                                        jmp   rax
.Lx397_4:
                                                                                        jmp   n387_match_alternate_s1
.Lx397_5:
                                                                                        jmp   n387_match_alternate_af
.Lx397_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx397_2:
                        test             rax, rax
                                                                                        je    .Lx397_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx397_7]
                        lea              rdx, [rip + .Lx397_8]
                                                                                        jmp   rax
.Lx397_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx397_2
.Lx397_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx397_2
.Lx397_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n387_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx397_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n387_match_alternate_s1
.Lx397_6:
                        add              rsp, 16
                                                                                        jmp   n387_match_alternate_af
n389_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n390_match_defer_α:
                        mov              rax, qword ptr [1879052800]                    # PAT$5$V0
                        mov              rdx, qword ptr [1879052808]
                        cmp              eax, 8
                                                                                        jne   .Lx398_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx398_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx398_10
.Lx398_9:
                        xor              eax, eax
.Lx398_10:
                        test             rax, rax
                                                                                        jz    .Lx398_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx398_4]
                        lea              rdx, [rip + .Lx398_5]
                                                                                        jmp   rax
.Lx398_4:
                                                                                        jmp   n387_match_alternate_s0
.Lx398_5:
                                                                                        jmp   n387_match_alternate_af
.Lx398_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx398_2:
                        test             rax, rax
                                                                                        je    .Lx398_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx398_7]
                        lea              rdx, [rip + .Lx398_8]
                                                                                        jmp   rax
.Lx398_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx398_2
.Lx398_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx398_2
.Lx398_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n387_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx398_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n387_match_alternate_s0
.Lx398_6:
                        add              rsp, 16
                                                                                        jmp   n387_match_alternate_af
n390_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$5_scanhit:
                        cmp              qword ptr [rbp + 160], 1
                                                                                        jne   .Lx399_0
                        mov              ecx, dword ptr [rbp + 152]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx399_0:
                                                                                        jmp   proc_PAT$5_γ
proc_PAT$5_scanfail:
                        cmp              qword ptr [rbp + 160], 1
                                                                                        jne   .Lx400_0
                        mov              eax, dword ptr [rbp + 152]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx400_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx400_0
                        mov              dword ptr [rbp + 152], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$5_attempt
.Lx400_0:
                                                                                        jmp   proc_PAT$5_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_β:
                                                                                        jmp   qword ptr [rbp + 128]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$5_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              rbp, qword ptr [rbp + 184]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_ω:
                        mov              rax, qword ptr [rbp + 176]
                        lea              rsp, [rbp + 192]
                        mov              rbp, qword ptr [rbp + 184]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$6_α
proc_PAT$6_α:
                        sub              rsp, 160
                        mov              qword ptr [rsp + 136], rcx
                        mov              qword ptr [rsp + 144], rdx
                        mov              qword ptr [rsp + 152], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 128], r8
                        mov              dword ptr [rsp + 120], r14d
proc_PAT$6_attempt:
proc_PAT$6_α_body:
                        lea              rax, [rip + n401_match_alternate_β]
                        mov              qword ptr [rbp + 96], rax
#-----------------------------------------------------------------------------------------------------------------------
n401_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx408_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n406_match_defer_α
.Lx408_21:
                        lea              rax, [rip + .Lx408_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n403_match_lit_α
n401_match_alternate_s0:
                        lea              rax, [rip + .Lx408_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n401_match_alternate_as
n401_match_alternate_s1:
                        lea              rax, [rip + .Lx408_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n401_match_alternate_as
.Lx408_40:
                                                                                        jmp   n406_match_defer_β
.Lx408_41:
                                                                                        jmp   n402_goto_β
n401_match_alternate_as:
                                                                                        jmp   proc_PAT$6_γ
n401_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n401_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx408_19:
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
n402_goto_α:
                                                                                        jmp   n401_match_alternate_af
n402_goto_β:
                                                                                        jmp   n401_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n403_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n401_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 40
                                                                                        jne   n401_match_alternate_af
                        add              r14d, 1
                                                                                        jmp   n404_match_defer_α
n403_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n401_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n404_match_defer_α:
                        mov              rax, qword ptr [1879052624]                    # expr
                        mov              rdx, qword ptr [1879052632]
                        cmp              eax, 8
                                                                                        jne   .Lx412_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx412_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx412_10
.Lx412_9:
                        xor              eax, eax
.Lx412_10:
                        test             rax, rax
                                                                                        jz    .Lx412_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx412_4]
                        lea              rdx, [rip + .Lx412_5]
                                                                                        jmp   rax
.Lx412_4:
                                                                                        jmp   n405_match_lit_α
.Lx412_5:
                                                                                        jmp   n403_match_lit_β
.Lx412_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S12]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx412_2:
                        test             rax, rax
                                                                                        je    .Lx412_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx412_7]
                        lea              rdx, [rip + .Lx412_8]
                                                                                        jmp   rax
.Lx412_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx412_2
.Lx412_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx412_2
.Lx412_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n403_match_lit_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx412_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n405_match_lit_α
.Lx412_6:
                        add              rsp, 16
                                                                                        jmp   n403_match_lit_β
n404_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n405_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n404_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 41
                                                                                        jne   n404_match_defer_β
                        add              r14d, 1
                                                                                        jmp   n401_match_alternate_s1
n405_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n404_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n406_match_defer_α:
                        mov              rax, qword ptr [1879052832]                    # PAT$6$V0
                        mov              rdx, qword ptr [1879052840]
                        cmp              eax, 8
                                                                                        jne   .Lx415_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx415_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx415_10
.Lx415_9:
                        xor              eax, eax
.Lx415_10:
                        test             rax, rax
                                                                                        jz    .Lx415_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx415_4]
                        lea              rdx, [rip + .Lx415_5]
                                                                                        jmp   rax
.Lx415_4:
                                                                                        jmp   n401_match_alternate_s0
.Lx415_5:
                                                                                        jmp   n401_match_alternate_af
.Lx415_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx415_2:
                        test             rax, rax
                                                                                        je    .Lx415_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx415_7]
                        lea              rdx, [rip + .Lx415_8]
                                                                                        jmp   rax
.Lx415_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx415_2
.Lx415_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx415_2
.Lx415_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n401_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx415_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n401_match_alternate_s0
.Lx415_6:
                        add              rsp, 16
                                                                                        jmp   n401_match_alternate_af
n406_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$6_scanhit:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   .Lx416_0
                        mov              ecx, dword ptr [rbp + 120]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx416_0:
                                                                                        jmp   proc_PAT$6_γ
proc_PAT$6_scanfail:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   .Lx417_0
                        mov              eax, dword ptr [rbp + 120]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx417_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx417_0
                        mov              dword ptr [rbp + 120], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$6_attempt
.Lx417_0:
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_β:
                                                                                        jmp   qword ptr [rbp + 96]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$6_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 136]
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_ω:
                        mov              rax, qword ptr [rbp + 144]
                        lea              rsp, [rbp + 160]
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$7_α
proc_PAT$7_α:
                        sub              rsp, 208
                        mov              qword ptr [rsp + 184], rcx
                        mov              qword ptr [rsp + 192], rdx
                        mov              qword ptr [rsp + 200], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 176], r8
                        mov              dword ptr [rsp + 168], r14d
proc_PAT$7_attempt:
proc_PAT$7_α_body:
                        lea              rax, [rip + n418_match_alternate_β]
                        mov              qword ptr [rbp + 144], rax
#-----------------------------------------------------------------------------------------------------------------------
n418_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx426_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n421_match_defer_α
.Lx426_21:
                        lea              rax, [rip + .Lx426_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n419_match_defer_α
n418_match_alternate_s0:
                        lea              rax, [rip + .Lx426_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n418_match_alternate_as
n418_match_alternate_s1:
                        lea              rax, [rip + .Lx426_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n418_match_alternate_as
.Lx426_40:
                                                                                        jmp   n420_goto_β
.Lx426_41:
                                                                                        jmp   n419_match_defer_β
n418_match_alternate_as:
                                                                                        jmp   proc_PAT$7_γ
n418_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n418_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx426_19:
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
n419_match_defer_α:
                        mov              rax, qword ptr [1879052576]                    # primary
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 8
                                                                                        jne   .Lx427_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx427_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx427_10
.Lx427_9:
                        xor              eax, eax
.Lx427_10:
                        test             rax, rax
                                                                                        jz    .Lx427_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx427_4]
                        lea              rdx, [rip + .Lx427_5]
                                                                                        jmp   rax
.Lx427_4:
                                                                                        jmp   n418_match_alternate_s1
.Lx427_5:
                                                                                        jmp   n418_match_alternate_af
.Lx427_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S14]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx427_2:
                        test             rax, rax
                                                                                        je    .Lx427_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx427_7]
                        lea              rdx, [rip + .Lx427_8]
                                                                                        jmp   rax
.Lx427_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx427_2
.Lx427_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx427_2
.Lx427_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n418_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx427_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n418_match_alternate_s1
.Lx427_6:
                        add              rsp, 16
                                                                                        jmp   n418_match_alternate_af
n419_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n420_goto_α:
                                                                                        jmp   n418_match_alternate_af
n420_goto_β:
                                                                                        jmp   n418_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n421_match_defer_α:
                        mov              rax, qword ptr [1879052848]                    # PAT$7$V0
                        mov              rdx, qword ptr [1879052856]
                        cmp              eax, 8
                                                                                        jne   .Lx429_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx429_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx429_10
.Lx429_9:
                        xor              eax, eax
.Lx429_10:
                        test             rax, rax
                                                                                        jz    .Lx429_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx429_4]
                        lea              rdx, [rip + .Lx429_5]
                                                                                        jmp   rax
.Lx429_4:
                                                                                        jmp   n422_match_assign_save_α
.Lx429_5:
                                                                                        jmp   n418_match_alternate_af
.Lx429_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S15]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx429_2:
                        test             rax, rax
                                                                                        je    .Lx429_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx429_7]
                        lea              rdx, [rip + .Lx429_8]
                                                                                        jmp   rax
.Lx429_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx429_2
.Lx429_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx429_2
.Lx429_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n418_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx429_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n422_match_assign_save_α
.Lx429_6:
                        add              rsp, 16
                                                                                        jmp   n418_match_alternate_af
n421_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n422_match_assign_save_α:
                        mov              dword ptr [rbp + 96], r14d
                                                                                        jmp   n423_match_defer_α
n422_match_assign_save_β:
                                                                                        jmp   n421_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n423_match_defer_α:
                        mov              rax, qword ptr [1879052592]                    # factor
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 8
                                                                                        jne   .Lx432_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx432_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx432_10
.Lx432_9:
                        xor              eax, eax
.Lx432_10:
                        test             rax, rax
                                                                                        jz    .Lx432_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx432_4]
                        lea              rdx, [rip + .Lx432_5]
                                                                                        jmp   rax
.Lx432_4:
                                                                                        jmp   n424_match_assign_cond_α
.Lx432_5:
                                                                                        jmp   n422_match_assign_save_β
.Lx432_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx432_2:
                        test             rax, rax
                                                                                        je    .Lx432_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx432_7]
                        lea              rdx, [rip + .Lx432_8]
                                                                                        jmp   rax
.Lx432_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx432_2
.Lx432_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx432_2
.Lx432_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n422_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx432_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n424_match_assign_cond_α
.Lx432_6:
                        add              rsp, 16
                                                                                        jmp   n422_match_assign_save_β
n423_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n424_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 96]
                        lea              rcx, [rip + .S17]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n418_match_alternate_s0
n424_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n423_match_defer_β
proc_PAT$7_scanhit:
                        cmp              qword ptr [rbp + 176], 1
                                                                                        jne   .Lx435_0
                        mov              ecx, dword ptr [rbp + 168]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx435_0:
                                                                                        jmp   proc_PAT$7_γ
proc_PAT$7_scanfail:
                        cmp              qword ptr [rbp + 176], 1
                                                                                        jne   .Lx436_0
                        mov              eax, dword ptr [rbp + 168]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx436_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx436_0
                        mov              dword ptr [rbp + 168], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$7_attempt
.Lx436_0:
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_β:
                                                                                        jmp   qword ptr [rbp + 144]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$7_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 184]
                        mov              rbp, qword ptr [rbp + 200]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_ω:
                        mov              rax, qword ptr [rbp + 192]
                        lea              rsp, [rbp + 208]
                        mov              rbp, qword ptr [rbp + 200]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$8_α
proc_PAT$8_α:
                        sub              rsp, 224
                        mov              qword ptr [rsp + 200], rcx
                        mov              qword ptr [rsp + 208], rdx
                        mov              qword ptr [rsp + 216], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 192], r8
                        mov              dword ptr [rsp + 184], r14d
proc_PAT$8_attempt:
proc_PAT$8_α_body:
                        lea              rax, [rip + n437_match_alternate_β]
                        mov              qword ptr [rbp + 160], rax
#-----------------------------------------------------------------------------------------------------------------------
n437_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx446_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n440_match_defer_α
.Lx446_21:
                        lea              rax, [rip + .Lx446_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n438_match_defer_α
n437_match_alternate_s0:
                        lea              rax, [rip + .Lx446_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n437_match_alternate_as
n437_match_alternate_s1:
                        lea              rax, [rip + .Lx446_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n437_match_alternate_as
.Lx446_40:
                                                                                        jmp   n439_goto_β
.Lx446_41:
                                                                                        jmp   n438_match_defer_β
n437_match_alternate_as:
                                                                                        jmp   proc_PAT$8_γ
n437_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n437_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx446_19:
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
n438_match_defer_α:
                        mov              rax, qword ptr [1879052592]                    # factor
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 8
                                                                                        jne   .Lx447_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx447_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx447_10
.Lx447_9:
                        xor              eax, eax
.Lx447_10:
                        test             rax, rax
                                                                                        jz    .Lx447_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx447_4]
                        lea              rdx, [rip + .Lx447_5]
                                                                                        jmp   rax
.Lx447_4:
                                                                                        jmp   n437_match_alternate_s1
.Lx447_5:
                                                                                        jmp   n437_match_alternate_af
.Lx447_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx447_2:
                        test             rax, rax
                                                                                        je    .Lx447_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx447_7]
                        lea              rdx, [rip + .Lx447_8]
                                                                                        jmp   rax
.Lx447_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx447_2
.Lx447_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx447_2
.Lx447_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n437_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx447_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n437_match_alternate_s1
.Lx447_6:
                        add              rsp, 16
                                                                                        jmp   n437_match_alternate_af
n438_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n439_goto_α:
                                                                                        jmp   n437_match_alternate_af
n439_goto_β:
                                                                                        jmp   n437_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n440_match_defer_α:
                        mov              rax, qword ptr [1879052592]                    # factor
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 8
                                                                                        jne   .Lx449_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx449_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx449_10
.Lx449_9:
                        xor              eax, eax
.Lx449_10:
                        test             rax, rax
                                                                                        jz    .Lx449_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx449_4]
                        lea              rdx, [rip + .Lx449_5]
                                                                                        jmp   rax
.Lx449_4:
                                                                                        jmp   n441_match_defer_α
.Lx449_5:
                                                                                        jmp   n437_match_alternate_af
.Lx449_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx449_2:
                        test             rax, rax
                                                                                        je    .Lx449_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx449_7]
                        lea              rdx, [rip + .Lx449_8]
                                                                                        jmp   rax
.Lx449_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx449_2
.Lx449_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx449_2
.Lx449_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n437_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx449_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n441_match_defer_α
.Lx449_6:
                        add              rsp, 16
                                                                                        jmp   n437_match_alternate_af
n440_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n441_match_defer_α:
                        mov              rax, qword ptr [1879052864]                    # PAT$8$V0
                        mov              rdx, qword ptr [1879052872]
                        cmp              eax, 8
                                                                                        jne   .Lx450_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx450_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx450_10
.Lx450_9:
                        xor              eax, eax
.Lx450_10:
                        test             rax, rax
                                                                                        jz    .Lx450_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx450_4]
                        lea              rdx, [rip + .Lx450_5]
                                                                                        jmp   rax
.Lx450_4:
                                                                                        jmp   n442_match_assign_save_α
.Lx450_5:
                                                                                        jmp   n440_match_defer_β
.Lx450_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S18]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx450_2:
                        test             rax, rax
                                                                                        je    .Lx450_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx450_7]
                        lea              rdx, [rip + .Lx450_8]
                                                                                        jmp   rax
.Lx450_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx450_2
.Lx450_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx450_2
.Lx450_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n440_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx450_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n442_match_assign_save_α
.Lx450_6:
                        add              rsp, 16
                                                                                        jmp   n440_match_defer_β
n441_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n442_match_assign_save_α:
                        mov              dword ptr [rbp + 112], r14d
                                                                                        jmp   n443_match_defer_α
n442_match_assign_save_β:
                                                                                        jmp   n441_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n443_match_defer_α:
                        mov              rax, qword ptr [1879052608]                    # term
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 8
                                                                                        jne   .Lx453_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx453_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx453_10
.Lx453_9:
                        xor              eax, eax
.Lx453_10:
                        test             rax, rax
                                                                                        jz    .Lx453_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx453_4]
                        lea              rdx, [rip + .Lx453_5]
                                                                                        jmp   rax
.Lx453_4:
                                                                                        jmp   n444_match_assign_cond_α
.Lx453_5:
                                                                                        jmp   n442_match_assign_save_β
.Lx453_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx453_2:
                        test             rax, rax
                                                                                        je    .Lx453_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx453_7]
                        lea              rdx, [rip + .Lx453_8]
                                                                                        jmp   rax
.Lx453_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx453_2
.Lx453_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx453_2
.Lx453_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n442_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx453_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n444_match_assign_cond_α
.Lx453_6:
                        add              rsp, 16
                                                                                        jmp   n442_match_assign_save_β
n443_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n444_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 112]
                        lea              rcx, [rip + .S20]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n437_match_alternate_s0
n444_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n443_match_defer_β
proc_PAT$8_scanhit:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   .Lx456_0
                        mov              ecx, dword ptr [rbp + 184]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx456_0:
                                                                                        jmp   proc_PAT$8_γ
proc_PAT$8_scanfail:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   .Lx457_0
                        mov              eax, dword ptr [rbp + 184]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx457_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx457_0
                        mov              dword ptr [rbp + 184], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$8_attempt
.Lx457_0:
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_β:
                                                                                        jmp   qword ptr [rbp + 160]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$8_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 200]
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_ω:
                        mov              rax, qword ptr [rbp + 208]
                        lea              rsp, [rbp + 224]
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$9_α
proc_PAT$9_α:
                        sub              rsp, 224
                        mov              qword ptr [rsp + 200], rcx
                        mov              qword ptr [rsp + 208], rdx
                        mov              qword ptr [rsp + 216], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 192], r8
                        mov              dword ptr [rsp + 184], r14d
proc_PAT$9_attempt:
proc_PAT$9_α_body:
                        lea              rax, [rip + n458_match_alternate_β]
                        mov              qword ptr [rbp + 160], rax
#-----------------------------------------------------------------------------------------------------------------------
n458_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx467_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n461_match_defer_α
.Lx467_21:
                        lea              rax, [rip + .Lx467_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n459_match_defer_α
n458_match_alternate_s0:
                        lea              rax, [rip + .Lx467_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n458_match_alternate_as
n458_match_alternate_s1:
                        lea              rax, [rip + .Lx467_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n458_match_alternate_as
.Lx467_40:
                                                                                        jmp   n460_goto_β
.Lx467_41:
                                                                                        jmp   n459_match_defer_β
n458_match_alternate_as:
                                                                                        jmp   proc_PAT$9_γ
n458_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n458_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx467_19:
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
n459_match_defer_α:
                        mov              rax, qword ptr [1879052608]                    # term
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 8
                                                                                        jne   .Lx468_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx468_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx468_10
.Lx468_9:
                        xor              eax, eax
.Lx468_10:
                        test             rax, rax
                                                                                        jz    .Lx468_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx468_4]
                        lea              rdx, [rip + .Lx468_5]
                                                                                        jmp   rax
.Lx468_4:
                                                                                        jmp   n458_match_alternate_s1
.Lx468_5:
                                                                                        jmp   n458_match_alternate_af
.Lx468_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx468_2:
                        test             rax, rax
                                                                                        je    .Lx468_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx468_7]
                        lea              rdx, [rip + .Lx468_8]
                                                                                        jmp   rax
.Lx468_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx468_2
.Lx468_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx468_2
.Lx468_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n458_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx468_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n458_match_alternate_s1
.Lx468_6:
                        add              rsp, 16
                                                                                        jmp   n458_match_alternate_af
n459_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n460_goto_α:
                                                                                        jmp   n458_match_alternate_af
n460_goto_β:
                                                                                        jmp   n458_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n461_match_defer_α:
                        mov              rax, qword ptr [1879052608]                    # term
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 8
                                                                                        jne   .Lx470_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx470_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx470_10
.Lx470_9:
                        xor              eax, eax
.Lx470_10:
                        test             rax, rax
                                                                                        jz    .Lx470_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx470_4]
                        lea              rdx, [rip + .Lx470_5]
                                                                                        jmp   rax
.Lx470_4:
                                                                                        jmp   n462_match_defer_α
.Lx470_5:
                                                                                        jmp   n458_match_alternate_af
.Lx470_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx470_2:
                        test             rax, rax
                                                                                        je    .Lx470_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx470_7]
                        lea              rdx, [rip + .Lx470_8]
                                                                                        jmp   rax
.Lx470_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx470_2
.Lx470_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx470_2
.Lx470_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n458_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx470_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n462_match_defer_α
.Lx470_6:
                        add              rsp, 16
                                                                                        jmp   n458_match_alternate_af
n461_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n462_match_defer_α:
                        mov              rax, qword ptr [1879052880]                    # PAT$9$V0
                        mov              rdx, qword ptr [1879052888]
                        cmp              eax, 8
                                                                                        jne   .Lx471_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx471_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx471_10
.Lx471_9:
                        xor              eax, eax
.Lx471_10:
                        test             rax, rax
                                                                                        jz    .Lx471_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx471_4]
                        lea              rdx, [rip + .Lx471_5]
                                                                                        jmp   rax
.Lx471_4:
                                                                                        jmp   n463_match_assign_save_α
.Lx471_5:
                                                                                        jmp   n461_match_defer_β
.Lx471_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S21]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx471_2:
                        test             rax, rax
                                                                                        je    .Lx471_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx471_7]
                        lea              rdx, [rip + .Lx471_8]
                                                                                        jmp   rax
.Lx471_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx471_2
.Lx471_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx471_2
.Lx471_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n461_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx471_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n463_match_assign_save_α
.Lx471_6:
                        add              rsp, 16
                                                                                        jmp   n461_match_defer_β
n462_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n463_match_assign_save_α:
                        mov              dword ptr [rbp + 112], r14d
                                                                                        jmp   n464_match_defer_α
n463_match_assign_save_β:
                                                                                        jmp   n462_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n464_match_defer_α:
                        mov              rax, qword ptr [1879052624]                    # expr
                        mov              rdx, qword ptr [1879052632]
                        cmp              eax, 8
                                                                                        jne   .Lx474_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx474_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx474_10
.Lx474_9:
                        xor              eax, eax
.Lx474_10:
                        test             rax, rax
                                                                                        jz    .Lx474_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx474_4]
                        lea              rdx, [rip + .Lx474_5]
                                                                                        jmp   rax
.Lx474_4:
                                                                                        jmp   n465_match_assign_cond_α
.Lx474_5:
                                                                                        jmp   n463_match_assign_save_β
.Lx474_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S12]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx474_2:
                        test             rax, rax
                                                                                        je    .Lx474_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx474_7]
                        lea              rdx, [rip + .Lx474_8]
                                                                                        jmp   rax
.Lx474_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx474_2
.Lx474_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx474_2
.Lx474_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n463_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx474_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n465_match_assign_cond_α
.Lx474_6:
                        add              rsp, 16
                                                                                        jmp   n463_match_assign_save_β
n464_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n465_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 112]
                        lea              rcx, [rip + .S20]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n458_match_alternate_s0
n465_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n464_match_defer_β
proc_PAT$9_scanhit:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   .Lx477_0
                        mov              ecx, dword ptr [rbp + 184]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx477_0:
                                                                                        jmp   proc_PAT$9_γ
proc_PAT$9_scanfail:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   .Lx478_0
                        mov              eax, dword ptr [rbp + 184]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx478_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx478_0
                        mov              dword ptr [rbp + 184], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$9_attempt
.Lx478_0:
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_β:
                                                                                        jmp   qword ptr [rbp + 160]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$9_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 200]
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_ω:
                        mov              rax, qword ptr [rbp + 208]
                        lea              rsp, [rbp + 224]
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__Push"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__Push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 3024
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__Pop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__Pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 3024
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__Unary"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__Unary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 3024
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__Binary"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__Binary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 3024
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "Push"
.Lstartup_pp4_0:        .string          "x"
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
                        lea              rsi, [rip + proc_Push_α]
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
.Lstartup_pname5:       .string          "Pop"
                        .align           8
.Lstartup_pnames5:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + .Lstartup_pnames5]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_Pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "Unary"
.Lstartup_pp6_0:        .string          "arg"
.Lstartup_pp6_1:        .string          "op"
                        .align           8
.Lstartup_pnames6:
                        .quad            .Lstartup_pp6_0
                        .quad            .Lstartup_pp6_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + .Lstartup_pnames6]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + proc_Unary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 2
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
.Lstartup_pname7:       .string          "Binary"
.Lstartup_pp7_0:        .string          "op"
.Lstartup_pp7_1:        .string          "left"
.Lstartup_pp7_2:        .string          "right"
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
                        lea              rsi, [rip + proc_Binary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 0
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
                        mov              esi, 32
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "PAT$1"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + proc_PAT$1_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 112
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "PAT$2"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + proc_PAT$2_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 256
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "PAT$3"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + proc_PAT$3_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 96
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname12:      .string          "PAT$4"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname12]
                        lea              rsi, [rip + proc_PAT$4_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 96
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname13:      .string          "PAT$5"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname13]
                        lea              rsi, [rip + proc_PAT$5_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 144
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname14:      .string          "PAT$6"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname14]
                        lea              rsi, [rip + proc_PAT$6_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 112
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname15:      .string          "PAT$7"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname15]
                        lea              rsi, [rip + proc_PAT$7_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 160
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname16:      .string          "PAT$8"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname16]
                        lea              rsi, [rip + proc_PAT$8_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname17:      .string          "PAT$9"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname17]
                        lea              rsi, [rip + proc_PAT$9_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "Push"
.Lgvan1:                .string          "x"
.Lgvan2:                .string          "Pop"
.Lgvan3:                .string          "Unary"
.Lgvan4:                .string          "arg"
.Lgvan5:                .string          "op"
.Lgvan6:                .string          "Binary"
.Lgvan7:                .string          "left"
.Lgvan8:                .string          "right"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "integer"
.Lgvan12:               .string          "exponent"
.Lgvan13:               .string          "epsilon"
.Lgvan14:               .string          "real"
.Lgvan15:               .string          "addop"
.Lgvan16:               .string          "mulop"
.Lgvan17:               .string          "constant"
.Lgvan18:               .string          "primary"
.Lgvan19:               .string          "factor"
.Lgvan20:               .string          "term"
.Lgvan21:               .string          "expr"
.Lgvan22:               .string          "line"
.Lgvan23:               .string          "PAT$1$V0"
.Lgvan24:               .string          "PAT$1$V1"
.Lgvan25:               .string          "PAT$2$V0"
.Lgvan26:               .string          "PAT$2$V1"
.Lgvan27:               .string          "PAT$2$V2"
.Lgvan28:               .string          "PAT$2$V3"
.Lgvan29:               .string          "PAT$2$V4"
.Lgvan30:               .string          "PAT$2$V5"
.Lgvan31:               .string          "PAT$2$V6"
.Lgvan32:               .string          "PAT$5$V0"
.Lgvan33:               .string          "PAT$5$V1"
.Lgvan34:               .string          "PAT$6$V0"
.Lgvan35:               .string          "PAT$7$V0"
.Lgvan36:               .string          "PAT$8$V0"
.Lgvan37:               .string          "PAT$9$V0"
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
                        .quad            .Lgvan15
                        .quad            .Lgvan16
                        .quad            .Lgvan17
                        .quad            .Lgvan18
                        .quad            .Lgvan19
                        .quad            .Lgvan20
                        .quad            .Lgvan21
                        .quad            .Lgvan22
                        .quad            .Lgvan23
                        .quad            .Lgvan24
                        .quad            .Lgvan25
                        .quad            .Lgvan26
                        .quad            .Lgvan27
                        .quad            .Lgvan28
                        .quad            .Lgvan29
                        .quad            .Lgvan30
                        .quad            .Lgvan31
                        .quad            .Lgvan32
                        .quad            .Lgvan33
                        .quad            .Lgvan34
                        .quad            .Lgvan35
                        .quad            .Lgvan36
                        .quad            .Lgvan37
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 38
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 38
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#-----------------------------------------------------------------------------------------------------------------------
n479_statement_begin_α:
                                                                                        jmp   n480_statement_end_α
n479_statement_begin_β:
                                                                                        jmp   n481_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n480_statement_end_α:
                                                                                        jmp   n481_statement_begin_α
#=======================================================================================================================
#          DEFINE('Push(x)')
#-----------------------------------------------------------------------------------------------------------------------
n481_statement_begin_α:
                                                                                        jmp   n482_func_activate_α
n481_statement_begin_β:
                                                                                        jmp   n484_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n482_func_activate_α:
                        lea              rax, [rip + Push_act_α]
                        mov              r11, qword ptr [rip + fn_cell$Push@GOTPCREL]
                        mov              qword ptr [r11 + 0], rax
                                                                                        jmp   n483_statement_end_α
n482_func_activate_β:
                                                                                        jmp   n481_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n483_statement_end_α:
                                                                                        jmp   n484_statement_begin_α
#=======================================================================================================================
#          stk      =  TABLE()                       :(PushEnd)
#-----------------------------------------------------------------------------------------------------------------------
n484_statement_begin_α:
                                                                                        jmp   n485_call_α
n484_statement_begin_β:
                                                                                        jmp   n488_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n485_call_α:
                        sub              rsp, 16
                        .section         .rodata
.Lrkfnzd758:            .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd758]                       # fn
                        xor              esi, esi                                       # args
                        mov              edx, 0                                         # nargs
                        call             rt_call_arr@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx757_240
                        add              rsp, 16
                                                                                        jmp   n484_statement_begin_β
.Lx757_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n486_assign_α
n485_call_β:
                        add              rsp, 16
                                                                                        jmp   n484_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n486_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052432], rax                    # stk
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n487_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n487_statement_end_α:
                                                                                        jmp   n488_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n488_statement_begin_α:
                                                                                        jmp   n489_statement_end_α
n488_statement_begin_β:
                        add              rsp, 16
                                                                                        jmp   n490_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n489_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n490_statement_begin_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#-----------------------------------------------------------------------------------------------------------------------
n490_statement_begin_α:
                                                                                        jmp   n491_func_activate_α
n490_statement_begin_β:
                                                                                        jmp   n493_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n491_func_activate_α:
                        lea              rax, [rip + Pop_act_α]
                        mov              r11, qword ptr [rip + fn_cell$Pop@GOTPCREL]
                        mov              qword ptr [r11 + 0], rax
                                                                                        jmp   n492_statement_end_α
n491_func_activate_β:
                                                                                        jmp   n490_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n492_statement_end_α:
                                                                                        jmp   n493_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n493_statement_begin_α:
                                                                                        jmp   n494_statement_end_α
n493_statement_begin_β:
                                                                                        jmp   n495_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n494_statement_end_α:
                                                                                        jmp   n495_statement_begin_α
#=======================================================================================================================
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n495_statement_begin_α:
                                                                                        jmp   n496_func_activate_α
n495_statement_begin_β:
                                                                                        jmp   n498_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n496_func_activate_α:
                        lea              rax, [rip + Unary_act_α]
                        mov              r11, qword ptr [rip + fn_cell$Unary@GOTPCREL]
                        mov              qword ptr [r11 + 0], rax
                                                                                        jmp   n497_statement_end_α
n496_func_activate_β:
                                                                                        jmp   n495_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n497_statement_end_α:
                                                                                        jmp   n498_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n498_statement_begin_α:
                                                                                        jmp   n499_statement_end_α
n498_statement_begin_β:
                                                                                        jmp   n500_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n499_statement_end_α:
                                                                                        jmp   n500_statement_begin_α
#=======================================================================================================================
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n500_statement_begin_α:
                                                                                        jmp   n501_func_activate_α
n500_statement_begin_β:
                                                                                        jmp   n503_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n501_func_activate_α:
                        lea              rax, [rip + Binary_act_α]
                        mov              r11, qword ptr [rip + fn_cell$Binary@GOTPCREL]
                        mov              qword ptr [r11 + 0], rax
                                                                                        jmp   n502_statement_end_α
n501_func_activate_β:
                                                                                        jmp   n500_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n502_statement_end_α:
                                                                                        jmp   n503_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n503_statement_begin_α:
                                                                                        jmp   n504_statement_end_α
n503_statement_begin_β:
                                                                                        jmp   n505_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n504_statement_end_α:
                                                                                        jmp   n505_statement_begin_α
#=======================================================================================================================
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n505_statement_begin_α:
                                                                                        jmp   n506_lit_string_α
n505_statement_begin_β:
                                                                                        jmp   n510_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n506_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx798_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n507_call_α
.Lx798_0:
                        .quad            .Lx798_0_s
.Lx798_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n507_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd800:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd800]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx799_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n505_statement_begin_β
.Lx799_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n508_assign_α
n507_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n505_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n508_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052464], rax                    # integer
                        mov              qword ptr [1879052472], rdx
                                                                                        jmp   n509_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n509_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n510_statement_begin_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n510_statement_begin_α:
                                                                                        jmp   n511_var_α
n510_statement_begin_β:
                                                                                        jmp   n519_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n511_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n512_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n512_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052672], rax                    # PAT$1$V1
                        mov              qword ptr [1879052680], rdx
                                                                                        jmp   n513_var_α
#-----------------------------------------------------------------------------------------------------------------------
n513_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052496]                    # epsilon
                        mov              rdx, qword ptr [1879052504]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n514_assign_α
n513_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n510_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n514_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052656], rax                    # PAT$1$V0
                        mov              qword ptr [1879052664], rdx
                                                                                        jmp   n515_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n515_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx810_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n516_call_α
n515_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n513_var_β
.Lx810_0:
                        .quad            .Lx810_0_s
.Lx810_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n516_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd812:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd812]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx811_240
                        add              rsp, 16
                                                                                        jmp   n515_lit_string_β
.Lx811_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n517_assign_α
n516_call_β:
                        add              rsp, 16
                                                                                        jmp   n515_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n517_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052480], rax                    # exponent
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n518_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n518_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n519_statement_begin_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n519_statement_begin_α:
                                                                                        jmp   n520_var_α
n519_statement_begin_β:
                                                                                        jmp   n538_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n520_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052480]                    # exponent
                        mov              rdx, qword ptr [1879052488]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n521_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n521_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052784], rax                    # PAT$2$V6
                        mov              qword ptr [1879052792], rdx
                                                                                        jmp   n522_var_α
#-----------------------------------------------------------------------------------------------------------------------
n522_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n523_assign_α
n522_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n519_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n523_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052768], rax                    # PAT$2$V5
                        mov              qword ptr [1879052776], rdx
                                                                                        jmp   n524_var_α
#-----------------------------------------------------------------------------------------------------------------------
n524_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052496]                    # epsilon
                        mov              rdx, qword ptr [1879052504]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n525_assign_α
n524_var_β:
                        add              rsp, 16
                                                                                        jmp   n522_var_β
#-----------------------------------------------------------------------------------------------------------------------
n525_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052752], rax                    # PAT$2$V4
                        mov              qword ptr [1879052760], rdx
                                                                                        jmp   n526_var_α
#-----------------------------------------------------------------------------------------------------------------------
n526_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052480]                    # exponent
                        mov              rdx, qword ptr [1879052488]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n527_assign_α
n526_var_β:
                        add              rsp, 16
                                                                                        jmp   n524_var_β
#-----------------------------------------------------------------------------------------------------------------------
n527_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052736], rax                    # PAT$2$V3
                        mov              qword ptr [1879052744], rdx
                                                                                        jmp   n528_var_α
#-----------------------------------------------------------------------------------------------------------------------
n528_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052496]                    # epsilon
                        mov              rdx, qword ptr [1879052504]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n529_assign_α
n528_var_β:
                        add              rsp, 16
                                                                                        jmp   n526_var_β
#-----------------------------------------------------------------------------------------------------------------------
n529_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052720], rax                    # PAT$2$V2
                        mov              qword ptr [1879052728], rdx
                                                                                        jmp   n530_var_α
#-----------------------------------------------------------------------------------------------------------------------
n530_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n531_assign_α
n530_var_β:
                        add              rsp, 16
                                                                                        jmp   n528_var_β
#-----------------------------------------------------------------------------------------------------------------------
n531_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052704], rax                    # PAT$2$V1
                        mov              qword ptr [1879052712], rdx
                                                                                        jmp   n532_var_α
#-----------------------------------------------------------------------------------------------------------------------
n532_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n533_assign_α
n532_var_β:
                        add              rsp, 16
                                                                                        jmp   n530_var_β
#-----------------------------------------------------------------------------------------------------------------------
n533_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052688], rax                    # PAT$2$V0
                        mov              qword ptr [1879052696], rdx
                                                                                        jmp   n534_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n534_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx832_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n535_call_α
n534_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n532_var_β
.Lx832_0:
                        .quad            .Lx832_0_s
.Lx832_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n535_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd834:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd834]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx833_240
                        add              rsp, 16
                                                                                        jmp   n534_lit_string_β
.Lx833_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n536_assign_α
n535_call_β:
                        add              rsp, 16
                                                                                        jmp   n534_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n536_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052512], rax                    # real
                        mov              qword ptr [1879052520], rdx
                                                                                        jmp   n537_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n537_statement_end_α:
                        add              rsp, 144
                                                                                        jmp   n538_statement_begin_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n538_statement_begin_α:
                                                                                        jmp   n539_statement_end_α
n538_statement_begin_β:
                                                                                        jmp   n540_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n539_statement_end_α:
                                                                                        jmp   n540_statement_begin_α
#=======================================================================================================================
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n540_statement_begin_α:
                                                                                        jmp   n541_lit_string_α
n540_statement_begin_β:
                                                                                        jmp   n545_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n541_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx844_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n542_call_α
.Lx844_0:
                        .quad            .Lx844_0_s
.Lx844_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n542_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd846:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd846]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx845_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n540_statement_begin_β
.Lx845_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n543_assign_α
n542_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n540_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n543_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052528], rax                    # addop
                        mov              qword ptr [1879052536], rdx
                                                                                        jmp   n544_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n544_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n545_statement_begin_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n545_statement_begin_α:
                                                                                        jmp   n546_lit_string_α
n545_statement_begin_β:
                                                                                        jmp   n550_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n546_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx852_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n547_call_α
.Lx852_0:
                        .quad            .Lx852_0_s
.Lx852_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n547_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd854:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd854]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx853_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n545_statement_begin_β
.Lx853_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n548_assign_α
n547_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n545_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n548_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052544], rax                    # mulop
                        mov              qword ptr [1879052552], rdx
                                                                                        jmp   n549_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n549_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n550_statement_begin_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n550_statement_begin_α:
                                                                                        jmp   n551_var_α
n550_statement_begin_β:
                                                                                        jmp   n559_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n551_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n552_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n552_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052816], rax                    # PAT$5$V1
                        mov              qword ptr [1879052824], rdx
                                                                                        jmp   n553_var_α
#-----------------------------------------------------------------------------------------------------------------------
n553_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052512]                    # real
                        mov              rdx, qword ptr [1879052520]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n554_assign_α
n553_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n550_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n554_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052800], rax                    # PAT$5$V0
                        mov              qword ptr [1879052808], rdx
                                                                                        jmp   n555_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n555_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx864_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n556_call_α
n555_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n553_var_β
.Lx864_0:
                        .quad            .Lx864_0_s
.Lx864_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n556_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd866:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd866]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx865_240
                        add              rsp, 16
                                                                                        jmp   n555_lit_string_β
.Lx865_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n557_assign_α
n556_call_β:
                        add              rsp, 16
                                                                                        jmp   n555_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n557_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052560], rax                    # constant
                        mov              qword ptr [1879052568], rdx
                                                                                        jmp   n558_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n558_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n559_statement_begin_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n559_statement_begin_α:
                                                                                        jmp   n560_statement_end_α
n559_statement_begin_β:
                                                                                        jmp   n561_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n560_statement_end_α:
                                                                                        jmp   n561_statement_begin_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n561_statement_begin_α:
                                                                                        jmp   n562_var_α
n561_statement_begin_β:
                                                                                        jmp   n568_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n562_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052560]                    # constant
                        mov              rdx, qword ptr [1879052568]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n563_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n563_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052832], rax                    # PAT$6$V0
                        mov              qword ptr [1879052840], rdx
                                                                                        jmp   n564_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n564_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx878_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n565_call_α
n564_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n561_statement_begin_β
.Lx878_0:
                        .quad            .Lx878_0_s
.Lx878_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n565_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd880:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd880]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx879_240
                        add              rsp, 16
                                                                                        jmp   n564_lit_string_β
.Lx879_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n566_assign_α
n565_call_β:
                        add              rsp, 16
                                                                                        jmp   n564_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n566_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052576], rax                    # primary
                        mov              qword ptr [1879052584], rdx
                                                                                        jmp   n567_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n567_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n568_statement_begin_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n568_statement_begin_α:
                                                                                        jmp   n569_statement_end_α
n568_statement_begin_β:
                                                                                        jmp   n570_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n569_statement_end_α:
                                                                                        jmp   n570_statement_begin_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n570_statement_begin_α:
                                                                                        jmp   n571_var_α
n570_statement_begin_β:
                                                                                        jmp   n577_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n571_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052528]                    # addop
                        mov              rdx, qword ptr [1879052536]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n572_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n572_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052848], rax                    # PAT$7$V0
                        mov              qword ptr [1879052856], rdx
                                                                                        jmp   n573_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n573_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx892_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n574_call_α
n573_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n570_statement_begin_β
.Lx892_0:
                        .quad            .Lx892_0_s
.Lx892_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n574_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd894:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd894]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx893_240
                        add              rsp, 16
                                                                                        jmp   n573_lit_string_β
.Lx893_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n575_assign_α
n574_call_β:
                        add              rsp, 16
                                                                                        jmp   n573_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n575_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052592], rax                    # factor
                        mov              qword ptr [1879052600], rdx
                                                                                        jmp   n576_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n576_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n577_statement_begin_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n577_statement_begin_α:
                                                                                        jmp   n578_statement_end_α
n577_statement_begin_β:
                                                                                        jmp   n579_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n578_statement_end_α:
                                                                                        jmp   n579_statement_begin_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n579_statement_begin_α:
                                                                                        jmp   n580_var_α
n579_statement_begin_β:
                                                                                        jmp   n586_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n580_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052544]                    # mulop
                        mov              rdx, qword ptr [1879052552]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n581_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n581_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052864], rax                    # PAT$8$V0
                        mov              qword ptr [1879052872], rdx
                                                                                        jmp   n582_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n582_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx906_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n583_call_α
n582_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n579_statement_begin_β
.Lx906_0:
                        .quad            .Lx906_0_s
.Lx906_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n583_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd908:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd908]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx907_240
                        add              rsp, 16
                                                                                        jmp   n582_lit_string_β
.Lx907_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n584_assign_α
n583_call_β:
                        add              rsp, 16
                                                                                        jmp   n582_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n584_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052608], rax                    # term
                        mov              qword ptr [1879052616], rdx
                                                                                        jmp   n585_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n585_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n586_statement_begin_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n586_statement_begin_α:
                                                                                        jmp   n587_statement_end_α
n586_statement_begin_β:
                                                                                        jmp   n588_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n587_statement_end_α:
                                                                                        jmp   n588_statement_begin_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n588_statement_begin_α:
                                                                                        jmp   n589_var_α
n588_statement_begin_β:
                                                                                        jmp   n595_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n589_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052528]                    # addop
                        mov              rdx, qword ptr [1879052536]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n590_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n590_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052880], rax                    # PAT$9$V0
                        mov              qword ptr [1879052888], rdx
                                                                                        jmp   n591_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n591_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx920_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n592_call_α
n591_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n588_statement_begin_β
.Lx920_0:
                        .quad            .Lx920_0_s
.Lx920_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n592_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd922:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd922]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx921_240
                        add              rsp, 16
                                                                                        jmp   n591_lit_string_β
.Lx921_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n593_assign_α
n592_call_β:
                        add              rsp, 16
                                                                                        jmp   n591_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n593_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052624], rax                    # expr
                        mov              qword ptr [1879052632], rdx
                                                                                        jmp   n594_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n594_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n595_statement_begin_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n595_statement_begin_α:
                                                                                        jmp   n596_statement_end_α
n595_statement_begin_β:
                                                                                        jmp   n597_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n596_statement_end_α:
                                                                                        jmp   n597_statement_begin_α
#=======================================================================================================================
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n597_statement_begin_α:
                                                                                        jmp   n598_lit_string_α
n597_statement_begin_β:
                                                                                        jmp   n602_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n598_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx932_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n599_lit_integer_α
.Lx932_0:
                        .quad            .Lx932_0_s
.Lx932_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n599_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx933_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n600_call_α
n599_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n597_statement_begin_β
.Lx933_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n600_call_α:
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
.Lrkfnzd935:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd935]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx934_240
                        add              rsp, 16
                                                                                        jmp   n599_lit_integer_β
.Lx934_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n601_statement_end_α
n600_call_β:
                        add              rsp, 16
                                                                                        jmp   n599_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n601_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n602_statement_begin_α
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n602_statement_begin_α:
                                                                                        jmp   n603_var_α
n602_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n603_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx940_0]                # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx940_240
                        add              rsp, 16
                                                                                        jmp   n602_statement_begin_β
.Lx940_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n604_assign_α
.Lx940_0:
                        .quad            .Lx940_0_s
.Lx940_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n604_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052640], rax                    # line
                        mov              qword ptr [1879052648], rdx
                                                                                        jmp   n605_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n605_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n606_statement_begin_α
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n606_statement_begin_α:
                                                                                        jmp   n607_var_α
n606_statement_begin_β:
                                                                                        jmp   n622_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n607_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052640]                    # line
                        mov              rdx, qword ptr [1879052648]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n608_var_α
#-----------------------------------------------------------------------------------------------------------------------
n608_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052624]                    # expr
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n609_assign_α
n608_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n622_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n609_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx948_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n610_match_begin_α
n609_assign_β:
                                                                                        jmp   n608_var_β
.Lx948_0:
                        .quad            .Lx948_0_s
.Lx948_0_s:
                        .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n610_match_begin_α:
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        sub              rsp, 64
                        mov              qword ptr [rbp + -40], r13                     # outer_Σ
                        mov              qword ptr [rbp + -32], r14                     # outer_δ
                        mov              qword ptr [rbp + -24], r15                     # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + -16], rax                     # cap_gen
                        mov              qword ptr [rbp + -48], rbp                     # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rbp + -56], rsp                     # zls2_mark
                        mov              dword ptr [rbp + -64], 0                       # start_δ
.Lx950_0:
                        mov              r14d, dword ptr [rbp + -64]
                                                                                        jmp   n611_lit_integer_α
n610_match_begin_β:
                        add              dword ptr [rbp + -64], 1
                        mov              eax, dword ptr [rbp + -64]
                        cmp              eax, r15d
                                                                                        jg    .Lx950_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx950_1
                                                                                        jmp   .Lx950_0
.Lx950_1:
n610_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rbp + -56]
                        mov              r13, qword ptr [rbp + -40]                     # outer_Σ
                        mov              r14, qword ptr [rbp + -32]                     # outer_δ
                        mov              r15, qword ptr [rbp + -24]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + -16]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + -48]                     # old_rbp
                        add              rsp, 64
                                                                                        jmp   n609_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n611_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx951_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n612_match_pos_α
.Lx951_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n612_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx952_240
                        add              rsp, 16
                                                                                        jmp   n610_match_begin_β
.Lx952_240:
                                                                                        jmp   n613_match_defer_α
n612_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n610_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n613_match_defer_α:
                        lea              rdi, [rip + .S22]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx953_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx953_4]
                        lea              rdx, [rip + .Lx953_5]
                                                                                        jmp   rax
.Lx953_4:
                                                                                        jmp   n614_lit_integer_α
.Lx953_5:
                                                                                        jmp   n610_match_begin_β
.Lx953_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S22]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx953_2:
                        test             rax, rax
                                                                                        je    .Lx953_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx953_7]
                        lea              rdx, [rip + .Lx953_8]
                                                                                        jmp   rax
.Lx953_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx953_2
.Lx953_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx953_2
.Lx953_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n610_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx953_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n614_lit_integer_α
.Lx953_6:
                        add              rsp, 16
                                                                                        jmp   n610_match_begin_β
n613_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n614_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx954_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n615_match_rpos_α
.Lx954_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n615_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        je    .Lx955_240
                        add              rsp, 16
                                                                                        jmp   n613_match_defer_β
.Lx955_240:
                                                                                        jmp   n616_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n616_match_end_α:
                        mov              r10, r12
.Lx957_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx957_9
                        mov              rsp, qword ptr [rbp + -56]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx957_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx957_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx957_1:
                        test             rax, rax
                                                                                        je    .Lx957_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx957_3]
                        lea              rdx, [rip + .Lx957_4]
                                                                                        jmp   rax
.Lx957_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx957_1
.Lx957_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx957_1
.Lx957_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx957_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx957_10
                        mov              r13, qword ptr [rbp + -40]                     # outer_Σ
                        mov              r14, qword ptr [rbp + -32]                     # outer_δ
                        mov              r15, qword ptr [rbp + -24]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + -48]                     # old_rbp
                                                                                        jmp   n617_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n617_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n618_statement_begin_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n618_statement_begin_α:
                                                                                        jmp   n619_call_α
n618_statement_begin_β:
                                                                                        jmp   n602_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n619_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx963_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx963_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx963_6]
                        lea              rdx, [rip + .Lx963_7]
                                                                                        jmp   rax
.Lx963_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx963_2
.Lx963_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx963_2
.Lx963_5:
                        add              rsp, 16
.Lx963_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx963_240
                        add              rsp, 16
                                                                                        jmp   n618_statement_begin_β
.Lx963_240:
                                                                                        jmp   n620_assign_α
n619_call_β:
                                                                                        jmp   n618_statement_begin_β
.Lx963_0:
                        .quad            .Lx963_0_s
.Lx963_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n620_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx964_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n621_statement_end_α
.Lx964_0:
                        .quad            .Lx964_0_s
.Lx964_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n621_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n602_statement_begin_α
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n622_statement_begin_α:
                                                                                        jmp   n623_lit_string_α
n622_statement_begin_β:
                                                                                        jmp   n602_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n623_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx969_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n624_assign_α
.Lx969_0:
                        .quad            .Lx969_0_s
.Lx969_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n624_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx970_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n625_statement_end_α
.Lx970_0:
                        .quad            .Lx970_0_s
.Lx970_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n625_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n602_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n626_goto_α:
                                                                                        jmp   n627_statement_begin_α
n626_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n627_statement_begin_α:
                                                                                        jmp   n628_var_α
n627_statement_begin_β:
                                                                                        jmp   n639_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n628_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n629_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n629_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx977_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n630_subscript_α
n629_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n627_statement_begin_β
.Lx977_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n630_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx978_240
                        add              rsp, 16
                                                                                        jmp   n629_lit_integer_β
.Lx978_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n631_var_α
n630_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n629_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n631_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n632_lit_integer_α
n631_var_β:
                        add              rsp, 16
                                                                                        jmp   n630_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n632_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx980_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n633_subscript_α
n632_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n631_var_β
.Lx980_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n633_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx981_240
                        add              rsp, 16
                                                                                        jmp   n632_lit_integer_β
.Lx981_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n634_deref_α
n633_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n632_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n634_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx982_240
                        add              rsp, 16
                                                                                        jmp   n633_subscript_β
.Lx982_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n635_lit_integer_α
n634_deref_β:
                        add              rsp, 16
                                                                                        jmp   n633_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n635_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx983_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n636_binop_α
n635_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n634_deref_β
.Lx983_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n636_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx984_240
                        add              rsp, 16
                                                                                        jmp   n635_lit_integer_β
.Lx984_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n637_assign_var_α
n636_binop_β:
                        add              rsp, 16
                                                                                        jmp   n635_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n637_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # subscript
                        mov              rsi, qword ptr [rsp + 120]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx985_240
                        add              rsp, 16
                                                                                        jmp   n636_binop_β
.Lx985_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n638_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n638_statement_end_α:
                        add              rsp, 160
                                                                                        jmp   n639_statement_begin_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n639_statement_begin_α:
                                                                                        jmp   n640_var_α
n639_statement_begin_β:
                                                                                        jmp   n648_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n640_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n641_var_α
#-----------------------------------------------------------------------------------------------------------------------
n641_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n642_lit_integer_α
n641_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n639_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n642_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx992_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n643_subscript_α
n642_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n641_var_β
.Lx992_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n643_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx993_240
                        add              rsp, 16
                                                                                        jmp   n642_lit_integer_β
.Lx993_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n644_deref_α
n643_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n642_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n644_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx994_240
                        add              rsp, 16
                                                                                        jmp   n643_subscript_β
.Lx994_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n645_subscript_α
n644_deref_β:
                        add              rsp, 16
                                                                                        jmp   n643_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n645_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx995_240
                        add              rsp, 16
                                                                                        jmp   n644_deref_β
.Lx995_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n646_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n646_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # subscript
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # Push
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n647_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n647_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n648_statement_begin_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n648_statement_begin_α:
                                                                                        jmp   n649_var_α
n648_statement_begin_β:
                                                                                        jmp   n654_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n649_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n650_call_α
#-----------------------------------------------------------------------------------------------------------------------
n650_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1003:           .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1003]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1002_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n648_statement_begin_β
.Lx1002_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n651_var_α
n650_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n648_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n651_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n652_assign_var_α
n651_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n648_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n652_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1005_240
                        add              rsp, 16
                                                                                        jmp   n651_var_β
.Lx1005_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n653_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n653_statement_end_α:
                                                                                        jmp   n654_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n654_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx1008_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n655_call_α
.Lx1008_0:
                        .quad            .Lx1008_0_s
.Lx1008_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n655_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1010:           .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1010]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1009_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n657_save_restore_α
.Lx1009_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n656_save_restore_α
n655_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n657_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n656_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx1012_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx1012_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n657_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx1014_0
                        mov              cl, 2
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx1014_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n658_goto_α:
                                                                                        jmp   n488_statement_begin_α
n658_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n659_goto_α:
                                                                                        jmp   n660_statement_begin_α
n659_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n660_statement_begin_α:
                                                                                        jmp   n661_var_α
n660_statement_begin_β:
                                                                                        jmp   n670_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n661_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n662_var_α
#-----------------------------------------------------------------------------------------------------------------------
n662_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n663_lit_integer_α
n662_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n660_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n663_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1021_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n664_subscript_α
n663_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n662_var_β
.Lx1021_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n664_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1022_240
                        add              rsp, 16
                                                                                        jmp   n663_lit_integer_β
.Lx1022_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n665_deref_α
n664_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n663_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n665_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1023_240
                        add              rsp, 16
                                                                                        jmp   n664_subscript_β
.Lx1023_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n666_subscript_α
n665_deref_β:
                        add              rsp, 16
                                                                                        jmp   n664_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n666_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1024_240
                        add              rsp, 16
                                                                                        jmp   n665_deref_β
.Lx1024_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n667_deref_α
n666_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n665_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n667_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1025_240
                        add              rsp, 16
                                                                                        jmp   n666_subscript_β
.Lx1025_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n668_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n668_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # deref
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # Pop
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n669_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n669_statement_end_α:
                        add              rsp, 112
                                                                                        jmp   n670_statement_begin_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n670_statement_begin_α:
                                                                                        jmp   n671_var_α
n670_statement_begin_β:
                                                                                        jmp   n656_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n671_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n672_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n672_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1032_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n673_subscript_α
n672_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n670_statement_begin_β
.Lx1032_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n673_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1033_240
                        add              rsp, 16
                                                                                        jmp   n672_lit_integer_β
.Lx1033_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n674_var_α
n673_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n672_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n674_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n675_lit_integer_α
n674_var_β:
                        add              rsp, 16
                                                                                        jmp   n673_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n675_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1035_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n676_subscript_α
n675_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n674_var_β
.Lx1035_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n676_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1036_240
                        add              rsp, 16
                                                                                        jmp   n675_lit_integer_β
.Lx1036_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n677_deref_α
n676_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n675_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n677_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1037_240
                        add              rsp, 16
                                                                                        jmp   n676_subscript_β
.Lx1037_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n678_lit_integer_α
n677_deref_β:
                        add              rsp, 16
                                                                                        jmp   n676_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n678_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1038_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n679_binop_α
n678_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n677_deref_β
.Lx1038_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n679_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1039_240
                        add              rsp, 16
                                                                                        jmp   n678_lit_integer_β
.Lx1039_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n680_assign_var_α
n679_binop_β:
                        add              rsp, 16
                                                                                        jmp   n678_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n680_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # subscript
                        mov              rsi, qword ptr [rsp + 120]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1040_240
                        add              rsp, 16
                                                                                        jmp   n679_binop_β
.Lx1040_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n681_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n681_statement_end_α:
                        add              rsp, 160
                                                                                        jmp   n656_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n682_goto_α:
                                                                                        jmp   n493_statement_begin_α
n682_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n683_goto_α:
                                                                                        jmp   n684_statement_begin_α
n683_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n684_statement_begin_α:
                                                                                        jmp   n685_call_α
n684_statement_begin_β:
                                                                                        jmp   n688_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n685_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1048_0]               # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1048_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1048_6]
                        lea              rdx, [rip + .Lx1048_7]
                                                                                        jmp   rax
.Lx1048_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1048_2
.Lx1048_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1048_2
.Lx1048_5:
                        add              rsp, 16
.Lx1048_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1048_240
                        add              rsp, 16
                                                                                        jmp   n684_statement_begin_β
.Lx1048_240:
                                                                                        jmp   n686_assign_α
n685_call_β:
                                                                                        jmp   n684_statement_begin_β
.Lx1048_0:
                        .quad            .Lx1048_0_s
.Lx1048_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n686_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # arg
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n687_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n687_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n688_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n688_statement_begin_α:
                                                                                        jmp   n689_call_α
n688_statement_begin_β:
                                                                                        jmp   n692_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n689_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1055_0]               # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1055_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1055_6]
                        lea              rdx, [rip + .Lx1055_7]
                                                                                        jmp   rax
.Lx1055_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1055_2
.Lx1055_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1055_2
.Lx1055_5:
                        add              rsp, 16
.Lx1055_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1055_240
                        add              rsp, 16
                                                                                        jmp   n688_statement_begin_β
.Lx1055_240:
                                                                                        jmp   n690_assign_α
n689_call_β:
                                                                                        jmp   n688_statement_begin_β
.Lx1055_0:
                        .quad            .Lx1055_0_s
.Lx1055_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n690_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # op
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n691_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n691_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n692_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n692_statement_begin_α:
                                                                                        jmp   n693_lit_string_α
n692_statement_begin_β:
                                                                                        jmp   n702_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n693_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx1061_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n694_call_α
.Lx1061_0:
                        .quad            .Lx1061_0_s
.Lx1061_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n694_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1063:           .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1063]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1062_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n692_statement_begin_β
.Lx1062_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n695_call_α
n694_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n692_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n695_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx1065_0]               # name
                        mov              esi, 1                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1065_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1065_6]
                        lea              rdx, [rip + .Lx1065_7]
                                                                                        jmp   rax
.Lx1065_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1065_2
.Lx1065_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # Push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1065_2
.Lx1065_5:
                        add              rsp, 32
.Lx1065_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1065_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n692_statement_begin_β
.Lx1065_240:
                                                                                        jmp   n696_var_α
n695_call_β:
                                                                                        jmp   n692_statement_begin_β
.Lx1065_0:
                        .quad            .Lx1065_0_s
.Lx1065_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n696_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n697_var_α
n696_var_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n692_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n697_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # arg
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n698_binop_α
n697_var_β:
                        add              rsp, 16
                                                                                        jmp   n696_var_β
#-----------------------------------------------------------------------------------------------------------------------
n698_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n699_call_α
n698_binop_β:
                        add              rsp, 16
                                                                                        jmp   n697_var_β
#-----------------------------------------------------------------------------------------------------------------------
n699_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1070:           .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1070]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1069_240
                        add              rsp, 16
                                                                                        jmp   n698_binop_β
.Lx1069_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n700_assign_var_α
n699_call_β:
                        add              rsp, 16
                                                                                        jmp   n698_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n700_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # call
                        mov              rsi, qword ptr [rsp + 88]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1071_240
                        add              rsp, 32
                                                                                        jmp   n698_binop_β
.Lx1071_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n701_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n701_statement_end_α:
                        add              rsp, 128
                                                                                        jmp   n702_statement_begin_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n702_statement_begin_α:
                                                                                        jmp   n703_lit_string_α
n702_statement_begin_β:
                                                                                        jmp   n654_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n703_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx1076_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n704_call_α
.Lx1076_0:
                        .quad            .Lx1076_0_s
.Lx1076_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n704_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1078:           .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1078]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1077_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n702_statement_begin_β
.Lx1077_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n705_assign_α
n704_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n702_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n705_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # Unary
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n706_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n706_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n654_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n707_goto_α:
                                                                                        jmp   n498_statement_begin_α
n707_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n708_goto_α:
                                                                                        jmp   n709_statement_begin_α
n708_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n709_statement_begin_α:
                                                                                        jmp   n710_call_α
n709_statement_begin_β:
                                                                                        jmp   n713_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n710_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1087_0]               # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1087_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1087_6]
                        lea              rdx, [rip + .Lx1087_7]
                                                                                        jmp   rax
.Lx1087_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1087_2
.Lx1087_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1087_2
.Lx1087_5:
                        add              rsp, 16
.Lx1087_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1087_240
                        add              rsp, 16
                                                                                        jmp   n709_statement_begin_β
.Lx1087_240:
                                                                                        jmp   n711_assign_α
n710_call_β:
                                                                                        jmp   n709_statement_begin_β
.Lx1087_0:
                        .quad            .Lx1087_0_s
.Lx1087_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n711_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052416], rax                    # right
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n712_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n712_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n713_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n713_statement_begin_α:
                                                                                        jmp   n714_call_α
n713_statement_begin_β:
                                                                                        jmp   n717_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n714_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1094_0]               # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1094_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1094_6]
                        lea              rdx, [rip + .Lx1094_7]
                                                                                        jmp   rax
.Lx1094_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1094_2
.Lx1094_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1094_2
.Lx1094_5:
                        add              rsp, 16
.Lx1094_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1094_240
                        add              rsp, 16
                                                                                        jmp   n713_statement_begin_β
.Lx1094_240:
                                                                                        jmp   n715_assign_α
n714_call_β:
                                                                                        jmp   n713_statement_begin_β
.Lx1094_0:
                        .quad            .Lx1094_0_s
.Lx1094_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n715_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # op
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n716_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n716_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n717_statement_begin_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n717_statement_begin_α:
                                                                                        jmp   n718_call_α
n717_statement_begin_β:
                                                                                        jmp   n721_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n718_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx1101_0]               # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1101_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1101_6]
                        lea              rdx, [rip + .Lx1101_7]
                                                                                        jmp   rax
.Lx1101_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1101_2
.Lx1101_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1101_2
.Lx1101_5:
                        add              rsp, 16
.Lx1101_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1101_240
                        add              rsp, 16
                                                                                        jmp   n717_statement_begin_β
.Lx1101_240:
                                                                                        jmp   n719_assign_α
n718_call_β:
                                                                                        jmp   n717_statement_begin_β
.Lx1101_0:
                        .quad            .Lx1101_0_s
.Lx1101_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n719_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052400], rax                    # left
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n720_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n720_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n721_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n721_statement_begin_α:
                                                                                        jmp   n722_lit_string_α
n721_statement_begin_β:
                                                                                        jmp   n737_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n722_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx1107_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n723_call_α
.Lx1107_0:
                        .quad            .Lx1107_0_s
.Lx1107_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n723_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1109:           .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1109]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1108_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n721_statement_begin_β
.Lx1108_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n724_call_α
n723_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n721_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n724_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx1111_0]               # name
                        mov              esi, 1                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx1111_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx1111_6]
                        lea              rdx, [rip + .Lx1111_7]
                                                                                        jmp   rax
.Lx1111_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx1111_2
.Lx1111_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # Push
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx1111_2
.Lx1111_5:
                        add              rsp, 32
.Lx1111_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx1111_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n721_statement_begin_β
.Lx1111_240:
                                                                                        jmp   n725_var_α
n724_call_β:
                                                                                        jmp   n721_statement_begin_β
.Lx1111_0:
                        .quad            .Lx1111_0_s
.Lx1111_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n725_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]                    # left
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n726_lit_string_α
n725_var_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n721_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n726_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1113_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n727_binop_α
n726_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n725_var_β
.Lx1113_0:
                        .quad            .Lx1113_0_s
.Lx1113_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n727_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n728_var_α
n727_binop_β:
                        add              rsp, 16
                                                                                        jmp   n726_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n728_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n729_binop_α
n728_var_β:
                        add              rsp, 16
                                                                                        jmp   n727_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n729_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n730_lit_string_α
n729_binop_β:
                        add              rsp, 16
                                                                                        jmp   n728_var_β
#-----------------------------------------------------------------------------------------------------------------------
n730_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1117_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n731_binop_α
n730_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n729_binop_β
.Lx1117_0:
                        .quad            .Lx1117_0_s
.Lx1117_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n731_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n732_var_α
n731_binop_β:
                        add              rsp, 16
                                                                                        jmp   n730_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n732_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052416]                    # right
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n733_binop_α
n732_var_β:
                        add              rsp, 16
                                                                                        jmp   n731_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n733_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n734_call_α
n733_binop_β:
                        add              rsp, 16
                                                                                        jmp   n732_var_β
#-----------------------------------------------------------------------------------------------------------------------
n734_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1122:           .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1122]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1121_240
                        add              rsp, 16
                                                                                        jmp   n733_binop_β
.Lx1121_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n735_assign_var_α
n734_call_β:
                        add              rsp, 16
                                                                                        jmp   n733_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n735_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 176]                     # call
                        mov              rsi, qword ptr [rsp + 184]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1123_240
                        add              rsp, 32
                                                                                        jmp   n733_binop_β
.Lx1123_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n736_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n736_statement_end_α:
                        add              rsp, 224
                                                                                        jmp   n737_statement_begin_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n737_statement_begin_α:
                                                                                        jmp   n738_lit_string_α
n737_statement_begin_β:
                                                                                        jmp   n654_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n738_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx1128_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n739_call_α
.Lx1128_0:
                        .quad            .Lx1128_0_s
.Lx1128_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n739_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1130:           .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1130]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1129_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n737_statement_begin_β
.Lx1129_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n740_assign_α
n739_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n737_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n740_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052384], rax                    # Binary
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n741_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n741_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n654_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n742_goto_α:
                                                                                        jmp   n503_statement_begin_α
n742_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n743_goto_α:
                                                                                        jmp   n602_statement_begin_α
n743_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n744_goto_α:
                                                                                        jmp   n622_statement_begin_α
n744_goto_β:
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
                        .section         .data
                        .align           8
fn_cell$Push:
                        .quad            rt_ab_undef_fn_stub
                        .section         .text
                        .intel_syntax    noprefix
Push_act_α:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 136
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        lea              rax, [rbp + 8]
                        mov              qword ptr [rbp + -32], rax
                        mov              rax, qword ptr [1879048216]
                        mov              qword ptr [rbp + -40], rax
                        mov              qword ptr [1879048216], rbp
                        lea              rax, [rip + .Lx1137_1]
                        mov              qword ptr [rbp + -64], rax
                        mov              rdi, rbp
                        call             rt_ab_enter_env@PLT
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              qword ptr [rbp + -112], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rbp + -104], rax
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              qword ptr [rbp + -128], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rbp + -120], rax
                        xor              eax, eax                                       # Push
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rax
                        xor              eax, eax                                       # x
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rax
                        movabs           rax, 139766086753676
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1137_2
                        mov              rdi, qword ptr [rip + .Lx1137_0]
                        call             mon_emit_call_bin@PLT
.Lx1137_2:
                                                                                        jmp   proc_Push_α
                                                                                        jmp   Push_act_γ
.Lx1137_1:
                        movzx            r9, cl
                        cmp              r9d, 2
                                                                                        je    .Lx1137_3
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                        mov              rdi, rbp
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        xor              ecx, ecx
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                                                                                        jmp   .Lx1137_4
.Lx1137_3:
                        mov              rdi, rbp
                        xor              esi, esi
                        xor              edx, edx
                        mov              ecx, 1
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
.Lx1137_4:
                        mov              rcx, qword ptr [rbp + -112]                    # Push
                        mov              qword ptr [1879052288], rcx
                        mov              rcx, qword ptr [rbp + -104]
                        mov              qword ptr [1879052296], rcx
                        mov              rcx, qword ptr [rbp + -128]                    # x
                        mov              qword ptr [1879052304], rcx
                        mov              rcx, qword ptr [rbp + -120]
                        mov              qword ptr [1879052312], rcx
                        mov              r10, qword ptr [rbp + -16]
                        mov              r11, qword ptr [rbp + -24]
                        mov              rcx, qword ptr [rbp + -40]
                        movabs           rax, 139766086753676
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1137_5
                        mov              rdi, qword ptr [rip + .Lx1137_0]
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        call             mon_emit_return_bin@PLT
.Lx1137_5:
                        mov              rax, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        mov              qword ptr [1879048216], rcx
                        cmp              r9d, 2
                                                                                        je    .Lx1137_6
                                                                                        jmp   r10
.Lx1137_6:
                                                                                        jmp   r11
Push_act_γ:
Push_act_ω:
.Lx1137_0:
                        .quad            .Lx1137_0_s
.Lx1137_0_s:
                        .string          "Push"
                        .section         .data
                        .align           8
fn_cell$Pop:
                        .quad            rt_ab_undef_fn_stub
                        .section         .text
                        .intel_syntax    noprefix
Pop_act_α:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 120
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        lea              rax, [rbp + 8]
                        mov              qword ptr [rbp + -32], rax
                        mov              rax, qword ptr [1879048216]
                        mov              qword ptr [rbp + -40], rax
                        mov              qword ptr [1879048216], rbp
                        lea              rax, [rip + .Lx1138_1]
                        mov              qword ptr [rbp + -64], rax
                        mov              rdi, rbp
                        call             rt_ab_enter_env@PLT
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rbp + -112], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rbp + -104], rax
                        xor              eax, eax
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rax
                        movabs           rax, 139766086753676
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1138_2
                        mov              rdi, qword ptr [rip + .Lx1138_0]
                        call             mon_emit_call_bin@PLT
.Lx1138_2:
                                                                                        jmp   proc_Pop_α
                                                                                        jmp   Pop_act_γ
.Lx1138_1:
                        movzx            r9, cl
                        cmp              r9d, 2
                                                                                        je    .Lx1138_3
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                        mov              rdi, rbp
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        xor              ecx, ecx
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                                                                                        jmp   .Lx1138_4
.Lx1138_3:
                        mov              rdi, rbp
                        xor              esi, esi
                        xor              edx, edx
                        mov              ecx, 1
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
.Lx1138_4:
                        mov              rcx, qword ptr [rbp + -112]
                        mov              qword ptr [1879052320], rcx
                        mov              rcx, qword ptr [rbp + -104]
                        mov              qword ptr [1879052328], rcx
                        mov              r10, qword ptr [rbp + -16]
                        mov              r11, qword ptr [rbp + -24]
                        mov              rcx, qword ptr [rbp + -40]
                        movabs           rax, 139766086753676
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1138_5
                        mov              rdi, qword ptr [rip + .Lx1138_0]
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        call             mon_emit_return_bin@PLT
.Lx1138_5:
                        mov              rax, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        mov              qword ptr [1879048216], rcx
                        cmp              r9d, 2
                                                                                        je    .Lx1138_6
                                                                                        jmp   r10
.Lx1138_6:
                                                                                        jmp   r11
Pop_act_γ:
Pop_act_ω:
.Lx1138_0:
                        .quad            .Lx1138_0_s
.Lx1138_0_s:
                        .string          "Pop"
                        .section         .data
                        .align           8
fn_cell$Unary:
                        .quad            rt_ab_undef_fn_stub
                        .section         .text
                        .intel_syntax    noprefix
Unary_act_α:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 152
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        lea              rax, [rbp + 8]
                        mov              qword ptr [rbp + -32], rax
                        mov              rax, qword ptr [1879048216]
                        mov              qword ptr [rbp + -40], rax
                        mov              qword ptr [1879048216], rbp
                        lea              rax, [rip + .Lx1139_1]
                        mov              qword ptr [rbp + -64], rax
                        mov              rdi, rbp
                        call             rt_ab_enter_env@PLT
                        mov              rax, qword ptr [1879052336]                    # Unary
                        mov              qword ptr [rbp + -112], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rbp + -104], rax
                        mov              rax, qword ptr [1879052352]                    # arg
                        mov              qword ptr [rbp + -128], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rbp + -120], rax
                        mov              qword ptr [rbp + -144], 0
                        mov              qword ptr [rbp + -136], 0
                        xor              eax, eax                                       # Unary
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rax
                        xor              eax, eax                                       # arg
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rax
                        movabs           rax, 139766086753676
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1139_2
                        mov              rdi, qword ptr [rip + .Lx1139_0]
                        call             mon_emit_call_bin@PLT
.Lx1139_2:
                                                                                        jmp   proc_Unary_α
                                                                                        jmp   Unary_act_γ
.Lx1139_1:
                        movzx            r9, cl
                        cmp              r9d, 2
                                                                                        je    .Lx1139_3
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                        mov              rdi, rbp
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        xor              ecx, ecx
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                                                                                        jmp   .Lx1139_4
.Lx1139_3:
                        mov              rdi, rbp
                        xor              esi, esi
                        xor              edx, edx
                        mov              ecx, 1
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
.Lx1139_4:
                        mov              rcx, qword ptr [rbp + -112]                    # Unary
                        mov              qword ptr [1879052336], rcx
                        mov              rcx, qword ptr [rbp + -104]
                        mov              qword ptr [1879052344], rcx
                        mov              rcx, qword ptr [rbp + -128]                    # arg
                        mov              qword ptr [1879052352], rcx
                        mov              rcx, qword ptr [rbp + -120]
                        mov              qword ptr [1879052360], rcx
                        mov              r10, qword ptr [rbp + -16]
                        mov              r11, qword ptr [rbp + -24]
                        mov              rcx, qword ptr [rbp + -40]
                        movabs           rax, 139766086753676
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1139_5
                        mov              rdi, qword ptr [rip + .Lx1139_0]
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        call             mon_emit_return_bin@PLT
.Lx1139_5:
                        mov              rax, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        mov              qword ptr [1879048216], rcx
                        cmp              r9d, 2
                                                                                        je    .Lx1139_6
                                                                                        jmp   r10
.Lx1139_6:
                                                                                        jmp   r11
Unary_act_γ:
Unary_act_ω:
.Lx1139_0:
                        .quad            .Lx1139_0_s
.Lx1139_0_s:
                        .string          "Unary"
                        .section         .data
                        .align           8
fn_cell$Binary:
                        .quad            rt_ab_undef_fn_stub
                        .section         .text
                        .intel_syntax    noprefix
Binary_act_α:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 168
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        lea              rax, [rbp + 8]
                        mov              qword ptr [rbp + -32], rax
                        mov              rax, qword ptr [1879048216]
                        mov              qword ptr [rbp + -40], rax
                        mov              qword ptr [1879048216], rbp
                        lea              rax, [rip + .Lx1140_1]
                        mov              qword ptr [rbp + -64], rax
                        mov              rdi, rbp
                        call             rt_ab_enter_env@PLT
                        mov              rax, qword ptr [1879052384]                    # Binary
                        mov              qword ptr [rbp + -112], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rbp + -104], rax
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              qword ptr [rbp + -128], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rbp + -120], rax
                        mov              qword ptr [rbp + -144], 0
                        mov              qword ptr [rbp + -136], 0
                        mov              qword ptr [rbp + -160], 0
                        mov              qword ptr [rbp + -152], 0
                        xor              eax, eax                                       # Binary
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rax
                        xor              eax, eax                                       # op
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rax
                        movabs           rax, 139766086753676
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1140_2
                        mov              rdi, qword ptr [rip + .Lx1140_0]
                        call             mon_emit_call_bin@PLT
.Lx1140_2:
                                                                                        jmp   proc_Binary_α
                                                                                        jmp   Binary_act_γ
.Lx1140_1:
                        movzx            r9, cl
                        cmp              r9d, 2
                                                                                        je    .Lx1140_3
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                        mov              rdi, rbp
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        xor              ecx, ecx
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                                                                                        jmp   .Lx1140_4
.Lx1140_3:
                        mov              rdi, rbp
                        xor              esi, esi
                        xor              edx, edx
                        mov              ecx, 1
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
.Lx1140_4:
                        mov              rcx, qword ptr [rbp + -112]                    # Binary
                        mov              qword ptr [1879052384], rcx
                        mov              rcx, qword ptr [rbp + -104]
                        mov              qword ptr [1879052392], rcx
                        mov              rcx, qword ptr [rbp + -128]                    # op
                        mov              qword ptr [1879052368], rcx
                        mov              rcx, qword ptr [rbp + -120]
                        mov              qword ptr [1879052376], rcx
                        mov              r10, qword ptr [rbp + -16]
                        mov              r11, qword ptr [rbp + -24]
                        mov              rcx, qword ptr [rbp + -40]
                        movabs           rax, 139766086753676
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1140_5
                        mov              rdi, qword ptr [rip + .Lx1140_0]
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        call             mon_emit_return_bin@PLT
.Lx1140_5:
                        mov              rax, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        mov              qword ptr [1879048216], rcx
                        cmp              r9d, 2
                                                                                        je    .Lx1140_6
                                                                                        jmp   r10
.Lx1140_6:
                                                                                        jmp   r11
Binary_act_γ:
Binary_act_ω:
.Lx1140_0:
                        .quad            .Lx1140_0_s
.Lx1140_0_s:
                        .string          "Binary"
                        .section         .rodata
.S0:                    .string          "PAT$1$V1"
.S1:                    .string          "PAT$1$V0"
.S2:                    .string          "PAT$2$V5"
.S3:                    .string          "PAT$2$V6"
.S4:                    .string          "PAT$2$V0"
.S5:                    .string          "PAT$2$V4"
.S6:                    .string          "PAT$2$V3"
.S7:                    .string          "PAT$2$V2"
.S8:                    .string          "PAT$2$V1"
.S9:                    .string          "*Push"
.S10:                   .string          "PAT$5$V1"
.S11:                   .string          "PAT$5$V0"
.S12:                   .string          "expr"
.S13:                   .string          "PAT$6$V0"
.S14:                   .string          "primary"
.S15:                   .string          "PAT$7$V0"
.S16:                   .string          "factor"
.S17:                   .string          "*Unary"
.S18:                   .string          "PAT$8$V0"
.S19:                   .string          "term"
.S20:                   .string          "*Binary"
.S21:                   .string          "PAT$9$V0"
.S22:                   .string          "PATV$0"
                        .text
                        .section         .rodata
.C0:
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.C1:
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
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

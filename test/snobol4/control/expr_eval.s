                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_α:
proc_LBL__Push_α_body:
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n0_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n1_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_subscript_α
.Lx29_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n2_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx30_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n11_var_α
.Lx30_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n3_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n4_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_subscript_α
.Lx32_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n5_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx33_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n11_var_α
.Lx33_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n6_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx34_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n11_var_α
.Lx34_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n7_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n8_binop_α
.Lx35_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n8_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx36_240
                        add              rsp, 16
                        add              rsp, 128
                                                                                        jmp   n11_var_α
.Lx36_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n9_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # subscript
                        mov              rsi, qword ptr [rsp + 120]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx37_240
                        add              rsp, 16
                        add              rsp, 144
                                                                                        jmp   n11_var_α
.Lx37_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n10_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_α:
                        add              rsp, 160
                                                                                        jmp   n11_var_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n12_var_α
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_subscript_α
.Lx42_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n14_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx43_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n19_var_α
.Lx43_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n15_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n15_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx44_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n19_var_α
.Lx44_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n16_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n16_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx45_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n19_var_α
.Lx45_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n17_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # subscript
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # Push
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n18_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_α:
                        add              rsp, 96
                                                                                        jmp   n19_var_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n19_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n20_call_α
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd51:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd51]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx50_240
                        add              rsp, 16
                                                                                        jmp   n24_lit_string_α
.Lx50_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_var_α
n20_call_β:
                        add              rsp, 16
                                                                                        jmp   n24_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n22_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx53_240
                        add              rsp, 16
                                                                                        jmp   n24_lit_string_α
.Lx53_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n23_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_α:
                                                                                        jmp   n24_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx56_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n25_call_α
.Lx56_0:
                        .quad            .Lx56_0_s
.Lx56_0_s:
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
.Lrkfnzd58:             .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd58]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx57_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n27_save_restore_α
.Lx57_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n26_save_restore_α
n25_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n27_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n26_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:
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
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_α:
proc_LBL__Pop_α_body:
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n63_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n64_var_α
#-----------------------------------------------------------------------------------------------------------------------
n64_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n65_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n66_subscript_α
.Lx86_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n66_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx87_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n72_var_α
.Lx87_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n67_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx88_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n72_var_α
.Lx88_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n68_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n68_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx89_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n72_var_α
.Lx89_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n69_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n69_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx90_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n72_var_α
.Lx90_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n70_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # deref
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # Pop
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n71_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_α:
                        add              rsp, 112
                                                                                        jmp   n72_var_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n72_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n73_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n74_subscript_α
.Lx95_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n74_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx96_240
                        add              rsp, 16
                                                                                        jmp   n83_save_restore_α
.Lx96_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n75_var_α
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n76_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n77_subscript_α
.Lx98_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n77_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx99_240
                        add              rsp, 16
                                                                                        jmp   n83_save_restore_α
.Lx99_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n78_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n78_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx100_240
                        add              rsp, 16
                                                                                        jmp   n83_save_restore_α
.Lx100_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n79_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n80_binop_α
.Lx101_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n80_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx102_240
                        add              rsp, 16
                                                                                        jmp   n83_save_restore_α
.Lx102_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n81_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n81_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # subscript
                        mov              rsi, qword ptr [rsp + 120]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx103_240
                        add              rsp, 16
                                                                                        jmp   n83_save_restore_α
.Lx103_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n82_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_α:
                                                                                        jmp   n83_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n83_save_restore_α:
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
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_α:
proc_LBL__Unary_α_body:
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n108_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx132_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx132_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx132_6]
                        lea              rdx, [rip + .Lx132_7]
                                                                                        jmp   rax
.Lx132_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx132_2
.Lx132_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx132_2
.Lx132_5:
                        add              rsp, 16
.Lx132_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx132_240
                        add              rsp, 16
                                                                                        jmp   n111_call_α
.Lx132_240:
                                                                                        jmp   n109_assign_α
n108_call_β:
                                                                                        jmp   n111_call_α
.Lx132_0:
                        .quad            .Lx132_0_s
.Lx132_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n109_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # arg
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n110_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_α:
                        add              rsp, 16
                                                                                        jmp   n111_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n111_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx137_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx137_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx137_6]
                        lea              rdx, [rip + .Lx137_7]
                                                                                        jmp   rax
.Lx137_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx137_2
.Lx137_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx137_2
.Lx137_5:
                        add              rsp, 16
.Lx137_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx137_240
                        add              rsp, 16
                                                                                        jmp   n114_lit_string_α
.Lx137_240:
                                                                                        jmp   n112_assign_α
n111_call_β:
                                                                                        jmp   n114_lit_string_α
.Lx137_0:
                        .quad            .Lx137_0_s
.Lx137_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n112_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # op
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n113_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_α:
                        add              rsp, 16
                                                                                        jmp   n114_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n114_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n115_call_α
.Lx141_0:
                        .quad            .Lx141_0_s
.Lx141_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n115_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd143:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd143]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx142_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n123_lit_string_α
.Lx142_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n116_call_α
n115_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n123_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n116_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx145_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx145_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx145_6]
                        lea              rdx, [rip + .Lx145_7]
                                                                                        jmp   rax
.Lx145_6:
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
                                                                                        jmp   .Lx145_2
.Lx145_7:
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
                                                                                        jmp   .Lx145_2
.Lx145_5:
                        add              rsp, 32
.Lx145_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx145_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n123_lit_string_α
.Lx145_240:
                                                                                        jmp   n117_var_α
n116_call_β:
                                                                                        jmp   n123_lit_string_α
.Lx145_0:
                        .quad            .Lx145_0_s
.Lx145_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n117_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n118_var_α
#-----------------------------------------------------------------------------------------------------------------------
n118_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # arg
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n119_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n119_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n120_call_α
#-----------------------------------------------------------------------------------------------------------------------
n120_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd150:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd150]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx149_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n123_lit_string_α
.Lx149_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n121_assign_var_α
n120_call_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n123_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n121_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # call
                        mov              rsi, qword ptr [rsp + 88]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx151_240
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n123_lit_string_α
.Lx151_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n122_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_α:
                        add              rsp, 128
                                                                                        jmp   n123_lit_string_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n123_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx154_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n124_call_α
.Lx154_0:
                        .quad            .Lx154_0_s
.Lx154_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n124_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd156:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd156]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx155_240
                        add              rsp, 16
                                                                                        jmp   n127_lit_string_α
.Lx155_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n125_assign_α
n124_call_β:
                        add              rsp, 16
                                                                                        jmp   n127_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n125_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # Unary
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n126_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_α:
                                                                                        jmp   n127_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n128_call_α
.Lx160_0:
                        .quad            .Lx160_0_s
.Lx160_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n128_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd162:            .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd162]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx161_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n130_save_restore_α
.Lx161_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n129_save_restore_α
n128_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n130_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n129_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n130_save_restore_α:
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
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_α:
proc_LBL__Binary_α_body:
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n167_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx200_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx200_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx200_6]
                        lea              rdx, [rip + .Lx200_7]
                                                                                        jmp   rax
.Lx200_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx200_2
.Lx200_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx200_2
.Lx200_5:
                        add              rsp, 16
.Lx200_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx200_240
                        add              rsp, 16
                                                                                        jmp   n170_call_α
.Lx200_240:
                                                                                        jmp   n168_assign_α
n167_call_β:
                                                                                        jmp   n170_call_α
.Lx200_0:
                        .quad            .Lx200_0_s
.Lx200_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n168_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052416], rax                    # right
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n169_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n169_statement_α:
                        add              rsp, 16
                                                                                        jmp   n170_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n170_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx205_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx205_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx205_6]
                        lea              rdx, [rip + .Lx205_7]
                                                                                        jmp   rax
.Lx205_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx205_2
.Lx205_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx205_2
.Lx205_5:
                        add              rsp, 16
.Lx205_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx205_240
                        add              rsp, 16
                                                                                        jmp   n173_call_α
.Lx205_240:
                                                                                        jmp   n171_assign_α
n170_call_β:
                                                                                        jmp   n173_call_α
.Lx205_0:
                        .quad            .Lx205_0_s
.Lx205_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n171_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # op
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n172_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_α:
                        add              rsp, 16
                                                                                        jmp   n173_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n173_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx210_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx210_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx210_6]
                        lea              rdx, [rip + .Lx210_7]
                                                                                        jmp   rax
.Lx210_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx210_2
.Lx210_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx210_2
.Lx210_5:
                        add              rsp, 16
.Lx210_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx210_240
                        add              rsp, 16
                                                                                        jmp   n176_lit_string_α
.Lx210_240:
                                                                                        jmp   n174_assign_α
n173_call_β:
                                                                                        jmp   n176_lit_string_α
.Lx210_0:
                        .quad            .Lx210_0_s
.Lx210_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n174_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052400], rax                    # left
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n175_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_α:
                        add              rsp, 16
                                                                                        jmp   n176_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n176_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx214_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n177_call_α
.Lx214_0:
                        .quad            .Lx214_0_s
.Lx214_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n177_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd216:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd216]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx215_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n191_lit_string_α
.Lx215_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n178_call_α
n177_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n191_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n178_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx218_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx218_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx218_6]
                        lea              rdx, [rip + .Lx218_7]
                                                                                        jmp   rax
.Lx218_6:
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
                                                                                        jmp   .Lx218_2
.Lx218_7:
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
                                                                                        jmp   .Lx218_2
.Lx218_5:
                        add              rsp, 32
.Lx218_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx218_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n191_lit_string_α
.Lx218_240:
                                                                                        jmp   n179_var_α
n178_call_β:
                                                                                        jmp   n191_lit_string_α
.Lx218_0:
                        .quad            .Lx218_0_s
.Lx218_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n179_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]                    # left
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n180_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n180_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n181_binop_α
.Lx220_0:
                        .quad            .Lx220_0_s
.Lx220_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n181_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n182_var_α
#-----------------------------------------------------------------------------------------------------------------------
n182_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n183_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n183_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n184_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n184_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx224_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n185_binop_α
.Lx224_0:
                        .quad            .Lx224_0_s
.Lx224_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n185_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n186_var_α
#-----------------------------------------------------------------------------------------------------------------------
n186_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052416]                    # right
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n187_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n187_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n188_call_α
#-----------------------------------------------------------------------------------------------------------------------
n188_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd229:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd229]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx228_240
                        add              rsp, 16
                        add              rsp, 192
                                                                                        jmp   n191_lit_string_α
.Lx228_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n189_assign_var_α
n188_call_β:
                        add              rsp, 16
                        add              rsp, 192
                                                                                        jmp   n191_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n189_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 176]                     # call
                        mov              rsi, qword ptr [rsp + 184]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx230_240
                        add              rsp, 16
                        add              rsp, 208
                                                                                        jmp   n191_lit_string_α
.Lx230_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n190_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n190_statement_α:
                        add              rsp, 224
                                                                                        jmp   n191_lit_string_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n191_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n192_call_α
.Lx233_0:
                        .quad            .Lx233_0_s
.Lx233_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n192_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd235:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd235]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx234_240
                        add              rsp, 16
                                                                                        jmp   n195_lit_string_α
.Lx234_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n193_assign_α
n192_call_β:
                        add              rsp, 16
                                                                                        jmp   n195_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n193_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052384], rax                    # Binary
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n194_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_α:
                                                                                        jmp   n195_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n195_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx239_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n196_call_α
.Lx239_0:
                        .quad            .Lx239_0_s
.Lx239_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n196_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd241:            .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd241]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx240_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n198_save_restore_α
.Lx240_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n197_save_restore_α
n196_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n198_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n197_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n198_save_restore_α:
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
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Push_α
proc_Push_α:
proc_Push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n246_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n247_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n247_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx251_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx251_1
.Lx251_0:
                        .quad            .Lx251_0_s
.Lx251_0_s:
                        .string          "Push"
.Lx251_1:
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
n252_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n253_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n253_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx257_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx257_1
.Lx257_0:
                        .quad            .Lx257_0_s
.Lx257_0_s:
                        .string          "Pop"
.Lx257_1:
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
n258_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n259_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n259_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx263_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx263_1
.Lx263_0:
                        .quad            .Lx263_0_s
.Lx263_0_s:
                        .string          "Unary"
.Lx263_1:
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
n264_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n265_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n265_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx269_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx269_1
.Lx269_0:
                        .quad            .Lx269_0_s
.Lx269_0_s:
                        .string          "Binary"
.Lx269_1:
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
                        lea              rax, [rip + n270_match_span_β]
                        mov              qword ptr [rbp + 16], rax
#-----------------------------------------------------------------------------------------------------------------------
n270_match_span_α:
                        sub              rsp, 16
                        lea              rdi, [rip + .C0]
                        movsxd           rcx, r14d
.Lx272_0:
                        cmp              ecx, r15d
                                                                                        jge   .Lx272_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx272_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx272_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx272_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx272_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx272_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx272_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx272_1
                        add              ecx, 1
                                                                                        jmp   .Lx272_0
.Lx272_1:
                        cmp              ecx, r14d
                                                                                        jg    .Lx272_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
.Lx272_240:
                        mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanhit
n270_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 48], 1
                                                                                        jne   .Lx273_0
                        mov              ecx, dword ptr [rbp + 40]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx273_0:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 48], 1
                                                                                        jne   .Lx274_0
                        mov              eax, dword ptr [rbp + 40]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx274_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx274_0
                        mov              dword ptr [rbp + 40], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$0_attempt
.Lx274_0:
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
                        sub              rsp, 176
                        mov              qword ptr [rsp + 152], rcx
                        mov              qword ptr [rsp + 160], rdx
                        mov              qword ptr [rsp + 168], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 144], r8
                        mov              dword ptr [rsp + 136], r14d
proc_PAT$1_attempt:
proc_PAT$1_α_body:
                        lea              rax, [rip + n275_match_sequence_β]
                        mov              qword ptr [rbp + 112], rax
#-----------------------------------------------------------------------------------------------------------------------
n275_match_sequence_α:
                        mov              dword ptr [rbp + 16], r14d
                                                                                        jmp   n280_match_any_α
n275_match_sequence_as:
                                                                                        jmp   proc_PAT$1_γ
n275_match_sequence_β:
                                                                                        jmp   n276_match_patref_β
n275_match_sequence_af:
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n276_match_patref_α:
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 8
                                                                                        jne   .Lx283_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx283_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx283_10
.Lx283_9:
                        xor              eax, eax
.Lx283_10:
                        test             rax, rax
                                                                                        jz    .Lx283_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx283_4]
                        lea              rdx, [rip + .Lx283_5]
                                                                                        jmp   rax
.Lx283_4:
                                                                                        jmp   proc_PAT$1_γ
.Lx283_5:
                                                                                        jmp   n277_match_alternate_β
.Lx283_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx283_2:
                        test             rax, rax
                                                                                        je    .Lx283_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx283_7]
                        lea              rdx, [rip + .Lx283_8]
                                                                                        jmp   rax
.Lx283_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx283_2
.Lx283_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx283_2
.Lx283_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n277_match_alternate_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx283_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   proc_PAT$1_γ
.Lx283_6:
                        add              rsp, 16
                                                                                        jmp   n277_match_alternate_β
n276_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n277_match_alternate_α:
                        mov              dword ptr [rbp + 48], r14d
                        lea              rax, [rip + .Lx285_21]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   n279_match_any_α
.Lx285_21:
                        lea              rax, [rip + .Lx285_19]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   n278_match_patref_α
n277_match_alternate_s0:
                        lea              rax, [rip + .Lx285_40]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n277_match_alternate_as
n277_match_alternate_s1:
                        lea              rax, [rip + .Lx285_41]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n277_match_alternate_as
.Lx285_40:
                                                                                        jmp   n279_match_any_β
.Lx285_41:
                                                                                        jmp   n278_match_patref_β
n277_match_alternate_as:
                                                                                        jmp   n276_match_patref_α
n277_match_alternate_β:
                        mov              rax, qword ptr [rbp + 56]
                                                                                        jmp   rax
n277_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 48]
                        mov              rax, qword ptr [rbp + 64]
                                                                                        jmp   rax
.Lx285_19:
                                                                                        jmp   n280_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n278_match_patref_α:
                        lea              rdi, [rip + .S1]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx286_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx286_4]
                        lea              rdx, [rip + .Lx286_5]
                                                                                        jmp   rax
.Lx286_4:
                                                                                        jmp   n277_match_alternate_s1
.Lx286_5:
                                                                                        jmp   n277_match_alternate_af
.Lx286_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx286_2:
                        test             rax, rax
                                                                                        je    .Lx286_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx286_7]
                        lea              rdx, [rip + .Lx286_8]
                                                                                        jmp   rax
.Lx286_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx286_2
.Lx286_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx286_2
.Lx286_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n277_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx286_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n277_match_alternate_s1
.Lx286_6:
                        add              rsp, 16
                                                                                        jmp   n277_match_alternate_af
n278_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n279_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n277_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx288_0
                        cmp              esi, 45
                                                                                        je    .Lx288_0
                                                                                        jmp   n277_match_alternate_af
.Lx288_0:
                        add              r14d, 1
                                                                                        jmp   n277_match_alternate_s0
n279_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n277_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n280_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   proc_PAT$1_ω
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C1]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    proc_PAT$1_ω
                        add              r14d, 1
                                                                                        jmp   n277_match_alternate_α
n280_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   proc_PAT$1_ω
proc_PAT$1_scanhit:
                        cmp              qword ptr [rbp + 144], 1
                                                                                        jne   .Lx291_0
                        mov              ecx, dword ptr [rbp + 136]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx291_0:
                                                                                        jmp   proc_PAT$1_γ
proc_PAT$1_scanfail:
                        cmp              qword ptr [rbp + 144], 1
                                                                                        jne   .Lx292_0
                        mov              eax, dword ptr [rbp + 136]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx292_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx292_0
                        mov              dword ptr [rbp + 136], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$1_attempt
.Lx292_0:
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_β:
                                                                                        jmp   qword ptr [rbp + 112]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$1_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 152]
                        mov              rbp, qword ptr [rbp + 168]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_ω:
                        mov              rax, qword ptr [rbp + 160]
                        lea              rsp, [rbp + 176]
                        mov              rbp, qword ptr [rbp + 168]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$2_α
proc_PAT$2_α:
                        sub              rsp, 336
                        mov              qword ptr [rsp + 312], rcx
                        mov              qword ptr [rsp + 320], rdx
                        mov              qword ptr [rsp + 328], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 304], r8
                        mov              dword ptr [rsp + 296], r14d
proc_PAT$2_attempt:
proc_PAT$2_α_body:
                        lea              rax, [rip + n293_match_alternate_β]
                        mov              qword ptr [rbp + 272], rax
#-----------------------------------------------------------------------------------------------------------------------
n293_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx307_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n297_match_sequence_α
.Lx307_21:
                        lea              rax, [rip + .Lx307_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n294_match_sequence_α
n293_match_alternate_s0:
                        lea              rax, [rip + .Lx307_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n293_match_alternate_as
n293_match_alternate_s1:
                        lea              rax, [rip + .Lx307_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n293_match_alternate_as
.Lx307_40:
                                                                                        jmp   n297_match_sequence_β
.Lx307_41:
                                                                                        jmp   n294_match_sequence_β
n293_match_alternate_as:
                                                                                        jmp   proc_PAT$2_γ
n293_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n293_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx307_19:
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n294_match_sequence_α:
                        mov              dword ptr [rbp + 224], r14d
                                                                                        jmp   n296_match_patref_α
n294_match_sequence_as:
                                                                                        jmp   n293_match_alternate_s1
n294_match_sequence_β:
                                                                                        jmp   n295_match_patref_β
n294_match_sequence_af:
                                                                                        jmp   n293_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n295_match_patref_α:
                        mov              rax, qword ptr [1879052480]                    # exponent
                        mov              rdx, qword ptr [1879052488]
                        cmp              eax, 8
                                                                                        jne   .Lx310_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx310_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx310_10
.Lx310_9:
                        xor              eax, eax
.Lx310_10:
                        test             rax, rax
                                                                                        jz    .Lx310_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx310_4]
                        lea              rdx, [rip + .Lx310_5]
                                                                                        jmp   rax
.Lx310_4:
                                                                                        jmp   n293_match_alternate_s1
.Lx310_5:
                                                                                        jmp   n296_match_patref_β
.Lx310_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx310_2:
                        test             rax, rax
                                                                                        je    .Lx310_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx310_7]
                        lea              rdx, [rip + .Lx310_8]
                                                                                        jmp   rax
.Lx310_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx310_2
.Lx310_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx310_2
.Lx310_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n296_match_patref_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx310_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n293_match_alternate_s1
.Lx310_6:
                        add              rsp, 16
                                                                                        jmp   n296_match_patref_β
n295_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n296_match_patref_α:
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 8
                                                                                        jne   .Lx311_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx311_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx311_10
.Lx311_9:
                        xor              eax, eax
.Lx311_10:
                        test             rax, rax
                                                                                        jz    .Lx311_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx311_4]
                        lea              rdx, [rip + .Lx311_5]
                                                                                        jmp   rax
.Lx311_4:
                                                                                        jmp   n295_match_patref_α
.Lx311_5:
                                                                                        jmp   n293_match_alternate_af
.Lx311_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx311_2:
                        test             rax, rax
                                                                                        je    .Lx311_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx311_7]
                        lea              rdx, [rip + .Lx311_8]
                                                                                        jmp   rax
.Lx311_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx311_2
.Lx311_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx311_2
.Lx311_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n293_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx311_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n295_match_patref_α
.Lx311_6:
                        add              rsp, 16
                                                                                        jmp   n293_match_alternate_af
n296_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n297_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n305_match_patref_α
n297_match_sequence_as:
                                                                                        jmp   n293_match_alternate_s0
n297_match_sequence_β:
                                                                                        jmp   n298_match_alternate_β
n297_match_sequence_af:
                                                                                        jmp   n293_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n298_match_alternate_α:
                        mov              dword ptr [rbp + 160], r14d
                        lea              rax, [rip + .Lx315_21]
                        mov              qword ptr [rbp + 176], rax
                                                                                        jmp   n300_match_patref_α
.Lx315_21:
                        lea              rax, [rip + .Lx315_19]
                        mov              qword ptr [rbp + 176], rax
                                                                                        jmp   n299_match_patref_α
n298_match_alternate_s0:
                        lea              rax, [rip + .Lx315_40]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n298_match_alternate_as
n298_match_alternate_s1:
                        lea              rax, [rip + .Lx315_41]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n298_match_alternate_as
.Lx315_40:
                                                                                        jmp   n300_match_patref_β
.Lx315_41:
                                                                                        jmp   n299_match_patref_β
n298_match_alternate_as:
                                                                                        jmp   n293_match_alternate_s0
n298_match_alternate_β:
                        mov              rax, qword ptr [rbp + 168]
                                                                                        jmp   rax
n298_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 160]
                        mov              rax, qword ptr [rbp + 176]
                                                                                        jmp   rax
.Lx315_19:
                                                                                        jmp   n301_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n299_match_patref_α:
                        lea              rdi, [rip + .S1]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx316_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx316_4]
                        lea              rdx, [rip + .Lx316_5]
                                                                                        jmp   rax
.Lx316_4:
                                                                                        jmp   n298_match_alternate_s1
.Lx316_5:
                                                                                        jmp   n298_match_alternate_af
.Lx316_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx316_2:
                        test             rax, rax
                                                                                        je    .Lx316_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx316_7]
                        lea              rdx, [rip + .Lx316_8]
                                                                                        jmp   rax
.Lx316_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx316_2
.Lx316_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx316_2
.Lx316_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n298_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx316_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n298_match_alternate_s1
.Lx316_6:
                        add              rsp, 16
                                                                                        jmp   n298_match_alternate_af
n299_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n300_match_patref_α:
                        mov              rax, qword ptr [1879052480]                    # exponent
                        mov              rdx, qword ptr [1879052488]
                        cmp              eax, 8
                                                                                        jne   .Lx317_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx317_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx317_10
.Lx317_9:
                        xor              eax, eax
.Lx317_10:
                        test             rax, rax
                                                                                        jz    .Lx317_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx317_4]
                        lea              rdx, [rip + .Lx317_5]
                                                                                        jmp   rax
.Lx317_4:
                                                                                        jmp   n298_match_alternate_s0
.Lx317_5:
                                                                                        jmp   n298_match_alternate_af
.Lx317_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx317_2:
                        test             rax, rax
                                                                                        je    .Lx317_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx317_7]
                        lea              rdx, [rip + .Lx317_8]
                                                                                        jmp   rax
.Lx317_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx317_2
.Lx317_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx317_2
.Lx317_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n298_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx317_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n298_match_alternate_s0
.Lx317_6:
                        add              rsp, 16
                                                                                        jmp   n298_match_alternate_af
n300_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n301_match_alternate_α:
                        mov              dword ptr [rbp + 96], r14d
                        lea              rax, [rip + .Lx319_21]
                        mov              qword ptr [rbp + 112], rax
                                                                                        jmp   n303_match_patref_α
.Lx319_21:
                        lea              rax, [rip + .Lx319_19]
                        mov              qword ptr [rbp + 112], rax
                                                                                        jmp   n302_match_patref_α
n301_match_alternate_s0:
                        lea              rax, [rip + .Lx319_40]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n301_match_alternate_as
n301_match_alternate_s1:
                        lea              rax, [rip + .Lx319_41]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n301_match_alternate_as
.Lx319_40:
                                                                                        jmp   n303_match_patref_β
.Lx319_41:
                                                                                        jmp   n302_match_patref_β
n301_match_alternate_as:
                                                                                        jmp   n298_match_alternate_α
n301_match_alternate_β:
                        mov              rax, qword ptr [rbp + 104]
                                                                                        jmp   rax
n301_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 96]
                        mov              rax, qword ptr [rbp + 112]
                                                                                        jmp   rax
.Lx319_19:
                                                                                        jmp   n304_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n302_match_patref_α:
                        lea              rdi, [rip + .S1]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx320_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx320_4]
                        lea              rdx, [rip + .Lx320_5]
                                                                                        jmp   rax
.Lx320_4:
                                                                                        jmp   n301_match_alternate_s1
.Lx320_5:
                                                                                        jmp   n301_match_alternate_af
.Lx320_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx320_2:
                        test             rax, rax
                                                                                        je    .Lx320_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx320_7]
                        lea              rdx, [rip + .Lx320_8]
                                                                                        jmp   rax
.Lx320_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx320_2
.Lx320_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx320_2
.Lx320_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n301_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx320_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n301_match_alternate_s1
.Lx320_6:
                        add              rsp, 16
                                                                                        jmp   n301_match_alternate_af
n302_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n303_match_patref_α:
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 8
                                                                                        jne   .Lx321_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx321_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx321_10
.Lx321_9:
                        xor              eax, eax
.Lx321_10:
                        test             rax, rax
                                                                                        jz    .Lx321_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx321_4]
                        lea              rdx, [rip + .Lx321_5]
                                                                                        jmp   rax
.Lx321_4:
                                                                                        jmp   n301_match_alternate_s0
.Lx321_5:
                                                                                        jmp   n301_match_alternate_af
.Lx321_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx321_2:
                        test             rax, rax
                                                                                        je    .Lx321_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx321_7]
                        lea              rdx, [rip + .Lx321_8]
                                                                                        jmp   rax
.Lx321_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx321_2
.Lx321_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx321_2
.Lx321_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n301_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx321_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n301_match_alternate_s0
.Lx321_6:
                        add              rsp, 16
                                                                                        jmp   n301_match_alternate_af
n303_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n304_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n305_match_patref_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 46
                                                                                        jne   n305_match_patref_β
                        add              r14d, 1
                                                                                        jmp   n301_match_alternate_α
n304_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n305_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n305_match_patref_α:
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 8
                                                                                        jne   .Lx324_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx324_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx324_10
.Lx324_9:
                        xor              eax, eax
.Lx324_10:
                        test             rax, rax
                                                                                        jz    .Lx324_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx324_4]
                        lea              rdx, [rip + .Lx324_5]
                                                                                        jmp   rax
.Lx324_4:
                                                                                        jmp   n304_match_lit_α
.Lx324_5:
                                                                                        jmp   n293_match_alternate_af
.Lx324_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx324_2:
                        test             rax, rax
                                                                                        je    .Lx324_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx324_7]
                        lea              rdx, [rip + .Lx324_8]
                                                                                        jmp   rax
.Lx324_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx324_2
.Lx324_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx324_2
.Lx324_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n293_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx324_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n304_match_lit_α
.Lx324_6:
                        add              rsp, 16
                                                                                        jmp   n293_match_alternate_af
n305_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$2_scanhit:
                        cmp              qword ptr [rbp + 304], 1
                                                                                        jne   .Lx325_0
                        mov              ecx, dword ptr [rbp + 296]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx325_0:
                                                                                        jmp   proc_PAT$2_γ
proc_PAT$2_scanfail:
                        cmp              qword ptr [rbp + 304], 1
                                                                                        jne   .Lx326_0
                        mov              eax, dword ptr [rbp + 296]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx326_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx326_0
                        mov              dword ptr [rbp + 296], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$2_attempt
.Lx326_0:
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_β:
                                                                                        jmp   qword ptr [rbp + 272]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$2_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              rbp, qword ptr [rbp + 328]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_ω:
                        mov              rax, qword ptr [rbp + 320]
                        lea              rsp, [rbp + 336]
                        mov              rbp, qword ptr [rbp + 328]
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
                        lea              rax, [rip + n329_match_assign_cond_β]
                        mov              qword ptr [rbp + 80], rax
#-----------------------------------------------------------------------------------------------------------------------
n327_match_assign_save_α:
                        sub              rsp, 64
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n328_match_any_α
n327_match_assign_save_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   proc_PAT$3_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n328_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx333_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
.Lx333_240:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx333_0
                        cmp              esi, 45
                                                                                        je    .Lx333_0
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
.Lx333_0:
                        add              r14d, 1
                                                                                        jmp   n329_match_assign_cond_α
n328_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n329_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S3]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        add              rsp, 64
                                                                                        jmp   proc_PAT$3_scanhit
n329_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n328_match_any_β
proc_PAT$3_scanhit:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   .Lx336_0
                        mov              ecx, dword ptr [rbp + 104]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx336_0:
                                                                                        jmp   proc_PAT$3_γ
proc_PAT$3_scanfail:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   .Lx337_0
                        mov              eax, dword ptr [rbp + 104]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx337_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx337_0
                        mov              dword ptr [rbp + 104], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$3_attempt
.Lx337_0:
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
                        lea              rax, [rip + n340_match_assign_cond_β]
                        mov              qword ptr [rbp + 80], rax
#-----------------------------------------------------------------------------------------------------------------------
n338_match_assign_save_α:
                        sub              rsp, 64
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n339_match_any_α
n338_match_assign_save_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   proc_PAT$4_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n339_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx344_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
.Lx344_240:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 42
                                                                                        je    .Lx344_0
                        cmp              esi, 47
                                                                                        je    .Lx344_0
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
.Lx344_0:
                        add              r14d, 1
                                                                                        jmp   n340_match_assign_cond_α
n339_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n340_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S3]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        add              rsp, 64
                                                                                        jmp   proc_PAT$4_scanhit
n340_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n339_match_any_β
proc_PAT$4_scanhit:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   .Lx347_0
                        mov              ecx, dword ptr [rbp + 104]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx347_0:
                                                                                        jmp   proc_PAT$4_γ
proc_PAT$4_scanfail:
                        cmp              qword ptr [rbp + 112], 1
                                                                                        jne   .Lx348_0
                        mov              eax, dword ptr [rbp + 104]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx348_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx348_0
                        mov              dword ptr [rbp + 104], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$4_attempt
.Lx348_0:
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
                        lea              rax, [rip + n351_match_assign_cond_β]
                        mov              qword ptr [rbp + 128], rax
#-----------------------------------------------------------------------------------------------------------------------
n349_match_assign_save_α:
                        sub              rsp, 112
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 64], 0
                        mov              qword ptr [rsp + 72], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        lea              rdi, [rbp + 48]                                # slot
                        mov              esi, r14d                                      # delta
                        call             rt_cap_push@PLT
                                                                                        jmp   n350_match_alternate_α
n349_match_assign_save_β:
                        lea              rdi, [rbp + 48]                                # slot
                        call             rt_cap_pop@PLT
                        add              rsp, 112
                                                                                        jmp   proc_PAT$5_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n350_match_alternate_α:
                        mov              dword ptr [rbp + 64], r14d
                        lea              rax, [rip + .Lx357_21]
                        mov              qword ptr [rbp + 80], rax
                                                                                        jmp   n353_match_patref_α
.Lx357_21:
                        lea              rax, [rip + .Lx357_19]
                        mov              qword ptr [rbp + 80], rax
                                                                                        jmp   n352_match_patref_α
n350_match_alternate_s0:
                        lea              rax, [rip + .Lx357_40]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n350_match_alternate_as
n350_match_alternate_s1:
                        lea              rax, [rip + .Lx357_41]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n350_match_alternate_as
.Lx357_40:
                                                                                        jmp   n353_match_patref_β
.Lx357_41:
                                                                                        jmp   n352_match_patref_β
n350_match_alternate_as:
                                                                                        jmp   n351_match_assign_cond_α
n350_match_alternate_β:
                        mov              rax, qword ptr [rbp + 72]
                                                                                        jmp   rax
n350_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 64]
                        mov              rax, qword ptr [rbp + 80]
                                                                                        jmp   rax
.Lx357_19:
                                                                                        jmp   n349_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n351_match_assign_cond_α:
                        lea              rdi, [rbp + 48]                                # slot
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S3]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        add              rsp, 112
                                                                                        jmp   proc_PAT$5_scanhit
n351_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n350_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n352_match_patref_α:
                        mov              rax, qword ptr [1879052464]                    # integer
                        mov              rdx, qword ptr [1879052472]
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
                                                                                        jmp   n350_match_alternate_s1
.Lx360_5:
                                                                                        jmp   n350_match_alternate_af
.Lx360_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]                               # varname
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
                                                                                        js    n350_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx360_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n350_match_alternate_s1
.Lx360_6:
                        add              rsp, 16
                                                                                        jmp   n350_match_alternate_af
n352_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n353_match_patref_α:
                        mov              rax, qword ptr [1879052496]                    # real
                        mov              rdx, qword ptr [1879052504]
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
                                                                                        jmp   n350_match_alternate_s0
.Lx361_5:
                                                                                        jmp   n350_match_alternate_af
.Lx361_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]                               # varname
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
                                                                                        js    n350_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx361_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n350_match_alternate_s0
.Lx361_6:
                        add              rsp, 16
                                                                                        jmp   n350_match_alternate_af
n353_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$5_scanhit:
                        cmp              qword ptr [rbp + 160], 1
                                                                                        jne   .Lx362_0
                        mov              ecx, dword ptr [rbp + 152]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx362_0:
                                                                                        jmp   proc_PAT$5_γ
proc_PAT$5_scanfail:
                        cmp              qword ptr [rbp + 160], 1
                                                                                        jne   .Lx363_0
                        mov              eax, dword ptr [rbp + 152]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx363_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx363_0
                        mov              dword ptr [rbp + 152], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$5_attempt
.Lx363_0:
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
                        sub              rsp, 176
                        mov              qword ptr [rsp + 152], rcx
                        mov              qword ptr [rsp + 160], rdx
                        mov              qword ptr [rsp + 168], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 144], r8
                        mov              dword ptr [rsp + 136], r14d
proc_PAT$6_attempt:
proc_PAT$6_α_body:
                        lea              rax, [rip + n364_match_alternate_β]
                        mov              qword ptr [rbp + 112], rax
#-----------------------------------------------------------------------------------------------------------------------
n364_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx371_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n369_match_patref_α
.Lx371_21:
                        lea              rax, [rip + .Lx371_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n365_match_sequence_α
n364_match_alternate_s0:
                        lea              rax, [rip + .Lx371_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n364_match_alternate_as
n364_match_alternate_s1:
                        lea              rax, [rip + .Lx371_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n364_match_alternate_as
.Lx371_40:
                                                                                        jmp   n369_match_patref_β
.Lx371_41:
                                                                                        jmp   n365_match_sequence_β
n364_match_alternate_as:
                                                                                        jmp   proc_PAT$6_γ
n364_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n364_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx371_19:
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
n365_match_sequence_α:
                        mov              dword ptr [rbp + 64], r14d
                                                                                        jmp   n368_match_lit_α
n365_match_sequence_as:
                                                                                        jmp   n364_match_alternate_s1
n365_match_sequence_β:
                                                                                        jmp   n366_match_lit_β
n365_match_sequence_af:
                                                                                        jmp   n364_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n366_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n367_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 41
                                                                                        jne   n367_match_defer_β
                        add              r14d, 1
                                                                                        jmp   n364_match_alternate_s1
n366_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n367_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n367_match_defer_α:
                        mov              rax, qword ptr [1879052608]                    # expr
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 8
                                                                                        jne   .Lx376_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx376_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx376_10
.Lx376_9:
                        xor              eax, eax
.Lx376_10:
                        test             rax, rax
                                                                                        jz    .Lx376_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx376_4]
                        lea              rdx, [rip + .Lx376_5]
                                                                                        jmp   rax
.Lx376_4:
                                                                                        jmp   n366_match_lit_α
.Lx376_5:
                                                                                        jmp   n368_match_lit_β
.Lx376_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx376_2:
                        test             rax, rax
                                                                                        je    .Lx376_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx376_7]
                        lea              rdx, [rip + .Lx376_8]
                                                                                        jmp   rax
.Lx376_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx376_2
.Lx376_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx376_2
.Lx376_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n368_match_lit_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx376_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n366_match_lit_α
.Lx376_6:
                        add              rsp, 16
                                                                                        jmp   n368_match_lit_β
n367_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n368_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n364_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 40
                                                                                        jne   n364_match_alternate_af
                        add              r14d, 1
                                                                                        jmp   n367_match_defer_α
n368_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n364_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n369_match_patref_α:
                        mov              rax, qword ptr [1879052544]                    # constant
                        mov              rdx, qword ptr [1879052552]
                        cmp              eax, 8
                                                                                        jne   .Lx379_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx379_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx379_10
.Lx379_9:
                        xor              eax, eax
.Lx379_10:
                        test             rax, rax
                                                                                        jz    .Lx379_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx379_4]
                        lea              rdx, [rip + .Lx379_5]
                                                                                        jmp   rax
.Lx379_4:
                                                                                        jmp   n364_match_alternate_s0
.Lx379_5:
                                                                                        jmp   n364_match_alternate_af
.Lx379_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx379_2:
                        test             rax, rax
                                                                                        je    .Lx379_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx379_7]
                        lea              rdx, [rip + .Lx379_8]
                                                                                        jmp   rax
.Lx379_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx379_2
.Lx379_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx379_2
.Lx379_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n364_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx379_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n364_match_alternate_s0
.Lx379_6:
                        add              rsp, 16
                                                                                        jmp   n364_match_alternate_af
n369_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$6_scanhit:
                        cmp              qword ptr [rbp + 144], 1
                                                                                        jne   .Lx380_0
                        mov              ecx, dword ptr [rbp + 136]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx380_0:
                                                                                        jmp   proc_PAT$6_γ
proc_PAT$6_scanfail:
                        cmp              qword ptr [rbp + 144], 1
                                                                                        jne   .Lx381_0
                        mov              eax, dword ptr [rbp + 136]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx381_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx381_0
                        mov              dword ptr [rbp + 136], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$6_attempt
.Lx381_0:
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_β:
                                                                                        jmp   qword ptr [rbp + 112]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$6_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 152]
                        mov              rbp, qword ptr [rbp + 168]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_ω:
                        mov              rax, qword ptr [rbp + 160]
                        lea              rsp, [rbp + 176]
                        mov              rbp, qword ptr [rbp + 168]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$7_α
proc_PAT$7_α:
                        sub              rsp, 224
                        mov              qword ptr [rsp + 200], rcx
                        mov              qword ptr [rsp + 208], rdx
                        mov              qword ptr [rsp + 216], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 192], r8
                        mov              dword ptr [rsp + 184], r14d
proc_PAT$7_attempt:
proc_PAT$7_α_body:
                        lea              rax, [rip + n382_match_alternate_β]
                        mov              qword ptr [rbp + 160], rax
#-----------------------------------------------------------------------------------------------------------------------
n382_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx390_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n384_match_sequence_α
.Lx390_21:
                        lea              rax, [rip + .Lx390_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n383_match_defer_α
n382_match_alternate_s0:
                        lea              rax, [rip + .Lx390_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n382_match_alternate_as
n382_match_alternate_s1:
                        lea              rax, [rip + .Lx390_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n382_match_alternate_as
.Lx390_40:
                                                                                        jmp   n384_match_sequence_β
.Lx390_41:
                                                                                        jmp   n383_match_defer_β
n382_match_alternate_as:
                                                                                        jmp   proc_PAT$7_γ
n382_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n382_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx390_19:
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
n383_match_defer_α:
                        mov              rax, qword ptr [1879052560]                    # primary
                        mov              rdx, qword ptr [1879052568]
                        cmp              eax, 8
                                                                                        jne   .Lx391_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx391_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx391_10
.Lx391_9:
                        xor              eax, eax
.Lx391_10:
                        test             rax, rax
                                                                                        jz    .Lx391_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx391_4]
                        lea              rdx, [rip + .Lx391_5]
                                                                                        jmp   rax
.Lx391_4:
                                                                                        jmp   n382_match_alternate_s1
.Lx391_5:
                                                                                        jmp   n382_match_alternate_af
.Lx391_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S7]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx391_2:
                        test             rax, rax
                                                                                        je    .Lx391_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx391_7]
                        lea              rdx, [rip + .Lx391_8]
                                                                                        jmp   rax
.Lx391_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx391_2
.Lx391_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx391_2
.Lx391_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n382_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx391_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n382_match_alternate_s1
.Lx391_6:
                        add              rsp, 16
                                                                                        jmp   n382_match_alternate_af
n383_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n384_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n388_match_patref_α
n384_match_sequence_as:
                                                                                        jmp   n382_match_alternate_s0
n384_match_sequence_β:
                                                                                        jmp   n387_match_assign_cond_β
n384_match_sequence_af:
                                                                                        jmp   n382_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n385_match_assign_save_α:
                        lea              rdi, [rbp + 112]                               # slot
                        mov              esi, r14d                                      # delta
                        call             rt_cap_push@PLT
                                                                                        jmp   n386_match_defer_α
n385_match_assign_save_β:
                        lea              rdi, [rbp + 112]                               # slot
                        call             rt_cap_pop@PLT
                                                                                        jmp   n388_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n386_match_defer_α:
                        mov              rax, qword ptr [1879052576]                    # factor
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 8
                                                                                        jne   .Lx396_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx396_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx396_10
.Lx396_9:
                        xor              eax, eax
.Lx396_10:
                        test             rax, rax
                                                                                        jz    .Lx396_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx396_4]
                        lea              rdx, [rip + .Lx396_5]
                                                                                        jmp   rax
.Lx396_4:
                                                                                        jmp   n387_match_assign_cond_α
.Lx396_5:
                                                                                        jmp   n385_match_assign_save_β
.Lx396_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx396_2:
                        test             rax, rax
                                                                                        je    .Lx396_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx396_7]
                        lea              rdx, [rip + .Lx396_8]
                                                                                        jmp   rax
.Lx396_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx396_2
.Lx396_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx396_2
.Lx396_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n385_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx396_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n387_match_assign_cond_α
.Lx396_6:
                        add              rsp, 16
                                                                                        jmp   n385_match_assign_save_β
n386_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n387_match_assign_cond_α:
                        lea              rdi, [rbp + 112]                               # slot
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S9]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n382_match_alternate_s0
n387_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n386_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n388_match_patref_α:
                        mov              rax, qword ptr [1879052512]                    # addop
                        mov              rdx, qword ptr [1879052520]
                        cmp              eax, 8
                                                                                        jne   .Lx399_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx399_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx399_10
.Lx399_9:
                        xor              eax, eax
.Lx399_10:
                        test             rax, rax
                                                                                        jz    .Lx399_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx399_4]
                        lea              rdx, [rip + .Lx399_5]
                                                                                        jmp   rax
.Lx399_4:
                                                                                        jmp   n385_match_assign_save_α
.Lx399_5:
                                                                                        jmp   n382_match_alternate_af
.Lx399_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx399_2:
                        test             rax, rax
                                                                                        je    .Lx399_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx399_7]
                        lea              rdx, [rip + .Lx399_8]
                                                                                        jmp   rax
.Lx399_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx399_2
.Lx399_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx399_2
.Lx399_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n382_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx399_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n385_match_assign_save_α
.Lx399_6:
                        add              rsp, 16
                                                                                        jmp   n382_match_alternate_af
n388_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$7_scanhit:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   .Lx400_0
                        mov              ecx, dword ptr [rbp + 184]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx400_0:
                                                                                        jmp   proc_PAT$7_γ
proc_PAT$7_scanfail:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   .Lx401_0
                        mov              eax, dword ptr [rbp + 184]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx401_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx401_0
                        mov              dword ptr [rbp + 184], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$7_attempt
.Lx401_0:
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_β:
                                                                                        jmp   qword ptr [rbp + 160]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$7_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 200]
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_ω:
                        mov              rax, qword ptr [rbp + 208]
                        lea              rsp, [rbp + 224]
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$8_α
proc_PAT$8_α:
                        sub              rsp, 240
                        mov              qword ptr [rsp + 216], rcx
                        mov              qword ptr [rsp + 224], rdx
                        mov              qword ptr [rsp + 232], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 208], r8
                        mov              dword ptr [rsp + 200], r14d
proc_PAT$8_attempt:
proc_PAT$8_α_body:
                        lea              rax, [rip + n402_match_alternate_β]
                        mov              qword ptr [rbp + 176], rax
#-----------------------------------------------------------------------------------------------------------------------
n402_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx411_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n404_match_sequence_α
.Lx411_21:
                        lea              rax, [rip + .Lx411_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n403_match_defer_α
n402_match_alternate_s0:
                        lea              rax, [rip + .Lx411_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n402_match_alternate_as
n402_match_alternate_s1:
                        lea              rax, [rip + .Lx411_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n402_match_alternate_as
.Lx411_40:
                                                                                        jmp   n404_match_sequence_β
.Lx411_41:
                                                                                        jmp   n403_match_defer_β
n402_match_alternate_as:
                                                                                        jmp   proc_PAT$8_γ
n402_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n402_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx411_19:
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
n403_match_defer_α:
                        mov              rax, qword ptr [1879052576]                    # factor
                        mov              rdx, qword ptr [1879052584]
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
                                                                                        jmp   n402_match_alternate_s1
.Lx412_5:
                                                                                        jmp   n402_match_alternate_af
.Lx412_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]                               # varname
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
                                                                                        js    n402_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx412_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n402_match_alternate_s1
.Lx412_6:
                        add              rsp, 16
                                                                                        jmp   n402_match_alternate_af
n403_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n404_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n409_match_defer_α
n404_match_sequence_as:
                                                                                        jmp   n402_match_alternate_s0
n404_match_sequence_β:
                                                                                        jmp   n407_match_assign_cond_β
n404_match_sequence_af:
                                                                                        jmp   n402_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n405_match_assign_save_α:
                        lea              rdi, [rbp + 128]                               # slot
                        mov              esi, r14d                                      # delta
                        call             rt_cap_push@PLT
                                                                                        jmp   n406_match_defer_α
n405_match_assign_save_β:
                        lea              rdi, [rbp + 128]                               # slot
                        call             rt_cap_pop@PLT
                                                                                        jmp   n408_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n406_match_defer_α:
                        mov              rax, qword ptr [1879052592]                    # term
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 8
                                                                                        jne   .Lx417_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx417_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx417_10
.Lx417_9:
                        xor              eax, eax
.Lx417_10:
                        test             rax, rax
                                                                                        jz    .Lx417_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx417_4]
                        lea              rdx, [rip + .Lx417_5]
                                                                                        jmp   rax
.Lx417_4:
                                                                                        jmp   n407_match_assign_cond_α
.Lx417_5:
                                                                                        jmp   n405_match_assign_save_β
.Lx417_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx417_2:
                        test             rax, rax
                                                                                        je    .Lx417_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx417_7]
                        lea              rdx, [rip + .Lx417_8]
                                                                                        jmp   rax
.Lx417_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx417_2
.Lx417_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx417_2
.Lx417_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n405_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx417_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n407_match_assign_cond_α
.Lx417_6:
                        add              rsp, 16
                                                                                        jmp   n405_match_assign_save_β
n406_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n407_match_assign_cond_α:
                        lea              rdi, [rbp + 128]                               # slot
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S12]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n402_match_alternate_s0
n407_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n406_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n408_match_patref_α:
                        mov              rax, qword ptr [1879052528]                    # mulop
                        mov              rdx, qword ptr [1879052536]
                        cmp              eax, 8
                                                                                        jne   .Lx420_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx420_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx420_10
.Lx420_9:
                        xor              eax, eax
.Lx420_10:
                        test             rax, rax
                                                                                        jz    .Lx420_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx420_4]
                        lea              rdx, [rip + .Lx420_5]
                                                                                        jmp   rax
.Lx420_4:
                                                                                        jmp   n405_match_assign_save_α
.Lx420_5:
                                                                                        jmp   n409_match_defer_β
.Lx420_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx420_2:
                        test             rax, rax
                                                                                        je    .Lx420_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx420_7]
                        lea              rdx, [rip + .Lx420_8]
                                                                                        jmp   rax
.Lx420_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx420_2
.Lx420_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx420_2
.Lx420_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n409_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx420_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n405_match_assign_save_α
.Lx420_6:
                        add              rsp, 16
                                                                                        jmp   n409_match_defer_β
n408_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n409_match_defer_α:
                        mov              rax, qword ptr [1879052576]                    # factor
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 8
                                                                                        jne   .Lx421_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx421_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx421_10
.Lx421_9:
                        xor              eax, eax
.Lx421_10:
                        test             rax, rax
                                                                                        jz    .Lx421_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx421_4]
                        lea              rdx, [rip + .Lx421_5]
                                                                                        jmp   rax
.Lx421_4:
                                                                                        jmp   n408_match_patref_α
.Lx421_5:
                                                                                        jmp   n402_match_alternate_af
.Lx421_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx421_2:
                        test             rax, rax
                                                                                        je    .Lx421_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx421_7]
                        lea              rdx, [rip + .Lx421_8]
                                                                                        jmp   rax
.Lx421_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx421_2
.Lx421_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx421_2
.Lx421_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n402_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx421_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n408_match_patref_α
.Lx421_6:
                        add              rsp, 16
                                                                                        jmp   n402_match_alternate_af
n409_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$8_scanhit:
                        cmp              qword ptr [rbp + 208], 1
                                                                                        jne   .Lx422_0
                        mov              ecx, dword ptr [rbp + 200]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx422_0:
                                                                                        jmp   proc_PAT$8_γ
proc_PAT$8_scanfail:
                        cmp              qword ptr [rbp + 208], 1
                                                                                        jne   .Lx423_0
                        mov              eax, dword ptr [rbp + 200]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx423_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx423_0
                        mov              dword ptr [rbp + 200], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$8_attempt
.Lx423_0:
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_β:
                                                                                        jmp   qword ptr [rbp + 176]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$8_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 216]
                        mov              rbp, qword ptr [rbp + 232]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_ω:
                        mov              rax, qword ptr [rbp + 224]
                        lea              rsp, [rbp + 240]
                        mov              rbp, qword ptr [rbp + 232]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$9_α
proc_PAT$9_α:
                        sub              rsp, 240
                        mov              qword ptr [rsp + 216], rcx
                        mov              qword ptr [rsp + 224], rdx
                        mov              qword ptr [rsp + 232], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 208], r8
                        mov              dword ptr [rsp + 200], r14d
proc_PAT$9_attempt:
proc_PAT$9_α_body:
                        lea              rax, [rip + n424_match_alternate_β]
                        mov              qword ptr [rbp + 176], rax
#-----------------------------------------------------------------------------------------------------------------------
n424_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx433_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n426_match_sequence_α
.Lx433_21:
                        lea              rax, [rip + .Lx433_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n425_match_defer_α
n424_match_alternate_s0:
                        lea              rax, [rip + .Lx433_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n424_match_alternate_as
n424_match_alternate_s1:
                        lea              rax, [rip + .Lx433_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n424_match_alternate_as
.Lx433_40:
                                                                                        jmp   n426_match_sequence_β
.Lx433_41:
                                                                                        jmp   n425_match_defer_β
n424_match_alternate_as:
                                                                                        jmp   proc_PAT$9_γ
n424_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n424_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx433_19:
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
n425_match_defer_α:
                        mov              rax, qword ptr [1879052592]                    # term
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 8
                                                                                        jne   .Lx434_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx434_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx434_10
.Lx434_9:
                        xor              eax, eax
.Lx434_10:
                        test             rax, rax
                                                                                        jz    .Lx434_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx434_4]
                        lea              rdx, [rip + .Lx434_5]
                                                                                        jmp   rax
.Lx434_4:
                                                                                        jmp   n424_match_alternate_s1
.Lx434_5:
                                                                                        jmp   n424_match_alternate_af
.Lx434_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx434_2:
                        test             rax, rax
                                                                                        je    .Lx434_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx434_7]
                        lea              rdx, [rip + .Lx434_8]
                                                                                        jmp   rax
.Lx434_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx434_2
.Lx434_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx434_2
.Lx434_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n424_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx434_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n424_match_alternate_s1
.Lx434_6:
                        add              rsp, 16
                                                                                        jmp   n424_match_alternate_af
n425_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n426_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n431_match_defer_α
n426_match_sequence_as:
                                                                                        jmp   n424_match_alternate_s0
n426_match_sequence_β:
                                                                                        jmp   n429_match_assign_cond_β
n426_match_sequence_af:
                                                                                        jmp   n424_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n427_match_assign_save_α:
                        lea              rdi, [rbp + 128]                               # slot
                        mov              esi, r14d                                      # delta
                        call             rt_cap_push@PLT
                                                                                        jmp   n428_match_defer_α
n427_match_assign_save_β:
                        lea              rdi, [rbp + 128]                               # slot
                        call             rt_cap_pop@PLT
                                                                                        jmp   n430_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n428_match_defer_α:
                        mov              rax, qword ptr [1879052608]                    # expr
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 8
                                                                                        jne   .Lx439_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx439_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx439_10
.Lx439_9:
                        xor              eax, eax
.Lx439_10:
                        test             rax, rax
                                                                                        jz    .Lx439_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx439_4]
                        lea              rdx, [rip + .Lx439_5]
                                                                                        jmp   rax
.Lx439_4:
                                                                                        jmp   n429_match_assign_cond_α
.Lx439_5:
                                                                                        jmp   n427_match_assign_save_β
.Lx439_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx439_2:
                        test             rax, rax
                                                                                        je    .Lx439_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx439_7]
                        lea              rdx, [rip + .Lx439_8]
                                                                                        jmp   rax
.Lx439_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx439_2
.Lx439_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx439_2
.Lx439_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n427_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx439_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n429_match_assign_cond_α
.Lx439_6:
                        add              rsp, 16
                                                                                        jmp   n427_match_assign_save_β
n428_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n429_match_assign_cond_α:
                        lea              rdi, [rbp + 128]                               # slot
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S12]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n424_match_alternate_s0
n429_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n428_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n430_match_patref_α:
                        mov              rax, qword ptr [1879052512]                    # addop
                        mov              rdx, qword ptr [1879052520]
                        cmp              eax, 8
                                                                                        jne   .Lx442_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx442_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx442_10
.Lx442_9:
                        xor              eax, eax
.Lx442_10:
                        test             rax, rax
                                                                                        jz    .Lx442_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx442_4]
                        lea              rdx, [rip + .Lx442_5]
                                                                                        jmp   rax
.Lx442_4:
                                                                                        jmp   n427_match_assign_save_α
.Lx442_5:
                                                                                        jmp   n431_match_defer_β
.Lx442_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx442_2:
                        test             rax, rax
                                                                                        je    .Lx442_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx442_7]
                        lea              rdx, [rip + .Lx442_8]
                                                                                        jmp   rax
.Lx442_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx442_2
.Lx442_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx442_2
.Lx442_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n431_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx442_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n427_match_assign_save_α
.Lx442_6:
                        add              rsp, 16
                                                                                        jmp   n431_match_defer_β
n430_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n431_match_defer_α:
                        mov              rax, qword ptr [1879052592]                    # term
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 8
                                                                                        jne   .Lx443_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx443_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx443_10
.Lx443_9:
                        xor              eax, eax
.Lx443_10:
                        test             rax, rax
                                                                                        jz    .Lx443_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx443_4]
                        lea              rdx, [rip + .Lx443_5]
                                                                                        jmp   rax
.Lx443_4:
                                                                                        jmp   n430_match_patref_α
.Lx443_5:
                                                                                        jmp   n424_match_alternate_af
.Lx443_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]                              # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx443_2:
                        test             rax, rax
                                                                                        je    .Lx443_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx443_7]
                        lea              rdx, [rip + .Lx443_8]
                                                                                        jmp   rax
.Lx443_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx443_2
.Lx443_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx443_2
.Lx443_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n424_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx443_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n430_match_patref_α
.Lx443_6:
                        add              rsp, 16
                                                                                        jmp   n424_match_alternate_af
n431_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$9_scanhit:
                        cmp              qword ptr [rbp + 208], 1
                                                                                        jne   .Lx444_0
                        mov              ecx, dword ptr [rbp + 200]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx444_0:
                                                                                        jmp   proc_PAT$9_γ
proc_PAT$9_scanfail:
                        cmp              qword ptr [rbp + 208], 1
                                                                                        jne   .Lx445_0
                        mov              eax, dword ptr [rbp + 200]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx445_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx445_0
                        mov              dword ptr [rbp + 200], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$9_attempt
.Lx445_0:
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_β:
                                                                                        jmp   qword ptr [rbp + 176]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$9_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 216]
                        mov              rbp, qword ptr [rbp + 232]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_ω:
                        mov              rax, qword ptr [rbp + 224]
                        lea              rsp, [rbp + 240]
                        mov              rbp, qword ptr [rbp + 232]
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
                        mov              esi, 2544
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
                        mov              esi, 2544
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
                        mov              esi, 2544
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
                        mov              esi, 2544
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
                        mov              esi, 128
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
                        mov              esi, 288
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
                        mov              esi, 128
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
                        mov              esi, 176
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
                        mov              esi, 192
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
                        mov              esi, 192
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
.Lgvan13:               .string          "real"
.Lgvan14:               .string          "addop"
.Lgvan15:               .string          "mulop"
.Lgvan16:               .string          "constant"
.Lgvan17:               .string          "primary"
.Lgvan18:               .string          "factor"
.Lgvan19:               .string          "term"
.Lgvan20:               .string          "expr"
.Lgvan21:               .string          "line"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 22
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 22
                        call             gva_register@PLT
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#-----------------------------------------------------------------------------------------------------------------------
n446_statement_α:
                                                                                        jmp   n447_statement_α
#=======================================================================================================================
#          DEFINE('Push(x)')
#-----------------------------------------------------------------------------------------------------------------------
n447_statement_α:
                                                                                        jmp   n448_call_α
#=======================================================================================================================
#          stk      =  TABLE()                       :(PushEnd)
#-----------------------------------------------------------------------------------------------------------------------
n448_call_α:
                        sub              rsp, 16
                        .section         .rodata
.Lrkfnzd637:            .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd637]                       # fn
                        xor              esi, esi                                       # args
                        mov              edx, 0                                         # nargs
                        call             rt_call_arr@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx636_240
                        add              rsp, 16
                                                                                        jmp   n451_statement_α
.Lx636_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n449_assign_α
n448_call_β:
                        add              rsp, 16
                                                                                        jmp   n451_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n449_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052432], rax                    # stk
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n450_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n450_statement_α:
                                                                                        jmp   n451_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n451_statement_α:
                        add              rsp, 16
                                                                                        jmp   n452_statement_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#-----------------------------------------------------------------------------------------------------------------------
n452_statement_α:
                                                                                        jmp   n453_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n453_statement_α:
                                                                                        jmp   n454_statement_α
#=======================================================================================================================
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n454_statement_α:
                                                                                        jmp   n455_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n455_statement_α:
                                                                                        jmp   n456_statement_α
#=======================================================================================================================
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n456_statement_α:
                                                                                        jmp   n457_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n457_statement_α:
                                                                                        jmp   n458_lit_string_α
#=======================================================================================================================
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n458_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx655_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n459_call_α
.Lx655_0:
                        .quad            .Lx655_0_s
.Lx655_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n459_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd657:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd657]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx656_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n462_lit_string_α
.Lx656_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n460_assign_α
n459_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n462_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n460_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052464], rax                    # integer
                        mov              qword ptr [1879052472], rdx
                                                                                        jmp   n461_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n461_statement_α:
                        add              rsp, 32
                                                                                        jmp   n462_lit_string_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n462_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx661_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n463_call_α
.Lx661_0:
                        .quad            .Lx661_0_s
.Lx661_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n463_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd663:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd663]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx662_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n466_lit_string_α
.Lx662_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n464_assign_α
n463_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n466_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n464_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052480], rax                    # exponent
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n465_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n465_statement_α:
                        add              rsp, 32
                                                                                        jmp   n466_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n466_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx667_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n467_call_α
.Lx667_0:
                        .quad            .Lx667_0_s
.Lx667_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n467_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd669:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd669]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx668_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n470_statement_α
.Lx668_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n468_assign_α
n467_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n470_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n468_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052496], rax                    # real
                        mov              qword ptr [1879052504], rdx
                                                                                        jmp   n469_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n469_statement_α:
                        add              rsp, 32
                                                                                        jmp   n470_statement_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n470_statement_α:
                                                                                        jmp   n471_lit_string_α
#=======================================================================================================================
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n471_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx675_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n472_call_α
.Lx675_0:
                        .quad            .Lx675_0_s
.Lx675_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n472_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd677:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd677]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx676_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n475_lit_string_α
.Lx676_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n473_assign_α
n472_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n475_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n473_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052512], rax                    # addop
                        mov              qword ptr [1879052520], rdx
                                                                                        jmp   n474_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n474_statement_α:
                        add              rsp, 32
                                                                                        jmp   n475_lit_string_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n475_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx681_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n476_call_α
.Lx681_0:
                        .quad            .Lx681_0_s
.Lx681_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n476_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd683:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd683]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx682_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n479_lit_string_α
.Lx682_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n477_assign_α
n476_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n479_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n477_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052528], rax                    # mulop
                        mov              qword ptr [1879052536], rdx
                                                                                        jmp   n478_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n478_statement_α:
                        add              rsp, 32
                                                                                        jmp   n479_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n479_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx687_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n480_call_α
.Lx687_0:
                        .quad            .Lx687_0_s
.Lx687_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n480_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd689:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd689]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx688_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n483_statement_α
.Lx688_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n481_assign_α
n480_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n483_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n481_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052544], rax                    # constant
                        mov              qword ptr [1879052552], rdx
                                                                                        jmp   n482_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n482_statement_α:
                        add              rsp, 32
                                                                                        jmp   n483_statement_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n483_statement_α:
                                                                                        jmp   n484_lit_string_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n484_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx695_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n485_call_α
.Lx695_0:
                        .quad            .Lx695_0_s
.Lx695_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n485_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd697:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd697]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx696_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n488_statement_α
.Lx696_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n486_assign_α
n485_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n488_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n486_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052560], rax                    # primary
                        mov              qword ptr [1879052568], rdx
                                                                                        jmp   n487_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n487_statement_α:
                        add              rsp, 32
                                                                                        jmp   n488_statement_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n488_statement_α:
                                                                                        jmp   n489_lit_string_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n489_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx703_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n490_call_α
.Lx703_0:
                        .quad            .Lx703_0_s
.Lx703_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n490_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd705:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd705]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx704_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n493_statement_α
.Lx704_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n491_assign_α
n490_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n493_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n491_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052576], rax                    # factor
                        mov              qword ptr [1879052584], rdx
                                                                                        jmp   n492_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n492_statement_α:
                        add              rsp, 32
                                                                                        jmp   n493_statement_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n493_statement_α:
                                                                                        jmp   n494_lit_string_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n494_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx711_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n495_call_α
.Lx711_0:
                        .quad            .Lx711_0_s
.Lx711_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n495_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd713:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd713]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx712_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n498_statement_α
.Lx712_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n496_assign_α
n495_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n498_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n496_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052592], rax                    # term
                        mov              qword ptr [1879052600], rdx
                                                                                        jmp   n497_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n497_statement_α:
                        add              rsp, 32
                                                                                        jmp   n498_statement_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n498_statement_α:
                                                                                        jmp   n499_lit_string_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n499_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx719_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n500_call_α
.Lx719_0:
                        .quad            .Lx719_0_s
.Lx719_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n500_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd721:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd721]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx720_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n503_statement_α
.Lx720_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n501_assign_α
n500_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n503_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n501_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052608], rax                    # expr
                        mov              qword ptr [1879052616], rdx
                                                                                        jmp   n502_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n502_statement_α:
                        add              rsp, 32
                                                                                        jmp   n503_statement_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n503_statement_α:
                                                                                        jmp   n504_lit_string_α
#=======================================================================================================================
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n504_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx727_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n505_lit_integer_α
.Lx727_0:
                        .quad            .Lx727_0_s
.Lx727_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n505_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx728_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n506_call_α
.Lx728_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n506_call_α:
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
.Lrkfnzd730:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd730]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx729_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n508_var_α
.Lx729_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n507_statement_α
n506_call_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n508_var_α
#-----------------------------------------------------------------------------------------------------------------------
n507_statement_α:
                        add              rsp, 48
                                                                                        jmp   n508_var_α
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n508_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx733_0]                # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx733_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx733_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n509_assign_α
.Lx733_0:
                        .quad            .Lx733_0_s
.Lx733_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n509_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052624], rax                    # line
                        mov              qword ptr [1879052632], rdx
                                                                                        jmp   n510_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n510_statement_α:
                        add              rsp, 16
                                                                                        jmp   n511_var_α
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n511_var_α:
                        sub              rsp, 2400
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 64], 0
                        mov              qword ptr [rsp + 72], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0
                        mov              qword ptr [rsp + 192], 0
                        mov              qword ptr [rsp + 200], 0
                        mov              qword ptr [rsp + 208], 0
                        mov              qword ptr [rsp + 216], 0
                        mov              qword ptr [rsp + 224], 0
                        mov              qword ptr [rsp + 232], 0
                        mov              qword ptr [rsp + 240], 0
                        mov              qword ptr [rsp + 248], 0
                        mov              qword ptr [rsp + 256], 0
                        mov              qword ptr [rsp + 264], 0
                        mov              qword ptr [rsp + 272], 0
                        mov              qword ptr [rsp + 280], 0
                        mov              qword ptr [rsp + 288], 0
                        mov              qword ptr [rsp + 296], 0
                        mov              qword ptr [rsp + 304], 0
                        mov              qword ptr [rsp + 312], 0
                        mov              qword ptr [rsp + 320], 0
                        mov              qword ptr [rsp + 328], 0
                        mov              qword ptr [rsp + 336], 0
                        mov              qword ptr [rsp + 344], 0
                        mov              qword ptr [rsp + 352], 0
                        mov              qword ptr [rsp + 360], 0
                        mov              qword ptr [rsp + 368], 0
                        mov              qword ptr [rsp + 376], 0
                        mov              qword ptr [rsp + 384], 0
                        mov              qword ptr [rsp + 392], 0
                        mov              qword ptr [rsp + 400], 0
                        mov              qword ptr [rsp + 408], 0
                        mov              qword ptr [rsp + 416], 0
                        mov              qword ptr [rsp + 424], 0
                        mov              qword ptr [rsp + 432], 0
                        mov              qword ptr [rsp + 440], 0
                        mov              qword ptr [rsp + 448], 0
                        mov              qword ptr [rsp + 456], 0
                        mov              qword ptr [rsp + 464], 0
                        mov              qword ptr [rsp + 472], 0
                        mov              qword ptr [rsp + 480], 0
                        mov              qword ptr [rsp + 488], 0
                        mov              qword ptr [rsp + 496], 0
                        mov              qword ptr [rsp + 504], 0
                        mov              qword ptr [rsp + 512], 0
                        mov              qword ptr [rsp + 520], 0
                        mov              qword ptr [rsp + 528], 0
                        mov              qword ptr [rsp + 536], 0
                        mov              qword ptr [rsp + 544], 0
                        mov              qword ptr [rsp + 552], 0
                        mov              qword ptr [rsp + 560], 0
                        mov              qword ptr [rsp + 568], 0
                        mov              qword ptr [rsp + 576], 0
                        mov              qword ptr [rsp + 584], 0
                        mov              qword ptr [rsp + 592], 0
                        mov              qword ptr [rsp + 600], 0
                        mov              qword ptr [rsp + 608], 0
                        mov              qword ptr [rsp + 616], 0
                        mov              qword ptr [rsp + 624], 0
                        mov              qword ptr [rsp + 632], 0
                        mov              qword ptr [rsp + 640], 0
                        mov              qword ptr [rsp + 648], 0
                        mov              qword ptr [rsp + 656], 0
                        mov              qword ptr [rsp + 664], 0
                        mov              qword ptr [rsp + 672], 0
                        mov              qword ptr [rsp + 680], 0
                        mov              qword ptr [rsp + 688], 0
                        mov              qword ptr [rsp + 696], 0
                        mov              qword ptr [rsp + 704], 0
                        mov              qword ptr [rsp + 712], 0
                        mov              qword ptr [rsp + 720], 0
                        mov              qword ptr [rsp + 728], 0
                        mov              qword ptr [rsp + 736], 0
                        mov              qword ptr [rsp + 744], 0
                        mov              qword ptr [rsp + 752], 0
                        mov              qword ptr [rsp + 760], 0
                        mov              qword ptr [rsp + 768], 0
                        mov              qword ptr [rsp + 776], 0
                        mov              qword ptr [rsp + 784], 0
                        mov              qword ptr [rsp + 792], 0
                        mov              qword ptr [rsp + 800], 0
                        mov              qword ptr [rsp + 808], 0
                        mov              qword ptr [rsp + 816], 0
                        mov              qword ptr [rsp + 824], 0
                        mov              qword ptr [rsp + 832], 0
                        mov              qword ptr [rsp + 840], 0
                        mov              qword ptr [rsp + 848], 0
                        mov              qword ptr [rsp + 856], 0
                        mov              qword ptr [rsp + 864], 0
                        mov              qword ptr [rsp + 872], 0
                        mov              qword ptr [rsp + 880], 0
                        mov              qword ptr [rsp + 888], 0
                        mov              qword ptr [rsp + 896], 0
                        mov              qword ptr [rsp + 904], 0
                        mov              qword ptr [rsp + 912], 0
                        mov              qword ptr [rsp + 920], 0
                        mov              qword ptr [rsp + 928], 0
                        mov              qword ptr [rsp + 936], 0
                        mov              qword ptr [rsp + 944], 0
                        mov              qword ptr [rsp + 952], 0
                        mov              qword ptr [rsp + 960], 0
                        mov              qword ptr [rsp + 968], 0
                        mov              qword ptr [rsp + 976], 0
                        mov              qword ptr [rsp + 984], 0
                        mov              qword ptr [rsp + 992], 0
                        mov              qword ptr [rsp + 1000], 0
                        mov              qword ptr [rsp + 1008], 0
                        mov              qword ptr [rsp + 1016], 0
                        mov              qword ptr [rsp + 1024], 0
                        mov              qword ptr [rsp + 1032], 0
                        mov              qword ptr [rsp + 1040], 0
                        mov              qword ptr [rsp + 1048], 0
                        mov              qword ptr [rsp + 1056], 0
                        mov              qword ptr [rsp + 1064], 0
                        mov              qword ptr [rsp + 1072], 0
                        mov              qword ptr [rsp + 1080], 0
                        mov              qword ptr [rsp + 1088], 0
                        mov              qword ptr [rsp + 1096], 0
                        mov              qword ptr [rsp + 1104], 0
                        mov              qword ptr [rsp + 1112], 0
                        mov              qword ptr [rsp + 1120], 0
                        mov              qword ptr [rsp + 1128], 0
                        mov              qword ptr [rsp + 1136], 0
                        mov              qword ptr [rsp + 1144], 0
                        mov              qword ptr [rsp + 1152], 0
                        mov              qword ptr [rsp + 1160], 0
                        mov              qword ptr [rsp + 1168], 0
                        mov              qword ptr [rsp + 1176], 0
                        mov              qword ptr [rsp + 1184], 0
                        mov              qword ptr [rsp + 1192], 0
                        mov              qword ptr [rsp + 1200], 0
                        mov              qword ptr [rsp + 1208], 0
                        mov              qword ptr [rsp + 1216], 0
                        mov              qword ptr [rsp + 1224], 0
                        mov              qword ptr [rsp + 1232], 0
                        mov              qword ptr [rsp + 1240], 0
                        mov              qword ptr [rsp + 1248], 0
                        mov              qword ptr [rsp + 1256], 0
                        mov              qword ptr [rsp + 1264], 0
                        mov              qword ptr [rsp + 1272], 0
                        mov              qword ptr [rsp + 1280], 0
                        mov              qword ptr [rsp + 1288], 0
                        mov              qword ptr [rsp + 1296], 0
                        mov              qword ptr [rsp + 1304], 0
                        mov              qword ptr [rsp + 1312], 0
                        mov              qword ptr [rsp + 1320], 0
                        mov              qword ptr [rsp + 1328], 0
                        mov              qword ptr [rsp + 1336], 0
                        mov              qword ptr [rsp + 1344], 0
                        mov              qword ptr [rsp + 1352], 0
                        mov              qword ptr [rsp + 1360], 0
                        mov              qword ptr [rsp + 1368], 0
                        mov              qword ptr [rsp + 1376], 0
                        mov              qword ptr [rsp + 1384], 0
                        mov              qword ptr [rsp + 1392], 0
                        mov              qword ptr [rsp + 1400], 0
                        mov              qword ptr [rsp + 1408], 0
                        mov              qword ptr [rsp + 1416], 0
                        mov              qword ptr [rsp + 1424], 0
                        mov              qword ptr [rsp + 1432], 0
                        mov              qword ptr [rsp + 1440], 0
                        mov              qword ptr [rsp + 1448], 0
                        mov              qword ptr [rsp + 1456], 0
                        mov              qword ptr [rsp + 1464], 0
                        mov              qword ptr [rsp + 1472], 0
                        mov              qword ptr [rsp + 1480], 0
                        mov              qword ptr [rsp + 1488], 0
                        mov              qword ptr [rsp + 1496], 0
                        mov              qword ptr [rsp + 1504], 0
                        mov              qword ptr [rsp + 1512], 0
                        mov              qword ptr [rsp + 1520], 0
                        mov              qword ptr [rsp + 1528], 0
                        mov              qword ptr [rsp + 1536], 0
                        mov              qword ptr [rsp + 1544], 0
                        mov              qword ptr [rsp + 1552], 0
                        mov              qword ptr [rsp + 1560], 0
                        mov              qword ptr [rsp + 1568], 0
                        mov              qword ptr [rsp + 1576], 0
                        mov              qword ptr [rsp + 1584], 0
                        mov              qword ptr [rsp + 1592], 0
                        mov              qword ptr [rsp + 1600], 0
                        mov              qword ptr [rsp + 1608], 0
                        mov              qword ptr [rsp + 1616], 0
                        mov              qword ptr [rsp + 1624], 0
                        mov              qword ptr [rsp + 1632], 0
                        mov              qword ptr [rsp + 1640], 0
                        mov              qword ptr [rsp + 1648], 0
                        mov              qword ptr [rsp + 1656], 0
                        mov              qword ptr [rsp + 1664], 0
                        mov              qword ptr [rsp + 1672], 0
                        mov              qword ptr [rsp + 1680], 0
                        mov              qword ptr [rsp + 1688], 0
                        mov              qword ptr [rsp + 1696], 0
                        mov              qword ptr [rsp + 1704], 0
                        mov              qword ptr [rsp + 1712], 0
                        mov              qword ptr [rsp + 1720], 0
                        mov              qword ptr [rsp + 1728], 0
                        mov              qword ptr [rsp + 1736], 0
                        mov              qword ptr [rsp + 1744], 0
                        mov              qword ptr [rsp + 1752], 0
                        mov              qword ptr [rsp + 1760], 0
                        mov              qword ptr [rsp + 1768], 0
                        mov              qword ptr [rsp + 1776], 0
                        mov              qword ptr [rsp + 1784], 0
                        mov              qword ptr [rsp + 1792], 0
                        mov              qword ptr [rsp + 1800], 0
                        mov              qword ptr [rsp + 1808], 0
                        mov              qword ptr [rsp + 1816], 0
                        mov              qword ptr [rsp + 1824], 0
                        mov              qword ptr [rsp + 1832], 0
                        mov              qword ptr [rsp + 1840], 0
                        mov              qword ptr [rsp + 1848], 0
                        mov              qword ptr [rsp + 1856], 0
                        mov              qword ptr [rsp + 1864], 0
                        mov              qword ptr [rsp + 1872], 0
                        mov              qword ptr [rsp + 1880], 0
                        mov              qword ptr [rsp + 1888], 0
                        mov              qword ptr [rsp + 1896], 0
                        mov              qword ptr [rsp + 1904], 0
                        mov              qword ptr [rsp + 1912], 0
                        mov              qword ptr [rsp + 1920], 0
                        mov              qword ptr [rsp + 1928], 0
                        mov              qword ptr [rsp + 1936], 0
                        mov              qword ptr [rsp + 1944], 0
                        mov              qword ptr [rsp + 1952], 0
                        mov              qword ptr [rsp + 1960], 0
                        mov              qword ptr [rsp + 1968], 0
                        mov              qword ptr [rsp + 1976], 0
                        mov              qword ptr [rsp + 1984], 0
                        mov              qword ptr [rsp + 1992], 0
                        mov              qword ptr [rsp + 2000], 0
                        mov              qword ptr [rsp + 2008], 0
                        mov              qword ptr [rsp + 2016], 0
                        mov              qword ptr [rsp + 2024], 0
                        mov              qword ptr [rsp + 2032], 0
                        mov              qword ptr [rsp + 2040], 0
                        mov              qword ptr [rsp + 2048], 0
                        mov              qword ptr [rsp + 2056], 0
                        mov              qword ptr [rsp + 2064], 0
                        mov              qword ptr [rsp + 2072], 0
                        mov              qword ptr [rsp + 2080], 0
                        mov              qword ptr [rsp + 2088], 0
                        mov              qword ptr [rsp + 2096], 0
                        mov              qword ptr [rsp + 2104], 0
                        mov              qword ptr [rsp + 2112], 0
                        mov              qword ptr [rsp + 2120], 0
                        mov              qword ptr [rsp + 2128], 0
                        mov              qword ptr [rsp + 2136], 0
                        mov              qword ptr [rsp + 2144], 0
                        mov              qword ptr [rsp + 2152], 0
                        mov              qword ptr [rsp + 2160], 0
                        mov              qword ptr [rsp + 2168], 0
                        mov              qword ptr [rsp + 2176], 0
                        mov              qword ptr [rsp + 2184], 0
                        mov              qword ptr [rsp + 2192], 0
                        mov              qword ptr [rsp + 2200], 0
                        mov              qword ptr [rsp + 2208], 0
                        mov              qword ptr [rsp + 2216], 0
                        mov              qword ptr [rsp + 2224], 0
                        mov              qword ptr [rsp + 2232], 0
                        mov              qword ptr [rsp + 2240], 0
                        mov              qword ptr [rsp + 2248], 0
                        mov              qword ptr [rsp + 2256], 0
                        mov              qword ptr [rsp + 2264], 0
                        mov              qword ptr [rsp + 2272], 0
                        mov              qword ptr [rsp + 2280], 0
                        mov              qword ptr [rsp + 2288], 0
                        mov              qword ptr [rsp + 2296], 0
                        mov              qword ptr [rsp + 2304], 0
                        mov              qword ptr [rsp + 2312], 0
                        mov              qword ptr [rsp + 2320], 0
                        mov              qword ptr [rsp + 2328], 0
                        mov              qword ptr [rsp + 2336], 0
                        mov              qword ptr [rsp + 2344], 0
                        mov              qword ptr [rsp + 2352], 0
                        mov              qword ptr [rsp + 2360], 0
                        mov              qword ptr [rsp + 2368], 0
                        mov              qword ptr [rsp + 2376], 0
                        mov              qword ptr [rsp + 2384], 0
                        mov              qword ptr [rsp + 2392], 0
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052624]                    # line
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n512_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n512_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [rsp + 2280], rbp                    # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rbp + 2288], r13                    # outer_Σ
                        mov              qword ptr [rbp + 2296], r14                    # outer_δ
                        mov              qword ptr [rbp + 2304], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2312], rax                    # cap_gen
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]                    # cas_top
                        mov              qword ptr [r10 + 0], 0
                        mov              qword ptr [r10 + 8], rsp                       # cas_rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [r10 + 16], rax                      # cas_patstk
                        add              r10, 24
                        mov              qword ptr [1879048192], r10                    # cas_top
                        mov              qword ptr [rbp + 2256], rsp                    # zls2_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2248], rax                    # patstk_mark
                        mov              dword ptr [rbp + 2240], 0                      # start_δ
.Lx739_0:
                        mov              r14d, dword ptr [rbp + 2240]
                                                                                        jmp   n513_match_sequence_α
n512_match_begin_β:
                        add              dword ptr [rbp + 2240], 1
                        mov              eax, dword ptr [rbp + 2240]
                        cmp              eax, r15d
                                                                                        jg    .Lx739_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx739_1
                                                                                        jmp   .Lx739_0
.Lx739_1:
                        mov              rax, qword ptr [rbp + 2248]                    # patstk_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2256]
                        mov              r10, qword ptr [1879048192]
.Lx739_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx739_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 2288]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 2296]                    # outer_δ
                        mov              r15, qword ptr [rbp + 2304]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 2312]                    # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 2184]                    # old_rbp
                        add              rsp, 2400
                                                                                        jmp   n524_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n513_match_sequence_α:
                        mov              dword ptr [rbp + 2416], r14d
                                                                                        jmp   n522_lit_integer_α
n513_match_sequence_as:
                                                                                        jmp   n514_match_end_α
n513_match_sequence_β:
                                                                                        jmp   n520_match_rpos_β
n513_match_sequence_af:
                                                                                        jmp   n512_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n514_match_end_α:
                        mov              rax, qword ptr [rbp + 2248]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2256]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx743_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx743_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx743_1:
                        test             rax, rax
                                                                                        je    .Lx743_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx743_3]
                        lea              rdx, [rip + .Lx743_4]
                                                                                        jmp   rax
.Lx743_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx743_1
.Lx743_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx743_1
.Lx743_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx743_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx743_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 2288]                    # outer_Σ
                        mov              r14, qword ptr [rbp + 2296]                    # outer_δ
                        mov              r15, qword ptr [rbp + 2304]                    # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 2312]                    # cap_gen
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n515_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n515_statement_α:
                        mov              rbp, qword ptr [rbp + 2280]                    # old_rbp
                        add              rsp, 2400
                                                                                        jmp   n516_call_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n516_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx747_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx747_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx747_6]
                        lea              rdx, [rip + .Lx747_7]
                                                                                        jmp   rax
.Lx747_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx747_2
.Lx747_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx747_2
.Lx747_5:
                        add              rsp, 16
.Lx747_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx747_240
                        add              rsp, 16
                                                                                        jmp   n508_var_α
.Lx747_240:
                                                                                        jmp   n517_assign_α
n516_call_β:
                                                                                        jmp   n508_var_α
.Lx747_0:
                        .quad            .Lx747_0_s
.Lx747_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n517_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx748_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n518_statement_α
.Lx748_0:
                        .quad            .Lx748_0_s
.Lx748_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n518_statement_α:
                        add              rsp, 16
                                                                                        jmp   n508_var_α
#-----------------------------------------------------------------------------------------------------------------------
n519_lit_integer_α:
                        mov              qword ptr [rsp + 2368], 3                      # result
                        mov              rax, qword ptr [rip + .Lx751_0]
                        mov              qword ptr [rsp + 2376], rax
                                                                                        jmp   n520_match_rpos_α
n519_lit_integer_β:
                                                                                        jmp   n521_match_patref_β
.Lx751_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n520_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n521_match_patref_β
                                                                                        jmp   n514_match_end_α
n520_match_rpos_β:
                                                                                        jmp   n521_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n521_match_patref_α:
                        mov              rax, qword ptr [1879052608]                    # expr
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 8
                                                                                        jne   .Lx753_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx753_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx753_10
.Lx753_9:
                        xor              eax, eax
.Lx753_10:
                        test             rax, rax
                                                                                        jz    .Lx753_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx753_4]
                        lea              rdx, [rip + .Lx753_5]
                                                                                        jmp   rax
.Lx753_4:
                                                                                        jmp   n519_lit_integer_α
.Lx753_5:
                                                                                        jmp   n512_match_begin_β
.Lx753_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx753_2:
                        test             rax, rax
                                                                                        je    .Lx753_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx753_7]
                        lea              rdx, [rip + .Lx753_8]
                                                                                        jmp   rax
.Lx753_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx753_2
.Lx753_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx753_2
.Lx753_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n512_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx753_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n519_lit_integer_α
.Lx753_6:
                        add              rsp, 16
                                                                                        jmp   n512_match_begin_β
n521_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n522_lit_integer_α:
                        mov              qword ptr [rsp + 2336], 3                      # result
                        mov              rax, qword ptr [rip + .Lx754_0]
                        mov              qword ptr [rsp + 2344], rax
                                                                                        jmp   n523_match_pos_α
n522_lit_integer_β:
                                                                                        jmp   n512_match_begin_β
.Lx754_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n523_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n512_match_begin_β
                                                                                        jmp   n521_match_patref_α
n523_match_pos_β:
                                                                                        jmp   n512_match_begin_β
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n524_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx756_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n525_assign_α
.Lx756_0:
                        .quad            .Lx756_0_s
.Lx756_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n525_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx757_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n526_statement_α
.Lx757_0:
                        .quad            .Lx757_0_s
.Lx757_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n526_statement_α:
                        add              rsp, 16
                                                                                        jmp   n508_var_α
#-----------------------------------------------------------------------------------------------------------------------
n527_goto_α:
                                                                                        jmp   n528_var_α
n527_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n528_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n529_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n529_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx762_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n530_subscript_α
.Lx762_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n530_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx763_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n539_var_α
.Lx763_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n531_var_α
#-----------------------------------------------------------------------------------------------------------------------
n531_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n532_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n532_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx765_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n533_subscript_α
.Lx765_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n533_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx766_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n539_var_α
.Lx766_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n534_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n534_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx767_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n539_var_α
.Lx767_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n535_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n535_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx768_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n536_binop_α
.Lx768_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n536_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx769_240
                        add              rsp, 16
                        add              rsp, 128
                                                                                        jmp   n539_var_α
.Lx769_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n537_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n537_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # subscript
                        mov              rsi, qword ptr [rsp + 120]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx770_240
                        add              rsp, 16
                        add              rsp, 144
                                                                                        jmp   n539_var_α
.Lx770_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n538_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n538_statement_α:
                        add              rsp, 160
                                                                                        jmp   n539_var_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n539_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n540_var_α
#-----------------------------------------------------------------------------------------------------------------------
n540_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n541_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n541_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx775_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n542_subscript_α
.Lx775_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n542_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx776_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n547_var_α
.Lx776_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n543_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n543_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx777_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n547_var_α
.Lx777_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n544_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n544_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx778_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n547_var_α
.Lx778_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n545_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n545_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # subscript
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # Push
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n546_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n546_statement_α:
                        add              rsp, 96
                                                                                        jmp   n547_var_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n547_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # Push
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n548_call_α
#-----------------------------------------------------------------------------------------------------------------------
n548_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd784:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd784]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx783_240
                        add              rsp, 16
                                                                                        jmp   n552_lit_string_α
.Lx783_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n549_var_α
n548_call_β:
                        add              rsp, 16
                                                                                        jmp   n552_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n549_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n550_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n550_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx786_240
                        add              rsp, 16
                                                                                        jmp   n552_lit_string_α
.Lx786_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n551_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n551_statement_α:
                                                                                        jmp   n552_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n552_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx789_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n553_call_α
.Lx789_0:
                        .quad            .Lx789_0_s
.Lx789_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n553_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd791:            .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd791]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx790_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n555_save_restore_α
.Lx790_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n554_save_restore_α
n553_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n555_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n554_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n555_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n556_goto_α:
                                                                                        jmp   n451_statement_α
n556_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n557_goto_α:
                                                                                        jmp   n558_var_α
n557_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n558_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n559_var_α
#-----------------------------------------------------------------------------------------------------------------------
n559_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n560_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n560_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx800_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n561_subscript_α
.Lx800_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n561_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx801_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n567_var_α
.Lx801_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n562_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n562_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx802_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n567_var_α
.Lx802_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n563_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n563_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx803_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n567_var_α
.Lx803_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n564_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n564_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx804_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n567_var_α
.Lx804_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n565_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n565_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # deref
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # Pop
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n566_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n566_statement_α:
                        add              rsp, 112
                                                                                        jmp   n567_var_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n567_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n568_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n568_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx809_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n569_subscript_α
.Lx809_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n569_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx810_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n554_save_restore_α
.Lx810_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n570_var_α
#-----------------------------------------------------------------------------------------------------------------------
n570_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]                    # stk
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n571_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n571_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx812_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n572_subscript_α
.Lx812_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n572_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx813_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n554_save_restore_α
.Lx813_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n573_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n573_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx814_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n554_save_restore_α
.Lx814_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n574_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n574_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx815_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n575_binop_α
.Lx815_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n575_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx816_240
                        add              rsp, 16
                        add              rsp, 128
                                                                                        jmp   n554_save_restore_α
.Lx816_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n576_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n576_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # subscript
                        mov              rsi, qword ptr [rsp + 120]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx817_240
                        add              rsp, 16
                        add              rsp, 144
                                                                                        jmp   n554_save_restore_α
.Lx817_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n577_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n577_statement_α:
                        add              rsp, 160
                                                                                        jmp   n554_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n578_goto_α:
                                                                                        jmp   n453_statement_α
n578_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n579_goto_α:
                                                                                        jmp   n580_call_α
n579_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n580_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx823_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx823_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx823_6]
                        lea              rdx, [rip + .Lx823_7]
                                                                                        jmp   rax
.Lx823_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx823_2
.Lx823_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx823_2
.Lx823_5:
                        add              rsp, 16
.Lx823_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx823_240
                        add              rsp, 16
                                                                                        jmp   n583_call_α
.Lx823_240:
                                                                                        jmp   n581_assign_α
n580_call_β:
                                                                                        jmp   n583_call_α
.Lx823_0:
                        .quad            .Lx823_0_s
.Lx823_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n581_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # arg
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n582_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n582_statement_α:
                        add              rsp, 16
                                                                                        jmp   n583_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n583_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx828_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx828_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx828_6]
                        lea              rdx, [rip + .Lx828_7]
                                                                                        jmp   rax
.Lx828_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx828_2
.Lx828_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx828_2
.Lx828_5:
                        add              rsp, 16
.Lx828_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx828_240
                        add              rsp, 16
                                                                                        jmp   n586_lit_string_α
.Lx828_240:
                                                                                        jmp   n584_assign_α
n583_call_β:
                                                                                        jmp   n586_lit_string_α
.Lx828_0:
                        .quad            .Lx828_0_s
.Lx828_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n584_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # op
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n585_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n585_statement_α:
                        add              rsp, 16
                                                                                        jmp   n586_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n586_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx832_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n587_call_α
.Lx832_0:
                        .quad            .Lx832_0_s
.Lx832_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n587_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd834:            .string          "SNO$WANTNM"
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
                        add              rsp, 16
                                                                                        jmp   n595_lit_string_α
.Lx833_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n588_call_α
n587_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n595_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n588_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx836_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx836_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx836_6]
                        lea              rdx, [rip + .Lx836_7]
                                                                                        jmp   rax
.Lx836_6:
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
                                                                                        jmp   .Lx836_2
.Lx836_7:
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
                                                                                        jmp   .Lx836_2
.Lx836_5:
                        add              rsp, 32
.Lx836_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx836_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n595_lit_string_α
.Lx836_240:
                                                                                        jmp   n589_var_α
n588_call_β:
                                                                                        jmp   n595_lit_string_α
.Lx836_0:
                        .quad            .Lx836_0_s
.Lx836_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n589_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n590_var_α
#-----------------------------------------------------------------------------------------------------------------------
n590_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # arg
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n591_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n591_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n592_call_α
#-----------------------------------------------------------------------------------------------------------------------
n592_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd841:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd841]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx840_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n595_lit_string_α
.Lx840_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n593_assign_var_α
n592_call_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n595_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n593_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # call
                        mov              rsi, qword ptr [rsp + 88]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx842_240
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n595_lit_string_α
.Lx842_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n594_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n594_statement_α:
                        add              rsp, 128
                                                                                        jmp   n595_lit_string_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n595_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx845_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n596_call_α
.Lx845_0:
                        .quad            .Lx845_0_s
.Lx845_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n596_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd847:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd847]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx846_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n552_lit_string_α
.Lx846_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n597_assign_α
n596_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n552_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n597_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # Unary
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n598_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n598_statement_α:
                        add              rsp, 32
                                                                                        jmp   n552_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n599_goto_α:
                                                                                        jmp   n455_statement_α
n599_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n600_goto_α:
                                                                                        jmp   n601_call_α
n600_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n601_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx854_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx854_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx854_6]
                        lea              rdx, [rip + .Lx854_7]
                                                                                        jmp   rax
.Lx854_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx854_2
.Lx854_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx854_2
.Lx854_5:
                        add              rsp, 16
.Lx854_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx854_240
                        add              rsp, 16
                                                                                        jmp   n604_call_α
.Lx854_240:
                                                                                        jmp   n602_assign_α
n601_call_β:
                                                                                        jmp   n604_call_α
.Lx854_0:
                        .quad            .Lx854_0_s
.Lx854_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n602_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052416], rax                    # right
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n603_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n603_statement_α:
                        add              rsp, 16
                                                                                        jmp   n604_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n604_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx859_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx859_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx859_6]
                        lea              rdx, [rip + .Lx859_7]
                                                                                        jmp   rax
.Lx859_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx859_2
.Lx859_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx859_2
.Lx859_5:
                        add              rsp, 16
.Lx859_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx859_240
                        add              rsp, 16
                                                                                        jmp   n607_call_α
.Lx859_240:
                                                                                        jmp   n605_assign_α
n604_call_β:
                                                                                        jmp   n607_call_α
.Lx859_0:
                        .quad            .Lx859_0_s
.Lx859_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n605_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # op
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n606_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n606_statement_α:
                        add              rsp, 16
                                                                                        jmp   n607_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n607_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx864_0]                # name
                        mov              esi, 0                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx864_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx864_6]
                        lea              rdx, [rip + .Lx864_7]
                                                                                        jmp   rax
.Lx864_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx864_2
.Lx864_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx864_2
.Lx864_5:
                        add              rsp, 16
.Lx864_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx864_240
                        add              rsp, 16
                                                                                        jmp   n610_lit_string_α
.Lx864_240:
                                                                                        jmp   n608_assign_α
n607_call_β:
                                                                                        jmp   n610_lit_string_α
.Lx864_0:
                        .quad            .Lx864_0_s
.Lx864_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n608_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052400], rax                    # left
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n609_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n609_statement_α:
                        add              rsp, 16
                                                                                        jmp   n610_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n610_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx868_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n611_call_α
.Lx868_0:
                        .quad            .Lx868_0_s
.Lx868_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n611_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd870:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd870]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx869_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n625_lit_string_α
.Lx869_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n612_call_α
n611_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n625_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n612_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx872_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 0                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx872_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx872_6]
                        lea              rdx, [rip + .Lx872_7]
                                                                                        jmp   rax
.Lx872_6:
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
                                                                                        jmp   .Lx872_2
.Lx872_7:
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
                                                                                        jmp   .Lx872_2
.Lx872_5:
                        add              rsp, 32
.Lx872_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx872_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n625_lit_string_α
.Lx872_240:
                                                                                        jmp   n613_var_α
n612_call_β:
                                                                                        jmp   n625_lit_string_α
.Lx872_0:
                        .quad            .Lx872_0_s
.Lx872_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n613_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]                    # left
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n614_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n614_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx874_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n615_binop_α
.Lx874_0:
                        .quad            .Lx874_0_s
.Lx874_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n615_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n616_var_α
#-----------------------------------------------------------------------------------------------------------------------
n616_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # op
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n617_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n617_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n618_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n618_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx878_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n619_binop_α
.Lx878_0:
                        .quad            .Lx878_0_s
.Lx878_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n619_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n620_var_α
#-----------------------------------------------------------------------------------------------------------------------
n620_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052416]                    # right
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n621_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n621_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n622_call_α
#-----------------------------------------------------------------------------------------------------------------------
n622_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd883:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd883]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx882_240
                        add              rsp, 16
                        add              rsp, 192
                                                                                        jmp   n625_lit_string_α
.Lx882_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n623_assign_var_α
n622_call_β:
                        add              rsp, 16
                        add              rsp, 192
                                                                                        jmp   n625_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n623_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 176]                     # call
                        mov              rsi, qword ptr [rsp + 184]                     # var
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx884_240
                        add              rsp, 16
                        add              rsp, 208
                                                                                        jmp   n625_lit_string_α
.Lx884_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n624_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n624_statement_α:
                        add              rsp, 224
                                                                                        jmp   n625_lit_string_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n625_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx887_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n626_call_α
.Lx887_0:
                        .quad            .Lx887_0_s
.Lx887_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n626_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd889:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd889]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx888_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n552_lit_string_α
.Lx888_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n627_assign_α
n626_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n552_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n627_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052384], rax                    # Binary
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n628_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n628_statement_α:
                        add              rsp, 32
                                                                                        jmp   n552_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n629_goto_α:
                                                                                        jmp   n457_statement_α
n629_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n630_goto_α:
                                                                                        jmp   n508_var_α
n630_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n631_goto_α:
                                                                                        jmp   n524_lit_string_α
n631_goto_β:
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
.S0:                    .string          "integer"
.S1:                    .string          "epsilon"
.S2:                    .string          "exponent"
.S3:                    .string          "*Push"
.S4:                    .string          "real"
.S5:                    .string          "expr"
.S6:                    .string          "constant"
.S7:                    .string          "primary"
.S8:                    .string          "factor"
.S9:                    .string          "*Unary"
.S10:                   .string          "addop"
.S11:                   .string          "term"
.S12:                   .string          "*Binary"
.S13:                   .string          "mulop"
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

                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "A"
.Lgvan1:                .string          "I"
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
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#         A = ARRAY(5)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n1_call_α
.Lx39_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n1_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd41:             .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd41]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx40_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n3_lit_integer_α
.Lx40_240:
                        mov              qword ptr [rsp + 0], rax                       # call
                        mov              qword ptr [rsp + 8], rdx                       # call
                                                                                        jmp   n2_assign_α
n1_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n3_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # call
                        mov              qword ptr [1879052288], rax                    # A
                        mov              qword ptr [1879052296], rdx                    # A
                        add              rsp, 32
                                                                                        jmp   n3_lit_integer_α
#=======================================================================================================================
#         I = 1
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n4_assign_α
.Lx43_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]                       # lit_integer
                        mov              qword ptr [1879052304], rax                    # I
                        mov              qword ptr [1879052312], rdx                    # I
                        add              rsp, 16
                                                                                        jmp   n5_var_α
#=======================================================================================================================
# FILL    A<I> = I * I
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # A
                        mov              rdx, qword ptr [1879052296]                    # A
                        mov              qword ptr [rsp + 0], rax                       # A
                        mov              qword ptr [rsp + 8], rdx                       # A
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # I
                        mov              rdx, qword ptr [1879052312]                    # I
                        mov              qword ptr [rsp + 0], rax                       # I
                        mov              qword ptr [rsp + 8], rdx                       # I
                                                                                        jmp   n7_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n7_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx47_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n12_var_α
.Lx47_240:
                        mov              qword ptr [rsp + 0], rax                       # subscript
                        mov              qword ptr [rsp + 8], rdx                       # subscript
                                                                                        jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # I
                        mov              rdx, qword ptr [1879052312]                    # I
                        mov              qword ptr [rsp + 0], rax                       # I
                        mov              qword ptr [rsp + 8], rdx                       # I
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # I
                        mov              rdx, qword ptr [1879052312]                    # I
                        mov              qword ptr [rsp + 0], rax                       # I
                        mov              qword ptr [rsp + 8], rdx                       # I
                                                                                        jmp   n10_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n10_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx50_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n12_var_α
.Lx50_240:
                        mov              qword ptr [rsp + 0], rax                       # binop
                        mov              qword ptr [rsp + 8], rdx                       # binop
                                                                                        jmp   n11_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 64]                      # subscript
                        mov              rsi, qword ptr [rsp + 72]                      # subscript
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx51_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n12_var_α
.Lx51_240:
                        mov              qword ptr [rsp + 0], rax                       # assign_var
                        mov              qword ptr [rsp + 8], rdx                       # assign_var
                        add              rsp, 112
                                                                                        jmp   n12_var_α
#=======================================================================================================================
#         I = I + 1
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # I
                        mov              rdx, qword ptr [1879052312]                    # I
                        mov              qword ptr [rsp + 0], rax                       # I
                        mov              qword ptr [rsp + 8], rdx                       # I
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n14_binop_α
.Lx53_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n14_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx54_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n16_var_α
.Lx54_240:
                        mov              qword ptr [rsp + 0], rax                       # binop
                        mov              qword ptr [rsp + 8], rdx                       # binop
                                                                                        jmp   n15_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # binop
                        mov              qword ptr [1879052304], rax                    # I
                        mov              qword ptr [1879052312], rdx                    # I
                        add              rsp, 48
                                                                                        jmp   n16_var_α
#=======================================================================================================================
#         LE(I, 5)                                                    :S(FILL)
#-----------------------------------------------------------------------------------------------------------------------
n16_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # I
                        mov              rdx, qword ptr [1879052312]                    # I
                        mov              qword ptr [rsp + 0], rax                       # I
                        mov              qword ptr [rsp + 8], rdx                       # I
                                                                                        jmp   n17_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx57_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n18_coerce_numeric_α
.Lx57_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n18_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 7
                                                                                        je    .Lx59_1
                        cmp              eax, 6
                                                                                        jne   .Lx59_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx59_0
.Lx59_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # coerce_numeric
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # coerce_numeric
                                                                                        jmp   n19_coerce_numeric_α
.Lx59_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # other
                        lea              rdx, [rsp + 0]                                 # coerce_numeric
                        mov              rcx, 118                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n19_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n19_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 7
                                                                                        je    .Lx61_1
                        cmp              eax, 6
                                                                                        jne   .Lx61_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 6
                                                                                        jne   .Lx61_0
.Lx61_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # coerce_numeric
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # coerce_numeric
                                                                                        jmp   n20_cmp_test_α
.Lx61_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # other
                        lea              rdx, [rsp + 0]                                 # coerce_numeric
                        mov              rcx, 119                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n20_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n20_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   .Lx63_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n21_lit_integer_α
.Lx63_240:
                        add              rsp, 80
                                                                                        jmp   n5_var_α
#=======================================================================================================================
#         I = 1
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n22_assign_α
.Lx64_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]                       # lit_integer
                        mov              qword ptr [1879052304], rax                    # I
                        mov              qword ptr [1879052312], rdx                    # I
                        add              rsp, 16
                                                                                        jmp   n23_var_α
#=======================================================================================================================
# READ    OUTPUT = A<I>
#-----------------------------------------------------------------------------------------------------------------------
n23_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # A
                        mov              rdx, qword ptr [1879052296]                    # A
                        mov              qword ptr [rsp + 0], rax                       # A
                        mov              qword ptr [rsp + 8], rdx                       # A
                                                                                        jmp   n24_var_α
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # I
                        mov              rdx, qword ptr [1879052312]                    # I
                        mov              qword ptr [rsp + 0], rax                       # I
                        mov              qword ptr [rsp + 8], rdx                       # I
                                                                                        jmp   n25_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n25_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx68_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n28_var_α
.Lx68_240:
                        mov              qword ptr [rsp + 0], rax                       # subscript
                        mov              qword ptr [rsp + 8], rdx                       # subscript
                                                                                        jmp   n26_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n26_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # subscript
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx69_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n28_var_α
.Lx69_240:
                        mov              qword ptr [rsp + 0], rax                       # deref
                        mov              qword ptr [rsp + 8], rdx                       # deref
                                                                                        jmp   n27_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # deref
                        mov              rdx, qword ptr [rsp + 8]                       # deref
                        mov              rdi, qword ptr [rip + .Lx70_0]                 # name
                        call             NV_SET_fn@PLT
                        add              rsp, 64
                                                                                        jmp   n28_var_α
.Lx70_0:
                        .quad            .Lx70_0_s
.Lx70_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         I = I + 1
#-----------------------------------------------------------------------------------------------------------------------
n28_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # I
                        mov              rdx, qword ptr [1879052312]                    # I
                        mov              qword ptr [rsp + 0], rax                       # I
                        mov              qword ptr [rsp + 8], rdx                       # I
                                                                                        jmp   n29_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx72_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n30_binop_α
.Lx72_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n30_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx73_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n32_var_α
.Lx73_240:
                        mov              qword ptr [rsp + 0], rax                       # binop
                        mov              qword ptr [rsp + 8], rdx                       # binop
                                                                                        jmp   n31_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # binop
                        mov              qword ptr [1879052304], rax                    # I
                        mov              qword ptr [1879052312], rdx                    # I
                        add              rsp, 48
                                                                                        jmp   n32_var_α
#=======================================================================================================================
#         LE(I, 5)                                                    :S(READ)
#-----------------------------------------------------------------------------------------------------------------------
n32_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # I
                        mov              rdx, qword ptr [1879052312]                    # I
                        mov              qword ptr [rsp + 0], rax                       # I
                        mov              qword ptr [rsp + 8], rdx                       # I
                                                                                        jmp   n33_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n34_coerce_numeric_α
.Lx76_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n34_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 7
                                                                                        je    .Lx78_1
                        cmp              eax, 6
                                                                                        jne   .Lx78_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx78_0
.Lx78_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # coerce_numeric
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # coerce_numeric
                                                                                        jmp   n35_coerce_numeric_α
.Lx78_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # other
                        lea              rdx, [rsp + 0]                                 # coerce_numeric
                        mov              rcx, 118                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n35_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n35_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 7
                                                                                        je    .Lx80_1
                        cmp              eax, 6
                                                                                        jne   .Lx80_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 6
                                                                                        jne   .Lx80_0
.Lx80_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # coerce_numeric
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # coerce_numeric
                                                                                        jmp   n36_cmp_test_α
.Lx80_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # other
                        lea              rdx, [rsp + 0]                                 # coerce_numeric
                        mov              rcx, 119                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n36_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n36_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   .Lx82_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   main_γ
.Lx82_240:
                        add              rsp, 80
                                                                                        jmp   n23_var_α
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_α:
                                                                                        jmp   n5_var_α
n37_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n38_goto_α:
                                                                                        jmp   n23_var_α
n38_goto_β:
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
                        .section         .note.GNU-stack,"",@progbits

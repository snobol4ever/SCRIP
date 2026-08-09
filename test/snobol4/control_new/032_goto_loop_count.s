                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "N"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 1
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 1
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         N = 0
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_lit_integer_α
n0_statement_begin_β:
                                                                                        jmp   n4_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx27_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_assign_α
.Lx27_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # N
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n3_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n4_statement_begin_α
#=======================================================================================================================
# LOOP    N = N + 1
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_begin_α:
                                                                                        jmp   n5_var_α
n4_statement_begin_β:
                                                                                        jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # N
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n7_binop_α
n6_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n4_statement_begin_β
.Lx34_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n7_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx35_240
                        add              rsp, 16
                                                                                        jmp   n6_lit_integer_β
.Lx35_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n8_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # N
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n10_statement_begin_α
#=======================================================================================================================
#         GT(N, 5)                                                    :S(DONE)
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:
                                                                                        jmp   n11_var_α
n10_statement_begin_β:
                                                                                        jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # N
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n12_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n13_coerce_numeric_α
n12_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n10_statement_begin_β
.Lx42_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n13_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx44_1
                        cmp              eax, 3
                                                                                        jne   .Lx44_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx44_0
.Lx44_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n14_coerce_numeric_α
.Lx44_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n14_coerce_numeric_α
n13_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n12_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n14_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx46_1
                        cmp              eax, 3
                                                                                        jne   .Lx46_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx46_0
.Lx46_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n15_cmp_test_α
.Lx46_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n15_cmp_test_α
n14_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n13_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n15_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx48_240
                        add              rsp, 16
                                                                                        jmp   n14_coerce_numeric_β
.Lx48_240:
                                                                                        jmp   n16_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n17_statement_begin_α
#=======================================================================================================================
# DONE    OUTPUT = N
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:
                                                                                        jmp   n18_var_α
n17_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # N
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n19_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx54_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n20_statement_end_α
.Lx54_0:
                        .quad            .Lx54_0_s
.Lx54_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         :(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_begin_α:
                                                                                        jmp   n22_statement_end_α
n21_statement_begin_β:
                                                                                        jmp   n4_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:
                                                                                        jmp   n4_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_goto_α:
                                                                                        jmp   n4_statement_begin_α
n23_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n24_goto_α:
                                                                                        jmp   n17_statement_begin_α
n24_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              edi, 1
                        call             exit@PLT
                        .section         .note.GNU-stack,"",@progbits

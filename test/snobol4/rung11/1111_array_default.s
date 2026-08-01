                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "b"
.Lgvan1:                .string          "output"
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
#         b = array(3, 10)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx25_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n1_lit_integer_α
.Lx25_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx26_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n2_call_α
.Lx26_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n2_call_α:
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
.Lbynamefnzd3:          .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd3]                     # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 99
                                                                                        jne   .Lx27_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n4_var_α
.Lx27_240:
                        mov              qword ptr [rsp + 0], rax                       # call
                        mov              qword ptr [rsp + 8], rdx                       # call
                                                                                        jmp   n3_assign_α
n2_call_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n4_var_α
#-----------------------------------------------------------------------------------------------------------------------
n3_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # call
                        mov              qword ptr [1879052288], rax                    # b
                        mov              qword ptr [1879052296], rdx                    # b
                        add              rsp, 48
                                                                                        jmp   n4_var_α
#=======================================================================================================================
#         b = array(3, 10)
#         differ(b<2>, 10)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # b
                        mov              rdx, qword ptr [1879052296]                    # b
                        mov              qword ptr [rsp + 0], rax                       # b
                        mov              qword ptr [rsp + 8], rdx                       # b
                                                                                        jmp   n5_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx30_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n6_subscript_α
.Lx30_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n6_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx31_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n12_var_α
.Lx31_240:
                        mov              qword ptr [rsp + 0], rax                       # subscript
                        mov              qword ptr [rsp + 8], rdx                       # subscript
                                                                                        jmp   n7_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n7_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # subscript
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx32_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n12_var_α
.Lx32_240:
                        mov              qword ptr [rsp + 0], rax                       # deref
                        mov              qword ptr [rsp + 8], rdx                       # deref
                                                                                        jmp   n8_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n9_call_α
.Lx33_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:
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
.Lbynamefnzd10:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd10]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 99
                                                                                        jne   .Lx34_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n12_var_α
.Lx34_240:
                        mov              qword ptr [rsp + 0], rax                       # call
                        mov              qword ptr [rsp + 8], rdx                       # call
                        add              rsp, 96
                                                                                        jmp   n10_lit_string_α
n9_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n12_var_α
#=======================================================================================================================
#         output = 'FAIL 1111/001: array default value'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1                         # lit_string
                        mov              dword ptr [rsp + 4], 34                        # lit_string
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_string
                                                                                        jmp   n11_assign_α
.Lx35_0:
                        .quad            .Lx35_0_s
.Lx35_0_s:
                        .string          "FAIL 1111/001: array default value"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # lit_string
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx                    # output
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(b<1>, 10)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # b
                        mov              rdx, qword ptr [1879052296]                    # b
                        mov              qword ptr [rsp + 0], rax                       # b
                        mov              qword ptr [rsp + 8], rdx                       # b
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n14_subscript_α
.Lx38_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n14_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx39_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n20_lit_string_α
.Lx39_240:
                        mov              qword ptr [rsp + 0], rax                       # subscript
                        mov              qword ptr [rsp + 8], rdx                       # subscript
                                                                                        jmp   n15_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n15_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # subscript
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx40_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n20_lit_string_α
.Lx40_240:
                        mov              qword ptr [rsp + 0], rax                       # deref
                        mov              qword ptr [rsp + 8], rdx                       # deref
                                                                                        jmp   n16_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx41_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n17_call_α
.Lx41_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n17_call_α:
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
.Lbynamefnzd18:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd18]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 99
                                                                                        jne   .Lx42_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n20_lit_string_α
.Lx42_240:
                        mov              qword ptr [rsp + 0], rax                       # call
                        mov              qword ptr [rsp + 8], rdx                       # call
                        add              rsp, 96
                                                                                        jmp   n18_lit_string_α
n17_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n20_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1111/002: default fills all slots' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1                         # lit_string
                        mov              dword ptr [rsp + 4], 38                        # lit_string
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_string
                                                                                        jmp   n19_assign_α
.Lx43_0:
                        .quad            .Lx43_0_s
.Lx43_0_s:
                        .string          "FAIL 1111/002: default fills all slots"
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # lit_string
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx                    # output
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'PASS 1111_array_default (2/2)'
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1                         # lit_string
                        mov              dword ptr [rsp + 4], 29                        # lit_string
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_string
                                                                                        jmp   n21_assign_α
.Lx45_0:
                        .quad            .Lx45_0_s
.Lx45_0_s:
                        .string          "PASS 1111_array_default (2/2)"
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # lit_string
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx                    # output
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n22_goto_α:
                                                                                        jmp   n12_var_α
n22_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n23_goto_α:
                                                                                        jmp   n20_lit_string_α
n23_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n24_goto_α:
                                                                                        jmp   main_γ
n24_goto_β:
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

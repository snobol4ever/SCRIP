                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "a"
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
#         a = array(3)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n1_call_α
.Lx20_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n1_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lbynamefnzd2:          .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd2]                     # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx21_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n3_var_α
.Lx21_240:
                        mov              qword ptr [rsp + 0], rax                       # call
                        mov              qword ptr [rsp + 8], rdx                       # call
                                                                                        jmp   n2_assign_α
n1_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # call
                        mov              qword ptr [1879052288], rax                    # a
                        mov              qword ptr [1879052296], rdx                    # a
                        add              rsp, 32
                                                                                        jmp   n3_var_α
#=======================================================================================================================
#         a<2> = 'x'
#-----------------------------------------------------------------------------------------------------------------------
n3_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # a
                        mov              rdx, qword ptr [1879052296]                    # a
                        mov              qword ptr [rsp + 0], rax                       # a
                        mov              qword ptr [rsp + 8], rdx                       # a
                                                                                        jmp   n4_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n5_subscript_α
.Lx24_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n5_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx25_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n8_var_α
.Lx25_240:
                        mov              qword ptr [rsp + 0], rax                       # subscript
                        mov              qword ptr [rsp + 8], rdx                       # subscript
                                                                                        jmp   n6_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1                         # lit_string
                        mov              dword ptr [rsp + 4], 1                         # lit_string
                        mov              rax, qword ptr [rip + .Lx26_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_string
                                                                                        jmp   n7_assign_var_α
.Lx26_0:
                        .quad            .Lx26_0_s
.Lx26_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n7_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # subscript
                        mov              rdx, qword ptr [rsp + 16]                      # val
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx27_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n8_var_α
.Lx27_240:
                        mov              qword ptr [rsp + 0], rax                       # assign_var
                        mov              qword ptr [rsp + 8], rdx                       # assign_var
                        add              rsp, 80
                                                                                        jmp   n8_var_α
#=======================================================================================================================
#         a<2> = 'x'
#         differ($.a<2>, 'x')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # a
                        mov              rdx, qword ptr [1879052296]                    # a
                        mov              qword ptr [rsp + 0], rax                       # a
                        mov              qword ptr [rsp + 8], rdx                       # a
                                                                                        jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n10_subscript_α
.Lx29_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n10_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx30_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n16_lit_string_α
.Lx30_240:
                        mov              qword ptr [rsp + 0], rax                       # subscript
                        mov              qword ptr [rsp + 8], rdx                       # subscript
                                                                                        jmp   n11_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n11_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # subscript
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx31_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n16_lit_string_α
.Lx31_240:
                        mov              qword ptr [rsp + 0], rax                       # deref
                        mov              qword ptr [rsp + 8], rdx                       # deref
                                                                                        jmp   n12_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1                         # lit_string
                        mov              dword ptr [rsp + 4], 1                         # lit_string
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_string
                                                                                        jmp   n13_call_α
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:
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
.Lbynamefnzd14:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd14]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 99
                                                                                        jne   .Lx33_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n16_lit_string_α
.Lx33_240:
                        mov              qword ptr [rsp + 0], rax                       # call
                        mov              qword ptr [rsp + 8], rdx                       # call
                        add              rsp, 96
                                                                                        jmp   n14_lit_string_α
n13_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n16_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 212/001: $.var<index> indirect array' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1                         # lit_string
                        mov              dword ptr [rsp + 4], 41                        # lit_string
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_string
                                                                                        jmp   n15_assign_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "FAIL 212/001: $.var<index> indirect array"
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # lit_string
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx                    # output
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'PASS 212_indirect_array (1/1)'
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1                         # lit_string
                        mov              dword ptr [rsp + 4], 29                        # lit_string
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_string
                                                                                        jmp   n17_assign_α
.Lx36_0:
                        .quad            .Lx36_0_s
.Lx36_0_s:
                        .string          "PASS 212_indirect_array (1/1)"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # lit_string
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx                    # output
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n18_goto_α:
                                                                                        jmp   n16_lit_string_α
n18_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n19_goto_α:
                                                                                        jmp   main_γ
n19_goto_β:
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

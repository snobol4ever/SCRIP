                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "qq"
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
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:
                                                                                        jmp   n2_statement_begin_α
#=======================================================================================================================
#         $'qq' = 'x'
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_call_α
.Lx46_0:
                        .quad            .Lx46_0_s
.Lx46_0_s:
                        .string          "qq"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd48:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd48]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx47_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n8_statement_begin_α
.Lx47_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n5_lit_string_α
n4_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n6_assign_var_α
.Lx49_0:
                        .quad            .Lx49_0_s
.Lx49_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx50_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n8_statement_begin_α
.Lx50_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n7_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n8_statement_begin_α
#=======================================================================================================================
#         differ(qq, 'x')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # qq
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx56_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n11_call_α
.Lx56_0:
                        .quad            .Lx56_0_s
.Lx56_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n11_call_α:
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
.Lbynamefnzd12:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd12]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx57_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n17_statement_begin_α
.Lx57_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n12_statement_end_α
n11_call_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n13_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 211/001: indirect assign sets named var' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:
                                                                                        jmp   n14_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 44
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n15_assign_α
.Lx62_0:
                        .quad            .Lx62_0_s
.Lx62_0_s:
                        .string          "FAIL 211/001: indirect assign sets named var"
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n16_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:
                                                                                        jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:
                                                                                        jmp   n18_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:
                                                                                        jmp   n19_statement_begin_α
#=======================================================================================================================
#         differ($'_no_such_var_')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_begin_α:
                                                                                        jmp   n20_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx72_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n21_call_α
.Lx72_0:
                        .quad            .Lx72_0_s
.Lx72_0_s:
                        .string          "_no_such_var_"
#-----------------------------------------------------------------------------------------------------------------------
n21_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd74:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd74]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx73_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n29_statement_begin_α
.Lx73_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n22_deref_α
n21_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n22_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # call
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx75_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n29_statement_begin_α
.Lx75_240:
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
.Lbynamefnzd24:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd24]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx76_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n29_statement_begin_α
.Lx76_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n24_statement_end_α
n23_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n25_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 211/002: undefined indirect is null'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_begin_α:
                                                                                        jmp   n26_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 40
                        mov              rax, qword ptr [rip + .Lx81_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n27_assign_α
.Lx81_0:
                        .quad            .Lx81_0_s
.Lx81_0_s:
                        .string          "FAIL 211/002: undefined indirect is null"
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:
                                                                                        jmp   n30_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:
                                                                                        jmp   n31_statement_begin_α
#=======================================================================================================================
#         output = 'PASS 211_indirect_assign (2/2)'
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:
                                                                                        jmp   n32_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n33_assign_α
.Lx91_0:
                        .quad            .Lx91_0_s
.Lx91_0_s:
                        .string          "PASS 211_indirect_assign (2/2)"
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n34_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:
                                                                                        jmp   n36_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_α:
                                                                                        jmp   n17_statement_begin_α
n37_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n38_goto_α:
                                                                                        jmp   n29_statement_begin_α
n38_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n39_goto_α:
                                                                                        jmp   n35_statement_begin_α
n39_goto_β:
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
                        .section         .note.GNU-stack,"",@progbits

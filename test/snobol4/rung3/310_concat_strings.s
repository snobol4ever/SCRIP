                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "output"
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
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:
                                                                                        jmp   n2_statement_begin_α
#=======================================================================================================================
#         differ('a' 'b', 'ab')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_lit_string_α
.Lx61_0:
                        .quad            .Lx61_0_s
.Lx61_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_binop_α
.Lx62_0:
                        .quad            .Lx62_0_s
.Lx62_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n5_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n7_call_α
.Lx64_0:
                        .quad            .Lx64_0_s
.Lx64_0_s:
                        .string          "ab"
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:
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
.Lbynamefnzd8:          .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd8]                     # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx65_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n13_statement_begin_α
.Lx65_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n8_statement_end_α
n7_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n9_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 310/001: two-string concat'      :(end)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n11_assign_α
.Lx70_0:
                        .quad            .Lx70_0_s
.Lx70_0_s:
                        .string          "FAIL 310/001: two-string concat"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n12_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:
                                                                                        jmp   n14_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:
                                                                                        jmp   n15_statement_begin_α
#=======================================================================================================================
#         differ('a' 'b' 'c', 'abc')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:
                                                                                        jmp   n16_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n17_lit_string_α
.Lx80_0:
                        .quad            .Lx80_0_s
.Lx80_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx81_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_binop_α
.Lx81_0:
                        .quad            .Lx81_0_s
.Lx81_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n19_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx83_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n20_binop_α
.Lx83_0:
                        .quad            .Lx83_0_s
.Lx83_0_s:
                        .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n20_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n22_call_α
.Lx85_0:
                        .quad            .Lx85_0_s
.Lx85_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:
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
.Lbynamefnzd23:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd23]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx86_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n28_statement_begin_α
.Lx86_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n23_statement_end_α
n22_call_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:
                        add              rsp, 112
                                                                                        jmp   n24_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 310/002: three-string concat'    :(end)
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:
                                                                                        jmp   n25_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n26_assign_α
.Lx91_0:
                        .quad            .Lx91_0_s
.Lx91_0_s:
                        .string          "FAIL 310/002: three-string concat"
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n27_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:
                                                                                        jmp   n29_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:
                                                                                        jmp   n30_statement_begin_α
#=======================================================================================================================
#         differ(('hello' ' ') 'world', 'hello world')                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:
                                                                                        jmp   n31_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n32_lit_string_α
.Lx101_0:
                        .quad            .Lx101_0_s
.Lx101_0_s:
                        .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx102_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n33_binop_α
.Lx102_0:
                        .quad            .Lx102_0_s
.Lx102_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n33_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n34_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx104_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n35_binop_α
.Lx104_0:
                        .quad            .Lx104_0_s
.Lx104_0_s:
                        .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n35_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n36_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx106_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n37_call_α
.Lx106_0:
                        .quad            .Lx106_0_s
.Lx106_0_s:
                        .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n37_call_α:
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
.Lbynamefnzd38:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd38]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx107_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n43_statement_begin_α
.Lx107_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n38_statement_end_α
n37_call_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:
                        add              rsp, 112
                                                                                        jmp   n39_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 310/003: left-associative concat' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:
                                                                                        jmp   n40_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx112_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n41_assign_α
.Lx112_0:
                        .quad            .Lx112_0_s
.Lx112_0_s:
                        .string          "FAIL 310/003: left-associative concat"
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n42_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:
                                                                                        jmp   n44_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:
                                                                                        jmp   n45_statement_begin_α
#=======================================================================================================================
#         output = 'PASS 310_concat_strings (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:
                                                                                        jmp   n46_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 29
                        mov              rax, qword ptr [rip + .Lx122_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n47_assign_α
.Lx122_0:
                        .quad            .Lx122_0_s
.Lx122_0_s:
                        .string          "PASS 310_concat_strings (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n48_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:
                                                                                        jmp   n50_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n51_goto_α:
                                                                                        jmp   n13_statement_begin_α
n51_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n52_goto_α:
                                                                                        jmp   n28_statement_begin_α
n52_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n53_goto_α:
                                                                                        jmp   n43_statement_begin_α
n53_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n54_goto_α:
                                                                                        jmp   n49_statement_begin_α
n54_goto_β:
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

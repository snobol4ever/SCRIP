                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "a"
.Lgvan1:                .string          "output"
.Lgvan2:                .string          "b"
.Lgvan3:                .string          "d"
.Lgvan4:                .string          "lfunc"
.Lgvan5:                .string          "x"
.Lgvan6:                .string          "checklocal"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 7
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 7
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
n0_statement_begin_β:
                                                                                        jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:
                                                                                        jmp   n2_statement_begin_α
#=======================================================================================================================
#         define('lfunc(a,b,c)d,e,f')               :(lfunc_end)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_lit_string_α
n2_statement_begin_β:
                                                                                        jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_call_α
.Lx169_0:
                        .quad            .Lx169_0_s
.Lx169_0_s:
                        .string          "lfunc(a,b,c)d,e,f"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lbynamefnzd5:          .string          "define"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd5]                     # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx170_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n2_statement_begin_β
.Lx170_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n5_statement_end_α
n4_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:
                                                                                        jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:
                                                                                        jmp   n7_statement_end_α
n6_statement_begin_β:
                        add              rsp, 32
                                                                                        jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n8_statement_begin_α
#=======================================================================================================================
#         a = 'global_a'
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:
                                                                                        jmp   n9_lit_string_α
n8_statement_begin_β:
                                                                                        jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n10_assign_α
.Lx179_0:
                        .quad            .Lx179_0_s
.Lx179_0_s:
                        .string          "global_a"
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # a
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n11_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n12_statement_begin_α
#=======================================================================================================================
#         d = 'global_d'
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:
                                                                                        jmp   n13_lit_string_α
n12_statement_begin_β:
                                                                                        jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_assign_α
.Lx185_0:
                        .quad            .Lx185_0_s
.Lx185_0_s:
                        .string          "global_d"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # d
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n15_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n16_statement_begin_α
#=======================================================================================================================
#         d = 'global_d'
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:
                                                                                        jmp   n17_statement_end_α
n16_statement_begin_β:
                                                                                        jmp   n18_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:
                                                                                        jmp   n18_statement_begin_α
#=======================================================================================================================
#         differ(lfunc('p', 'q', 'r'), 'aabbdd')                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:
                                                                                        jmp   n19_lit_string_α
n18_statement_begin_β:
                                                                                        jmp   n30_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n20_lit_string_α
.Lx195_0:
                        .quad            .Lx195_0_s
.Lx195_0_s:
                        .string          "p"
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx196_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n21_lit_string_α
n20_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n18_statement_begin_β
.Lx196_0:
                        .quad            .Lx196_0_s
.Lx196_0_s:
                        .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n22_call_α
n21_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n20_lit_string_β
.Lx197_0:
                        .quad            .Lx197_0_s
.Lx197_0_s:
                        .string          "r"
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:
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
.Lbynamefnzd23:         .string          "lfunc"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd23]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 3                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 48
                        cmp              eax, 104
                                                                                        jne   .Lx198_240
                        add              rsp, 16
                                                                                        jmp   n21_lit_string_β
.Lx198_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n23_lit_string_α
n22_call_β:
                        add              rsp, 16
                                                                                        jmp   n21_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n24_call_α
n23_lit_string_β:
                        add              rsp, 32
                                                                                        jmp   n21_lit_string_β
.Lx199_0:
                        .quad            .Lx199_0_s
.Lx199_0_s:
                        .string          "aabbdd"
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:
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
.Lbynamefnzd25:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd25]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx200_240
                        add              rsp, 16
                                                                                        jmp   n23_lit_string_β
.Lx200_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n25_statement_end_α
n24_call_β:
                        add              rsp, 16
                                                                                        jmp   n23_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n26_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1012/004: lfunc return value'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:
                                                                                        jmp   n27_lit_string_α
n26_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_assign_α
.Lx205_0:
                        .quad            .Lx205_0_s
.Lx205_0_s:
                        .string          "FAIL 1012/004: lfunc return value"
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n29_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:
                                                                                        jmp   n31_statement_end_α
n30_statement_begin_β:
                                                                                        jmp   n32_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:
                                                                                        jmp   n32_statement_begin_α
#=======================================================================================================================
#         ident(a, 'global_a')                           :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:
                                                                                        jmp   n33_var_α
n32_statement_begin_β:
                                                                                        jmp   n70_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # a
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n34_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx216_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n35_call_α
n34_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n32_statement_begin_β
.Lx216_0:
                        .quad            .Lx216_0_s
.Lx216_0_s:
                        .string          "global_a"
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:
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
.Lbynamefnzd36:         .string          "ident"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd36]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx217_240
                        add              rsp, 16
                                                                                        jmp   n34_lit_string_β
.Lx217_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n36_statement_end_α
n35_call_β:
                        add              rsp, 16
                                                                                        jmp   n34_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:
                                                                                        jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:
                                                                                        jmp   n38_statement_end_α
n37_statement_begin_β:
                        add              rsp, 48
                                                                                        jmp   n39_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n39_statement_begin_α
#=======================================================================================================================
#         ident(d, 'global_d')                           :s(e006)
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:
                                                                                        jmp   n40_var_α
n39_statement_begin_β:
                                                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # d
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n41_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx227_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n42_call_α
n41_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n39_statement_begin_β
.Lx227_0:
                        .quad            .Lx227_0_s
.Lx227_0_s:
                        .string          "global_d"
#-----------------------------------------------------------------------------------------------------------------------
n42_call_α:
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
.Lbynamefnzd43:         .string          "ident"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd43]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx228_240
                        add              rsp, 16
                                                                                        jmp   n41_lit_string_β
.Lx228_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n43_statement_end_α
n42_call_β:
                        add              rsp, 16
                                                                                        jmp   n41_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:
                                                                                        jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:
                                                                                        jmp   n45_statement_end_α
n44_statement_begin_β:
                        add              rsp, 48
                                                                                        jmp   n46_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n46_statement_begin_α
#=======================================================================================================================
#         define('checklocal()x')                        :(cl_end)
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:
                                                                                        jmp   n47_lit_string_α
n46_statement_begin_β:
                                                                                        jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx237_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n48_call_α
.Lx237_0:
                        .quad            .Lx237_0_s
.Lx237_0_s:
                        .string          "checklocal()x"
#-----------------------------------------------------------------------------------------------------------------------
n48_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lbynamefnzd49:         .string          "define"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd49]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx238_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n46_statement_begin_β
.Lx238_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n49_statement_end_α
n48_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n46_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_end_α:
                                                                                        jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_begin_α:
                                                                                        jmp   n51_statement_end_α
n50_statement_begin_β:
                        add              rsp, 32
                                                                                        jmp   n52_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n52_statement_begin_α
#=======================================================================================================================
#         differ(checklocal())       :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:
                                                                                        jmp   n53_call_α
n52_statement_begin_β:
                                                                                        jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:
                        sub              rsp, 16
                        .section         .rodata
.Lbynamefnzd54:         .string          "checklocal"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd54]                    # fn
                        xor              esi, esi                                       # args
                        mov              edx, 0                                         # nargs
                        call             rt_call_arr@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx247_240
                        add              rsp, 16
                                                                                        jmp   n52_statement_begin_β
.Lx247_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n54_call_α
n53_call_β:
                        add              rsp, 16
                                                                                        jmp   n52_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n54_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lbynamefnzd55:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd55]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx248_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n52_statement_begin_β
.Lx248_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n55_statement_end_α
n54_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n52_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n56_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1012/007: local null on fresh call' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_begin_α:
                                                                                        jmp   n57_lit_string_α
n56_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n58_assign_α
.Lx253_0:
                        .quad            .Lx253_0_s
.Lx253_0_s:
                        .string          "FAIL 1012/007: local null on fresh call"
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n59_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:
                                                                                        jmp   n61_statement_end_α
n60_statement_begin_β:
                                                                                        jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:
                                                                                        jmp   n62_statement_begin_α
#=======================================================================================================================
#         output = 'PASS 1012_func_locals (7/7)'
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_begin_α:
                                                                                        jmp   n63_lit_string_α
n62_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx263_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n64_assign_α
.Lx263_0:
                        .quad            .Lx263_0_s
.Lx263_0_s:
                        .string          "PASS 1012_func_locals (7/7)"
#-----------------------------------------------------------------------------------------------------------------------
n64_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n65_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1012/006: global d not clobbered' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:
                                                                                        jmp   n67_lit_string_α
n66_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n68_assign_α
.Lx269_0:
                        .quad            .Lx269_0_s
.Lx269_0_s:
                        .string          "FAIL 1012/006: global d not clobbered"
#-----------------------------------------------------------------------------------------------------------------------
n68_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n69_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1012/005: global a not clobbered' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:
                                                                                        jmp   n71_lit_string_α
n70_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx275_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n72_assign_α
.Lx275_0:
                        .quad            .Lx275_0_s
.Lx275_0_s:
                        .string          "FAIL 1012/005: global a not clobbered"
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n73_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:
                                                                                        jmp   n75_statement_end_α
n74_statement_begin_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n76_goto_α:
                                                                                        jmp   n77_statement_begin_α
n76_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_begin_α:
                                                                                        jmp   n78_statement_end_α
n77_statement_begin_β:
                                                                                        jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:
                                                                                        jmp   n79_statement_begin_α
#=======================================================================================================================
#         ident(a, 'p')              :s(e001)
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:
                                                                                        jmp   n80_var_α
n79_statement_begin_β:
                                                                                        jmp   n127_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # a
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n81_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx291_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n82_call_α
n81_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n79_statement_begin_β
.Lx291_0:
                        .quad            .Lx291_0_s
.Lx291_0_s:
                        .string          "p"
#-----------------------------------------------------------------------------------------------------------------------
n82_call_α:
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
.Lbynamefnzd83:         .string          "ident"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd83]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx292_240
                        add              rsp, 16
                                                                                        jmp   n81_lit_string_β
.Lx292_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n83_statement_end_α
n82_call_β:
                        add              rsp, 16
                                                                                        jmp   n81_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n84_statement_begin_α
#=======================================================================================================================
# e001    ident(b, 'q')              :s(e002)
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_begin_α:
                                                                                        jmp   n85_var_α
n84_statement_begin_β:
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n85_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # b
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n86_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx298_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n87_call_α
n86_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n84_statement_begin_β
.Lx298_0:
                        .quad            .Lx298_0_s
.Lx298_0_s:
                        .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n87_call_α:
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
.Lbynamefnzd88:         .string          "ident"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd88]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx299_240
                        add              rsp, 16
                                                                                        jmp   n86_lit_string_β
.Lx299_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n88_statement_end_α
n87_call_β:
                        add              rsp, 16
                                                                                        jmp   n86_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:
                                                                                        jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:
                                                                                        jmp   n90_statement_end_α
n89_statement_begin_β:
                        add              rsp, 48
                                                                                        jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n91_statement_begin_α
#=======================================================================================================================
#         differ(d)                  :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:
                                                                                        jmp   n92_var_α
n91_statement_begin_β:
                                                                                        jmp   n99_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # d
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n93_call_α
#-----------------------------------------------------------------------------------------------------------------------
n93_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lbynamefnzd94:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd94]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx309_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n91_statement_begin_β
.Lx309_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n94_statement_end_α
n93_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n91_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n95_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1012/003: local d should be null' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_begin_α:
                                                                                        jmp   n96_lit_string_α
n95_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx314_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n97_assign_α
.Lx314_0:
                        .quad            .Lx314_0_s
.Lx314_0_s:
                        .string          "FAIL 1012/003: local d should be null"
#-----------------------------------------------------------------------------------------------------------------------
n97_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n98_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_begin_α:
                                                                                        jmp   n100_statement_end_α
n99_statement_begin_β:
                                                                                        jmp   n101_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_end_α:
                                                                                        jmp   n101_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_begin_α:
                                                                                        jmp   n102_lit_string_α
n101_statement_begin_β:
                                                                                        jmp   n105_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n102_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx324_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n103_assign_α
.Lx324_0:
                        .quad            .Lx324_0_s
.Lx324_0_s:
                        .string          "aa"
#-----------------------------------------------------------------------------------------------------------------------
n103_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # a
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n104_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n105_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_begin_α:
                                                                                        jmp   n106_lit_string_α
n105_statement_begin_β:
                                                                                        jmp   n109_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx330_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n107_assign_α
.Lx330_0:
                        .quad            .Lx330_0_s
.Lx330_0_s:
                        .string          "bb"
#-----------------------------------------------------------------------------------------------------------------------
n107_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # b
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n108_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n109_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_begin_α:
                                                                                        jmp   n110_lit_string_α
n109_statement_begin_β:
                                                                                        jmp   n113_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx336_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n111_assign_α
.Lx336_0:
                        .quad            .Lx336_0_s
.Lx336_0_s:
                        .string          "dd"
#-----------------------------------------------------------------------------------------------------------------------
n111_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # d
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n112_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n113_statement_begin_α
#=======================================================================================================================
#         lfunc = a b d                                  :(return)
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_begin_α:
                                                                                        jmp   n114_var_α
n113_statement_begin_β:
                                                                                        jmp   n122_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n114_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # a
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n115_var_α
#-----------------------------------------------------------------------------------------------------------------------
n115_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # b
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n116_binop_α
n115_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n113_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n116_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n117_var_α
n116_binop_β:
                        add              rsp, 16
                                                                                        jmp   n115_var_β
#-----------------------------------------------------------------------------------------------------------------------
n117_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # d
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n118_binop_α
n117_var_β:
                        add              rsp, 16
                                                                                        jmp   n116_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n118_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n119_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n119_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # lfunc
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n120_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_end_α:
                                                                                        jmp   n121_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n121_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx351_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx351_1
.Lx351_0:
                        .quad            .Lx351_0_s
.Lx351_0_s:
                        .string          "return"
.Lx351_1:
                        add              rsp, 80
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n122_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx353_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx353_1
.Lx353_0:
                        .quad            .Lx353_0_s
.Lx353_0_s:
                        .string          "return"
.Lx353_1:
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 1012/002: arg b should be q'    :(end)
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_begin_α:
                                                                                        jmp   n124_lit_string_α
n123_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n124_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx356_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n125_assign_α
.Lx356_0:
                        .quad            .Lx356_0_s
.Lx356_0_s:
                        .string          "FAIL 1012/002: arg b should be q"
#-----------------------------------------------------------------------------------------------------------------------
n125_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n126_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1012/001: arg a should be p'    :(end)
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_begin_α:
                                                                                        jmp   n128_lit_string_α
n127_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n128_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx362_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n129_assign_α
.Lx362_0:
                        .quad            .Lx362_0_s
.Lx362_0_s:
                        .string          "FAIL 1012/001: arg a should be p"
#-----------------------------------------------------------------------------------------------------------------------
n129_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n130_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n131_goto_α:
                                                                                        jmp   n84_statement_begin_α
n131_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n132_goto_α:
                                                                                        jmp   n89_statement_begin_α
n132_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n133_goto_α:
                                                                                        jmp   n99_statement_begin_α
n133_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n134_goto_α:
                                                                                        jmp   n6_statement_begin_α
n134_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n135_goto_α:
                                                                                        jmp   n30_statement_begin_α
n135_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n136_goto_α:
                                                                                        jmp   n37_statement_begin_α
n136_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n137_goto_α:
                                                                                        jmp   n44_statement_begin_α
n137_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n138_goto_α:
                                                                                        jmp   n139_statement_begin_α
n138_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_begin_α:
                                                                                        jmp   n140_statement_end_α
n139_statement_begin_β:
                                                                                        jmp   n141_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_end_α:
                                                                                        jmp   n141_statement_begin_α
#=======================================================================================================================
#         differ(x)                  :f(e007_inner)
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_begin_α:
                                                                                        jmp   n142_var_α
n141_statement_begin_β:
                                                                                        jmp   n151_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n142_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # x
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n143_call_α
#-----------------------------------------------------------------------------------------------------------------------
n143_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lbynamefnzd144:        .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd144]                   # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx381_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n141_statement_begin_β
.Lx381_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n144_statement_end_α
n143_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n141_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n145_statement_begin_α
#=======================================================================================================================
#         checklocal = 'local-not-null'                  :(return)
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_begin_α:
                                                                                        jmp   n146_lit_string_α
n145_statement_begin_β:
                                                                                        jmp   n150_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n146_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx386_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n147_assign_α
.Lx386_0:
                        .quad            .Lx386_0_s
.Lx386_0_s:
                        .string          "local-not-null"
#-----------------------------------------------------------------------------------------------------------------------
n147_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052384], rax                    # checklocal
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n148_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:
                                                                                        jmp   n149_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n149_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx391_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx391_1
.Lx391_0:
                        .quad            .Lx391_0_s
.Lx391_0_s:
                        .string          "return"
.Lx391_1:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n150_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx393_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx393_1
.Lx393_0:
                        .quad            .Lx393_0_s
.Lx393_0_s:
                        .string          "return"
.Lx393_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n151_statement_begin_α:
                                                                                        jmp   n152_statement_end_α
n151_statement_begin_β:
                                                                                        jmp   n153_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_end_α:
                                                                                        jmp   n153_statement_begin_α
#=======================================================================================================================
#         checklocal =                                   :(return)
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α:
                                                                                        jmp   n154_lit_string_α
n153_statement_begin_β:
                                                                                        jmp   n158_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n154_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx400_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n155_assign_α
.Lx400_0:
                        .quad            .Lx400_0_s
.Lx400_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052384], rax                    # checklocal
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n156_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:
                                                                                        jmp   n157_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n157_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx405_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx405_1
.Lx405_0:
                        .quad            .Lx405_0_s
.Lx405_0_s:
                        .string          "return"
.Lx405_1:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n158_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx407_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx407_1
.Lx407_0:
                        .quad            .Lx407_0_s
.Lx407_0_s:
                        .string          "return"
.Lx407_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n159_goto_α:
                                                                                        jmp   n151_statement_begin_α
n159_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n160_goto_α:
                                                                                        jmp   n50_statement_begin_α
n160_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n161_goto_α:
                                                                                        jmp   n60_statement_begin_α
n161_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n162_goto_α:
                                                                                        jmp   n74_statement_begin_α
n162_goto_β:
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

                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "jlab"
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
n0_statement_begin_β:
                                                                                        jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:
                                                                                        jmp   n2_statement_begin_α
#=======================================================================================================================
# jlab    define('jlab(a,b,c)d,e,f')
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_lit_string_α
n2_statement_begin_β:
                                                                                        jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 16
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_call_α
.Lx134_0:
                        .quad            .Lx134_0_s
.Lx134_0_s:
                        .string          "jlab(a,b,c)d,e,f"
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
                                                                                        jne   .Lx135_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n2_statement_begin_β
.Lx135_240:
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
#         differ(arg(.jlab, 1), 'A')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:
                                                                                        jmp   n9_lit_string_α
n8_statement_begin_β:
                                                                                        jmp   n20_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n10_call_α
.Lx144_0:
                        .quad            .Lx144_0_s
.Lx144_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n10_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd146:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd146]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx145_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n8_statement_begin_β
.Lx145_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n11_lit_integer_α
n10_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n8_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx147_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n12_call_α
n11_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n8_statement_begin_β
.Lx147_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n12_call_α:
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
.Lbynamefnzd13:         .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd13]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx148_240
                        add              rsp, 16
                                                                                        jmp   n11_lit_integer_β
.Lx148_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n13_lit_string_α
n12_call_β:
                        add              rsp, 16
                                                                                        jmp   n11_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_call_α
n13_lit_string_β:
                        add              rsp, 32
                                                                                        jmp   n11_lit_integer_β
.Lx149_0:
                        .quad            .Lx149_0_s
.Lx149_0_s:
                        .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:
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
.Lbynamefnzd15:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd15]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx150_240
                        add              rsp, 16
                                                                                        jmp   n13_lit_string_β
.Lx150_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n15_statement_end_α
n14_call_β:
                        add              rsp, 16
                                                                                        jmp   n13_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n16_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1017/001: arg(.jlab,1) = A'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:
                                                                                        jmp   n17_lit_string_α
n16_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx155_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_assign_α
.Lx155_0:
                        .quad            .Lx155_0_s
.Lx155_0_s:
                        .string          "FAIL 1017/001: arg(.jlab,1) = A"
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n19_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:
                                                                                        jmp   n21_statement_end_α
n20_statement_begin_β:
                                                                                        jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:
                                                                                        jmp   n22_statement_begin_α
#=======================================================================================================================
#         differ(arg(.jlab, 3), 'C')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:
                                                                                        jmp   n23_lit_string_α
n22_statement_begin_β:
                                                                                        jmp   n34_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx165_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n24_call_α
.Lx165_0:
                        .quad            .Lx165_0_s
.Lx165_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd167:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd167]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx166_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n22_statement_begin_β
.Lx166_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n25_lit_integer_α
n24_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n22_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx168_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n26_call_α
n25_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n22_statement_begin_β
.Lx168_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n26_call_α:
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
.Lbynamefnzd27:         .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd27]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx169_240
                        add              rsp, 16
                                                                                        jmp   n25_lit_integer_β
.Lx169_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n27_lit_string_α
n26_call_β:
                        add              rsp, 16
                                                                                        jmp   n25_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx170_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_call_α
n27_lit_string_β:
                        add              rsp, 32
                                                                                        jmp   n25_lit_integer_β
.Lx170_0:
                        .quad            .Lx170_0_s
.Lx170_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n28_call_α:
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
.Lbynamefnzd29:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd29]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx171_240
                        add              rsp, 16
                                                                                        jmp   n27_lit_string_β
.Lx171_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n29_statement_end_α
n28_call_β:
                        add              rsp, 16
                                                                                        jmp   n27_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n30_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1017/002: arg(.jlab,3) = C'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:
                                                                                        jmp   n31_lit_string_α
n30_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n32_assign_α
.Lx176_0:
                        .quad            .Lx176_0_s
.Lx176_0_s:
                        .string          "FAIL 1017/002: arg(.jlab,3) = C"
#-----------------------------------------------------------------------------------------------------------------------
n32_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n33_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_begin_α:
                                                                                        jmp   n35_statement_end_α
n34_statement_begin_β:
                                                                                        jmp   n36_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:
                                                                                        jmp   n36_statement_begin_α
#=======================================================================================================================
#         arg(.jlab, 0)              :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:
                                                                                        jmp   n37_lit_string_α
n36_statement_begin_β:
                                                                                        jmp   n46_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx186_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n38_call_α
.Lx186_0:
                        .quad            .Lx186_0_s
.Lx186_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n38_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd188:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd188]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx187_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n36_statement_begin_β
.Lx187_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n39_lit_integer_α
n38_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n36_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx189_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n40_call_α
n39_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n36_statement_begin_β
.Lx189_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n40_call_α:
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
.Lbynamefnzd41:         .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd41]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx190_240
                        add              rsp, 16
                                                                                        jmp   n39_lit_integer_β
.Lx190_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n41_statement_end_α
n40_call_β:
                        add              rsp, 16
                                                                                        jmp   n39_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n42_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1017/003: arg(.jlab,0) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:
                                                                                        jmp   n43_lit_string_α
n42_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n44_assign_α
.Lx195_0:
                        .quad            .Lx195_0_s
.Lx195_0_s:
                        .string          "FAIL 1017/003: arg(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n44_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n45_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:
                                                                                        jmp   n47_statement_end_α
n46_statement_begin_β:
                                                                                        jmp   n48_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:
                                                                                        jmp   n48_statement_begin_α
#=======================================================================================================================
#         arg(.jlab, 4)              :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:
                                                                                        jmp   n49_lit_string_α
n48_statement_begin_β:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n50_call_α
.Lx205_0:
                        .quad            .Lx205_0_s
.Lx205_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n50_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd207:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd207]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx206_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n48_statement_begin_β
.Lx206_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n51_lit_integer_α
n50_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n48_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n52_call_α
n51_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n48_statement_begin_β
.Lx208_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
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
.Lbynamefnzd53:         .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd53]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx209_240
                        add              rsp, 16
                                                                                        jmp   n51_lit_integer_β
.Lx209_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_statement_end_α
n52_call_β:
                        add              rsp, 16
                                                                                        jmp   n51_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n54_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1017/004: arg(.jlab,4) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:
                                                                                        jmp   n55_lit_string_α
n54_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx214_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n56_assign_α
.Lx214_0:
                        .quad            .Lx214_0_s
.Lx214_0_s:
                        .string          "FAIL 1017/004: arg(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n57_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_begin_α:
                                                                                        jmp   n59_statement_end_α
n58_statement_begin_β:
                                                                                        jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:
                                                                                        jmp   n60_statement_begin_α
#=======================================================================================================================
#         differ(local(.jlab, 1), 'D')                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:
                                                                                        jmp   n61_lit_string_α
n60_statement_begin_β:
                                                                                        jmp   n72_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx224_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n62_call_α
.Lx224_0:
                        .quad            .Lx224_0_s
.Lx224_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd226:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd226]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx225_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n60_statement_begin_β
.Lx225_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n63_lit_integer_α
n62_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n60_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx227_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n64_call_α
n63_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n60_statement_begin_β
.Lx227_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:
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
.Lbynamefnzd65:         .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd65]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx228_240
                        add              rsp, 16
                                                                                        jmp   n63_lit_integer_β
.Lx228_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n65_lit_string_α
n64_call_β:
                        add              rsp, 16
                                                                                        jmp   n63_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx229_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n66_call_α
n65_lit_string_β:
                        add              rsp, 32
                                                                                        jmp   n63_lit_integer_β
.Lx229_0:
                        .quad            .Lx229_0_s
.Lx229_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n66_call_α:
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
.Lbynamefnzd67:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd67]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx230_240
                        add              rsp, 16
                                                                                        jmp   n65_lit_string_β
.Lx230_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_statement_end_α
n66_call_β:
                        add              rsp, 16
                                                                                        jmp   n65_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n68_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1017/005: local(.jlab,1) = D'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:
                                                                                        jmp   n69_lit_string_α
n68_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_assign_α
.Lx235_0:
                        .quad            .Lx235_0_s
.Lx235_0_s:
                        .string          "FAIL 1017/005: local(.jlab,1) = D"
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n71_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:
                                                                                        jmp   n73_statement_end_α
n72_statement_begin_β:
                                                                                        jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:
                                                                                        jmp   n74_statement_begin_α
#=======================================================================================================================
#         differ(local(.jlab, 3), 'F')                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:
                                                                                        jmp   n75_lit_string_α
n74_statement_begin_β:
                                                                                        jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx245_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n76_call_α
.Lx245_0:
                        .quad            .Lx245_0_s
.Lx245_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n76_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd247:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd247]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx246_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_β
.Lx246_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n77_lit_integer_α
n76_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n74_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx248_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n78_call_α
n77_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n74_statement_begin_β
.Lx248_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n78_call_α:
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
.Lbynamefnzd79:         .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd79]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx249_240
                        add              rsp, 16
                                                                                        jmp   n77_lit_integer_β
.Lx249_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n79_lit_string_α
n78_call_β:
                        add              rsp, 16
                                                                                        jmp   n77_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx250_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n80_call_α
n79_lit_string_β:
                        add              rsp, 32
                                                                                        jmp   n77_lit_integer_β
.Lx250_0:
                        .quad            .Lx250_0_s
.Lx250_0_s:
                        .string          "F"
#-----------------------------------------------------------------------------------------------------------------------
n80_call_α:
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
.Lbynamefnzd81:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd81]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx251_240
                        add              rsp, 16
                                                                                        jmp   n79_lit_string_β
.Lx251_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n81_statement_end_α
n80_call_β:
                        add              rsp, 16
                                                                                        jmp   n79_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n82_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1017/006: local(.jlab,3) = F'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_begin_α:
                                                                                        jmp   n83_lit_string_α
n82_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx256_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n84_assign_α
.Lx256_0:
                        .quad            .Lx256_0_s
.Lx256_0_s:
                        .string          "FAIL 1017/006: local(.jlab,3) = F"
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n85_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:
                                                                                        jmp   n87_statement_end_α
n86_statement_begin_β:
                                                                                        jmp   n88_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_end_α:
                                                                                        jmp   n88_statement_begin_α
#=======================================================================================================================
#         local(.jlab, 0)            :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_begin_α:
                                                                                        jmp   n89_lit_string_α
n88_statement_begin_β:
                                                                                        jmp   n98_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n89_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n90_call_α
.Lx266_0:
                        .quad            .Lx266_0_s
.Lx266_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n90_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd268:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd268]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx267_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n88_statement_begin_β
.Lx267_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n91_lit_integer_α
n90_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n88_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n92_call_α
n91_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n88_statement_begin_β
.Lx269_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n92_call_α:
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
.Lbynamefnzd93:         .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd93]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx270_240
                        add              rsp, 16
                                                                                        jmp   n91_lit_integer_β
.Lx270_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n93_statement_end_α
n92_call_β:
                        add              rsp, 16
                                                                                        jmp   n91_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n94_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1017/007: local(.jlab,0) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_begin_α:
                                                                                        jmp   n95_lit_string_α
n94_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx275_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n96_assign_α
.Lx275_0:
                        .quad            .Lx275_0_s
.Lx275_0_s:
                        .string          "FAIL 1017/007: local(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n96_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n97_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_begin_α:
                                                                                        jmp   n99_statement_end_α
n98_statement_begin_β:
                                                                                        jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:
                                                                                        jmp   n100_statement_begin_α
#=======================================================================================================================
#         local(.jlab, 4)            :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α:
                                                                                        jmp   n101_lit_string_α
n100_statement_begin_β:
                                                                                        jmp   n110_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx285_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n102_call_α
.Lx285_0:
                        .quad            .Lx285_0_s
.Lx285_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n102_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd287:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd287]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx286_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n100_statement_begin_β
.Lx286_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n103_lit_integer_α
n102_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n100_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n103_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx288_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n104_call_α
n103_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n100_statement_begin_β
.Lx288_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n104_call_α:
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
.Lbynamefnzd105:        .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd105]                   # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx289_240
                        add              rsp, 16
                                                                                        jmp   n103_lit_integer_β
.Lx289_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n105_statement_end_α
n104_call_β:
                        add              rsp, 16
                                                                                        jmp   n103_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_end_α:
                        add              rsp, 64
                                                                                        jmp   n106_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 1017/008: local(.jlab,4) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_begin_α:
                                                                                        jmp   n107_lit_string_α
n106_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n108_assign_α
.Lx294_0:
                        .quad            .Lx294_0_s
.Lx294_0_s:
                        .string          "FAIL 1017/008: local(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n108_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n109_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_begin_α:
                                                                                        jmp   n111_statement_end_α
n110_statement_begin_β:
                                                                                        jmp   n112_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_end_α:
                                                                                        jmp   n112_statement_begin_α
#=======================================================================================================================
#         output = 'PASS 1017_arg_local (8/8)'
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_begin_α:
                                                                                        jmp   n113_lit_string_α
n112_statement_begin_β:
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx304_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n114_assign_α
.Lx304_0:
                        .quad            .Lx304_0_s
.Lx304_0_s:
                        .string          "PASS 1017_arg_local (8/8)"
#-----------------------------------------------------------------------------------------------------------------------
n114_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # output
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n115_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_begin_α:
                                                                                        jmp   n117_statement_end_α
n116_statement_begin_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n118_goto_α:
                                                                                        jmp   n2_statement_begin_α
n118_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n119_goto_α:
                                                                                        jmp   n20_statement_begin_α
n119_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n120_goto_α:
                                                                                        jmp   n34_statement_begin_α
n120_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n121_goto_α:
                                                                                        jmp   n46_statement_begin_α
n121_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n122_goto_α:
                                                                                        jmp   n58_statement_begin_α
n122_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n123_goto_α:
                                                                                        jmp   n72_statement_begin_α
n123_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n124_goto_α:
                                                                                        jmp   n86_statement_begin_α
n124_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n125_goto_α:
                                                                                        jmp   n98_statement_begin_α
n125_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n126_goto_α:
                                                                                        jmp   n110_statement_begin_α
n126_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n127_goto_α:
                                                                                        jmp   n116_statement_begin_α
n127_goto_β:
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

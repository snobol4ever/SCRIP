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
#         differ(3 + 2, 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_lit_integer_α
.Lx141_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_binop_α
.Lx142_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n5_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx143_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n13_statement_begin_α
.Lx143_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n7_call_α
.Lx144_0:
                        .quad            5
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
                                                                                        jne   .Lx145_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n13_statement_begin_α
.Lx145_240:
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
#         output = 'FAIL 410/001: 3+2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n11_assign_α
.Lx150_0:
                        .quad            .Lx150_0_s
.Lx150_0_s:
                        .string          "FAIL 410/001: 3+2"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n12_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:
                                                                                        jmp   n14_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:
                                                                                        jmp   n15_statement_begin_α
#=======================================================================================================================
#         differ(3 - 2, 1)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:
                                                                                        jmp   n16_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n17_lit_integer_α
.Lx160_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx161_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_binop_α
.Lx161_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx162_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n26_statement_begin_α
.Lx162_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n19_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx163_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n20_call_α
.Lx163_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
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
.Lbynamefnzd21:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd21]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx164_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n26_statement_begin_α
.Lx164_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_statement_end_α
n20_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n22_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 410/002: 3-2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:
                                                                                        jmp   n23_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n24_assign_α
.Lx169_0:
                        .quad            .Lx169_0_s
.Lx169_0_s:
                        .string          "FAIL 410/002: 3-2"
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n25_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:
                                                                                        jmp   n27_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:
                                                                                        jmp   n28_statement_begin_α
#=======================================================================================================================
#         differ(3 * 2, 6)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:
                                                                                        jmp   n29_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n30_lit_integer_α
.Lx179_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx180_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n31_binop_α
.Lx180_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n31_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_mul@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx181_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n39_statement_begin_α
.Lx181_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx182_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n33_call_α
.Lx182_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n33_call_α:
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
.Lbynamefnzd34:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd34]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx183_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n39_statement_begin_α
.Lx183_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n34_statement_end_α
n33_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n39_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n35_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 410/003: 3*2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:
                                                                                        jmp   n36_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx188_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n37_assign_α
.Lx188_0:
                        .quad            .Lx188_0_s
.Lx188_0_s:
                        .string          "FAIL 410/003: 3*2"
#-----------------------------------------------------------------------------------------------------------------------
n37_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n38_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:
                                                                                        jmp   n40_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:
                                                                                        jmp   n41_statement_begin_α
#=======================================================================================================================
#         differ(5 / 2, 2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx198_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n43_lit_integer_α
.Lx198_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n44_binop_α
.Lx199_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n44_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_div@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx200_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n52_statement_begin_α
.Lx200_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n46_call_α
.Lx201_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:
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
.Lbynamefnzd47:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd47]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx202_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n52_statement_begin_α
.Lx202_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n47_statement_end_α
n46_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n52_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n48_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 410/004: 5/2 integer division'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:
                                                                                        jmp   n49_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx207_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n50_assign_α
.Lx207_0:
                        .quad            .Lx207_0_s
.Lx207_0_s:
                        .string          "FAIL 410/004: 5/2 integer division"
#-----------------------------------------------------------------------------------------------------------------------
n50_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n51_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:
                                                                                        jmp   n53_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:
                                                                                        jmp   n54_statement_begin_α
#=======================================================================================================================
#         differ(2 ** 3, 8)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:
                                                                                        jmp   n55_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx217_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n56_lit_integer_α
.Lx217_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx218_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n57_binop_α
.Lx218_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n57_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_pow@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx219_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n65_statement_begin_α
.Lx219_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n58_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n59_call_α
.Lx220_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n59_call_α:
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
.Lbynamefnzd60:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd60]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx221_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n65_statement_begin_α
.Lx221_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n60_statement_end_α
n59_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n61_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 410/005: 2**3'                  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:
                                                                                        jmp   n62_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 18
                        mov              rax, qword ptr [rip + .Lx226_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n63_assign_α
.Lx226_0:
                        .quad            .Lx226_0_s
.Lx226_0_s:
                        .string          "FAIL 410/005: 2**3"
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n64_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:
                                                                                        jmp   n66_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:
                                                                                        jmp   n67_statement_begin_α
#=======================================================================================================================
#         differ('3' + 2, 5)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:
                                                                                        jmp   n68_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx236_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n69_lit_integer_α
.Lx236_0:
                        .quad            .Lx236_0_s
.Lx236_0_s:
                        .string          "3"
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx237_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_binop_α
.Lx237_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n70_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx238_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n78_statement_begin_α
.Lx238_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n71_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx239_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n72_call_α
.Lx239_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n72_call_α:
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
.Lbynamefnzd73:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd73]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx240_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n78_statement_begin_α
.Lx240_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n73_statement_end_α
n72_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n74_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 410/006: string+int coerce'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:
                                                                                        jmp   n75_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx245_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n76_assign_α
.Lx245_0:
                        .quad            .Lx245_0_s
.Lx245_0_s:
                        .string          "FAIL 410/006: string+int coerce"
#-----------------------------------------------------------------------------------------------------------------------
n76_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n77_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:
                                                                                        jmp   n79_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_end_α:
                                                                                        jmp   n80_statement_begin_α
#=======================================================================================================================
#         differ(3 + '-2', 1)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_begin_α:
                                                                                        jmp   n81_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx255_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n82_lit_string_α
.Lx255_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx256_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n83_binop_α
.Lx256_0:
                        .quad            .Lx256_0_s
.Lx256_0_s:
                        .string          "-2"
#-----------------------------------------------------------------------------------------------------------------------
n83_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx257_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n91_statement_begin_α
.Lx257_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n84_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx258_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n85_call_α
.Lx258_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n85_call_α:
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
.Lbynamefnzd86:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd86]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx259_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n91_statement_begin_α
.Lx259_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n86_statement_end_α
n85_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n87_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 410/007: int+neg-string'        :(end)
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_begin_α:
                                                                                        jmp   n88_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n89_assign_α
.Lx264_0:
                        .quad            .Lx264_0_s
.Lx264_0_s:
                        .string          "FAIL 410/007: int+neg-string"
#-----------------------------------------------------------------------------------------------------------------------
n89_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n90_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:
                                                                                        jmp   n92_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_end_α:
                                                                                        jmp   n93_statement_begin_α
#=======================================================================================================================
#         differ('1' + '0', 1)                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_begin_α:
                                                                                        jmp   n94_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n95_lit_string_α
.Lx274_0:
                        .quad            .Lx274_0_s
.Lx274_0_s:
                        .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx275_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n96_binop_α
.Lx275_0:
                        .quad            .Lx275_0_s
.Lx275_0_s:
                        .string          "0"
#-----------------------------------------------------------------------------------------------------------------------
n96_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx276_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n104_statement_begin_α
.Lx276_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n97_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n98_call_α
.Lx277_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n98_call_α:
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
.Lbynamefnzd99:         .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd99]                    # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx278_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n104_statement_begin_α
.Lx278_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n99_statement_end_α
n98_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n104_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n100_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 410/008: string+string coerce'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α:
                                                                                        jmp   n101_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx283_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n102_assign_α
.Lx283_0:
                        .quad            .Lx283_0_s
.Lx283_0_s:
                        .string          "FAIL 410/008: string+string coerce"
#-----------------------------------------------------------------------------------------------------------------------
n102_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n103_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_begin_α:
                                                                                        jmp   n105_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_end_α:
                                                                                        jmp   n106_statement_begin_α
#=======================================================================================================================
#         differ(5 + '', 5)                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_begin_α:
                                                                                        jmp   n107_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx293_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n108_lit_string_α
.Lx293_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n108_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n109_binop_α
.Lx294_0:
                        .quad            .Lx294_0_s
.Lx294_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n109_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx295_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n117_statement_begin_α
.Lx295_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n110_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx296_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n111_call_α
.Lx296_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n111_call_α:
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
.Lbynamefnzd112:        .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd112]                   # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx297_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n117_statement_begin_α
.Lx297_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n112_statement_end_α
n111_call_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n117_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n113_statement_begin_α
#=======================================================================================================================
#         output = 'FAIL 410/009: null addend is zero'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_begin_α:
                                                                                        jmp   n114_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n114_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx302_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n115_assign_α
.Lx302_0:
                        .quad            .Lx302_0_s
.Lx302_0_s:
                        .string          "FAIL 410/009: null addend is zero"
#-----------------------------------------------------------------------------------------------------------------------
n115_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n116_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_begin_α:
                                                                                        jmp   n118_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n118_statement_end_α:
                                                                                        jmp   n119_statement_begin_α
#=======================================================================================================================
#         output = 'PASS 410_arith_int (9/9)'
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_begin_α:
                                                                                        jmp   n120_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 24
                        mov              rax, qword ptr [rip + .Lx312_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n121_assign_α
.Lx312_0:
                        .quad            .Lx312_0_s
.Lx312_0_s:
                        .string          "PASS 410_arith_int (9/9)"
#-----------------------------------------------------------------------------------------------------------------------
n121_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # output
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n122_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n123_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_begin_α:
                                                                                        jmp   n124_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n125_goto_α:
                                                                                        jmp   n13_statement_begin_α
n125_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n126_goto_α:
                                                                                        jmp   n26_statement_begin_α
n126_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n127_goto_α:
                                                                                        jmp   n39_statement_begin_α
n127_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n128_goto_α:
                                                                                        jmp   n52_statement_begin_α
n128_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n129_goto_α:
                                                                                        jmp   n65_statement_begin_α
n129_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n130_goto_α:
                                                                                        jmp   n78_statement_begin_α
n130_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n131_goto_α:
                                                                                        jmp   n91_statement_begin_α
n131_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n132_goto_α:
                                                                                        jmp   n104_statement_begin_α
n132_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n133_goto_α:
                                                                                        jmp   n117_statement_begin_α
n133_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n134_goto_α:
                                                                                        jmp   n123_statement_begin_α
n134_goto_β:
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
